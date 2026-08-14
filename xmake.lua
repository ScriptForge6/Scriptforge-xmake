includes("define.lua")

set_project(SCRIPT_NAME)
set_version(VERSION)

add_defines(string.format("SCRIPT_NAME=\"%s\"", SCRIPT_NAME))
add_defines(string.format("VERSION=\"%s\"", VERSION))
add_defines(string.format("LICENSE=\"%s\"", LICENSE))
add_defines(string.format("START_YEAR=%d", START_YEAR))
add_defines(string.format("AUTHOR=\"%s\"", AUTHOR))

add_rules("mode.debug", "mode.release")

set_policy("package.install_locally", true)
add_requires("nlohmann_json >=3.11.0")
add_requires("utfcpp >=4.0.6")
add_requires("gtest >=1.15.0")

set_defaultmode("mode.debug")

set_languages("c++23")
set_warnings("allextra")

includes(LIB_LUA)
includes(TEST_LUA)