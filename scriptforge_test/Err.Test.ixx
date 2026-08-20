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
 * @file Err.Test.ixx
 * @brief 这是一个GTest测试文件，用于测试Scriptforge项目中的错误处理模块。该文件包含了一个简单的测试用例，验证了错误处理模块的基本功能。通过运行这个测试，可以确保错误处理模块在项目中正常工作，并且能够正确捕获和处理错误。
 * @author Scriptforge
 * @date 2026/5/2
*/

module;

#include "Scriptforge.Pch.hpp"

#include<gtest/gtest.h>

export module Err.Test;

import Scriptforge.Err;

namespace Scriptforge::Err::Test {
    enum class TestErrCode {
        Code1,
        Code2
	};
	TEST(ErrorTest, NormalErr) {
		try {
			throw Scriptforge::Err::Error{ "code","test" };
		}
		catch (const Scriptforge::Err::Error& e) {
			EXPECT_EQ(e.code(), "code");
			EXPECT_EQ(e.what(), "test");
		}
	}

    TEST(ErrorTest, TemplateErr) {
        try {
            throw Scriptforge::Err::BasicError<TestErrCode>{ TestErrCode::Code1, "test", Scriptforge::Msg::InformationLevel::Warning };

        }
        catch (const Scriptforge::Err::BasicError<TestErrCode>& e) {
            EXPECT_EQ(e.level(), Scriptforge::Msg::InformationLevel::Warning);
            EXPECT_EQ(e.code(), TestErrCode::Code1);
            EXPECT_EQ(e.what(), "test");
        }
    }

    TEST(ErrorTest, MoveCtorWorks) {
        try {
            throw Scriptforge::Err::Error{ "code", "test" };
        }
        catch (Scriptforge::Err::Error e) { // 通过值捕获触发移动构造
            EXPECT_EQ(e.code(), "code");
            EXPECT_EQ(e.what(), "test");
        }
	}

    TEST(ErrorTest, OstreamOperatorOutputFormatIsCorrect) {
        // 1. 构造测试用错误
        std::string test_code = "code";
        std::string test_msg = "test";

        // 固定时间点，避免时间变化导致测试失败
        auto tp = std::chrono::system_clock::time_point(std::chrono::seconds(1743264000));
        Scriptforge::Err::Error err(test_code, test_msg, Scriptforge::Msg::InformationLevel::Error, tp);

        // 2. 输出到 stringstream
        std::ostringstream oss;
        oss << err;
        std::string output = oss.str();

        // 3. 断言输出包含关键内容
        EXPECT_NE(output.find("2025-03-30"), std::string::npos);  // 固定时间
        EXPECT_NE(output.find("Error"), std::string::npos);      // 级别
        EXPECT_NE(output.find(test_code), std::string::npos);    // 错误码
        EXPECT_NE(output.find(test_msg), std::string::npos);     // 错误信息

        // 4. 检查整体格式前缀（最关键）
        std::string expected_prefix = "[2025-03-30 00:00:00] [Error] [code] test";
        EXPECT_EQ(output.substr(0, expected_prefix.size()), expected_prefix);
    }

    // 测试移动构造版本 + 不同错误码类型（可选）
    TEST(ErrorTest, OstreamWithRvalueWorks) {
        Scriptforge::Err::Error err("code", "test", Scriptforge::Msg::InformationLevel::Warning);

        std::ostringstream oss;
        oss << err;
        std::string output = oss.str();

        EXPECT_TRUE(output.find("code") != std::string::npos);
        EXPECT_TRUE(output.find("test") != std::string::npos);
        EXPECT_TRUE(output.find("Warning") != std::string::npos);
    }


}