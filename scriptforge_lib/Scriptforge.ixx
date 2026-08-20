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
 * @file Scriptforge.ixx
 * @brief 定义了 `Scriptforge` 模块，并导入了该模块下的多个子模块。
 * @author Scriptforge
 * @date 2026/3/29
 */

module;

#include <chrono>
#include <string_view>

export module Scriptforge;

export import Scriptforge.AntiDebug;
export import Scriptforge.Err;
export import Scriptforge.Log;
export import Scriptforge.Tree;
export import Scriptforge.Version;
export import Scriptforge.Msg;
export import Scriptforge.ErrCode;
export import Scriptforge.ErrCode.throwError;
export import Scriptforge.Local;
export import Scriptforge.ThreadError;
export import Scriptforge.LanguageCode;
export import Scriptforge.StringConversion;
export import Scriptforge.RingBuffer;


constexpr std::string_view ProjectName{ SCRIPT_NAME };
constexpr std::string_view ProjectVersion{ VERSION };
constexpr std::string_view ProjectContributor{ AUTHOR };
constexpr std::string_view ProjectLicense{ LICENSE };
constexpr std::chrono::year ProjectStartYear{ START_YEAR };
namespace Scriptforge {
		inline namespace Version {
			export Scriptforge::Version::VersionInfo versionInfo{
				ProjectName,
				ProjectVersion,
				ProjectContributor,
				ProjectLicense,
				ProjectStartYear
			};

	}

}
