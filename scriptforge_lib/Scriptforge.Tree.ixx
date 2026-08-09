// Copyright 2025-2026 Scriptforge
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
 * @file Scriptforge.Tree.ixx
 * @brief 定义了 `Scriptforge::Tree` 模块，其中包含了一个树结构的实现。该模块提供了一个 `Tree` 类，用于表示树结构，并包含了一个嵌套的 `TreeNode` 结构体来表示树节点。`Tree` 类提供了构造函数、添加节点、删除节点、设置语言和获取语言等成员函数，以及一个 `ConstTreeIterator` 类和一个 `TreeIterator` 类，用于遍历树结构。通过使用这些定义，可以方便地创建和操作树结构，提高代码的可读性和可维护性。
 * @author Scriptforge
 * @date 2026/3/29
 */
module;

#include <cstddef>

#include "Scriptforge.Pch.hpp"

export module Scriptforge.Tree;

import Scriptforge.Err;
import Scriptforge.ErrCode;
import Scriptforge.ErrCode.throwError;
import Scriptforge.Local;


export namespace Scriptforge {
    inline namespace Tree {
        export enum class TreeTraversalOrder {
            PreOrder,
            PostOrder,
            LevelOrder
        };
        export
            template <typename TreeType>
        class ConstTreeIterator {
        public:
            using value_type = typename TreeType::value_type;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const reference;
			using pointer = value_type*;
            using const_pointer = const pointer;
            using size_type = size_t;
            using iterator_category = std::forward_iterator_tag;
            ConstTreeIterator() = default;
            explicit ConstTreeIterator(typename TreeType::nodeptr node, TreeTraversalOrder order = TreeTraversalOrder::LevelOrder);
            const_reference operator*() const;
            const_pointer operator->() const;
            ConstTreeIterator<TreeType>& operator++();
            ConstTreeIterator<TreeType> operator++(int);
            bool operator==(const ConstTreeIterator<TreeType>& other) const;
            bool operator!=(const ConstTreeIterator<TreeType>& other) const;
            TreeType::nodeptr current_node() const;
        protected:
            typename TreeType::nodeptr m_current_node;
            TreeTraversalOrder m_traversal_order;
            std::queue<typename TreeType::nodeptr> m_queue;

            // 私有辅助函数
            void advance_preorder();
            void advance_postorder();
            void advance_levelorder();
            bool has_children() const;
            void go_to_first_child();
            void find_next_sibling_or_ancestor();
            bool move_to_next_sibling(const typename TreeType::nodeptr& current,
                const typename TreeType::nodeptr& father);
        };

        export
            template <typename TreeType>
		class TreeIterator : public ConstTreeIterator<TreeType> {
        public:
            using value_type = typename TreeType::value_type;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using pointer = value_type*;
            using size_type = size_t;
            using iterator_category = std::forward_iterator_tag;
            TreeIterator() = default;
            explicit TreeIterator(typename TreeType::nodeptr node, TreeTraversalOrder order = TreeTraversalOrder::LevelOrder);
			reference operator*();
			pointer operator->();
            TreeIterator<TreeType>& operator++();
            TreeIterator<TreeType> operator++(int);
        private:
        };

        template <typename T>
        concept TreeRequires =
            std::copy_constructible<T> &&
            std::move_constructible<T> &&
            requires(T a) {
                { T{} } -> std::same_as<T>;
        };
        export
            template<typename T, typename Alloc = std::allocator<T>>
			requires TreeRequires<T>
        class Tree {
        private:
            struct TreeNode;
        public:
            using value_type = T;
            using reference = value_type&;
            using const_reference = const value_type&;
            using size_type = size_t;
            using iterator = TreeIterator<Tree<value_type, Alloc>>;
            using const_iterator = ConstTreeIterator<Tree<value_type, Alloc>>;
            using allocator_type = Alloc;
            using nodeptr = std::shared_ptr<TreeNode>;

            //构造函数
            Tree(const allocator_type& alloc = allocator_type(), const Scriptforge::Local::Lang& lang = Lang{});
            Tree(const Tree<T, Alloc>& other);

            nodeptr root() const noexcept;                     //返回根节点
            nodeptr del(nodeptr node);                         //删除节点

            //添加节点
            nodeptr add(nodeptr father);
            nodeptr add(nodeptr father, T& node);
            nodeptr add(nodeptr father, const T& node);
            
            void setLang(Scriptforge::Lang lang);         //设置语言

            Scriptforge::Lang lang() const noexcept;      //返回语言

            allocator_type allocator() const noexcept;     //返回分配器

			//迭代器
            iterator begin(TreeTraversalOrder order = TreeTraversalOrder::LevelOrder);
			const_iterator begin(TreeTraversalOrder order = TreeTraversalOrder::LevelOrder) const;
			const_iterator cbegin(TreeTraversalOrder order = TreeTraversalOrder::LevelOrder) const;
			iterator end(TreeTraversalOrder order = TreeTraversalOrder::LevelOrder);
            const_iterator end(TreeTraversalOrder order = TreeTraversalOrder::LevelOrder) const;
            const_iterator cend(TreeTraversalOrder order = TreeTraversalOrder::LevelOrder) const;

        private:
            struct TreeNode {
                TreeNode() = default;
                explicit TreeNode(const T& v) : node(v) {}
                std::weak_ptr<TreeNode> father;

                std::vector<nodeptr, typename std::allocator_traits<Alloc>::template rebind_alloc<nodeptr>> children;
                T node;
            };

            Scriptforge::Local::Lang m_lang;
            nodeptr m_root;
            allocator_type m_alloc;

            template<typename U>
            nodeptr createNode(U&& value);
        };
    }
}



namespace Scriptforge {
    inline namespace Tree {
		template<typename T, typename Alloc>
			requires TreeRequires<T>
        Tree<T, Alloc>::Tree(const allocator_type& alloc, const Scriptforge::Local::Lang& lang)
            : m_alloc(alloc), m_lang(lang) {
            m_root = createNode(T());
		}

		template<typename T, typename Alloc>
			requires TreeRequires<T>
        Tree<T, Alloc>::Tree(const Tree<T, Alloc>& other)
            : m_alloc(other.allocator()), m_lang(other.lang()) {
            auto deepCopy = [&](auto&& self, const nodeptr& original, const nodeptr& parent) -> nodeptr {
                if (!original) return nullptr;
                nodeptr copy_node = createNode(original->node);
                copy_node->father = parent;
                for (const auto& child : original->children) {
                    auto child_copy = self(self, child, copy_node);
                    if (child_copy) {
                        copy_node->children.push_back(child_copy);
                    }
                }
                return copy_node;
				};
            m_root = deepCopy(deepCopy, other.root(), nullptr);
        }

		template<typename T, typename Alloc>
			requires TreeRequires<T>
        Tree<T, Alloc>::nodeptr Tree<T, Alloc>::root() const  noexcept {
            return m_root;
		}

		template<typename T, typename Alloc>
			requires TreeRequires<T>
        Tree<T, Alloc>::nodeptr Tree<T, Alloc>::del(nodeptr node) {
            if (!node) {
				Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeEmptyNode, __func__, m_lang);
            }
			if (!node.father.lock()) {
                if (node != m_root) {
                    Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeOrphanedNode, __func__, m_lang);
                } else {
                    m_root.reset();
                }
                return nullptr;
            }
            auto& vec = node.father.lock()->children;
			auto it = std::find(vec.begin(), vec.end(), node);
			if (it != vec.end()) {
				vec.erase(it);
			}
			return node;
        }

        /*
        //ConstTreeIterator实现部分

        //构造函数
        template<typename TreeType>
        ConstTreeIterator<TreeType>::ConstTreeIterator(typename TreeType::nodeptr node, TreeTraversalOrder order)
            : m_current_node(node), m_traversal_order(order) {
            if (m_traversal_order == TreeTraversalOrder::LevelOrder) {
                if (node) m_queue.push(node);
                if (!m_queue.empty())
                    m_current_node = m_queue.front();
            }
        }
        //解引用操作符
        template<typename TreeType>
        typename ConstTreeIterator<TreeType>::const_reference
            ConstTreeIterator<TreeType>::operator*() const {
            return m_current_node->node;
        }
        //箭头操作符
        template<typename TreeType>
        typename ConstTreeIterator<TreeType>::const_pointer
            ConstTreeIterator<TreeType>::operator->() const {
            return &(m_current_node->node);
        }
        //前置递增操作符
        template<typename TreeType>
        ConstTreeIterator<TreeType>&
            ConstTreeIterator<TreeType>::operator++() {
            switch (m_traversal_order) {
            case TreeTraversalOrder::PreOrder:
                advance_preorder();
                break;
            case TreeTraversalOrder::PostOrder:
                advance_postorder();
                break;
            case TreeTraversalOrder::LevelOrder:
                advance_levelorder();
                break;
            }
            return *this;
        }
        //后置递增操作符
        template<typename TreeType>
        ConstTreeIterator<TreeType>
            ConstTreeIterator<TreeType>::operator++(int) {
            ConstTreeIterator temp = *this;
            ++(*this);
            return temp;
        }
        //相等比较操作符
        template<typename TreeType>
        bool ConstTreeIterator<TreeType>::operator==(const ConstTreeIterator<TreeType>& other) const {
            return m_current_node == other.m_current_node;
        }
        //不等比较操作符
        template<typename TreeType>
        bool ConstTreeIterator<TreeType>::operator!=(const ConstTreeIterator<TreeType>& other) const {
            return !(*this == other);
        }
        //返回树指针
        template<typename TreeType>
        TreeType::nodeptr ConstTreeIterator<TreeType>::current_node() const {
            return m_current_node;
        }
        //私有辅助函数
        template<typename TreeType>
        void ConstTreeIterator<TreeType>::advance_preorder() {
            if (!m_current_node) return;

            if (has_children()) {
                go_to_first_child();
            }
            else {
                find_next_sibling_or_ancestor();
            }
        }
        template<typename TreeType>
        bool ConstTreeIterator<TreeType>::has_children() const {
            return !m_current_node->children.empty();
        }
        template<typename TreeType>
        void ConstTreeIterator<TreeType>::go_to_first_child() {
            m_current_node = m_current_node->children.front();
        }
        template<typename TreeType>
        void ConstTreeIterator<TreeType>::find_next_sibling_or_ancestor() {
            auto temp = m_current_node;
            while (temp) {
                auto father = temp->father.lock();
                if (!father) {
                    m_current_node = nullptr;
                    return;
                }

                if (move_to_next_sibling(temp, father)) {
                    break;
                }

                temp = father;
            }

            if (!temp) {
                m_current_node = nullptr;
            }
        }
        template<typename TreeType>
        bool ConstTreeIterator<TreeType>::move_to_next_sibling(const typename TreeType::nodeptr& current,
            const typename TreeType::nodeptr& father) {
            auto& siblings = father->children;
            auto it = std::find(siblings.begin(), siblings.end(), current);

            if (it != siblings.end() && ++it != siblings.end()) {
                m_current_node = *it;
                return true;
            }
            return false;
        }

        template<typename TreeType>
        void ConstTreeIterator<TreeType>::advance_postorder() {
            if (!m_current_node) return;

            auto temp = m_current_node;
            while (temp) {
                auto father = temp->father.lock();
                if (!father) {
                    m_current_node = nullptr;
                    return;
                }
                auto& siblings = father->children;
                auto it = std::find(siblings.begin(), siblings.end(), temp);
                if (it != siblings.end()) {
                    ++it;
                    if (it != siblings.end()) {
                        m_current_node = *it;
                        while (!m_current_node->children.empty()) {
                            m_current_node = m_current_node->children.front();
                        }
                        return;
                    }
                    else {
                        m_current_node = father;
                        return;
                    }
                }
                temp = father;
            }

            // 遍历完成
            m_current_node = nullptr;
        }

        template <typename TreeType>
        void ConstTreeIterator<TreeType>::advance_levelorder() {
            if (!m_queue.empty())
                m_queue.pop();
            if (m_current_node) {
                for (const auto& child : m_current_node->children) {
                    if (child) m_queue.push(child);
                }
            }
            if (!m_queue.empty())
                m_current_node = m_queue.front();
            else
                m_current_node = nullptr;
        }

        //TreeIterator实现
		template<typename TreeType>
        typename TreeIterator<TreeType>::reference
            TreeIterator<TreeType>::operator*() {
            return const_cast<typename TreeType::reference>(ConstTreeIterator<TreeType>::operator*());
		}
        
        template<typename TreeType>
        typename TreeIterator<TreeType>::pointer
            TreeIterator<TreeType>::operator->() {
            return const_cast<typename TreeType::pointer>(ConstTreeIterator<TreeType>::operator->());
		}

		//TreeIterator实现部分

		//构造函数
		template<typename TreeType>
        TreeIterator<TreeType>::TreeIterator(typename TreeType::nodeptr node, TreeTraversalOrder order)
            : ConstTreeIterator<TreeType>(node, order) {}

		//前置递增操作符
		template<typename TreeType>
        TreeIterator<TreeType>&
            TreeIterator<TreeType>::operator++() {
            ConstTreeIterator<TreeType>::operator++();
            return *this;
		}
		template<typename TreeType>
        TreeIterator<TreeType>
            TreeIterator<TreeType>::operator++(int) {
            TreeIterator temp = *this;
            ++(*this);
            return temp;
        }

        //Tree<T>实现部分

        //构造函数
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        Tree<T, Alloc>::Tree(const allocator_type& alloc, Scriptforge::Lang lang)
            : alloc_(alloc), m_lang(lang) {
            m_root = create_node(T());
        }

        template<typename T, typename Alloc>
            requires TreeRequires<T>
        Tree<T, Alloc>::Tree(const T& node, const allocator_type& alloc)
            : alloc_(alloc) {
            m_root = create_node(node);
        }

        template<typename T, typename Alloc>
            requires TreeRequires<T>
        Tree<T, Alloc>::Tree(const Tree<T, Alloc>& other)
            : alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_)) {
            auto deep_copy = [&](auto&& self, const nodeptr& original, const nodeptr& parent) -> nodeptr {
                if (!original) return nullptr;
                nodeptr copy_node = create_node(original->node);
                copy_node->father = parent;
                for (const auto& child : original->children) {
                    auto child_copy = self(self, child, copy_node);
                    if (child_copy) {
                        copy_node->children.push_back(child_copy);
                    }
                }
                return copy_node;
                };
            m_root = deep_copy(deep_copy, other.m_root, nullptr);
        }

        //返回根节点
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::root() const {
            return m_root;
        }

        //删除节点
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::del(nodeptr node) {
			if (!node) Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeEmptyNode, __func__, m_lang);
            nodeptr father = node->father.lock();
            if (!father) {
                if (node != m_root) Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeOrphanedNode, __func__, m_lang);
                else m_root.reset();
                return nullptr;
            }
            auto& vec = father->children;
            auto it = std::find(vec.begin(), vec.end(), node);
			if (it == vec.end()) Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeInvalidNode, __func__, m_lang);

            vec.erase(it);
            return father;
        }

        //添加节点
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father) {
			if (!father) Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeEmptyNode, __func__, m_lang);
            nodeptr newnode = create_node(T());
            newnode->father = father;
            father->children.push_back(newnode);
            return newnode;
        }

        template<typename T, typename Alloc>
            requires TreeRequires<T>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, T& node) {
			if (!father) Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeEmptyNode, __func__, m_lang);
            nodeptr newnode = create_node(node);
            newnode->father = father;
            father->children.push_back(newnode);
            return newnode;
        }

        template<typename T, typename Alloc>
            requires TreeRequires<T>
        typename Tree<T, Alloc>::nodeptr Tree<T, Alloc>::add(nodeptr father, const T& node) {
			if (!father) Scriptforge::ErrCode::throwError(Scriptforge::ErrCode::ErrCode::TreeEmptyNode, __func__, m_lang);
            nodeptr newnode = create_node(node);
            newnode->father = father;
            father->children.push_back(newnode);
            return newnode;
        }

        //设置语言
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        void Tree<T, Alloc>::setLang(Scriptforge::Lang lang) {
            m_lang.setLocale(lang.getLanguageName());
        }

        //返回语言
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        Scriptforge::Lang Tree<T, Alloc>::getLang() const noexcept {
            return m_lang;
        }


        //返回分配器
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        typename Tree<T, Alloc>::allocator_type Tree<T, Alloc>::getAllocator() const noexcept {
            return alloc_;
        }

        //创建新节点
        template<typename T, typename Alloc>
            requires TreeRequires<T>
        template<typename U>
        typename Tree<T, Alloc>::nodeptr
            Tree<T, Alloc>::create_node(U&& value) {
            using node_allocator = typename std::allocator_traits<Alloc>
                ::template rebind_alloc<TreeNode>;
            using node_alloc_traits = std::allocator_traits<node_allocator>;
            node_allocator node_alloc(alloc_);
            TreeNode* raw = node_alloc.allocate(1);
            try {
                node_alloc_traits::construct(node_alloc, raw, std::forward<U>(value));
            }
            catch (...) {
                node_alloc.deallocate(raw, 1);
                throw;
            }
            return nodeptr(raw, [&node_alloc](TreeNode* p) noexcept {
                if (p) {
                    node_alloc_traits::destroy(node_alloc, p);
                    node_alloc.deallocate(p, 1);
                }
                });
        }*/
    }
}