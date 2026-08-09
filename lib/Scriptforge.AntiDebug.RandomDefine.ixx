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
 * @file Scriptforge.AntiDebug.RandomDefine.ixx
 * @brief 用于生成随机数的头文件。
 * @author Scriptforge
 * @date  2026/4/25
*/

export module Scriptforge.AntiDebug.RandomDefine;
import Scriptforge.AntiDebug.Random; // This file will be created when you build the project or run the project "GenerateRandom".
#define CAT2(a, b) a##b
#define CAT(a, b)  CAT2(a, b)
#define ADNS    CAT(adns, NS_RANDOM)
#define ADCL    CAT(ad, CLS_RANDOM)
#define F1      CAT(f, F1_RANDOM)
#define F2      CAT(f, F2_RANDOM)
#define F3      CAT(f, F3_RANDOM)
#define F4      CAT(f, F4_RANDOM)
#define F5      CAT(f, F5_RANDOM)
#define F6      CAT(f, F6_RANDOM)
#define V1      CAT(v, V1_RANDOM)
#define V2      CAT(v, V2_RANDOM)
#define V3      CAT(v, V3_RANDOM)
#define V4      CAT(v, V4_RANDOM)