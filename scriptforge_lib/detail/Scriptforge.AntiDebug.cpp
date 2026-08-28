// Copyright 2026 Scriptforge
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file Scriptforge.AntiDebug.cpp
 * @brief 由于MSVC不支持将内联汇编放在模块接口单元中，因此将 `Scriptforge::AntiDebug` 模块中的`Scriptforge::AntiDebug::AntiDebugger::anti_debug`函数实现放在了这个源文件中。
 * @author Scriptforge
 * @date 2026/4/25
*/

 // Warning: It is only compatible with Windows!
module;

#include "Scriptforge.Define.hpp"
#include "Scriptforge.Pch.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <tlhelp32.h>
#include <intrin.h>
#include <psapi.h>

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif
#endif

module Scriptforge.AntiDebug;

import Scriptforge.AntiDebug.RandomDefine;
import Scriptforge.Local;

SCRIPTFORGE_BEGIN
namespace ADNS {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4702)
#endif

#if defined(_WIN32) || defined(_WIN64)
    namespace {
        constexpr uint32_t hashConst(const char* str, uint32_t value = 0) noexcept {
            return *str ? hashConst(str + 1, value * 33 + static_cast<uint8_t>(*str)) : value;
        }

        HMODULE getNtdllModule() noexcept {
            HMODULE hMods[1024]{};
            DWORD cbNeeded{};

            if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded)) {
                for (DWORD i = 0; i < cbNeeded / sizeof(HMODULE); ++i) {
                    char szModName[MAX_PATH]{};
                    GetModuleBaseNameA(GetCurrentProcess(), hMods[i], szModName, MAX_PATH);
                    if (hashConst(szModName) == hashConst("ntdll.dll"))
                        return hMods[i];
                }
            }
            return GetModuleHandleA("ntdll.dll");
        }

        UINT_PTR getPeb() noexcept {
#ifdef _WIN64
            return __readgsqword(0x60);
#else
            return __readfsdword(0x30);
#endif
        }

        void forceKillProcess() noexcept {
            const auto ntdll = getNtdllModule();
            if (!ntdll) return;

            using NtTerminateProc = NTSTATUS(NTAPI*)(HANDLE, NTSTATUS);
            const auto ntTerm = reinterpret_cast<NtTerminateProc>(
                GetProcAddress(ntdll, "NtTerminateProcess"));

            if (ntTerm) ntTerm(GetCurrentProcess(), 0);
            TerminateProcess(GetCurrentProcess(), 0);
            ExitProcess(0);
            abort();
        }

        bool checkDebugPort() noexcept {
            const auto ntdll = getNtdllModule();
            if (!ntdll) return false;

            using NtQueryInfoProc = NTSTATUS(NTAPI*)(HANDLE, UINT, PVOID, ULONG, PULONG);
            const auto ntQip = reinterpret_cast<NtQueryInfoProc>(
                GetProcAddress(ntdll, "NtQueryInformationProcess"));

            if (!ntQip) return false;
            uint64_t debugPort{};
            if (NT_SUCCESS(ntQip(GetCurrentProcess(), 7, &debugPort, sizeof(debugPort), 0)))
                return debugPort != 0;
            return false;
        }

        bool checkDebugRegisters() noexcept {
#ifdef _WIN64
            return (__readdr(0) | __readdr(1) | __readdr(2) | __readdr(3)) != 0;
#else
            CONTEXT ctx{};
            ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
            if (GetThreadContext(GetCurrentThread(), &ctx))
                return (ctx.Dr0 | ctx.Dr1 | ctx.Dr2 | ctx.Dr3) != 0;
            return false;
#endif
        }

        bool checkParentIsDebugger() noexcept {
            const auto pid = GetCurrentProcessId();
            const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap == INVALID_HANDLE_VALUE) return false;

            PROCESSENTRY32 pe{ sizeof(pe) };
            DWORD ppid{};
            if (Process32First(snap, &pe)) {
                do {
                    if (pe.th32ProcessID == pid) {
                        ppid = pe.th32ParentProcessID;
                        break;
                    }
                } while (Process32Next(snap, &pe));
            }
            CloseHandle(snap);
            if (!ppid) return false;

            const auto hp = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, 0, ppid);
            if (!hp) return false;

            wchar_t path[MAX_PATH]{};
            DWORD len = MAX_PATH;
            bool detected = false;

            if (QueryFullProcessImageNameW(hp, 0, path, &len)) {
                if (const auto name = wcsrchr(path, L'\\')) {
                    detected = !_wcsicmp(name + 1, L"x64dbg.exe")
                        || !_wcsicmp(name + 1, L"x32dbg.exe")
                        || !_wcsicmp(name + 1, L"ollydbg.exe")
                        || !_wcsicmp(name + 1, L"devenv.exe");
                }
            }
            CloseHandle(hp);
            return detected;
        }

        bool isDebuggerDetected() noexcept {
            return checkDebugPort()
                || checkDebugRegisters()
                || checkParentIsDebugger();
        }
    }
#endif

    // ======================================================

    ADCL::ADCL() : V1(false), V2(false) {}
    ADCL::~ADCL() { stop(); }

    void ADCL::F1() {
        if (V4.load(std::memory_order_acquire)) {
            return;
        }

        // 重置状态
        V1.store(false, std::memory_order_release);
        V2.store(false, std::memory_order_release);
        V4.store(true, std::memory_order_release);

        // 启动后台检测线程，立刻返回，异步执行F6(antiDebug)
        m_antiDebugThread = std::thread(&ADCL::F6, this);
    }

    void ADCL::F2() {
        V1 = true;

        std::lock_guard lock(V3);
        if (m_antiDebugThread.joinable()) {
            m_antiDebugThread.join();
        }
        V4 = false;
    }


    bool ADCL::F3() const {
        return V2;
    }
#if defined(_WIN32) || defined(_WIN64)
    bool ADCL::F4() noexcept {
        return isDebuggerDetected();
    }

    void ADCL::F5() noexcept {
        forceKillProcess();
    }
#else
#pragma message("The Operation System is not supported for the implementation of AntiDebugger")
    bool ADCL::F4() noexcept {
        return false;
    }
	void ADCL::F5() noexcept {
        return;
	}
#endif
    void ADCL::F6() {
        while (!V1) {
            if (isAntiDebug()) {
                V1.store(true, std::memory_order_release);
                V2.store(true, std::memory_order_release);
                killProcess();
            }

            if (V1) {
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(97));
        }
    }

#ifdef _MSC_VER
#pragma warning(pop)
#endif
}
SCRIPTFORGE_END