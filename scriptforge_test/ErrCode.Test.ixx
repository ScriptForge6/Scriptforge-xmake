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
 * @file ErrCode.ixx
 * @brief 这是一个GTest测试文件，用于测试Scriptforge项目中的错误代码模块。该文件包含了一个简单的测试用例，验证了错误代码模块的基本功能。通过运行这个测试，可以确保错误代码模块在项目中正常工作，并且能够正确处理和传递错误信息。
 * @author Scriptforge
 * @date 2026/4/11
*/

module;

#include<gtest/gtest.h>
#include "Scriptforge.Pch.hpp"

export module ErrCode.Test;

import Scriptforge.LanguageCode;
import Scriptforge.Local;
import Scriptforge.Msg;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
using namespace Scriptforge::ErrCode;


namespace Scriptforge::ErrCode::Test {
    TEST(ErrCodeTest, ToStringValidCodes) {

        // 测试 Tree 错误代码
        EXPECT_EQ(to_string(ErrCode::TreeInvalidNode), "Tree1");
        EXPECT_EQ(to_string(ErrCode::TreeEmptyNode), "Tree2");
        EXPECT_EQ(to_string(ErrCode::TreeOrphanedNode), "Tree3");

        // 测试 Log 错误代码
        EXPECT_EQ(to_string(ErrCode::LogCannotOpenLogFile), "Log1");
        // 测试 BitPack 错误代码
        EXPECT_EQ(to_string(ErrCode::AntiDebugOSNotSupported), "AntiDebug1");

        // 测试 Local 错误代码
        EXPECT_EQ(to_string(ErrCode::LocalLanguageFileNotFound), "Local1");
        EXPECT_EQ(to_string(ErrCode::LocalInvalidLanguageFile), "Local2");
        EXPECT_EQ(to_string(ErrCode::LocalInvalidLanguageCode), "Local3");

        // 测试 RingBuffer 错误代码
        EXPECT_EQ(to_string(ErrCode::RingBufferCapacityBeZero), "RingBuffer1");
    }

    TEST(ErrCodeTest, ToStringUnknownCode) {

        // 测试未知错误代码类型
        ErrCode unknownCode = static_cast<ErrCode>(99999);
        EXPECT_EQ(to_string(unknownCode), "Unknown error");
        // 测试边界情况
        ErrCode boundaryCode1 = static_cast<ErrCode>(0);
        EXPECT_EQ(to_string(boundaryCode1), "Err0");
    }

    TEST(ErrCodeTest, EnumValuesCorrect) {
        using namespace Scriptforge::ErrCode;

        // 验证枚举值是否正确
        EXPECT_EQ(static_cast<int>(ErrCode::TreeInvalidNode), 10001);
        EXPECT_EQ(static_cast<int>(ErrCode::TreeEmptyNode), 10002);
        EXPECT_EQ(static_cast<int>(ErrCode::TreeOrphanedNode), 10003);

        EXPECT_EQ(static_cast<int>(ErrCode::LogCannotOpenLogFile), 20001);

        EXPECT_EQ(static_cast<int>(ErrCode::AntiDebugOSNotSupported), 30001);

        EXPECT_EQ(static_cast<int>(ErrCode::LocalLanguageFileNotFound), 40001);
        EXPECT_EQ(static_cast<int>(ErrCode::LocalInvalidLanguageFile), 40002);
        EXPECT_EQ(static_cast<int>(ErrCode::LocalInvalidLanguageCode), 40003);

        EXPECT_EQ(static_cast<int>(ErrCode::RingBufferCapacityBeZero), 50001);
    }

    TEST(ErrCodeTest, OstreamPrint) {
        ErrCode code = ErrCode::TreeInvalidNode;
        std::cout << code;
    }
}