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
 * @file Scriptforge.Argv.ixx
 * @brief 定义了 `Scriptforge::Argv` 模块，其中包含了一个 `Argv` 类，用于处理命令行参数。
 * @author Scriptforge
 * @date 2026/5/25
 */

module;

#include <cstdint>

#include "Scriptforge.Pch.hpp"

export module Scriptforge.Argv;

namespace Scriptforge::Argv {

    export namespace Hash {
        struct FNV_1a_32 {
            static constexpr uint32_t hash(std::string_view s) {
                uint32_t h = 0x811C9DC5u;
                for (char c : s)
                    h = (h ^ (uint8_t)c) * 0x01000193u;
                return h;
            }
        };
    }

    /**
    * @details 一个用于检查类型是否为有效的 argv 命令的概念。
    * 可以编写以下类型的命令结构体/类：
    * ```cpp
    * struct ArgvCommand {
    *     static constexpr std::string_view name = "--command";
    *     static constexpr std::string_view shortName = "-c";
    *     static void run(std::vector<std::string>::iterator it, std::string_view arg, std::ostream& os, std::ostream& err, std::istream& is) {
    *         os << "Running command: " << arg << std::endl;
    *     }
    * };
    * ```
   */
    template<typename T>
    concept isArgvCommand = requires(std::vector<std::string>::iterator it, std::string_view s, std::ostream & os, std::ostream & err, std::istream & is) {
        { T::name } -> std::convertible_to<std::string_view>;
        { T::shortName } -> std::convertible_to<std::string_view>;
        T::run(it, s, os, err, is);
    };

    /**
    * @details 一个用于检查类型是否为有效的 argv 未知命令的概念。
    * 可以编写以下类型的命令结构体/类：
    * ```cpp
    * struct ArgvUnknown {
    *     static void run(std::vector<std::string>::iterator it, std::string_view arg, std::ostream& os, std::ostream& err, std::istream& is) {
    *         err << "Running unknown command: " << arg << std::endl;
    *     }
    * };
    * ```
   */
    template<typename T>
    concept isArgvUnknown = requires(std::vector<std::string>::iterator it, std::string_view s, std::ostream & os, std::ostream & err, std::istream & is) {
        T::run(it, s, os, err, is);
    };

    template<typename HashT>
    consteval bool testHashConstexpr() {
        // 随便传一段字符串，能编译通过说明 HashT::hash 是 constexpr
        constexpr std::string_view test_str = "test";
        constexpr auto val = HashT::hash(test_str);
        return true;
    }

    /*
     * @details 一个用于检查类型是否具有 constexpr hash 函数的概念。
     * 可以编写以下类型的哈希结构体/类：
     * ```cpp
     * struct MyHash {
     *     static constexpr uint32_t hash(std::string_view s) {
     *         //TODO: 实现哈希函数
     *     }
     * };
     * ```
    */
    template<typename HashT>
    concept isHash = requires(std::string_view s) {
        testHashConstexpr<HashT>();
        { HashT::hash(s) } -> std::convertible_to<uint32_t>;
    };

    namespace DetectHashCollision {
        template<isHash Hash, isArgvCommand... Cmd>
        consteval auto collectAllHashes() {
            // 计算总数量：每个命令 2 个哈希（name + shortName）
            constexpr size_t total = sizeof...(Cmd) * 2;
            std::array<uint32_t, total> arr{};

            size_t idx = 0;
            // 折叠表达式遍历所有命令
            ([&] {
                arr[idx++] = Hash::hash(Cmd::name);
                arr[idx++] = Hash::hash(Cmd::shortName);
                }(), ...);

            return arr;
        }

        template<size_t N>
        consteval bool hasHashCollision(const std::array<uint32_t, N>& hashes) {
            for (size_t i = 0; i < N; i++) {
                for (size_t j = i + 1; j < N; j++) {
                    if (hashes[i] == hashes[j]) {
                        return true;
                    }
                }
            }
            return false;
        }

        template<isHash Hash, isArgvUnknown Unknown, isArgvCommand... Cmd>
        consteval bool noCommandHashCollision() {
            constexpr auto hash_arr = collectAllHashes<Hash, Cmd...>();
            return !hasHashCollision<hash_arr.size()>(hash_arr);
        }
    }



    // 给 run 模板加约束：必须无哈希碰撞
    template<typename Hash, typename Unknown, typename... Cmd>
    concept isAllCommandHashUnique =
        isHash<Hash> && isArgvUnknown<Unknown> && (isArgvCommand<Cmd> && ...) &&
        DetectHashCollision::noCommandHashCollision<Hash, Unknown, Cmd...>();

    export
        template<isHash HashT>
    class ArgvCli {
    public:
        ArgvCli() = default;
        ArgvCli(const ArgvCli&) = delete;
        ArgvCli& operator=(const ArgvCli&) = delete;
        ArgvCli(ArgvCli&&) noexcept = default;
        ArgvCli& operator=(ArgvCli&&) noexcept = default;
        ArgvCli(const int& argc, char* argv[], std::ostream& os = std::cout, std::ostream& err = std::cerr, std::istream& is = std::cin);
        void init(const int& argc, char* argv[], std::ostream& os = std::cout, std::ostream& err = std::cerr, std::istream& is = std::cin);
        template<isArgvUnknown UnknownCommand, isArgvCommand... Commands>
            requires (isAllCommandHashUnique<HashT, UnknownCommand, Commands...>)
        void run();
        void stop();
        const std::vector<std::string>& getArgv() const;
    private:
        std::vector<std::string> m_argv;
        std::ostream* m_os{ &std::cout };
        std::ostream* m_err{ &std::cerr };
        std::istream* m_is{ &std::cin };
        bool m_stopFlag = false;
    };
}

namespace Scriptforge::Argv {
    template<isHash HashT>
    ArgvCli<HashT>::ArgvCli(const int& argc, char* argv[], std::ostream& os, std::ostream& err, std::istream& is) {
        init(argc, argv, os, err, is);
    }

    template<isHash HashT>
    void ArgvCli<HashT>::init(const int& argc, char* argv[], std::ostream& os, std::ostream& err, std::istream& is) {
        m_os = &os;
        m_err = &err;
        m_is = &is;
        m_argv.assign(argv + 1, argv + argc);
    }

    template<isHash HashT>
    template<isArgvUnknown UnknownCommand, isArgvCommand... Commands>
        requires (isAllCommandHashUnique<HashT, UnknownCommand, Commands...>)
    void ArgvCli<HashT>::run() {
        m_stopFlag = false;
        for (auto it = m_argv.begin(); it != m_argv.end(); ++it) {
            const auto& arg = *it;
            uint32_t h = HashT::hash(arg);

            bool found = false;
            if (arg.starts_with('-') && !m_stopFlag) {
                // 折叠表达式 = 生成 SWITCH 跳转表
                ([&] {
                    if (h == HashT::hash(Commands::name) ||
                        h == HashT::hash(Commands::shortName)) {
                        Commands::run(it, arg, *m_os, *m_err, *m_is);
                        found = true;
                    }
                    }(), ...);
            }
            else {
                continue;
            }
            if (!found) {
                UnknownCommand::run(it, arg, *m_os, *m_err, *m_is);
                break;
            }
        }
    }

    template<isHash HashT>
    void ArgvCli<HashT>::stop() {
        m_stopFlag = true;
    }

	template<isHash HashT>
	const std::vector<std::string>& ArgvCli<HashT>::getArgv() const {
		return m_argv;
	}
}