// Copyright 2025-2026 Scriptforge
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
 * @file Scriptforge.Err.ixx
 * @brief 定义了 `Scriptforge::Err` 模块，其中包含了一个 `Error` 类，用于表示错误信息。该类提供了构造函数、成员函数以及一个友元函数，用于获取错误信息和错误代码，并支持将错误信息输出到流中。通过使用该类，可以方便地处理和传递错误信息，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/3/29
*/
module;

#include "Scriptforge.Pch.hpp"

export module Scriptforge.Err;
import Scriptforge.Msg;

namespace Scriptforge {
    inline namespace Err {
        template <typename CodeT, typename T, typename Clock>
        concept ErrorRequires = Scriptforge::Msg::MessageRequires<T, Clock>&&
            std::copyable<CodeT>;

        export 
			template<typename CodeT = std::string, typename T = std::string, typename Clock = std::chrono::system_clock>
			requires ErrorRequires<CodeT, T, Clock>
        class BasicError : public Scriptforge::Msg::BasicMessage<T, Clock> {
        public:
            BasicError(const CodeT& code = CodeT{}, const T& msg = T{}, Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Error, typename Clock::time_point tp = Clock::now());
			BasicError(CodeT&& code, T&& msg, Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Error, typename Clock::time_point tp = Clock::now());

			const T& what() const noexcept;
            const CodeT& code() const;

        private:
            CodeT m_code;
        };

        export using Error = BasicError<>;

		export
			template <typename CodeT, typename T, typename Clock>
			requires ErrorRequires<CodeT, T, Clock>&&
			requires { Clock::to_time_t(std::declval<typename Clock::time_point>()); }&&
			requires { !std::is_same_v<Clock, std::chrono::steady_clock>; }
		std::ostream& operator<<(std::ostream& os, const BasicError<CodeT, T, Clock>& error);

		export
			template <typename T, typename Clock>
			requires MessageRequires<T, Clock>&&
		std::same_as<Clock, std::chrono::steady_clock>
			std::ostream& operator<<(std::ostream& os, const BasicMessage<T, Clock>& msg);
    }
}

namespace Scriptforge {
    inline namespace Err {
		template<typename CodeT, typename T, typename Clock>
			requires ErrorRequires<CodeT, T, Clock>
        BasicError<CodeT, T, Clock>::BasicError(const CodeT& code, const T& msg, Scriptforge::Msg::InformationLevel level, typename Clock::time_point tp)
            : Scriptforge::Msg::BasicMessage<T, Clock>(msg, level, tp), m_code(code) {}

        template<typename CodeT, typename T, typename Clock>
            requires ErrorRequires<CodeT, T, Clock>
		BasicError<CodeT, T, Clock>::BasicError(CodeT&& code, T&& msg, Scriptforge::Msg::InformationLevel level, typename Clock::time_point tp)
            : Scriptforge::Msg::BasicMessage<T, Clock>(std::move(msg), level, tp), m_code(std::move(code)) {}

        template<typename CodeT, typename T, typename Clock>
            requires ErrorRequires<CodeT, T, Clock>
        const T& BasicError<CodeT, T, Clock>::what() const noexcept {
            return this->message();
		}

        template<typename CodeT, typename T, typename Clock>
			requires ErrorRequires<CodeT, T, Clock>
        const CodeT& BasicError<CodeT, T, Clock>::code() const {
            return m_code;
		}

		template <typename CodeT, typename T, typename Clock>
			requires ErrorRequires<CodeT, T, Clock>&&
			requires { Clock::to_time_t(std::declval<typename Clock::time_point>()); }&&
			requires { !std::is_same_v<Clock, std::chrono::steady_clock>; }
		std::ostream& operator<<(std::ostream& os, const BasicError<CodeT, T, Clock>& error) {
			// 拆分输出，不再把时区相关变量全部丢进std::format
			auto tp = error.time();
			auto zt = std::chrono::zoned_time{ std::chrono::current_zone(), tp };
			auto local = zt.get_local_time();
			auto days = floor<std::chrono::days>(local);
			std::chrono::year_month_day ymd{ days };
			std::chrono::hh_mm_ss      hms{ local - days };

			// 直接输出！安全！干净！
			os << std::format("[{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}] [{}] [{}] {}",
				static_cast<int>(ymd.year()),
				static_cast<unsigned>(ymd.month()),
				static_cast<unsigned>(ymd.day()),
				hms.hours().count(),
				hms.minutes().count(),
				hms.seconds().count(),
				getInformationLevel(error.level()),
				error.code(),
				error.message());
			return os;
		}

		template <typename T, typename Clock>
			requires MessageRequires<T, Clock>&&
		std::same_as<Clock, std::chrono::steady_clock>
			std::ostream& operator<<(std::ostream& os, const BasicMessage<T, Clock>& msg) {
			os << "[ " << msg.time().time_since_epoch().count()
				<< "] [" << getInformationLevel(msg.level())
				<< "] [" << msg.code()
				<< "] " << msg.message();
			return os;
		}
    }
}