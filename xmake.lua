SCRIPT_NAME = "Scriptforge-xmake"
VERSION = "0.1.2-dev-withBug"
LICENSE = "Apache License 2.0"
START_YEAR = 2025
AUTHOR = "Scriptforge"
IS_STATIC = true
IS_MODULE = true
PRE_NAME = "pre"
LIB_NAME = "scriptforge_lib"
TEST_NAME = "scriptforge_test"
PRE_DIR = string.vformat("$(projectdir)/%s", PRE_NAME)
LIB_DIR = string.vformat("$(projectdir)/%s", LIB_NAME)
TEST_DIR = string.vformat("$(projectdir)/%s", TEST_NAME)
PRE_LUA = PRE_DIR .. "/xmake.lua"
LIB_LUA = LIB_DIR .. "/xmake.lua"
TEST_LUA = TEST_DIR .. "/xmake.lua"

ANTIDEB_RAND_FILE = "Scriptforge.AntiDebug.Random.ixx"

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

includes(PRE_LUA)
includes(LIB_LUA)
includes(TEST_LUA)