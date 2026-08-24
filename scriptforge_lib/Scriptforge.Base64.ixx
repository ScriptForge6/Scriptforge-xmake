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

#include "Scriptforge.Define.hpp"
#include "Scriptforge.Pch.hpp"

#define _SF_BASE_BEGIN _SF_BEGIN inline namespace Base {
#define _SF_BASE_END   _SF_END }

#define _SF_BASE _SF Base::
#define _SF_BASE64 _SF_BASE BasicBase64<CharSet>::
#define  _SF_BASE64_TEM template<isBaseCharSet CharSet>

export module Scriptforge.Base64;

import Scriptforge.ByteBuffer;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Local;

_SF_BASE_BEGIN
namespace CharSet {
    struct Base64Set {
        inline static constexpr std::array<char, 64> set = []() constexpr {
            std::array<char, 64> m;
            for (size_t i = 0; i <= 25; i++) {
                m[i] = 'A' + i;
            }
            for (size_t i = 26; i <= 51; i++) {
                m[i] = 'a' + i - 26;
            }
            for (size_t i = 52; i <= 61; i++) {
                m[i] = '0' + i - 52;
            }
            m[62] = '+';
            m[63] = '/';
            return m;
            }();
        inline static constexpr std::optional<char> padding_char = '=';
    };
    struct Base64UrlSet {
        inline static constexpr std::array<char, 64> set = []() constexpr {
            std::array<char, 64> m;
            for (size_t i = 0; i <= 25; i++) {
                m[i] = 'A' + i;
            }
            for (size_t i = 26; i <= 51; i++) {
                m[i] = 'a' + i - 26;
            }
            for (size_t i = 52; i <= 61; i++) {
                m[i] = '0' + i - 52;
            }
            m[62] = '-';
            m[63] = '_';
            return m;
            }();
        inline static constexpr std::optional<char> padding_char = std::nullopt;
    };
}
/**
 * @details 一个用于检查类型是否为有效的 Base 字符表的概念。
 * 可以编写以下类型的结构体/类：
 * ```cpp
 * struct Base64Set {
 *     inline static constexpr std::array<char, 64> set;
 *     inline static constexpr std::optional<char> padding_char;
 * };
 * ```
 */
template <typename T>
concept isBaseCharSet = requires {
    { T::set } -> std::convertible_to<typename std::array<char, 64>>;
    { T::padding_char } ->std::convertible_to<std::optional<char>>;
};

export
template <isBaseCharSet CharSet>
class BasicBase64 {
public:
    using container = Scriptforge::Buffer::ByteBuffer;
    BasicBase64() = default;
    BasicBase64(const BasicBase64&) = default;
    BasicBase64(BasicBase64&&) = default;
    BasicBase64(std::string_view base, const Scriptforge::Local::Lang& lang);
    BasicBase64(const container& raw, const Scriptforge::Local::Lang& lang);
    void set(std::string_view base);
    void set(const container& raw);
    void setLang(const Scriptforge::Local::Lang& lang);
    std::string encode() const;
    container decode() const;
private:
    std::variant<std::string, container> m_base;
    Scriptforge::Local::Lang m_lang;
};

export using Base64 = BasicBase64<CharSet::Base64Set>;
export using Base64Url = BasicBase64<CharSet::Base64UrlSet>;
_SF_BASE_END

_SF_BASE_BEGIN
template<isBaseCharSet CharSet>
_SF_BASE64 BasicBase64(std::string_view base, const Scriptforge::Local::Lang& lang)
    : m_base(base), m_lang(lang) {}

_SF_BASE64_TEM
_SF_BASE64 BasicBase64(const container& raw, const Scriptforge::Local::Lang& lang)
    : m_base(raw), m_lang(lang) {}

_SF_BASE64_TEM
void _SF_BASE64 set(std::string_view base) {
    m_base = base;
}

_SF_BASE64_TEM
void _SF_BASE64 set(const container& raw) {
    m_base = raw;
}

_SF_BASE64_TEM
void _SF_BASE64 setLang(const Scriptforge::Local::Lang& lang) {
    m_lang = lang;
}

_SF_BASE64_TEM
std::string _SF_BASE64 encode() const {

    if (std::holds_alternative<std::string>(m_base)) {
        return std::get<std::string>(m_base);
    }

    const auto& buf = std::get<container>(m_base);
    size_t len = buf.size();
    uint8_t mod = len % 3;

    std::string temp_out; // 先用临时串，不污染m_base直到全部成功

    // 处理完整3字节块
    size_t full_block_count = (len - mod) / 3;
    for (size_t i = 0; i < full_block_count; ++i) {
        size_t offset = i * 3;
        uint32_t s = (static_cast<uint32_t>(buf[offset]) << 16)
            | (static_cast<uint32_t>(buf[offset + 1]) << 8)
            | static_cast<uint32_t>(buf[offset + 2]);

        uint8_t a[4];
        a[0] = (s >> 18) & 0x3F;
        a[1] = (s >> 12) & 0x3F;
        a[2] = (s >> 6) & 0x3F;
        a[3] = s & 0x3F;

        for (uint8_t idx : a) {
            temp_out += CharSet::set.at(static_cast<size_t>(idx));
        }
    }

    // ========== 处理尾部不足3字节的剩余字节 ==========
    if (mod == 1) {
        // 剩余 1 byte
        uint8_t b0 = std::get<container>(m_base)[len - 1];
        uint8_t idx0 = (b0 >> 2) & 0x3F;
        uint8_t idx1 = (b0 & 0x03) << 4;
        temp_out += CharSet::set.at(idx0);
        temp_out += CharSet::set.at(idx1);
    }
    else if (mod == 2) {
        // 剩余 2 bytes
        uint8_t b0 = std::get<container>(m_base)[len - 2];
        uint8_t b1 = std::get<container>(m_base)[len - 1];
        uint8_t idx0 = (b0 >> 2) & 0x3F;
        uint8_t idx1 = ((b0 & 0x03) << 4) | ((b1 >> 4) & 0x0F);
        uint8_t idx2 = (b1 & 0x0F) << 2;
        temp_out += CharSet::set.at(idx0);
        temp_out += CharSet::set.at(idx1);
        temp_out += CharSet::set.at(idx2);
    }

    if (CharSet::padding_char.has_value()) {
        char pad_char = CharSet::padding_char.value();
        if (mod == 1) {
            temp_out += std::string(2, pad_char);
        }
        else if (mod == 2) {
            temp_out += std::string(1, pad_char);
        }
    }

    return temp_out;
}


_SF_BASE64_TEM
_SF_BASE64 container _SF_BASE64 decode() const {
    if (std::holds_alternative<container>(m_base)) {
        return std::get<container>(m_base);
    }


}
_SF_BASE_END