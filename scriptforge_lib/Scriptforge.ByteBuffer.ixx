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
 * @file Scriptforge.ByteBuffer.ixx
 * @brief 实现了一个二进制容器，提供更方便的二进制操作。
 * @author Scriptforge
 * @date 2026/8/21
*/

module;

#include "Scriptforge.Define.hpp"
#include "Scriptforge.Pch.hpp"

#include <utf8.h>

export module Scriptforge.ByteBuffer;

import Scriptforge.StringConversion;

SCRIPTFORGE_BUFFER_BEGIN

export namespace Hex {
	struct HexUppercaseSeparate {
		inline static constexpr std::array<char, 16> set = []() constexpr {
			std::array<char, 16> m;
			for (size_t i = 0; i <= 9; i++) {
				m[i] = '0' + i;
			}
			for (size_t i = 10; i <= 15; i++) {
				m[i] = 'A' + i - 10;
			}
			return m;
			}();
		inline static constexpr std::optional<char> separate = ' ';
	};

	struct HexUppercase {
		inline static constexpr std::array<char, 16> set = []() constexpr {
			std::array<char, 16> m;
			for (size_t i = 0; i <= 9; i++) {
				m[i] = '0' + i;
			}
			for (size_t i = 10; i <= 15; i++) {
				m[i] = 'A' + i - 10;
			}
			return m;
			}();
		inline static constexpr std::optional<char> separate = std::nullopt;
	};

	struct HexSeparate {
		inline static constexpr std::array<char, 16> set = []() constexpr {
			std::array<char, 16> m;
			for (size_t i = 0; i <= 9; i++) {
				m[i] = '0' + i;
			}
			for (size_t i = 10; i <= 15; i++) {
				m[i] = 'a' + i - 10;
			}
			return m;
			}();
		inline static constexpr std::optional<char> separate = ' ';
	};

	struct Hex {
		inline static constexpr std::array<char, 16> set = []() constexpr {
			std::array<char, 16> m;
			for (size_t i = 0; i <= 9; i++) {
				m[i] = '0' + i;
			}
			for (size_t i = 10; i <= 15; i++) {
				m[i] = 'a' + i - 10;
			}
			return m;
			}();
		inline static constexpr std::optional<char> separate = std::nullopt;
	};
}


/**
 * @details 一个用于检查类型是否为有效的16进制字符表的概念。
 * 可以编写以下类型的结构体/类：
 * ```cpp
 * struct HexSet {
 *     inline static constexpr std::array<char, 16> set;
 *     inline static constexpr std::optional<char> separate;
 * };
 * ```
 */
template<typename T>
concept isHex = requires {
	{ T::set } -> std::convertible_to<typename std::array<char, 16>>;
	{ T::separate } -> std::convertible_to<std::optional<char>>;
};


export namespace Bin {
	struct Bin {
		inline static constexpr char32_t bin = '.';
	};
}

/**
 * @details 一个用于检查类型是否为有效的二进制字符表的概念。
 * 可以编写以下类型的结构体/类：
 * ```cpp
 * struct BinSet {
 *     inline static constexpr char bin;
 *     inline static constexpr std::optional<char> separate;
 * };
 * ```
 */
template<typename T>
concept isBin = requires {
	{ T::bin } -> std::convertible_to<char32_t>;
};

export
template <typename Alloc = std::allocator<uint8_t>>
class BasicByteBuffer {
public:
	using container = std::vector<uint8_t, Alloc>;
	using value_type = container::value_type;
	using allocator_type = container::allocator_type;
	using reference = container::reference;
	using const_reference = container::const_reference;
	using pointer = container::pointer;
	using const_pointer = container::const_pointer;
	using difference_type = container::difference_type;
	using size_type = container::size_type;
	using iterator = container::iterator;
	using const_iterator = container::const_iterator;
	using reverse_iterator = container::reverse_iterator;
	using const_reverse_iterator = container::const_reverse_iterator;

	BasicByteBuffer() = default;
	BasicByteBuffer(const SCRIPTFORGE_BYTEBUFFER_A&) = default;
	BasicByteBuffer(SCRIPTFORGE_BYTEBUFFER_A&&) = default;
	BasicByteBuffer(size_type n);
	BasicByteBuffer(size_type n, const value_type& value);
	template<class InputIt>
	BasicByteBuffer(InputIt first, InputIt last);
	explicit BasicByteBuffer(value_type byte);
	explicit BasicByteBuffer(std::span<const value_type> init);

	[[nodiscard]] size_type size() const noexcept;
	[[nodiscard]] size_type max_size() const noexcept;
	[[nodiscard]] bool empty() const noexcept;
	void reserve(size_type cap);
	void resize(size_type count);
	void resize(size_type count, const value_type& value);
	[[nodiscard]] size_type capacity() const noexcept;
	void clear();
	void shrink_to_fit();

	reference operator[](size_type pos);
	const_reference operator[](size_type pos) const;

	reference at(size_type pos);
	const_reference at(size_type pos) const;

	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

	[[nodiscard]] pointer data() noexcept;
	[[nodiscard]] const_pointer data() const noexcept;

	iterator begin() noexcept;
	iterator end() noexcept;
	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept;
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator rend() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

	template<std::unsigned_integral T, bool le = true>
	SCRIPTFORGE_BYTEBUFFER_A& push_back(T value);
	template<std::unsigned_integral T>
	SCRIPTFORGE_BYTEBUFFER_A& push_back(T value, bool le);
	template<SCRIPTFORGE_STRINGCONVERSION is_basic_string T>
	SCRIPTFORGE_BYTEBUFFER_A& push_back(T value);
	template<std::unsigned_integral T>
	SCRIPTFORGE_BYTEBUFFER_A& push_back_le(T value);
	template<std::unsigned_integral T>
	SCRIPTFORGE_BYTEBUFFER_A& push_back_be(T value);
	void pop_back();

	template<class... Args>
	iterator emplace(iterator pos, Args&&... args);
	template<class... Args>
	reference emplace_back(Args&&... args);

	iterator insert(const_iterator pos, const value_type& value);
	iterator insert(const_iterator pos, value_type&& value);
	iterator insert(const_iterator pos, size_type count, const value_type& value);
	template <class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last);
	iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);
	iterator insert(const_iterator pos, std::span<const value_type> list);

	iterator erase(iterator pos);
	iterator erase(iterator first, iterator last);

	SCRIPTFORGE_BYTEBUFFER_A& operator=(const SCRIPTFORGE_BYTEBUFFER_A& other);
	SCRIPTFORGE_BYTEBUFFER_A& operator=(SCRIPTFORGE_BYTEBUFFER_A&& other) noexcept;
	SCRIPTFORGE_BYTEBUFFER_A& operator=(std::initializer_list<value_type> ilist);

	void assign(size_type n, const value_type& val);
	template<class InputIt>
	void assign(InputIt first, InputIt last);
	void assign(std::initializer_list<value_type> ilist);

	void swap(SCRIPTFORGE_BYTEBUFFER_A& other) noexcept;

	const container& underlying() const;
	std::string to_string() const;
	template<isHex HexTag>
	std::string to_hex() const;
	std::string to_hex(const bool uppercase = false, const bool separate = true) const;
	template<isBin BinTag>
	std::string to_bin() const;   

	operator container() const;

	friend auto operator<=>(const SCRIPTFORGE_BYTEBUFFER_A& lhs, const SCRIPTFORGE_BYTEBUFFER_A& rhs);

private:
	container m_buf;
};

export
SCRIPTFORGE_BYTEBUFFER_TEM
auto operator<=>(const SCRIPTFORGE_BYTEBUFFER_A& lhs, const SCRIPTFORGE_BYTEBUFFER_A& rhs);

export
SCRIPTFORGE_BYTEBUFFER_TEM_BEGIN, class U SCRIPTFORGE_BYTEBUFFER_TEM_END
constexpr auto erase(SCRIPTFORGE_BYTEBUFFER_A& c, const U& value);

export
SCRIPTFORGE_BYTEBUFFER_TEM_BEGIN, class Pred SCRIPTFORGE_BYTEBUFFER_TEM_END
constexpr auto erase_if(SCRIPTFORGE_BYTEBUFFER_A& c, Pred pred);

export using ByteBuffer = BasicByteBuffer<>;

SCRIPTFORGE_BUFFER_LITERALS_BEGIN

export ByteBuffer operator""_sf_bb(const char* c, std::size_t n);

SCRIPTFORGE_BUFFER_LITERALS_END

SCRIPTFORGE_BUFFER_END


SCRIPTFORGE_BUFFER_BEGIN

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER BasicByteBuffer(size_type n)
	: m_buf(n) {}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER BasicByteBuffer(size_type n, const value_type& value)
	: m_buf(n, value) {}

SCRIPTFORGE_BYTEBUFFER_TEM
template<class InputIt>
SCRIPTFORGE_BYTEBUFFER BasicByteBuffer(InputIt first, InputIt last)
	: m_buf(first, last) {}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER BasicByteBuffer(value_type byte)
	: m_buf(1, byte) {}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER BasicByteBuffer(std::span<const value_type> init)
	: m_buf(init.begin(), init.end()) {}


SCRIPTFORGE_BYTEBUFFER_TEM
[[nodiscard]] typename SCRIPTFORGE_BYTEBUFFER size_type SCRIPTFORGE_BYTEBUFFER max_size() const noexcept {
	return m_buf.max_size();
}

SCRIPTFORGE_BYTEBUFFER_TEM
[[nodiscard]] typename SCRIPTFORGE_BYTEBUFFER size_type SCRIPTFORGE_BYTEBUFFER size() const noexcept {
	return m_buf.size();
}

SCRIPTFORGE_BYTEBUFFER_TEM
[[nodiscard]] bool SCRIPTFORGE_BYTEBUFFER empty() const noexcept {
	return m_buf.empty();
}

SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER reserve(size_type cap) {
	m_buf.reserve(cap);
}

SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER resize(size_type count) {
	m_buf.resize(count);
}

SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER resize(size_type count, const value_type& value) {
	m_buf.resize(count, value);
}

SCRIPTFORGE_BYTEBUFFER_TEM
[[nodiscard]] typename SCRIPTFORGE_BYTEBUFFER size_type SCRIPTFORGE_BYTEBUFFER capacity() const noexcept {
	return m_buf.capacity();
}

SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER clear() {
	m_buf.clear();
}

SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER shrink_to_fit() {
	m_buf.shrink_to_fit();
}


SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER reference SCRIPTFORGE_BYTEBUFFER operator[](size_type pos) {
	return m_buf[pos];
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reference SCRIPTFORGE_BYTEBUFFER operator[](size_type pos) const {
	return m_buf[pos];
}


SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER reference SCRIPTFORGE_BYTEBUFFER at(size_type pos) {
	return m_buf.at(pos);
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reference SCRIPTFORGE_BYTEBUFFER at(size_type pos) const {
	return m_buf.at(pos);
}


SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER reference SCRIPTFORGE_BYTEBUFFER front() {
	return m_buf.front();
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reference SCRIPTFORGE_BYTEBUFFER front() const {
	return m_buf.front();
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER reference SCRIPTFORGE_BYTEBUFFER back() {
	return m_buf.back();
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reference SCRIPTFORGE_BYTEBUFFER back() const {
	return m_buf.back();
}


SCRIPTFORGE_BYTEBUFFER_TEM
[[nodiscard]] typename SCRIPTFORGE_BYTEBUFFER pointer SCRIPTFORGE_BYTEBUFFER data() noexcept {
	return m_buf.data();
}

SCRIPTFORGE_BYTEBUFFER_TEM
[[nodiscard]] typename SCRIPTFORGE_BYTEBUFFER const_pointer SCRIPTFORGE_BYTEBUFFER data() const noexcept {
	return m_buf.data();
}


SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER begin() noexcept {
	return m_buf.begin();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER end() noexcept {
	return m_buf.end();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER reverse_iterator SCRIPTFORGE_BYTEBUFFER rbegin() noexcept {
	return m_buf.rbegin();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER reverse_iterator SCRIPTFORGE_BYTEBUFFER rend() noexcept {
	return m_buf.rend();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_iterator SCRIPTFORGE_BYTEBUFFER begin() const noexcept {
	return m_buf.cbegin();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_iterator SCRIPTFORGE_BYTEBUFFER end() const noexcept {
	return m_buf.cend();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reverse_iterator SCRIPTFORGE_BYTEBUFFER rbegin() const noexcept {
	return m_buf.crbegin();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reverse_iterator SCRIPTFORGE_BYTEBUFFER rend() const noexcept {
	return m_buf.crend();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_iterator SCRIPTFORGE_BYTEBUFFER cbegin() const noexcept {
	return m_buf.cbegin();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_iterator SCRIPTFORGE_BYTEBUFFER cend() const noexcept {
	return m_buf.cend();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reverse_iterator SCRIPTFORGE_BYTEBUFFER crbegin() const noexcept {
	return m_buf.crbegin();
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER const_reverse_iterator SCRIPTFORGE_BYTEBUFFER crend() const noexcept {
	return m_buf.crend();
}


SCRIPTFORGE_BYTEBUFFER_TEM
template<std::unsigned_integral T, bool le>
auto SCRIPTFORGE_BYTEBUFFER push_back(T value) -> SCRIPTFORGE_BYTEBUFFER_A& {
	constexpr size_t N = sizeof(T);
	for (size_t i = 0; i < N; i++) {
		uint8_t byte;
		if constexpr (le) {
			byte = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
		}
		else {
			byte = static_cast<uint8_t>((value >> ((N - 1 - i) * 8)) & 0xFF);
		}
		m_buf.push_back(byte);
	}
	return *this;
}

SCRIPTFORGE_BYTEBUFFER_TEM
template<std::unsigned_integral T>
auto SCRIPTFORGE_BYTEBUFFER push_back(T value, bool le) -> SCRIPTFORGE_BYTEBUFFER_A& {
	if (le) {
		return push_back_le<T>(value);
	}
	else {
		return push_back_be<T>(value);
	}
}

SCRIPTFORGE_BYTEBUFFER_TEM
template<std::unsigned_integral T>
auto SCRIPTFORGE_BYTEBUFFER push_back_le(T value) -> SCRIPTFORGE_BYTEBUFFER_A& {
	return push_back<T, true>(value);
}

SCRIPTFORGE_BYTEBUFFER_TEM
template<std::unsigned_integral T>
auto SCRIPTFORGE_BYTEBUFFER push_back_be(T value) -> SCRIPTFORGE_BYTEBUFFER_A& {
	return push_back<T, false>(value);
}

SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER pop_back() {
	m_buf.pop_back();
}

SCRIPTFORGE_BYTEBUFFER_TEM
template<class... Args>
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER emplace(iterator pos, Args&&... args) {
	return m_buf.emplace(pos, std::forward<Args>(args)...);
}

SCRIPTFORGE_BYTEBUFFER_TEM
template<class... Args>
SCRIPTFORGE_BYTEBUFFER reference SCRIPTFORGE_BYTEBUFFER emplace_back(Args&&... args) {
	return m_buf.emplace_back(std::forward<Args>(args)...);
}


SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER insert(const_iterator pos, const value_type& value) {
	return m_buf.insert(pos, value);
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER insert(const_iterator pos, value_type&& value) {
	return m_buf.insert(pos, value);
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER insert(const_iterator pos, size_type count, const value_type& value) {
	return m_buf.insert(pos, count, value);
}
SCRIPTFORGE_BYTEBUFFER_TEM
template <class InputIt>
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER insert(const_iterator pos, InputIt first, InputIt last) {
	return m_buf.insert(pos, first, last);
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER insert(const_iterator pos, std::initializer_list<value_type> ilist) {
	return m_buf.insert(pos, ilist);
}
SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER insert(const_iterator pos, std::span<const value_type> list) {
	return m_buf.insert(pos, list.begin(), list.end());
}


SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER erase(iterator pos) {
	return m_buf.erase(pos);
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER iterator SCRIPTFORGE_BYTEBUFFER erase(iterator first, iterator last) {
	return m_buf.erase(first, last);
}


SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER assign(size_type n, const value_type& val) {
	m_buf.assign(n, val);
}
SCRIPTFORGE_BYTEBUFFER_TEM
template <class InputIt>
void SCRIPTFORGE_BYTEBUFFER assign(InputIt first, InputIt last) {
	m_buf.assign(first, last);
}
SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER assign(std::initializer_list<value_type> ilist) {
	m_buf.assign(ilist);
}


SCRIPTFORGE_BYTEBUFFER_TEM
void SCRIPTFORGE_BYTEBUFFER swap(SCRIPTFORGE_BYTEBUFFER_A& other) noexcept {
	m_buf.swap(other.m_buf);
}


SCRIPTFORGE_BYTEBUFFER_TEM
const SCRIPTFORGE_BYTEBUFFER container& SCRIPTFORGE_BYTEBUFFER underlying() const {
	return m_buf;
}

SCRIPTFORGE_BYTEBUFFER_TEM
std::string SCRIPTFORGE_BYTEBUFFER to_string() const {
	return std::string{
		reinterpret_cast<const char*>(m_buf.data()),
		m_buf.size()
	};
}

SCRIPTFORGE_BYTEBUFFER_TEM
template<isHex HexTag>
std::string SCRIPTFORGE_BYTEBUFFER to_hex() const {
	constexpr const auto& tbl = HexTag::set;
	constexpr auto sep = HexTag::separate;

	const size_t count = m_buf.size();
	if (count == 0)
		return {};

	size_t out_size = count * 2;
	if constexpr (sep.has_value()) {
		out_size += count - 1;
	}

	std::string out;
	out.reserve(out_size);

	for (size_t i = 0; i < count; ++i) {
		const uint8_t b = static_cast<uint8_t>(m_buf[i]);
		out += tbl[(b >> 4) & 0x0F];
		out += tbl[b & 0x0F];

		if constexpr (sep.has_value()) {
			if (i != count - 1) {
				out += sep.value();
			}
		}
	}
	return out;
}

SCRIPTFORGE_BYTEBUFFER_TEM
std::string SCRIPTFORGE_BYTEBUFFER to_hex(const bool uppercase, const bool separate) const {
	if (uppercase && separate)
		return to_hex<Hex::HexUppercaseSeparate>();
	else if (uppercase)
		return to_hex<Hex::HexUppercase>();
	else if (separate)
		return to_hex<Hex::HexSeparate>();
	else
		return to_hex<Hex::Hex>();
}

SCRIPTFORGE_BYTEBUFFER_TEM
template<isBin BinTag>
std::string SCRIPTFORGE_BYTEBUFFER to_bin() const {
	return utf8::replace_invalid(std::u8string(m_buf.begin(), m_buf.end()), BinTag::bin);
}


SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER operator container() const {
	return underlying();
}



SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER_A& SCRIPTFORGE_BYTEBUFFER operator=(const SCRIPTFORGE_BYTEBUFFER_A& other) {
	m_buf = other.m_buf;
	return *this;
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER_A& SCRIPTFORGE_BYTEBUFFER operator=(SCRIPTFORGE_BYTEBUFFER_A&& other) noexcept {
	m_buf = std::move(other.m_buf);
	return *this;
}

SCRIPTFORGE_BYTEBUFFER_TEM
SCRIPTFORGE_BYTEBUFFER_A& SCRIPTFORGE_BYTEBUFFER operator=(std::initializer_list<value_type> ilist) {
	m_buf = ilist;
	return *this;
}


SCRIPTFORGE_BYTEBUFFER_TEM
auto operator<=>(const SCRIPTFORGE_BYTEBUFFER_A& lhs, const SCRIPTFORGE_BYTEBUFFER_A& rhs) {
	return lhs.m_buf <=> rhs.m_buf;
}


SCRIPTFORGE_BYTEBUFFER_TEM_BEGIN, class U SCRIPTFORGE_BYTEBUFFER_TEM_END
constexpr auto erase(SCRIPTFORGE_BYTEBUFFER_A& c, const U& value) {
	return std::erase(c.m_buf, value);
}

SCRIPTFORGE_BYTEBUFFER_TEM_BEGIN, class Pred SCRIPTFORGE_BYTEBUFFER_TEM_END
constexpr auto erase_if(SCRIPTFORGE_BYTEBUFFER_A& c, Pred pred) {
	return std::erase_if(c.m_buf, pred);
}

SCRIPTFORGE_BUFFER_LITERALS_BEGIN
ByteBuffer operator""_sf_bb(const char* c, std::size_t n) {
	auto sp = std::span(reinterpret_cast<const uint8_t*>(c), n);
	return ByteBuffer(sp);
}
SCRIPTFORGE_BUFFER_LITERALS_END

SCRIPTFORGE_BUFFER_END