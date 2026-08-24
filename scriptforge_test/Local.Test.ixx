module;
#include "Scriptforge.Pch.hpp"

#include <gtest/gtest.h>

export module Local.Test;

import Scriptforge.Err;
import Scriptforge.LanguageCode;
import Scriptforge.Local;
import Scriptforge.ErrCode;

namespace fs = std::filesystem;
using namespace Scriptforge::Code;

namespace Scriptforge::Local::Test {

    TEST(LocalTest, AutoLoadAllLanguages) {
        // 👉 直接返回 vector<Language>
        auto langs = getAvailableLanguages("./lang");
        ASSERT_FALSE(langs.empty()) << "未找到语言文件！";

        for (auto lang : langs) {
            SCOPED_TRACE(("Test language: " +
                ENUM_TO_ISO639_1().at(lang)).c_str());

            Lang loc(lang, "./lang");

            EXPECT_TRUE(loc.isLoaded());
            EXPECT_EQ(loc.getLanguageCode(), lang);
        }
    }
}