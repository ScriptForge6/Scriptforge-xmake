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

#define _SF_BUFFER_BEGIN _SF_BEGIN inline namespace Buffer {
#define  _SF_BUFFER_END   _SF_END }

#define _SF_BUFFER _SF Buffer::
#define _SF_BYTEBUFFER_A _SF_BUFFER BasicByteBuffer<Alloc>
#define _SF_BYTEBUFFER _SF_BUFFER BasicByteBuffer<Alloc>::
#define _SF_BYTEBUFFER_TEM_BEGIN template <typename Alloc
#define _SF_BYTEBUFFER_TEM_END >
#define _SF_BYTEBUFFER_TEM _SF_BYTEBUFFER_TEM_BEGIN _SF_BYTEBUFFER_TEM_END

export module Scriptforge.ByteBuffer;

_SF_BUFFER_BEGIN
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
	BasicByteBuffer(const _SF_BYTEBUFFER_A&) = default;
	BasicByteBuffer(_SF_BYTEBUFFER_A&&) = default;
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
	_SF_BYTEBUFFER_A& push_back(T value);
	template<std::unsigned_integral T>
	_SF_BYTEBUFFER_A& push_back(T value, bool le);
	template<std::unsigned_integral T>
	_SF_BYTEBUFFER_A& push_back_le(T value);
	template<std::unsigned_integral T>
	_SF_BYTEBUFFER_A& push_back_be(T value);
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

	_SF_BYTEBUFFER_A& operator=(const _SF_BYTEBUFFER_A& other);
	_SF_BYTEBUFFER_A& operator=(_SF_BYTEBUFFER_A&& other) noexcept;
	_SF_BYTEBUFFER_A& operator=(std::initializer_list<value_type> ilist);

	void assign(size_type n, const value_type& val);
	template<class InputIt>
	void assign(InputIt first, InputIt last);
	void assign(std::initializer_list<value_type> ilist);

	void swap(_SF_BYTEBUFFER_A& other) noexcept;

	friend auto operator<=>(const _SF_BYTEBUFFER_A& lhs, const _SF_BYTEBUFFER_A& rhs);
private:
	container m_buf;
};

export
_SF_BYTEBUFFER_TEM
auto operator<=>(const _SF_BYTEBUFFER_A& lhs, const _SF_BYTEBUFFER_A& rhs);

export
_SF_BYTEBUFFER_TEM_BEGIN, class U _SF_BYTEBUFFER_TEM_END
constexpr auto erase(_SF_BYTEBUFFER_A& c, const U& value);

export
_SF_BYTEBUFFER_TEM_BEGIN, class Pred _SF_BYTEBUFFER_TEM_END
constexpr auto erase_if(_SF_BYTEBUFFER_A& c, Pred pred);

export using ByteBuffer = BasicByteBuffer<>;

_SF_BUFFER_END


_SF_BUFFER_BEGIN

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER BasicByteBuffer(size_type n)
	: m_buf(n) {}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER BasicByteBuffer(size_type n, const value_type& value)
	: m_buf(n, value) {}

_SF_BYTEBUFFER_TEM
template<class InputIt>
_SF_BYTEBUFFER BasicByteBuffer(InputIt first, InputIt last)
	: m_buf(first, last) {}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER BasicByteBuffer(value_type byte)
	: m_buf(1, byte) {}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER BasicByteBuffer(std::span<const value_type> init)
	: m_buf(init.begin(), init.end()) {}


_SF_BYTEBUFFER_TEM
[[nodiscard]] typename _SF_BYTEBUFFER size_type _SF_BYTEBUFFER max_size() const noexcept {
	return m_buf.max_size();
}

_SF_BYTEBUFFER_TEM
[[nodiscard]] typename _SF_BYTEBUFFER size_type _SF_BYTEBUFFER size() const noexcept {
	return m_buf.size();
}

_SF_BYTEBUFFER_TEM
[[nodiscard]] bool _SF_BYTEBUFFER empty() const noexcept {
	return m_buf.empty();
}

_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER reserve(size_type cap) {
	m_buf.reserve(cap);
}

_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER resize(size_type count) {
	m_buf.resize(count);
}

_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER resize(size_type count, const value_type& value) {
	m_buf.resize(count, value);
}

_SF_BYTEBUFFER_TEM
[[nodiscard]] typename _SF_BYTEBUFFER size_type _SF_BYTEBUFFER capacity() const noexcept {
	return m_buf.capacity();
}

_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER clear() {
	m_buf.clear();
}

_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER shrink_to_fit() {
	m_buf.shrink_to_fit();
}


_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER reference _SF_BYTEBUFFER operator[](size_type pos) {
	return m_buf[pos];
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reference _SF_BYTEBUFFER operator[](size_type pos) const {
	return m_buf[pos];
}


_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER reference _SF_BYTEBUFFER at(size_type pos) {
	return m_buf.at(pos);
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reference _SF_BYTEBUFFER at(size_type pos) const {
	return m_buf.at(pos);
}


_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER reference _SF_BYTEBUFFER front() {
	return m_buf.front();
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reference _SF_BYTEBUFFER front() const {
	return m_buf.front();
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER reference _SF_BYTEBUFFER back() {
	return m_buf.back();
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reference _SF_BYTEBUFFER back() const {
	return m_buf.back();
}


_SF_BYTEBUFFER_TEM
[[nodiscard]] typename _SF_BYTEBUFFER pointer _SF_BYTEBUFFER data() noexcept {
	return m_buf.data();
}

_SF_BYTEBUFFER_TEM
[[nodiscard]] typename _SF_BYTEBUFFER const_pointer _SF_BYTEBUFFER data() const noexcept {
	return m_buf.data();
}


_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER begin() noexcept {
	return m_buf.begin();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER end() noexcept {
	return m_buf.end();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER reverse_iterator _SF_BYTEBUFFER rbegin() noexcept {
	return m_buf.rbegin();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER reverse_iterator _SF_BYTEBUFFER rend() noexcept {
	return m_buf.rend();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_iterator _SF_BYTEBUFFER begin() const noexcept {
	return m_buf.cbegin();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_iterator _SF_BYTEBUFFER end() const noexcept {
	return m_buf.cend();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reverse_iterator _SF_BYTEBUFFER rbegin() const noexcept {
	return m_buf.crbegin();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reverse_iterator _SF_BYTEBUFFER rend() const noexcept {
	return m_buf.crend();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_iterator _SF_BYTEBUFFER cbegin() const noexcept {
	return m_buf.cbegin();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_iterator _SF_BYTEBUFFER cend() const noexcept {
	return m_buf.cend();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reverse_iterator _SF_BYTEBUFFER crbegin() const noexcept {
	return m_buf.crbegin();
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER const_reverse_iterator _SF_BYTEBUFFER crend() const noexcept {
	return m_buf.crend();
}


_SF_BYTEBUFFER_TEM
template<std::unsigned_integral T, bool le>
auto _SF_BYTEBUFFER push_back(T value) -> _SF_BYTEBUFFER_A& {
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

_SF_BYTEBUFFER_TEM
template<std::unsigned_integral T>
auto _SF_BYTEBUFFER push_back(T value, bool le) -> _SF_BYTEBUFFER_A& {
	if (le) {
		return push_back_le<T>(value);
	}
	else {
		return push_back_be<T>(value);
	}
}

_SF_BYTEBUFFER_TEM
template<std::unsigned_integral T>
auto _SF_BYTEBUFFER push_back_le(T value) -> _SF_BYTEBUFFER_A& {
	return push_back<T, true>(value);
}

_SF_BYTEBUFFER_TEM
template<std::unsigned_integral T>
auto _SF_BYTEBUFFER push_back_be(T value) -> _SF_BYTEBUFFER_A& {
	return push_back<T, false>(value);
}

_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER pop_back() {
	m_buf.pop_back();
}

_SF_BYTEBUFFER_TEM
template<class... Args>
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER emplace(iterator pos, Args&&... args) {
	return m_buf.emplace(pos, std::forward<Args>(args)...);
}

_SF_BYTEBUFFER_TEM
template<class... Args>
_SF_BYTEBUFFER reference _SF_BYTEBUFFER emplace_back(Args&&... args) {
	return m_buf.emplace_back(std::forward<Args>(args)...);
}


_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER insert(const_iterator pos, const value_type& value) {
	return m_buf.insert(pos, value);
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER insert(const_iterator pos, value_type&& value) {
	return m_buf.insert(pos, value);
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER insert(const_iterator pos, size_type count, const value_type& value) {
	return m_buf.insert(pos, count, value);
}
_SF_BYTEBUFFER_TEM
template <class InputIt>
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER insert(const_iterator pos, InputIt first, InputIt last) {
	return m_buf.insert(pos, first, last);
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER insert(const_iterator pos, std::initializer_list<value_type> ilist) {
	return m_buf.insert(pos, ilist);
}
_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER insert(const_iterator pos, std::span<const value_type> list) {
	return m_buf.insert(pos, list.begin(), list.end());
}


_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER erase(iterator pos) {
	return m_buf.erase(pos);
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER iterator _SF_BYTEBUFFER erase(iterator first, iterator last) {
	return m_buf.erase(first, last);
}


_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER assign(size_type n, const value_type& val) {
	m_buf.assign(n, val);
}
_SF_BYTEBUFFER_TEM
template <class InputIt>
void _SF_BYTEBUFFER assign(InputIt first, InputIt last) {
	m_buf.assign(first, last);
}
_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER assign(std::initializer_list<value_type> ilist) {
	m_buf.assign(ilist);
}


_SF_BYTEBUFFER_TEM
void _SF_BYTEBUFFER swap(_SF_BYTEBUFFER_A& other) noexcept {
	m_buf.swap(other.m_buf);
}


_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER_A& _SF_BYTEBUFFER operator=(const _SF_BYTEBUFFER_A& other) {
	m_buf = other.m_buf;
	return *this;
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER_A& _SF_BYTEBUFFER operator=(_SF_BYTEBUFFER_A&& other) noexcept {
	m_buf = std::move(other.m_buf);
	return *this;
}

_SF_BYTEBUFFER_TEM
_SF_BYTEBUFFER_A& _SF_BYTEBUFFER operator=(std::initializer_list<value_type> ilist) {
	m_buf = ilist;
	return *this;
}


_SF_BYTEBUFFER_TEM
auto operator<=>(const _SF_BYTEBUFFER_A& lhs, const _SF_BYTEBUFFER_A& rhs) {
	return lhs.m_buf <=> rhs.m_buf;
}


_SF_BYTEBUFFER_TEM_BEGIN, class U _SF_BYTEBUFFER_TEM_END
constexpr auto erase(_SF_BYTEBUFFER_A& c, const U& value) {
	return std::erase(c.m_buf, value);
}

_SF_BYTEBUFFER_TEM_BEGIN, class Pred _SF_BYTEBUFFER_TEM_END
constexpr auto erase_if(_SF_BYTEBUFFER_A& c, Pred pred) {
	return std::erase_if(c.m_buf, pred);
}
_SF_BUFFER_END