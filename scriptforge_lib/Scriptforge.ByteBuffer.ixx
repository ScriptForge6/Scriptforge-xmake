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

#include "Scriptforge.Pch.hpp"

#define BYTEBUFFER_API(x) \
template <typename Alloc> \
x ByteBuffer<Alloc>
#define BYTEBUFFER ::Scriptforge::Buffer::ByteBuffer<Alloc>

export module Scriptforge.ByteBuffer;

namespace Scriptforge {
	inline namespace Buffer {
		export
			template <typename Alloc = std::allocator<uint8_t>>
			class ByteBuffer {
		public:
			using value_type = uint8_t;
			using allocator_type = Alloc;
			using container = std::vector<uint8_t, allocator_type>;
			using reference = value_type&;
			using const_reference = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;
			using difference_type = container::difference_type;
			using size_type = size_t;
			using iterator = container::iterator;
			using const_iterator = container::const_iterator;

			ByteBuffer() = default;
			ByteBuffer(const ByteBuffer&) = default;
			ByteBuffer(ByteBuffer&&) = default;
			explicit ByteBuffer(value_type byte);
			explicit ByteBuffer(std::span<const uint8_t> init);


		private:
			container m_buf;
		};
	}
}

namespace Scriptforge {
	inline namespace Buffer {
		BYTEBUFFER_API()::ByteBuffer(BYTEBUFFER::value_type byte) {
			m_buf.push_back(byte);
		}

		BYTEBUFFER_API()::ByteBuffer(std::span<const uint8_t> init)
			: m_buf(init.begin(), init.end()) {}


	}
}