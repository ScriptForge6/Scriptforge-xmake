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
 * @file Scriptforge.StringConversion.Test.ixx
 * @brief 定义了 `Scriptforge::StringConversion::Test` 模块，其中包含了一系列使用 Google Test 框架编写的单元测试，用于验证 `Scriptforge::StringConversion` 模块中字符串转换函数的正确性。这些测试涵盖了不同字符串类型之间的转换，包括 `std::string`、`std::u8string`、`std::u16string`、`std::u32string` 和 `std::wstring`，以及空字符串和相同类型转换的情况。
 * @author Scriptforge
 * @date 2026/4/28
 */

module;
#include <gtest/gtest.h>
#include "Scriptforge.Pch.hpp"

export module Scriptforge.StringConversion.Test;
import Scriptforge.StringConversion;

using namespace Scriptforge::StringConversion;

namespace Scriptforge::StringConversion::Test {
	TEST(StringConversionTest, canConvertStringToU8) {
		std::string source = "Hello UTF-8 世界";
		auto u8Str = str_convert<std::u8string>(source);
		auto back = str_convert<std::string>(u8Str);
		EXPECT_EQ(source, back);
	}

	TEST(StringConversionTest, canConvertStringToU16) {
		std::string source = "Test UTF-8 转 UTF-16";
		auto u16Str = str_convert<std::u16string>(source);
		auto back = str_convert<std::string>(u16Str);
		EXPECT_EQ(source, back);
	}

	TEST(StringConversionTest, canConvertStringToU32) {
		std::string source = "Hello 你好 🌍";
		auto u32Str = str_convert<std::u32string>(source);
		auto back = str_convert<std::string>(u32Str);
		EXPECT_EQ(source, back);
	}

	TEST(StringConversionTest, canConvertStringToWstring) {
		std::string source = "Wide string test 测试";
		auto wStr = str_convert<std::wstring>(source);
		auto back = str_convert<std::string>(wStr);
		EXPECT_EQ(source, back);
	}

	TEST(StringConversionTest, canConvertU8ToAllTypes) {
		std::u8string source = u8"U8 转全部类型测试";
		std::string sourceStr = str_convert<std::string>(source);

		auto toStr = str_convert<std::string>(source);
		auto toU16 = str_convert<std::u16string>(source);
		auto toU32 = str_convert<std::u32string>(source);
		auto toW = str_convert<std::wstring>(source);

		EXPECT_EQ(toStr, sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU16), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU32), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toW), sourceStr);
	}

	TEST(StringConversionTest, canConvertU16ToAllTypes) {
		std::u16string source = u"U16 转全部类型测试";
		std::string sourceStr = str_convert<std::string>(source);

		auto toStr = str_convert<std::string>(source);
		auto toU8 = str_convert<std::u8string>(source);
		auto toU32 = str_convert<std::u32string>(source);
		auto toW = str_convert<std::wstring>(source);

		EXPECT_EQ(toStr, sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU8), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU32), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toW), sourceStr);
	}

	TEST(StringConversionTest, canConvertU32ToAllTypes) {
		std::u32string source = U"U32 转全部类型测试";
		std::string sourceStr = str_convert<std::string>(source);

		auto toStr = str_convert<std::string>(source);
		auto toU8 = str_convert<std::u8string>(source);
		auto toU16 = str_convert<std::u16string>(source);
		auto toW = str_convert<std::wstring>(source);

		EXPECT_EQ(toStr, sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU8), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU16), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toW), sourceStr);
	}

	TEST(StringConversionTest, canConvertWstringToAllTypes) {
		std::wstring source = L"Wstring 转全部类型测试";
		std::string sourceStr = str_convert<std::string>(source);

		auto toStr = str_convert<std::string>(source);
		auto toU8 = str_convert<std::u8string>(source);
		auto toU16 = str_convert<std::u16string>(source);
		auto toU32 = str_convert<std::u32string>(source);

		EXPECT_EQ(toStr, sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU8), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU16), sourceStr);
		EXPECT_EQ(str_convert<std::string>(toU32), sourceStr);
	}

	TEST(StringConversionTest, sameTypeReturnsCopy) {
		std::string source = "Same type test";
		auto same = str_convert<std::string>(source);
		EXPECT_EQ(source, same);
	}

	TEST(StringConversionTest, emptyStringConvertsSafely) {
		std::string empty{};
		auto u8 = str_convert<std::u8string>(empty);
		auto u16 = str_convert<std::u16string>(empty);
		auto u32 = str_convert<std::u32string>(empty);
		auto w = str_convert<std::wstring>(empty);

		EXPECT_TRUE(u8.empty());
		EXPECT_TRUE(u16.empty());
		EXPECT_TRUE(u32.empty());
		EXPECT_TRUE(w.empty());
	}
}