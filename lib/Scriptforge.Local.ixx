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

#include <nlohmann/json.hpp>

#include "Scriptforge.Pch.hpp"

export module Scriptforge.Local;

import Scriptforge.StringConversion;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.LanguageCode;
namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace Scriptforge::StringConversion;

namespace Scriptforge {
    inline namespace Local {

        export class JsonWithFormat : public nlohmann::json {
        public:
            using nlohmann::json::json;
            JsonWithFormat(const nlohmann::json& j);

            template<typename T, typename... Args>
                requires is_basic_string<T>
            T format(const T& key, Args&&... args) const;
            operator json() const;
        };

        export [[noreturn]] void throwErrWithoutJson(Scriptforge::ErrCode::ErrCode code, const std::string& func, const std::string& message); //edit:added "export "
        [[noreturn]] void throwFileNotFoundError(const fs::path& p, Scriptforge::LanguageCode::Language lang, const std::string& func);
        [[noreturn]] void throwInvalidFileError(const fs::path& p, Scriptforge::LanguageCode::Language lang, const std::string& func);

        export class Lang {
        public:
            Lang(Scriptforge::LanguageCode::Language lang = Scriptforge::LanguageCode::Language::Neutral, fs::path p = { "./lang" });
            void reload();
            void setLocale(const Scriptforge::LanguageCode::Language lang);
            Scriptforge::LanguageCode::Language getLanguageCode() const;
            template<typename T>
                requires is_basic_string<T>
            T getLanguageNameISO639_1() const;
            template<typename T>
                requires is_basic_string<T>
            T getLanguageName() const;
            fs::path getLangPath() const;
            void setLangPath(const fs::path& path);
            template<typename T>
                requires is_basic_string<T>
            T at(const T& key) const;
            template<typename T>
                requires is_basic_string<T>
            const JsonWithFormat atJ(const T& key) const;
            template<typename T>
                requires is_basic_string<T>
            T value(const T& key, const T& defaultValue) const;
            template<typename T, typename... Args>
                requires is_basic_string<T>
            T format(const T& key, Args&&... args) const;
            template<typename T>
                requires is_basic_string<T>
            bool has(const T& key) const;
            template<typename T>
                requires is_basic_string<T>
            std::vector<T> getKeys() const;
            const JsonWithFormat& getJson() const;
            bool isLoaded() const;
            template<typename T>
                requires is_basic_string<T>
            JsonWithFormat operator[](const T& key);
        private:
            void loadLanguageFile(Scriptforge::LanguageCode::Language lang, fs::path path);
            void LanguageIsLegal(Scriptforge::LanguageCode::Language lang) const;
            void pathIsLegal(const fs::path& path) const;
            Scriptforge::LanguageCode::Language m_lang;
            JsonWithFormat j;
            fs::path m_path;
        };

        export std::vector<Scriptforge::LanguageCode::Language> getAvailableLanguages(const fs::path& path = { "./lang" });

    }
}

namespace Scriptforge {
    inline namespace Local {

        JsonWithFormat::JsonWithFormat(const nlohmann::json& j)
            : nlohmann::json(j)
        {}
        template<typename T, typename... Args>
            requires is_basic_string<T>
        T JsonWithFormat::format(const T& key, Args&&... args) const {
            std::string fmt = value(key, key);

            std::string formatted = std::vformat(
                std::move(fmt),
                std::make_format_args(std::forward<Args>(args)...)
            );

            return str_convert<T, std::string>(formatted);
        }

        JsonWithFormat::operator json() const {
            return static_cast<json>(*this);
        }

        [[noreturn]] void throwErrWithoutJson(Scriptforge::ErrCode::ErrCode code, const std::string& func, const std::string& message) {
            throw Scriptforge::BasicError<Scriptforge::ErrCode::ErrCode>{ code, func + ": " + message };
        }
        [[noreturn]] void throwFileNotFoundError(const fs::path& p, Scriptforge::LanguageCode::Language lang, const std::string& func) {
            if (lang == Scriptforge::LanguageCode::Language::Chinese) {
                throwErrWithoutJson(Scriptforge::ErrCode::ErrCode::LocalLanguageFileNotFound, func, "指定的语言文件不存在: " + p.string());
            }
            else {
                throwErrWithoutJson(Scriptforge::ErrCode::ErrCode::LocalLanguageFileNotFound, func, "The specified language file does not exist: " + p.string());
            }
        }
        [[noreturn]] void throwInvalidFileError(const fs::path& p, Scriptforge::LanguageCode::Language lang, const std::string& func) {
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

        template<typename T>
            requires is_basic_string<T>
        T Lang::getLanguageNameISO639_1() const {
            return str_convert<T, std::string>(Scriptforge::LanguageCode::ENUM_TO_ISO639_1().at(m_lang));
        }

        template<typename T>
            requires is_basic_string<T>
        T Lang::getLanguageName() const {
            return str_convert<T>(j.value("language_name", Scriptforge::LanguageCode::ENUM_TO_ISO639_1().at(m_lang)));
        }


        fs::path Lang::getLangPath() const {
            return m_path;
        }
        void Lang::setLangPath(const fs::path& path) {
            m_path = path;
            loadLanguageFile(m_lang, path);
        }


        template<typename T>
            requires is_basic_string<T>
        T Lang::at(const T& key) const {
            return str_convert<T, std::string>(j.at(str_convert<std::string, T>(key)));
        }

        template<typename T>
            requires is_basic_string<T>
        const JsonWithFormat Lang::atJ(const T& key) const {
            return JsonWithFormat(j.at(str_convert<std::string>(key)));
        }

        template<typename T>
            requires is_basic_string<T>
        T Lang::value(const T& key, const T& defaultValue) const {
            return str_convert<T, std::string>(j.value(str_convert<std::string, T>(key), str_convert<std::string, T>(defaultValue)));
        }


        template<typename T, typename... Args>
            requires is_basic_string<T>
        T Lang::format(const T& key, Args&&... args) const {
            return j.format(key, std::forward<Args>(args)...);
        }


        template<typename T>
            requires is_basic_string<T>
        bool Lang::has(const T& key) const {
            return j.contains(str_convert<std::string>(key));
        }


        template<typename T>
            requires is_basic_string<T>
        std::vector<T> Lang::getKeys() const {
            const auto& obj = j.get_ref<const nlohmann::json::object_t&>();

            std::vector<T> keys;
            keys.reserve(obj.size());

            for (const auto& kv : obj) {
                keys.emplace_back(str_convert<T, std::string>(kv.first));
            }

            return keys;
        }


        const JsonWithFormat& Lang::getJson() const {
            return j;
        }

        bool Lang::isLoaded() const {
            return !j.empty();
        }


        template<typename T>
            requires is_basic_string<T>
        JsonWithFormat Lang::operator[](const T& key) {
            return JsonWithFormat(j[str_convert<std::string>(key)]);
        }



        void Lang::loadLanguageFile(Scriptforge::LanguageCode::Language lang, fs::path path) {

            LanguageIsLegal(lang);
            m_lang = lang;

            auto langCode = getLanguageNameISO639_1<std::string>();
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