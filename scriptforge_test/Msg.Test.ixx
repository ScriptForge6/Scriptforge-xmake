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
 * @file Msg.Test.ixx
 * @brief 这是一个GTest测试文件，用于测试Scriptforge项目中的消息模块。该文件包含了一个简单的测试用例，验证了消息模块的基本功能。通过运行这个测试，可以确保消息模块在项目中正常工作，并且能够正确处理和显示消息。
 * @author Scriptforge
 * @date 2026/5/3
 */

module;

#include<gtest/gtest.h>
#include "Scriptforge.Pch.hpp"

export module Msg.Test;

import Scriptforge.Msg;

namespace Scriptforge::Msg::Test {
    enum class TestMsgEnum {
        Code1,
        Code2
	};
	TEST(MsgTest, InformationLevel) {
		EXPECT_EQ(static_cast<int>(InformationLevel::Info), 1);
		EXPECT_EQ(static_cast<int>(InformationLevel::Warning), 2);
		EXPECT_EQ(static_cast<int>(InformationLevel::Error), 3);
	}

    TEST(MsgTest, NormalMsg) {
        Scriptforge::Msg::Message msg{ "test" };
        EXPECT_EQ(msg.message(), "test");
    }

    TEST(MsgTest, TemplateMsg) {
    Scriptforge::Msg::BasicMessage<TestMsgEnum> msg{ TestMsgEnum::Code1, Scriptforge::Msg::InformationLevel::Warning };
    EXPECT_EQ(msg.message(), TestMsgEnum::Code1);
    EXPECT_EQ(msg.level(), Scriptforge::Msg::InformationLevel::Warning);
    }

    TEST(MsgTest, MoveCtorWorks) {
        Scriptforge::Msg::Message msg { "test" };
        EXPECT_EQ(msg.message(), "test");
    }

    TEST(MsgTest, OstreamOperatorOutputFormatIsCorrect) {
        std::string test_msg = "test";

        auto tp = std::chrono::system_clock::time_point(std::chrono::seconds(1743264000));
        Scriptforge::Msg::Message msg(
            test_msg,
            Scriptforge::Msg::InformationLevel::Error,
            tp
        );

        std::ostringstream oss;
        oss << msg;
        std::string output = oss.str();

        // 断言包含关键内容
        EXPECT_NE(output.find("2025-03-30"), std::string::npos);
        EXPECT_NE(output.find("Error"), std::string::npos);
        EXPECT_NE(output.find(test_msg), std::string::npos);

        // 断言前缀格式完全匹配
        std::string expected_prefix = "[2025-03-30 00:00:00] [Error] test";
        EXPECT_EQ(output.substr(0, expected_prefix.size()), expected_prefix);
    }

    TEST(MsgTest, OstreamWithRvalueWorks) {
        Scriptforge::Msg::Message msg("test", Scriptforge::Msg::InformationLevel::Warning);

        std::ostringstream oss;
        oss << msg;
        std::string output = oss.str();

        EXPECT_TRUE(output.find("test") != std::string::npos);
        EXPECT_TRUE(output.find("Warning") != std::string::npos);
    }
}