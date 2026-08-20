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
 * @file Scriptforge.ErrCode.throwError.ixx
 * @brief 定义了 `Scriptforge::ErrCode` 模块，其中包含一个throwError函数的实现，以保证不出现循环依赖。该函数接受一个错误代码、函数名称、语言对象和参数列表，并根据提供的信息构建错误消息，然后抛出一个包含错误代码和消息的异常。通过使用该函数，可以方便地处理和传递错误信息，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/3/29
 */
module;

#include "Scriptforge.Pch.hpp"

export module Scriptforge.ErrCode.throwError;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.Msg;
import Scriptforge.LanguageCode;
import Scriptforge.Local;

namespace Scriptforge::ErrCode {
    export
        [[noreturn]] void throwError(
            ErrCode code,
            std::string_view func,
            const Scriptforge::Local::Lang& lang,
            Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Error
        );
    export
        template <typename... Args>
    [[noreturn]] void throwError(
        ErrCode code,
        const std::string& func,
        const Scriptforge::Local::Lang& lang,
        Scriptforge::Msg::InformationLevel level = Scriptforge::Msg::InformationLevel::Error,
        Args&&... args
    );
}

namespace Scriptforge::ErrCode {
    
    void throwError(
        ErrCode code,
        std::string_view func,
        const Scriptforge::Local::Lang& lang,
        Scriptforge::Msg::InformationLevel level
    ) {
        std::string baseStr = lang.atJ("SFError").at(std::to_string(int(code)));
        throw Scriptforge::Err::BasicError<ErrCode> {
            code,
            std::string(func) + ": " + baseStr,
            level
        };
    }

    template <typename... Args>
    void throwError(
        ErrCode code,
        const std::string& func,
        const Scriptforge::Local::Lang& lang,
        Scriptforge::Msg::InformationLevel level,
        Args&&... args
    ) {
        std::string baseStr = std::format(lang.atJ("SFError").at(std::to_string(int(code))), std::forward<Args>(args)...);
        throw Scriptforge::Err::BasicError<ErrCode> {
            code,
            std::string(func) + ": " + baseStr,
			level
		};
    }

}