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
 * @file Scriptforge.Base64.ixx
 * @brief 实现了Base64和Base64url的编解码
 * @author Scriptforge
 * @date 2026/8/21
*/
module;

#include "Scriptforge.Pch.hpp"

#define BASICBASE64_API(x)       \
template <isBaseCharSet CharSet> \
x BasicBase64<CharSet>

export module Scriptforge.Base64;

import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Local;

namespace Scriptforge {
	inline namespace Base {
        namespace CharSet {
            struct Base64Set {
                inline static std::array<char, 65> set = []() {
                    std::array<char, 65> m;
                    for (size_t i = 0; i <= 25; i++) {
                        m[i] = 'A' + i;
                    }
                    for (size_t i = 26; i <= 51; i++) {
                        m[i] = 'a' + i - 26;
                    }
                    for (size_t i = 52; i <= 62; i++) {
                        m[i] = '0' + i - 52;
                    }
                    m[62] = '+';
                    m[63] = '/';
                    m[64] = '=';
                    return m;
                    }();
            };
        }
        /**
         * @details 一个用于检查类型是否为有效的 Base 字符表的概念。
         * 可以编写以下类型的结构体/类：
         * ```cpp
         * struct Base64Set {
         *     inline static std::array<char, 64> set;
         * };
         * ```
         */
		template <typename T>
        concept isBaseCharSet = requires {
            { T::set } ->std::convertible_to<typename std::array<char, 65>>;
        };

        export 
            template <isBaseCharSet CharSet>
        class BasicBase64 {
        public:
            BasicBase64() = default;
            BasicBase64(const BasicBase64&) = default;
            BasicBase64(BasicBase64&&) = default;
            BasicBase64(std::string_view base, const Scriptforge::Local::Lang& lang);
            BasicBase64(const std::vector<unsigned char>& raw, const Scriptforge::Local::Lang& lang);
            void set(std::string_view base);
            void set(std::vector<unsigned char> raw);
            void setLang(const Scriptforge::Local::Lang& lang);
            std::string encode() const;
            std::vector<unsigned char> decode() const;
        private:
            std::string m_base;
            std::vector<unsigned char> m_raw;
            Scriptforge::Local::Lang m_lang;
        };

        using Base64 = BasicBase64<CharSet::Base64Set>;
	}
}

namespace Scriptforge {
    inline namespace Base {
        BASICBASE64_API()::BasicBase64(std::string_view base, const Scriptforge::Local::Lang& lang)
            : m_base(base), m_lang(lang) {}

        BASICBASE64_API()::BasicBase64(const std::vector<unsigned char>& raw, const Scriptforge::Local::Lang& lang)
            : m_raw(raw), m_lang(lang) {}

        BASICBASE64_API(void)::set(std::string_view base) {
            m_raw.clear();
            m_base = base;
        }

        BASICBASE64_API(void)::set(std::vector<unsigned char> raw) {
            m_base = std::string{};
            m_raw = raw;
        }

        BASICBASE64_API(void)::setLang(const Scriptforge::Local::Lang& lang) {
            m_lang = lang;
        }

        BASICBASE64_API(std::string)::encode() const {
            if (m_base) {
                return m_base;
            }
            if (m_raw.empty()) {
                Scriptforge::ErrCode::throwError(
                    Scriptforge::ErrCode::ErrCode::Base64Empty,
                    __func__,
                    m_lang
                );
            }
            size_t len = m_raw.size();
            uint8_t end = len % 3;
            std::string postfix;
            for (size_t i = 0; i <= end; i++) {
                postfix.push_back(set.at(64));
       
            }
            for (size_t i = 0; i <= (len - end) / 3 - 1; i++) {
                uint32_t s = (m_raw[i * 3] << 16) | (m_raw[i * 3 + 1] << 8) | m_raw[i * 3 + 2];
                uint8_t a[4];
                a[3] = s & 0x3F;           // 低6位
                a[2] = (s >> 6) & 0x3F;    // 中间6位
                a[1] = (s >> 12) & 0x3F;   // 第二个中间6位
                a[0] = (s >> 18) & 0x3F;   // 高6位
                for (const auto& j : a) {
                    m_base += set.at(size_t(j));
                }

            }
            m_base += postfix;
            return m_base;
        }

        BASICBASE64_API(std::vector<unsigned char>)::decode() const {
            if (m_raw) {
                return m_raw;
            }
            if (m_base.empty()) {
                Scriptforge::ErrCode::throwError(
                    Scriptforge::ErrCode::ErrCode::Base64Empty,
                    __func__,
                    m_lang
                );
            }

        }
    }
}