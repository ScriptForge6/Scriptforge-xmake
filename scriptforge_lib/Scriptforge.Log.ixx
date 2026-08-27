SF// Copyright 2025-2026 Scriptforge
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
 * @file Scriptforge.Log.ixx
 * @brief 定义了 Scriptforge::Log 模块，包含线程安全异步 Logger
 * @author Scriptforge
 * @date 2026/3/29
 */
/*
export module Scriptforge.Log;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.ThreadError;
import Scriptforge.Msg;
import Scriptforge.Local;
import Scriptforge.LanguageCode;
import Scriptforge.Pch;

namespace Scriptforge {
    inline namespace Log {
        namespace fs = std::filesystem;

        export
            template <typename T, typename Clock>
        concept LoggerRequires = Scriptforge::Msg::MessageRequires<T, Clock>;

        export
            template <typename T = std::string, typename Clock = std::chrono::system_clock>
            requires LoggerRequires<T, Clock>
        class Logger {
        public:
            Logger(const fs::path& file, const Scriptforge::Local::Lang& lang,
                Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Info);
            Logger(fs::path&& file, const Scriptforge::Local::Lang& lang,
                Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Info);
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;
            ~Logger();

            void start();
            void stop();

            void setLogLevel(Scriptforge::Msg::InformationLevel level);
            Scriptforge::Msg::InformationLevel getLogLevel() const;

            void log(const Scriptforge::BasicMessage<T, Clock>& msg);
            fs::path getPath() const;

        private:
            void process();
            void fileIsLegal(const fs::path& file) const;
            void writeMessageToFile(const Scriptforge::BasicMessage<T, Clock>& msg);

        private:
            fs::path m_path;
            Scriptforge::Local::Lang m_lang;
            Scriptforge::Msg::InformationLevel m_level;
            mutable std::mutex m_mtx;
            std::condition_variable m_cv;
            std::thread m_thread;
            bool m_isRunning = false;
            bool m_stopFlag = true;
            std::queue<Scriptforge::BasicMessage<T, Clock>> m_logQueue;
        };

    }
}

namespace Scriptforge {
	inline namespace Log {

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        Logger<T, Clock>::Logger(const fs::path& file, const Scriptforge::Local::Lang& lang,
            Scriptforge::Msg::InformationLevel level)
            : m_path(file), m_lang(lang), m_level(level) {
            fileIsLegal(file);
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        Logger<T, Clock>::Logger(fs::path&& file, const Scriptforge::Local::Lang& lang,
            Scriptforge::Msg::InformationLevel level)
            : m_path(std::move(file)), m_lang(lang), m_level(level) {
            fileIsLegal(m_path);
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        Logger<T, Clock>::~Logger() {
            stop();
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        void Logger<T, Clock>::start() {
            std::lock_guard lock(m_mtx);
            if (m_isRunning) return;

            m_stopFlag = false;
            m_isRunning = true;
            m_thread = std::thread(&Logger::process, this);
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        void Logger<T, Clock>::stop() {
            std::unique_lock lock(m_mtx);
            if (!m_isRunning) return;

            m_stopFlag = true;
            lock.unlock();
            m_cv.notify_all();

            if (m_thread.joinable())
                m_thread.join();
            m_isRunning = false;
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        void Logger<T, Clock>::setLogLevel(Scriptforge::Msg::InformationLevel level) {
            std::lock_guard lock(m_mtx);
            m_level = level;
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        Scriptforge::Msg::InformationLevel Logger<T, Clock>::getLogLevel() const {
            std::lock_guard lock(m_mtx);
            return m_level;
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        void Logger<T, Clock>::log(const Scriptforge::BasicMessage<T, Clock>& msg) {
            std::unique_lock lock(m_mtx);
            if (msg.getLevel() < m_level)
                return;

            if (!m_isRunning) {
                Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::LogIsNotRunning, __func__, m_lang);
            }

            m_logQueue.push(msg);
            lock.unlock();
            m_cv.notify_one();
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        fs::path Logger<T, Clock>::getPath() const {
            std::lock_guard lock(m_mtx);
            return m_path;
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        void Logger<T, Clock>::process() {
            try {
                while (true) {
                    std::unique_lock lock(m_mtx);
                    // 带超时等待，避免永久阻塞
                    if (!m_cv.wait_for(lock, std::chrono::milliseconds(100),
                        [this] { return m_stopFlag || !m_logQueue.empty(); })) {
                        continue;
                    }

                    if (m_stopFlag && m_logQueue.empty())
                        break;

                    // ✅ 批量取出消息，减少锁竞争
                    std::queue<Scriptforge::BasicMessage<T, Clock>> tempQueue;
                    tempQueue.swap(m_logQueue);

                    lock.unlock();

                    // ✅ 无锁批量写入
                    while (!tempQueue.empty()) {
                        auto msg = std::move(tempQueue.front());
                        tempQueue.pop();
                        writeMessageToFile(msg);
                    }
                }

                // 退出前清空剩余日志
                std::lock_guard lock(m_mtx);
                while (!m_logQueue.empty()) {
                    writeMessageToFile(m_logQueue.front());
                    m_logQueue.pop();
                }
            }
            catch (...) {
                // 记录内部错误，不向外抛
            }

            std::lock_guard lock(m_mtx);
            m_isRunning = false;
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        void Logger<T, Clock>::writeMessageToFile(const Scriptforge::BasicMessage<T, Clock>& msg) {
            std::ofstream ofs(m_path, std::ios::app);
            if (!ofs) {
                Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile, __func__, m_lang);
            }
            ofs << msg << '\n';
            ofs.flush();
        }

        template <typename T, typename Clock>
            requires LoggerRequires<T, Clock>
        void Logger<T, Clock>::fileIsLegal(const fs::path& file) const {
            if (fs::exists(file)) {
                if (!fs::is_regular_file(file)) {
                    Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile, __func__, m_lang);
                }
            }
            else {
                if (file.has_parent_path()) {
                    std::error_code ec;
                    fs::create_directories(file.parent_path(), ec);
                    if (ec) {
                        Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile, __func__, m_lang);
                    }
                }
                std::ofstream test(file);
                if (!test) {
                    Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::LogCannotOpenLogFile, __func__, m_lang);
                }
            }
        }

    } // namespace Log
} // namespace Scriptforge
*/
module;

#include "Scriptforge.Pch.hpp"

export module Scriptforge.Log;
import Scriptforge.Local;
import Scriptforge.Msg;
import Scriptforge.ThreadError;

namespace Scriptforge {
    inline namespace Log {
        namespace fs = std::filesystem;

        export
            template <typename T, typename Clock>
        concept LoggerRequires = Scriptforge::Msg::MessageRequires<T, Clock>;

        export
            template <typename T = std::string, typename Clock = std::chrono::system_clock>
            requires LoggerRequires<T, Clock>
        class Logger {
        public:
            Logger(const fs::path& file, const Scriptforge::Local::Lang& lang,
                Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Info);
            Logger(fs::path&& file, const Scriptforge::Local::Lang& lang,
                Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Info);
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;
            ~Logger();

            void start();
            void stop();

            void setLogLevel(Scriptforge::Msg::InformationLevel level);
            Scriptforge::Msg::InformationLevel getLogLevel() const;

            void log(const Scriptforge::BasicMessage<T, Clock>& msg);
            fs::path getPath() const;

        private:
            void process();
            void fileIsLegal(const fs::path& file) const;
            void writeMessageToFile(const Scriptforge::BasicMessage<T, Clock>& msg);

        private:
            fs::path m_path;
            Scriptforge::Local::Lang m_lang;
            Scriptforge::Msg::InformationLevel m_level;
            mutable std::mutex m_mtx;
            std::condition_variable m_cv;
            std::thread m_thread;
            bool m_isRunning = false;
            bool m_stopFlag = true;
            std::queue<Scriptforge::BasicMessage<T, Clock>> m_logQueue;
        };

    }
}