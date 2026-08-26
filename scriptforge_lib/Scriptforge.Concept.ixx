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
 * @file Scriptforge.Concept.ixx
 * @brief 实现了一些预定的概念
 * @author Scriptforge
 * @date 2026/8/26
*/

module;

#include "Scriptforge.Define.hpp"
#include "Scriptforge.Pch.hpp"

export module Scriptforge.Concept;

_SF_CONCEPT_BEGIN

template<bool... Bs>
constexpr size_t countTrue = (0 + ... + (Bs ? 1 : 0));

export
template<bool... Bs>
concept exactlyOne = countTrue<Bs...> == 1;

export
template<bool... Bs>
concept atLeastOne = countTrue<Bs...> >= 1;

export
template<bool... Bs>
concept noneTrue = countTrue<Bs...> == 0;

export
template<bool... Bs>
concept atMostOne = countTrue<Bs...> <= 1;


_SF_CONCEPT_END