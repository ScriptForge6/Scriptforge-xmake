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
module;

#include <cstddef>

#include "Scriptforge.Pch.hpp"

export module Scriptforge.RingBuffer;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Msg;
import Scriptforge.Local;
import Scriptforge.Log;
namespace Scriptforge {
	inline namespace RingBuffer {
		export 
			template<typename RingBufferType>
		class ConstRingBufferIterator {
		public:
			using value_type = typename RingBufferType::value_type;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;
			using node_container_iterator = typename RingBufferType::node_container::const_iterator;
			using size_type = typename RingBufferType::size_type;
			bool operator==(const ConstRingBufferIterator<RingBufferType>&) const = default;
			auto operator<=>(const ConstRingBufferIterator<RingBufferType>& other) const = default;
			const_reference operator*() const;
			const_pointer operator->() const;
			ConstRingBufferIterator<RingBufferType>& operator++();
			ConstRingBufferIterator<RingBufferType> operator++(int);
			ConstRingBufferIterator<RingBufferType>& operator--();
			ConstRingBufferIterator<RingBufferType> operator--(int);
			ConstRingBufferIterator<RingBufferType> operator+(const int src);
			ConstRingBufferIterator<RingBufferType> operator+=(const int src);
			ConstRingBufferIterator<RingBufferType> operator-(const int src);
			ConstRingBufferIterator<RingBufferType> operator-=(const int src);
		};

		template<typename T,typename Alloc>
		concept RingBufferRequires = requires(T t, Alloc a) {
			std::cout << t;
			requires std::copyable<T>;
			requires std::destructible<T>;
		};

		export
			template <typename T = Scriptforge::Message, typename Alloc = std::allocator<T>>
			requires RingBufferRequires<T, Alloc>
		class RingBuffer {
		public:
			using value_type = T;
			using reference = value_type&;
			using const_reference = const value_type&;
			using size_type = size_t;
			using iterator = int;
			using const_iterator = ConstRingBufferIterator<RingBuffer<value_type, Alloc>>;
			using node_container = std::vector<value_type, Alloc>;

			RingBuffer(const size_type capacity = 50, const Scriptforge::Lang lang = Scriptforge::Lang{});
			value_type size() const noexcept;
			value_type max_size() const noexcept;
			bool empty() const noexcept;
			bool operator==(const RingBuffer& other) const = default;
			bool operator!=(const RingBuffer& other) const = default;
			void add(const value_type& item);
			void print()const;
			template <typename U>
			void log(Scriptforge::Logger <value_type,U>& logger) const;
			void clear() noexcept;
		private:
			friend class ConstRingBufferIterator<RingBuffer<value_type, Alloc>>;
			Scriptforge::Lang m_lang;
			size_type m_capacity;
			node_container m_buffer;
		};
	}
}

namespace Scriptforge {
	inline namespace RingBuffer {
		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		RingBuffer<T, Alloc>::RingBuffer(const size_type capacity, const Scriptforge::Lang lang)
			:m_lang(lang) {
			if (capacity == 0) {
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::RingBufferCapacityBeZero, __func__, m_lang);
			}
		}

		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		RingBuffer<T, Alloc>::value_type RingBuffer<T, Alloc>::size() const noexcept {
			return m_buffer.size();
		}

		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		RingBuffer<T, Alloc>::value_type RingBuffer<T, Alloc>::max_size() const noexcept {
			return m_capacity;
		}

		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		bool RingBuffer<T, Alloc>::empty() const noexcept {
			return m_buffer.empty();
		}

		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		void RingBuffer<T, Alloc>::add(const value_type& item) {
			if (m_buffer.size() < m_capacity) {
				m_buffer.push_back(item);
			}
			else {
				// Remove the oldest item and add the new item
				m_buffer.erase(m_buffer.begin());
				m_buffer.push_back(item);
			}
		}
		 
		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		void RingBuffer<T, Alloc>::print() const {
			for (const auto& item : m_buffer) {
				std::cout << item << std::endl;
			}
		}

		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		void RingBuffer<T, Alloc>::clear() noexcept {
			m_buffer.clear();
			m_buffer.shrink_to_fit();
		}

		template <typename T, typename Alloc>
			requires RingBufferRequires<T, Alloc>
		template <typename U>
		void RingBuffer<T, Alloc>::log(Scriptforge::Logger<value_type, U>& logger) const {
			for (const auto& item : m_buffer) {
				logger.log(item);
			}
		}
	}
}