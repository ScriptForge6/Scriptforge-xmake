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
 * @file Scriptforge.ErrCode.ixx
 * @brief 定义了 `Scriptforge::ErrCode` 模块，其中包含了一个枚举类 `ErrCode`，用于表示不同类型的错误代码。该模块还提供了一个函数 `toString`，用于将错误代码转换为字符串表示，以及一个函数 `throwError`，用于抛出带有错误代码和相关信息的异常。通过使用这些定义，可以方便地处理和传递错误信息，提高代码的可读性和可维护性。
 * author Scriptforge
 * @data 2026/3/29
*/
module;

#include "Scriptforge.Pch.hpp"

export module Scriptforge.ErrCode;
import Scriptforge.Err;
import Scriptforge.LanguageCode;


namespace Scriptforge::ErrCode {
	export enum class ErrCode {
		TreeInvalidNode = 10001,      // Tree0001: Invalid node
		TreeEmptyNode = 10002,      // Tree0002: Empty node
		TreeOrphanedNode = 10003,      // Tree0003: Orphaned node
		LogCannotOpenLogFile = 20001,       // Log0001: Cannot open log file
		LogIsNotRunning = 20002,       // Log0002: Logger is not running
		AntiDebugOSNotSupported = 30001,       // AntiDebug0001: Operating system not supported
		LocalLanguageFileNotFound = 40001,    // Local0001: Language file not found
		LocalInvalidLanguageFile = 40002,     // Local0002: Invalid language file
		LocalInvalidLanguageCode = 40003,     // Local0003: Invalid language code
		RingBufferCapacityBeZero = 50001,       // RingBuffer0001: Capacity must be greater than 0
		ProcessPoolInvalidPoolSize = 60001,       // ProcessPool0002: Invalid pool size
		ThreadErrorThreadAlreadyRunning = 70001,       // ThreadError0001: Thread is already running
		ThreadErrorThreadNoTask = 70002,       // ThreadError0002: Thread is not running

	};
	export std::string to_string(ErrCode code) {
		std::string result;
		int codeType = static_cast<int>(code) / 10000;
		switch (codeType) {
		case 0:
			result += "Err";
			break;
		case 1:
			result += "Tree";
			break;
		case 2:
			result += "Log";
			break;
		case 3:
			result += "AntiDebug";
			break;
		case 4:
			result += "Local";
			break;
		case 5:
			result += "RingBuffer";
			break;
		case 6:
			result += "ProcessPool";
			break;
		case 7:
			result += "ThreadError";
			break;
		default:
			return "Unknown error";
		}
		result += std::to_string(static_cast<int>(code) % 10000);
		return result;
	}

	export std::ostream& operator<<(std::ostream& os, const ErrCode& errCode) {
		os << to_string(errCode);
		return os;
	}
}
export
template<>
struct std::formatter<Scriptforge::ErrCode::ErrCode>
{
	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.begin();
	}
	template<typename FmtCtx>
	auto format(const Scriptforge::ErrCode::ErrCode& code, FmtCtx& ctx) const {
		return std::format_to(ctx.out(), "{}", Scriptforge::ErrCode::to_string(code));
	}
};