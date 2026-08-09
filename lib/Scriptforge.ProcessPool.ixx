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
 * @file Scriptforge.ProcessPool.ixx
 * @brief 定义了 Scriptforge::ProcessPool 模块，包含一个 ProcessPool 类，用于管理和执行多个子进程。该类提供了添加任务、等待所有任务完成以及获取子进程的输出等功能。通过使用 ProcessPool，可以方便地并行执行多个任务，提高程序的效率和性能。
 * @author Scriptforge
 * @date 2026/5/4
 */
module;

#include "Scriptforge.Pch.hpp"

export module Scriptforge.ProcessPool;
import Scriptforge.Local;
import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.ThreadError;

namespace Scriptforge {
	inline namespace ProcessPool {
		export 
			template<typename Alloc = std::allocator<Scriptforge::Err::ThreadError<true>>>
			class ProcessPool {
		public:
			using value_type = Scriptforge::Err::ThreadError<true>;
			using reference = value_type&;
			using const_reference = const value_type&;
			using size_type = size_t;
			using iterator = std::vector<value_type>::iterator;
			using const_iterator = std::vector<value_type>::const_iterator;
			using allocator_type = Alloc;

			explicit ProcessPool(const size_type& poolSize, const Scriptforge::Local::Lang& lang);
			explicit ProcessPool(size_type&& poolSize, Scriptforge::Local::Lang&& lang);
			ProcessPool(const ProcessPool&) = delete;
			ProcessPool& operator=(const ProcessPool&) = delete;

			~ProcessPool();

			void addTask(const std::function<void()>& task);
			void waitAll();
			void stopAll();

			size_type getPoolSize() const noexcept;
			void setPoolSize(const size_type& size);
		private:
			std::vector<value_type, allocator_type> m_tasks;
			size_type m_size;
			std::mutex m_mtx;
			Scriptforge::Local::Lang m_lang;
		};
	}
}

namespace Scriptforge {
	inline namespace ProcessPool {
		template<typename Alloc>
		ProcessPool<Alloc>::ProcessPool(const size_type& poolSize, const Scriptforge::Local::Lang& lang)
			: m_size(poolSize), m_lang(lang) {
			std::lock_guard<std::mutex> lock(m_mtx);
			m_tasks.reserve(poolSize);
		}

		template<typename Alloc>
		ProcessPool<Alloc>::ProcessPool(size_type&& poolSize, Scriptforge::Local::Lang&& lang)
			: m_size(std::move(poolSize)), m_lang(std::move(lang)) {
			std::lock_guard<std::mutex> lock(m_mtx);
			m_tasks.reserve(m_size);
		}

		template<typename Alloc>
		ProcessPool<Alloc>::~ProcessPool() {
			stopAll();
			waitAll();
		}

		template<typename Alloc>
		void ProcessPool<Alloc>::addTask(const std::function<void()>& task) {
			std::lock_guard<std::mutex> lock(m_mtx);
			if (m_tasks.size() < m_size) {
				m_tasks.emplace_back();
				m_tasks.back().threadStart(task);
			}
			else {
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::ProcessPoolInvalidPoolSize, __func__, m_lang);
			}
		}

		template<typename Alloc>
		void ProcessPool<Alloc>::waitAll() {
			std::lock_guard<std::mutex> lock(m_mtx);
			for (auto& task : m_tasks) {
				task.waitForCompletion();
			}
		}

		template<typename Alloc>
		void ProcessPool<Alloc>::stopAll() {
			std::lock_guard<std::mutex> lock(m_mtx);
			for (auto& task : m_tasks) {
				if (task.isRunning()) {
					task.requestStop();
				}
			}
		}

		template<typename Alloc>
		ProcessPool<Alloc>::size_type ProcessPool<Alloc>::getPoolSize() const noexcept {
			std::lock_guard<std::mutex> lock(m_mtx);
			return m_size;
		}

		template<typename Alloc>
		void ProcessPool<Alloc>::setPoolSize(const size_type& size) {
			std::lock_guard<std::mutex> lock(m_mtx);
			m_size = size;
			m_tasks.reserve(size);
		}

	}
}