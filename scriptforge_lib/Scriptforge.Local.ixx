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
 * @file Scriptforge.Local.ixx
 * @brief 定义了 `Scriptforge::Local` 模块，其中包含了一个 `Lang` 类，用于处理与语言相关的本地化功能。该类包含一个构造函数，接受一个语言枚举值和一个文件路径参数，用于指定语言文件的位置。
 * @author Scriptforge
 * @date 2026/3/29
 */
module;

#include "Scriptforge.Pch.hpp"

#include <nlohmann/json.hpp>

export module Scriptforge.Local;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.LanguageCode;
import Scriptforge.StringConversion;

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace Scriptforge::StringConversion;

namespace Scriptforge {
    inline namespace Local {

        [[noreturn]] void throwErrWithoutJson(Scriptforge::ErrCode::ErrCode code, std::string_view func, std::string_view message);
        [[noreturn]] void throwFileNotFoundError(const fs::path& p, Scriptforge::LanguageCode::Language lang, std::string_view func);
        [[noreturn]] void throwInvalidFileError(const fs::path& p, Scriptforge::LanguageCode::Language lang, std::string_view func);


        export class Lang {
        public:
            Lang(Scriptforge::LanguageCode::Language lang = Scriptforge::LanguageCode::Language::Neutral, fs::path p = { "./lang" });
            void reload();
            void setLocale(const Scriptforge::LanguageCode::Language lang);
            Scriptforge::LanguageCode::Language getLanguageCode() const;
            std::string getLanguageNameISO639_1() const;
            std::string getLanguageName() const;
            fs::path getLangPath() const;
            void setLangPath(const fs::path& path);
            std::string at(std::string_view key) const;
            const json atJ(std::string_view key) const;
            std::string value(std::string_view key, std::string_view defaultValue) const;
            bool has(std::string_view key) const;
            std::vector<std::string> getKeys() const;
            const json& getJson() const;
            bool isLoaded() const;
            json operator[](std::string_view key);
        private:
            void loadLanguageFile(Scriptforge::LanguageCode::Language lang, fs::path path);
            void LanguageIsLegal(Scriptforge::LanguageCode::Language lang) const;
            void pathIsLegal(const fs::path& path) const;
            Scriptforge::LanguageCode::Language m_lang;
            json j;
            fs::path m_path;
        };

        export std::vector<Scriptforge::LanguageCode::Language> getAvailableLanguages(const fs::path& path = { "./lang" });

    }
}

namespace Scriptforge {
    inline namespace Local {

        [[noreturn]] void throwErrWithoutJson(Scriptforge::ErrCode::ErrCode code, std::string_view func, std::string_view message) {
            throw Scriptforge::BasicError<Scriptforge::ErrCode::ErrCode>{ code, std::string(func) + ": " + std::string(message) };
        }
        [[noreturn]] void throwFileNotFoundError(const fs::path& p, Scriptforge::LanguageCode::Language lang, std::string_view func) {
            if (lang == Scriptforge::LanguageCode::Language::Chinese) {
                throwErrWithoutJson(Scriptforge::ErrCode::ErrCode::LocalLanguageFileNotFound, func, "指定的语言文件不存在: " + p.string());
            }
            else {
                throwErrWithoutJson(Scriptforge::ErrCode::ErrCode::LocalLanguageFileNotFound, func, "The specified language file does not exist: " + p.string());
            }
        }
        [[noreturn]] void throwInvalidFileError(const fs::path& p, Scriptforge::LanguageCode::Language lang, std::string_view func) {
            if (lang == Scriptforge::LanguageCode::Language::Chinese) {
                throwErrWithoutJson(Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageFile, func, "指定的语言文件无效: " + p.string());
            }
            else {
                throwErrWithoutJson(Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageFile, func, "The specified language file is invalid: " + p.string());
            }
        }


        Lang::Lang(Scriptforge::LanguageCode::Language lang, fs::path p) {
            loadLanguageFile(lang, p);
        }

        void Lang::reload() {
            loadLanguageFile(m_lang, m_path);
        }


        void Lang::setLocale(const Scriptforge::LanguageCode::Language lang) {
            m_lang = lang;
            loadLanguageFile(lang, m_path);
        }


        Scriptforge::LanguageCode::Language Lang::getLanguageCode() const {
            return m_lang;
        }

        std::string Lang::getLanguageNameISO639_1() const {
            if (m_lang == Scriptforge::LanguageCode::Language::Neutral) {
                return Scriptforge::LanguageCode::ENUM_TO_ISO639_1().at(Scriptforge::LanguageCode::neutral);
            }
            return Scriptforge::LanguageCode::ENUM_TO_ISO639_1().at(m_lang);
        }
        
        std::string Lang::getLanguageName() const {
            return j.value("language_name", Scriptforge::LanguageCode::ENUM_TO_ISO639_1().at(m_lang));
        }


        fs::path Lang::getLangPath() const {
            return m_path;
        }
        void Lang::setLangPath(const fs::path& path) {
            m_path = path;
            loadLanguageFile(m_lang, path);
        }


        std::string Lang::at(std::string_view key) const {
            return j.at(key);
        }

        const json Lang::atJ(std::string_view key) const {
            return j.at(key);
        }

        std::string Lang::value(std::string_view key, std::string_view defaultValue) const {
            return std::string(j.value(key, defaultValue));
        }


        bool Lang::has(std::string_view key) const {
            return j.contains(key);
        }


        std::vector<std::string> Lang::getKeys() const {
            const auto& obj = j.get_ref<const nlohmann::json::object_t&>();

            std::vector<std::string> keys;
            keys.reserve(obj.size());

            for (const auto& kv : obj) {
                keys.emplace_back(kv.first);
            }

            return keys;
        }


        const json& Lang::getJson() const {
            return j;
        }

        bool Lang::isLoaded() const {
            return !j.empty();
        }


        json Lang::operator[](std::string_view key) {
            return json(j[key]);
        }



        void Lang::loadLanguageFile(Scriptforge::LanguageCode::Language lang, fs::path path) {

            LanguageIsLegal(lang);
            m_lang = lang;

            auto langCode = getLanguageNameISO639_1();
            fs::path filename = path / (langCode + ".json");

            pathIsLegal(filename);

            m_path = path;

            std::ifstream jsoninput(filename.string());

            if (!jsoninput.is_open()) {
                throwFileNotFoundError(filename, lang, __func__);
            }

            try {
                jsoninput >> j;
            }
            catch (const json::parse_error&) {
                throwInvalidFileError(filename, lang, __func__);
            }
            jsoninput.close();
        }

        void Lang::LanguageIsLegal(Scriptforge::LanguageCode::Language lang) const {
            if (lang == Scriptforge::LanguageCode::Language::Invariant) {
                throwErrWithoutJson(Scriptforge::ErrCode::ErrCode::LocalInvalidLanguageCode, __func__, "Invariant is not valid language codes for loading language files.");
            }
        }

        void Lang::pathIsLegal(const fs::path& filename) const {
            if (!fs::exists(filename)) {
                throwFileNotFoundError(filename, m_lang, __func__);
            }
            if (!fs::is_regular_file(filename)) {
                throwInvalidFileError(filename, m_lang, __func__);
            }
        }


        std::vector<LanguageCode::Language> getAvailableLanguages(const fs::path& path) {

            if (!fs::exists(path)) {
                throwFileNotFoundError(path, Scriptforge::LanguageCode::Language::Neutral, __func__);
            }

            std::vector<Scriptforge::LanguageCode::Language> languages;

            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.path().extension() != ".json")
                    continue;

                // 获取文件名（例如 zh、en）
                std::string iso = entry.path().stem().string();

                // 自动转成 Language 枚举
                auto it = Scriptforge::LanguageCode::ISO639_1_TO_ENUM.find(iso);
                if (it != Scriptforge::LanguageCode::ISO639_1_TO_ENUM.end()) {
                    languages.push_back(it->second);
                }
            }

            return languages;
        }
    }
}