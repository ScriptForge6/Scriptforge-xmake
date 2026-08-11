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
 * @file AntiDeug.Test.ixx
 * @brief 这是一个GTest测试文件，用于测试Scriptforge项目中的反调试模块。该文件包含了一个简单的测试用例，验证了反调试模块的基本功能。通过运行这个测试，可以确保反调试模块在项目中正常工作，并且能够正确检测和处理调试器的存在。
 * @author Scriptforge
 * @date 2026/4/30
*/

module;

#include<gtest/gtest.h>
#include "Scriptforge.Pch.hpp"

export module AntiDebug.Test;

import Scriptforge.AntiDebug;
import Scriptforge.AntiDebug.RandomDefine;

namespace Scriptforge::AntiDebug::Test {
	class AntiDebugTestFalse : public Scriptforge::AntiDebug::AntiDebugger {
	private:
		virtual bool F4() noexcept override {
			return false; // 模拟没有调试器附加
		}
		virtual void F5() noexcept override {
			return; // 模拟不执行任何操作
		}
	};
	class AntiDebugTestTrue : public Scriptforge::AntiDebug::AntiDebugger {
	private:
		virtual bool F4() noexcept override {
			return true; // 模拟有调试器附加
		}
		virtual void F5() noexcept override {
			return; // 模拟不执行任何操作
		}
	};
	TEST(AntiDebugTest, AntiDebuggerFalse) {
		AntiDebugTestFalse ad;
		ad.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_FALSE(ad.isDebuggerPresent()); // 期望没有调试器附加
		ad.stop();
	}
	TEST(AntiDebugTest, AntiDebuggerTrue) {
		AntiDebugTestTrue ad;
		ad.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_TRUE(ad.isDebuggerPresent()); // 期望有调试器附加
		ad.stop();
	}
}