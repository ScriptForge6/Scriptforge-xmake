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
PROJECTDIR = os.projectdir()
PRE_DIR = path.join(PROJECTDIR, PRE_NAME)
LIB_DIR = path.join(PROJECTDIR, LIB_NAME)
TEST_DIR = path.join(PROJECTDIR, TEST_NAME)
PRE_LUA = path.join(PRE_DIR, "xmake.lua")
LIB_LUA = path.join(LIB_DIR, "xmake.lua")
TEST_LUA = path.join(TEST_DIR, "xmake.lua")

ANTIDEB_RAND_FILE = "Scriptforge.AntiDebug.Random.ixx"

ANTIDEB_RAND_PATH = path.join(LIB_DIR, ANTIDEB_RAND_FILE)