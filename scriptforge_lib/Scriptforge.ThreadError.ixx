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
module;

#include "Scriptforge.Pch.hpp"

export module Scriptforge.ThreadError;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Local;
import Scriptforge.Msg;

namespace Scriptforge {
    inline namespace Err {

        export template <bool Async = false>
            class ThreadError {
            public:
                ThreadError(Scriptforge::Local::Lang lang);  // 改为值语义，避免悬空引用
                ~ThreadError();

                ThreadError(const ThreadError&) = delete;
                ThreadError& operator=(const ThreadError&) = delete;
                ThreadError(ThreadError&&) = delete;
                ThreadError& operator=(ThreadError&&) = delete;

                void setThreadFunction(std::function<void(std::stop_token)> run);
                void start();
                void stop();

                void waitForCompletion();
                bool isRunning() const;
                std::future<void> getFuture();

            private:
                struct State {
                    std::mutex mtx;
                    std::atomic<bool> isRunning{ false };
                    std::exception_ptr storedException;
                    std::function<void(std::stop_token)> taskFunc;
                    std::promise<void> prom;
                    std::future<void> future;
                };

                static void threadFunc(std::shared_ptr<State> state, std::stop_token st);

                Scriptforge::Local::Lang m_lang;  // 值语义
                std::shared_ptr<State> m_state;
                std::jthread m_thread;
                std::atomic<bool> m_starting{ false };
        };

    }
}

namespace Scriptforge {
    inline namespace Err {

        template<bool Async>
        ThreadError<Async>::ThreadError(Scriptforge::Local::Lang lang)
            : m_lang(std::move(lang))
            , m_state(std::make_shared<State>()) {}

        template <bool Async>
        ThreadError<Async>::~ThreadError() {
            stop();
        }

        template<bool Async>
        void ThreadError<Async>::setThreadFunction(std::function<void(std::stop_token)> run) {
            std::lock_guard<std::mutex> lock(m_state->mtx);
            if (m_state->isRunning.load(std::memory_order_acquire)) {
                throw std::logic_error("cannot set task while thread is running");
            }
            m_state->taskFunc = std::move(run);
        }

        template<bool Async>
        void ThreadError<Async>::start() {
            bool expected = false;
            if (!m_starting.compare_exchange_strong(expected, true,
                std::memory_order_acq_rel)) {
                Scriptforge::ErrCode::throwError(
                    Scriptforge::ErrCode::ErrCode::ThreadErrorThreadAlreadyRunning,
                    __func__, m_lang);
            }

            struct StartingGuard {
                std::atomic<bool>& flag;
                ~StartingGuard() { flag.store(false, std::memory_order_release); }
            } guard{ m_starting };

            std::jthread oldThread;

            // 阶段一：在锁内完成所有状态校验与数据准备
            {
                std::lock_guard<std::mutex> lock(m_state->mtx);

                if (!m_state->taskFunc) {
                    Scriptforge::ErrCode::throwError(
                        Scriptforge::ErrCode::ErrCode::ThreadErrorThreadNoTask,
                        __func__, m_lang);
                }

                if (m_state->isRunning.load(std::memory_order_acquire)) {
                    Scriptforge::ErrCode::throwError(
                        Scriptforge::ErrCode::ErrCode::ThreadErrorThreadAlreadyRunning,
                        __func__, m_lang);
                }

                m_state->storedException = nullptr;

                if constexpr (Async) {
                    m_state->prom = std::promise<void>();
                    m_state->future = m_state->prom.get_future();
                }

                if (m_thread.joinable()) {
                    oldThread = std::move(m_thread);
                }

                m_state->isRunning.store(true, std::memory_order_release);
            }

            // 阶段二：在锁外 join 旧线程
            if (oldThread.joinable()) {
                try {
                    oldThread.join();
                }
                catch (...) {
                    // 忽略 join 异常，继续启动新线程
                }
            }

            // 阶段三：在锁外创建新线程
            try {
                m_thread = std::jthread([state = m_state](std::stop_token st) {
                    threadFunc(state, st);
                    });
            }
            catch (...) {
                std::lock_guard<std::mutex> lock(m_state->mtx);
                m_state->isRunning.store(false, std::memory_order_release);
                throw;
            }
        }

        template<bool Async>
        void ThreadError<Async>::stop() {
            std::jthread threadToJoin;

            {
                std::lock_guard<std::mutex> lock(m_state->mtx);
                if (m_thread.joinable()) {
                    m_thread.request_stop();
                    threadToJoin = std::move(m_thread);
                }
            }

            if (threadToJoin.joinable()) {
                try {
                    threadToJoin.join();
                }
                catch (...) {
                    // 忽略 join 异常，确保后续状态清理继续执行
                }
            }

            // 后备：确保状态一致
            {
                std::lock_guard<std::mutex> lock(m_state->mtx);
                m_state->isRunning.store(false, std::memory_order_release);
            }
        }

        template<bool Async>
        void ThreadError<Async>::waitForCompletion() {
            if constexpr (Async) {
                std::future<void> fut;
                bool hasFuture = false;
                {
                    std::lock_guard<std::mutex> lock(m_state->mtx);
                    if (m_state->future.valid()) {
                        fut = std::move(m_state->future);
                        hasFuture = true;
                    }
                }

                if (hasFuture) {
                    // 通过 future 等待；异常会通过 future 抛出
                    fut.get();
                }
                else {
                    // Future 已被移走或无效，回退到直接 join
                    std::jthread threadToJoin;
                    {
                        std::lock_guard<std::mutex> lock(m_state->mtx);
                        if (m_thread.joinable()) {
                            threadToJoin = std::move(m_thread);
                        }
                    }

                    if (threadToJoin.joinable()) {
                        try {
                            threadToJoin.join();
                        }
                        catch (...) {
                            // 忽略 join 异常
                        }
                    }

                    std::lock_guard<std::mutex> lock(m_state->mtx);
                    if (m_state->storedException) {
                        std::rethrow_exception(m_state->storedException);
                    }
                }
            }
            else {
                std::jthread threadToJoin;
                {
                    std::lock_guard<std::mutex> lock(m_state->mtx);
                    if (m_thread.joinable()) {
                        threadToJoin = std::move(m_thread);
                    }
                }

                if (threadToJoin.joinable()) {
                    try {
                        threadToJoin.join();
                    }
                    catch (...) {
                        // 忽略 join 异常，确保异常检查继续执行
                    }
                }

                std::lock_guard<std::mutex> lock(m_state->mtx);
                if (m_state->storedException) {
                    std::rethrow_exception(m_state->storedException);
                }
            }
        }

        template<bool Async>
        bool ThreadError<Async>::isRunning() const {
            return m_state->isRunning.load(std::memory_order_acquire);
        }

        template<bool Async>
        std::future<void> ThreadError<Async>::getFuture() {
            if constexpr (!Async) {
                throw std::logic_error("getFuture is only available in async mode");
            }

            std::lock_guard<std::mutex> lock(m_state->mtx);
            if (!m_state->future.valid()) {
                throw std::logic_error("no valid completion future");
            }
            return std::move(m_state->future);
        }

        template<bool Async>
        void ThreadError<Async>::threadFunc(
            std::shared_ptr<State> state,
            std::stop_token st)
        {
            // RAII 保证：无论发生任何异常，isRunning 最终必定为 false
            struct IsRunningGuard {
                std::atomic<bool>* flag;
                ~IsRunningGuard() noexcept {
                    flag->store(false, std::memory_order_release);
                }
            } runningGuard{ &state->isRunning };

            try {
                // 在 try 内拷贝 task，防止拷贝异常泄漏到 jthread 外导致 terminate
                std::function<void(std::stop_token)> task;
                {
                    std::lock_guard<std::mutex> lock(state->mtx);
                    task = state->taskFunc;
                }

                if (task) {
                    task(st);
                }

                if constexpr (Async) {
                    std::lock_guard<std::mutex> lock(state->mtx);
                    try {
                        state->prom.set_value();
                    }
                    catch (...) {
                        // 忽略 set_value 可能抛出的 std::future_error
                    }
                }
            }
            catch (...) {
                std::exception_ptr ep = std::current_exception();
                {
                    std::lock_guard<std::mutex> lock(state->mtx);
                    state->storedException = ep;
                }

                if constexpr (Async) {
                    std::lock_guard<std::mutex> lock(state->mtx);
                    try {
                        state->prom.set_exception(ep);
                    }
                    catch (...) {
                        // 忽略 set_exception 可能抛出的 std::future_error
                    }
                }
            }
        }

    }
}