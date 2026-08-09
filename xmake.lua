SCRIPT_NAME = "Scriptforge-xmake"
VERSION = "0.1.2-dev-withBug"
LICENSE = "Apache License 2.0"
START_YEAR = 2025
AUTHOR = "Scriptforge"
IS_STATIC = true
IS_MODULE = true

set_project(SCRIPT_NAME)
set_version(VERSION)

set_policy("package.install_locally", true)
add_requires("nlohmann_json >=3.11.0")
add_requires("utfcpp >=4.0.6")
add_requires("gtest >=1.15.0")

set_languages("c++23")
set_warnings("all")
set_optimize("none")

includes("lib/xmake.lua")
includes("test/xmake.lua")