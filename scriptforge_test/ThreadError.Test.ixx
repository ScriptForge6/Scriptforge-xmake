module;

#include "Scriptforge.Pch.hpp"

#include <gtest/gtest.h>

export module ThreadError.Test;


import Scriptforge.Local;
import Scriptforge.ThreadError;
import Scriptforge.ErrCode;

namespace fs = std::filesystem;
using namespace Scriptforge::Local;
using namespace Scriptforge::Err;

namespace Scriptforge::Err::Test {
    TEST(ThreadErrorTest, SyncStartAndCompletion) {
        auto langs = getAvailableLanguages("./lang");
        ASSERT_FALSE(langs.empty()) << "未找到语言文件";

        Lang lang(langs.front(), "./lang");
        ThreadError<> th(lang);

        std::atomic<bool> ran{ false };
        th.setThreadFunction([&](std::stop_token) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            ran.store(true, std::memory_order_release);
            });

        th.start();
        EXPECT_TRUE(th.isRunning());
        th.waitForCompletion();
        EXPECT_FALSE(th.isRunning());
        EXPECT_TRUE(ran.load(std::memory_order_acquire));
    }

    TEST(ThreadErrorTest, StopRequestsStop) {
        auto langs = getAvailableLanguages("./lang");
        ASSERT_FALSE(langs.empty()) << "未找到语言文件";

        Lang lang(langs.front(), "./lang");
        ThreadError<> th(lang);

        std::atomic<bool> observedStop{ false };
        th.setThreadFunction([&](std::stop_token st) {
            while (!st.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            observedStop.store(true, std::memory_order_release);
            });

        th.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        th.stop();

        EXPECT_FALSE(th.isRunning());
        EXPECT_TRUE(observedStop.load(std::memory_order_acquire));
    }

    TEST(ThreadErrorTest, AsyncExceptionPropagatesViaFuture) {
        auto langs = getAvailableLanguages("./lang");
        ASSERT_FALSE(langs.empty()) << "未找到语言文件";

        Lang lang(langs.front(), "./lang");
        ThreadError<true> th(lang);

        th.setThreadFunction([&](std::stop_token) {
            throw std::runtime_error("boom");
            });

        th.start();
        auto fut = th.getFuture();
        EXPECT_THROW(fut.get(), std::runtime_error);
    }
}
