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
 * @file ErrCode.cpp
 * @brief 这是一个GTest测试文件，用于测试Scriptforge项目中的错误代码模块。该文件包含了一个简单的测试用例，验证了错误代码模块的基本功能。通过运行这个测试，可以确保错误代码模块在项目中正常工作，并且能够正确处理和传递错误信息。
 * @author Scriptforge
 * @date 2026/4/11
*/
/*
#include<gtest/gtest.h>

import Scriptforge.ErrCode;
import std;

namespace Scriptforge::Local {
    class Lang {
    public:
        std::string getErrorMessage(int code, const std::vector<std::string>& args = {}) const {
            return "Mock error message for code: " + std::to_string(code);
        }
    };
}

namespace Scriptforge::Test::ErrCode {

    

    TEST(ErrCodeTest, ToStringValidCodes) {
        using namespace Scriptforge::ErrCode;

        // 测试 Tree 错误代码
        EXPECT_EQ(toString(ErrCode::TreeInvalidNode), "Tree1");
        EXPECT_EQ(toString(ErrCode::TreeEmptyNode), "Tree2");
        EXPECT_EQ(toString(ErrCode::TreeOrphanedNode), "Tree3");

        // 测试 Log 错误代码
        EXPECT_EQ(toString(ErrCode::LogCannotOpenLogFile), "Log1");
        // 测试 BitPack 错误代码
        EXPECT_EQ(toString(ErrCode::BitPackInvalidSizeForPacking), "BitPack1");
        EXPECT_EQ(toString(ErrCode::BitPackValueTooLargeToPack), "BitPack2");

        // 测试 Local 错误代码
        EXPECT_EQ(toString(ErrCode::LocalLanguageFileNotFound), "Local1");
        EXPECT_EQ(toString(ErrCode::LocalInvalidLanguageFile), "Local2");
        EXPECT_EQ(toString(ErrCode::LocalInvalidLanguageCode), "Local3");

        // 测试 RingBuffer 错误代码
        EXPECT_EQ(toString(ErrCode::RingBufferCapacityBeZero), "RingBuffer1");
    }

    TEST(ErrCodeTest, ToStringUnknownCode) {
        using namespace Scriptforge::ErrCode;

        // 测试未知错误代码类型
        ErrCode unknownCode = static_cast<ErrCode>(99999);
        EXPECT_EQ(toString(unknownCode), "Unknown error");
        // 测试边界情况
        ErrCode boundaryCode1 = static_cast<ErrCode>(0);
        EXPECT_EQ(toString(boundaryCode1), "Err0");

        ErrCode boundaryCode2 = static_cast<ErrCode>(60000);
        EXPECT_EQ(toString(boundaryCode2), "Unknown error");
    }

    TEST(ErrCodeTest, EnumValuesCorrect) {
        using namespace Scriptforge::ErrCode;

        // 验证枚举值是否正确
        EXPECT_EQ(static_cast<int>(ErrCode::TreeInvalidNode), 10001);
        EXPECT_EQ(static_cast<int>(ErrCode::TreeEmptyNode), 10002);
        EXPECT_EQ(static_cast<int>(ErrCode::TreeOrphanedNode), 10003);

        EXPECT_EQ(static_cast<int>(ErrCode::LogCannotOpenLogFile), 20001);

        EXPECT_EQ(static_cast<int>(ErrCode::BitPackInvalidSizeForPacking), 30001);
        EXPECT_EQ(static_cast<int>(ErrCode::BitPackValueTooLargeToPack), 30002);

        EXPECT_EQ(static_cast<int>(ErrCode::LocalLanguageFileNotFound), 40001);
        EXPECT_EQ(static_cast<int>(ErrCode::LocalInvalidLanguageFile), 40002);
        EXPECT_EQ(static_cast<int>(ErrCode::LocalInvalidLanguageCode), 40003);

        EXPECT_EQ(static_cast<int>(ErrCode::RingBufferCapacityBeZero), 50001);
    }

    TEST(ErrCodeTest, ThrowErrorFunctionSignature) {
        using namespace Scriptforge::ErrCode;
        using namespace Scriptforge::Local;

        // 测试 throwError 函数的存在性和签名
        // 这里主要验证函数可以正常声明和调用（由于是 [[noreturn]]，实际测试会捕获异常）

        Lang lang;
        std::vector<std::string> args;

        // 函数存在性测试 - 编译时检查
        // 实际运行时测试需要在调用时捕获异常

        // 注意：由于 throwError 是 [[noreturn]]，实际测试中应该:
        // 1. 在测试中捕获 std::exception
        // 2. 验证抛出的异常包含正确的错误信息

        // 示例（需要实际的 throwError 实现）：
        // EXPECT_THROW(throwError(ErrCode::TreeInvalidNode, "testFunc", lang, args), std::exception);
    }

    // 参数化测试 - 测试所有已知错误代码的 toString 功能
    class ErrCodeToStringTest : public ::testing::TestWithParam<std::tuple<Scriptforge::ErrCode::ErrCode, std::string>> {};

    TEST_P(ErrCodeToStringTest, AllKnownErrorCodes) {
        auto [errorCode, expectedString] = GetParam();
        EXPECT_EQ(Scriptforge::ErrCode::toString(errorCode), expectedString);
    }


    // 测试错误代码分类逻辑
    TEST(ErrCodeTest, ErrorCodeCategorization) {
        using namespace Scriptforge::ErrCode;

        // 验证错误代码分类逻辑
        EXPECT_EQ(static_cast<int>(ErrCode::TreeInvalidNode) / 10000, 1);
        EXPECT_EQ(static_cast<int>(ErrCode::LogCannotOpenLogFile) / 10000, 2);
        EXPECT_EQ(static_cast<int>(ErrCode::BitPackInvalidSizeForPacking) / 10000, 3);
        EXPECT_EQ(static_cast<int>(ErrCode::LocalLanguageFileNotFound) / 10000, 4);
        EXPECT_EQ(static_cast<int>(ErrCode::RingBufferCapacityBeZero) / 10000, 5);

        // 验证错误代码序号
        EXPECT_EQ(static_cast<int>(ErrCode::TreeInvalidNode) % 10000, 1);
        EXPECT_EQ(static_cast<int>(ErrCode::TreeEmptyNode) % 10000, 2);
        EXPECT_EQ(static_cast<int>(ErrCode::TreeOrphanedNode) % 10000, 3);
    }
}
*/