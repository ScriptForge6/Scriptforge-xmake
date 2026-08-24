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
 * @file Scriptforge.StringConversion.ixx
 * @brief 定义了 `Scriptforge::StringConversion` 模块，其中包含了一系列字符串转换函数，用于在不同的字符串类型之间进行转换，包括 `std::string`、`std::u8string`、`std::u16string`、`std::u32string` 和 `std::wstring`。这些函数使用了 UTF-8 编码作为中间格式，确保了转换的正确性和兼容性。此外，还提供了一个通用的 `str_convert` 模板函数，可以根据目标类型和源类型自动选择合适的转换函数进行转换。
 * @author Scriptforge
 * @date 2026/4/28
 */

module;

#include "Scriptforge.Define.hpp"
#include "Scriptforge.Pch.hpp"

#include <utf8.h>

export module Scriptforge.StringConversion;

_SF_STRINGCONVERSION_BEGIN

export template <typename T>
concept is_basic_string = requires(T str) {
    requires std::same_as<T, std::basic_string<typename T::value_type>>;
};

template <typename...>
constexpr inline bool always_false = false;

std::u8string stringToU8(const std::string& str);
std::string u8ToString(const std::u8string& str);
std::u16string stringToU16(const std::string& str);
std::u32string stringToU32(const std::string& str);
std::string u16ToString(const std::u16string& str);
std::string u32ToString(const std::u32string& str);
std::u32string u16ToU32(const std::u16string& str);
std::u16string u32ToU16(const std::u32string& str);
std::wstring stringToWstring(const std::string& str);
std::string wstringToString(const std::wstring& str);
std::u8string wstringToU8(const std::wstring& str);
std::wstring u8ToWstring(const std::u8string& str);
std::u16string wstringToU16(const std::wstring& str);
std::wstring u16ToWstring(const std::u16string& str);
std::u32string wstringToU32(const std::wstring& str);
std::wstring u32ToWstring(const std::u32string& str);


// ------------------------------
// 对外唯一接口：str_convert
// ------------------------------
export template <class ToStr, class FromStr>
    requires is_basic_string<ToStr>&& is_basic_string<FromStr>
ToStr str_convert(const FromStr& str);

_SF_STRINGCONVERSION_END


_SF_STRINGCONVERSION_BEGIN

static_assert(sizeof(char) == sizeof(char8_t), "char and char8_t must be same size");
static_assert(sizeof(wchar_t) == sizeof(char16_t) || sizeof(wchar_t) == sizeof(char32_t), "wchar_t must be 16 or 32 bits");

constexpr bool isWchar16Bits() noexcept {
    return sizeof(wchar_t) == sizeof(char16_t);
}

// string <-> u8
std::u8string stringToU8(const std::string& str) {
    return { str.begin(), str.end() };
}
std::string u8ToString(const std::u8string& str) {
    return { str.begin(), str.end() };
}

// string <-> u16 / u32
std::u16string stringToU16(const std::string& str) {
    std::u16string res;
    utf8::utf8to16(str.begin(), str.end(), std::back_inserter(res));
    return res;
}
std::u32string stringToU32(const std::string& str) {
    std::u32string res;
    utf8::utf8to32(str.begin(), str.end(), std::back_inserter(res));
    return res;
}
std::string u16ToString(const std::u16string& str) {
    std::string res;
    utf8::utf16to8(str.begin(), str.end(), std::back_inserter(res));
    return res;
}
std::string u32ToString(const std::u32string& str) {
    std::string res;
    utf8::utf32to8(str.begin(), str.end(), std::back_inserter(res));
    return res;
}

// 16 <-> 32
std::u32string u16ToU32(const std::u16string& str) {
    std::u32string res;
    std::u8string t;
    utf8::utf16to8(str.begin(), str.end(), std::back_inserter(t));
    utf8::utf8to32(t.begin(), t.end(), std::back_inserter(res));
    return res;
}
std::u16string u32ToU16(const std::u32string& str) {
    std::u16string res;
    std::u8string t;
    utf8::utf32to8(str.begin(), str.end(), std::back_inserter(t));
    utf8::utf8to16(t.begin(), t.end(), std::back_inserter(res));
    return res;
}

// string <-> wstring
std::wstring stringToWstring(const std::string& str) {
    if constexpr (isWchar16Bits()) {
        auto u16 = stringToU16(str);
        return { u16.begin(), u16.end() };
    }
    else {
        auto u32 = stringToU32(str);
        return { u32.begin(), u32.end() };
    }
}
std::string wstringToString(const std::wstring& str) {
    if constexpr (isWchar16Bits()) {
        return u16ToString({ str.begin(), str.end() });
    }
    else {
        return u32ToString({ str.begin(), str.end() });
    }
}

// wstring <-> u8
std::u8string wstringToU8(const std::wstring& str) {
    std::u8string res;
    if constexpr (isWchar16Bits()) {
        utf8::utf16to8(str.begin(), str.end(), std::back_inserter(res));
    }
    else {
        utf8::utf32to8(str.begin(), str.end(), std::back_inserter(res));
    }
    return res;
}
std::wstring u8ToWstring(const std::u8string& str) {
    return stringToWstring(u8ToString(str));
}

// wstring <-> u16
std::u16string wstringToU16(const std::wstring& str) {
    if constexpr (isWchar16Bits()) {
        return { str.begin(), str.end() };
    }
    else {
        return u32ToU16({ str.begin(), str.end() });
    }
}
std::wstring u16ToWstring(const std::u16string& str) {
    if constexpr (isWchar16Bits()) {
        return { str.begin(), str.end() };
    }
    else {
        return u32ToWstring(u16ToU32(str));
    }
}

// wstring <-> u32
std::u32string wstringToU32(const std::wstring& str) {
    if constexpr (isWchar16Bits()) {
        return u16ToU32({ str.begin(), str.end() });
    }
    else {
        return { str.begin(), str.end() };
    }
}

std::wstring u32ToWstring(const std::u32string& str) {
    if constexpr (isWchar16Bits()) {
        return u16ToWstring(u32ToU16(str));
    }
    else {
        return { str.begin(), str.end() };
    }
}
template <class ToStr, class FromStr>
    requires is_basic_string<ToStr>&& is_basic_string<FromStr>
ToStr str_convert(const FromStr& str) {
    // 同类型直接返回
    if constexpr (std::same_as<ToStr, FromStr>) {
        return str;
    }

    // 目标：string
    else if constexpr (std::same_as<ToStr, std::string>) {
        if constexpr (std::same_as<FromStr, std::u8string>)   return u8ToString(str);
        if constexpr (std::same_as<FromStr, std::u16string>)  return u16ToString(str);
        if constexpr (std::same_as<FromStr, std::u32string>)  return u32ToString(str);
        if constexpr (std::same_as<FromStr, std::wstring>)    return wstringToString(str);
    }

    // 目标：u8string
    else if constexpr (std::same_as<ToStr, std::u8string>) {
        if constexpr (std::same_as<FromStr, std::string>)     return stringToU8(str);
        if constexpr (std::same_as<FromStr, std::wstring>)    return wstringToU8(str);
        if constexpr (std::same_as<FromStr, std::u16string>)  return stringToU8(u16ToString(str));
        if constexpr (std::same_as<FromStr, std::u32string>)  return stringToU8(u32ToString(str));
    }

    // 目标：u16string
    else if constexpr (std::same_as<ToStr, std::u16string>) {
        if constexpr (std::same_as<FromStr, std::string>)     return stringToU16(str);
        if constexpr (std::same_as<FromStr, std::wstring>)    return wstringToU16(str);
        if constexpr (std::same_as<FromStr, std::u8string>)   return stringToU16(u8ToString(str));
        if constexpr (std::same_as<FromStr, std::u32string>)  return u32ToU16(str);
    }

    // 目标：u32string
    else if constexpr (std::same_as<ToStr, std::u32string>) {
        if constexpr (std::same_as<FromStr, std::string>)     return stringToU32(str);
        if constexpr (std::same_as<FromStr, std::wstring>)    return wstringToU32(str);
        if constexpr (std::same_as<FromStr, std::u8string>)   return stringToU32(u8ToString(str));
        if constexpr (std::same_as<FromStr, std::u16string>)  return u16ToU32(str);
    }

    // 目标：wstring
    else if constexpr (std::same_as<ToStr, std::wstring>) {
        if constexpr (std::same_as<FromStr, std::string>)     return stringToWstring(str);
        if constexpr (std::same_as<FromStr, std::u8string>)   return u8ToWstring(str);
        if constexpr (std::same_as<FromStr, std::u16string>)  return u16ToWstring(str);
        if constexpr (std::same_as<FromStr, std::u32string>)  return u32ToWstring(str);
    }

    // 不支持的转换
    else {
        static_assert(always_false<ToStr, FromStr>, "不支持的字符串转换类型");
    }
}

_SF_STRINGCONVERSION_END