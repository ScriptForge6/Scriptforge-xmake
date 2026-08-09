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
 * @file Scriptforge.Version.ixx
 * @brief 定义了 Scriptforge::Version 模块，包含 VersionInfo 类，用于管理项目版本、版权、许可证信息。
 * @author Scriptforge
 * @date 2026/3/29
 */
module;

#include "Scriptforge.Pch.hpp"

export module Scriptforge.Version;

namespace Scriptforge {
    inline namespace Version {
        export class VersionInfo {
        public:
            // 构造函数
            VersionInfo(
                std::string_view projectName,
                std::string_view projectVersion,
                std::string_view projectContributor,
                std::string_view projectLicense,
                std::chrono::year projectStartYear
            ) noexcept;

            // 版本信息
            std::string getVersion() const noexcept;
            std::string getCopyright() const noexcept;
            std::string getYearInterval() const noexcept;
            std::string getFullInfo() const noexcept;

        private:
            static std::chrono::year getCompileYear() noexcept;

            std::string_view m_projectName;
            std::string_view m_projectVersion;
            std::string_view m_projectContributor;
            std::string_view m_projectLicense;
            std::chrono::year m_projectStartYear;
            std::chrono::year m_projectCurrentYear;
        };

        // 支持直接打印
        export std::ostream& operator<<(std::ostream& os, const VersionInfo& info) {
            return os << info.getFullInfo();
        }
    }
}

namespace Scriptforge {
    inline namespace Version {
        std::chrono::year VersionInfo::getCompileYear() noexcept {
            constexpr std::string_view date = __DATE__;
            // __DATE__ 格式：MMM DD YYYY 共 11 字符
            constexpr auto yearPos = date.size() - 4;
            const int year = std::stoi(std::string(date.substr(yearPos)));
            return std::chrono::year{ year };
        }

        VersionInfo::VersionInfo(
            std::string_view projectName,
            std::string_view projectVersion,
            std::string_view projectContributor,
            std::string_view projectLicense,
            std::chrono::year projectStartYear
        ) noexcept
            : m_projectName(projectName)
            , m_projectVersion(projectVersion)
            , m_projectContributor(projectContributor)
            , m_projectLicense(projectLicense)
            , m_projectStartYear(projectStartYear)
            , m_projectCurrentYear(getCompileYear())
        {}

        std::string VersionInfo::getYearInterval() const noexcept {
            const int start = static_cast<int>(m_projectStartYear);
            const int current = static_cast<int>(m_projectCurrentYear);

            if (start >= current) {
                return std::to_string(start);
            }
            return std::to_string(start) + "-" + std::to_string(current);
        }

        std::string VersionInfo::getVersion() const noexcept {
            return std::string(m_projectName) + " version " + std::string(m_projectVersion);
        }

        std::string VersionInfo::getCopyright() const noexcept {
            return "Copyright " + getYearInterval() + " " + std::string(m_projectContributor)
                + "\nLicensed under the " + std::string(m_projectLicense);
        }

        std::string VersionInfo::getFullInfo() const noexcept {
            return getVersion() + "\n" + getCopyright();
        }
    }
}