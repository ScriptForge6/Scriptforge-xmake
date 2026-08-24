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

#include "Scriptforge.Define.hpp"
#include "Scriptforge.Pch.hpp"

export module Scriptforge.ThreadError;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Local;
import Scriptforge.Msg;

_SF_ERR_BEGIN

export template <bool Async = false>
class ThreadError {
public:
    ThreadError(const Scriptforge::Local::Lang& lang);
    ThreadError(Scriptforge::Local::Lang&& lang);
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

    Scriptforge::Local::Lang m_lang;
    std::shared_ptr<State> m_state;
    std::jthread m_thread;
    std::atomic<bool> m_starting{ false };
};

_SF_ERR_END

_SF_ERR_BEGIN

_SF_THREADERROR_TEM
_SF_THREADERROR ThreadError(const Scriptforge::Local::Lang& lang)
    : m_lang(lang)
    , m_state(std::make_shared<State>()) {}

_SF_THREADERROR_TEM
_SF_THREADERROR ThreadError(Scriptforge::Local::Lang&& lang)
    : m_lang(std::move(lang))
    , m_state(std::make_shared<State>()) {}

_SF_THREADERROR_TEM
_SF_THREADERROR ~ThreadError() {
    stop();
}

_SF_THREADERROR_TEM
void _SF_THREADERROR setThreadFunction(std::function<void(std::stop_token)> run) {
    std::lock_guard<std::mutex> lock(m_state->mtx);
    if (m_state->isRunning.load(std::memory_order_acquire)) {
        throw std::logic_error("cannot set task while thread is running");
    }
    m_state->taskFunc = std::move(run);
}

_SF_THREADERROR_TEM
void _SF_THREADERROR start() {
    bool expected = false;
    if (!m_starting.compare_exchange_strong(expected, true,
        std::memory_order_acq_rel)) {
        _SF_THROW throwError(
            _SF_CODE ErrCode::ThreadErrorThreadAlreadyRunning,
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
            _SF_THROW throwError(
                _SF_CODE ErrCode::ThreadErrorThreadNoTask,
                __func__, m_lang);
        }

        if (m_state->isRunning.load(std::memory_order_acquire)) {
            _SF_THROW throwError(
                _SF_CODE ErrCode::ThreadErrorThreadAlreadyRunning,
                __func__, m_lang);
        }

        m_state->storedException = nullptr;

        if constexpr (Async) {
            m_state->prom = std::promise<void>();
            m_state->future = m_state->prom.get_future();
        }

        if (m_thread.joinable()) {  // 是否需要进一步检查如果返回 false 的可能性
            oldThread = std::move(m_thread);
        }

        m_state->isRunning.store(true, std::memory_order_release);
    }

    // 阶段二：在锁外 join 旧线程
    if (oldThread.joinable()) {   // 是否需要进一步检查如果返回 false 的可能性
        try {
            oldThread.join();
        }
        catch (...) {
            // 忽略 join 异常，继续启动新线程  //是否需要完整忽略
        }
    }

    // 阶段三：在锁外创建新线程
    try {
        m_thread = std::jthread([state = m_state](std::stop_token st) { //拷贝m_state没加锁，是否安全
            threadFunc(state, st);
            });
    }
    catch (...) {
        std::lock_guard<std::mutex> lock(m_state->mtx);
        m_state->isRunning.store(false, std::memory_order_release);
        throw;
    }
}

_SF_THREADERROR_TEM
void _SF_THREADERROR stop() {
    std::jthread threadToJoin;

    {
        std::lock_guard<std::mutex> lock(m_state->mtx);
        if (m_thread.joinable()) {  // 是否需要进一步检查如果返回 false 的可能性
            m_thread.request_stop();
            threadToJoin = std::move(m_thread);
        }
    }

    if (threadToJoin.joinable()) {
        try {
            threadToJoin.join();
        }
        catch (...) {
            // 忽略 join 异常，确保后续状态清理继续执行  //是否需要完整忽略
        }
    }

    // 后备：确保状态一致
    {
        std::lock_guard<std::mutex> lock(m_state->mtx);
        m_state->isRunning.store(false, std::memory_order_release);
    }
}

_SF_THREADERROR_TEM
void _SF_THREADERROR waitForCompletion() {
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

_SF_THREADERROR_TEM
bool _SF_THREADERROR isRunning() const {
    return m_state->isRunning.load(std::memory_order_acquire);
}

_SF_THREADERROR_TEM
std::future<void> _SF_THREADERROR getFuture() {
    if constexpr (!Async) {
        throw std::logic_error("getFuture is only available in async mode");
    }

    std::lock_guard<std::mutex> lock(m_state->mtx);
    if (!m_state->future.valid()) {
        throw std::logic_error("no valid completion future");
    }
    return std::move(m_state->future);
}

_SF_THREADERROR_TEM
void _SF_THREADERROR threadFunc(
    std::shared_ptr<State> state,
    std::stop_token st) {
    struct IsRunningGuard {
        std::atomic<bool>* flag;
        ~IsRunningGuard() noexcept {
            flag->store(false, std::memory_order_release);
        }
    } runningGuard{ &state->isRunning };   //未加锁使用std::shared_ptr访问m_state，是否安全

    try {
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
                // 忽略 set_value 可能抛出的 std::future_error   //是否需要选择忽略，而非爆出
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
            std::lock_guard<std::mutex> lock(state->mtx);   //是否需要和上部代码合并
            try {
                state->prom.set_exception(ep);
            }
            catch (...) {
                // 忽略 set_exception 可能抛出的 std::future_error   //是否需要选择忽略，而非爆出
            }
        }
    }
}

_SF_ERR_END