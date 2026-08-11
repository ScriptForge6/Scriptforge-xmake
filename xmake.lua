

SCRIPT_NAME = "Scriptforge-xmake"
VERSION = "0.1.2-dev-withBug"
LICENSE = "Apache License 2.0"
START_YEAR = 2025
AUTHOR = "Scriptforge"
IS_STATIC = true
IS_MODULE = true

set_project(SCRIPT_NAME)
set_version(VERSION)

add_rules("mode.debug", "mode.release")

set_policy("package.install_locally", true)
add_requires("nlohmann_json >=3.11.0")
add_requires("utfcpp >=4.0.6")
add_requires("gtest >=1.15.0")

add_packages("nlohmann_json")
add_packages("utfcpp")

set_defaultmode("mode.debug")

add_defines(string.format("SCRIPT_NAME=\"%s\"", SCRIPT_NAME))
add_defines(string.format("VERSION=\"%s\"", VERSION))
add_defines(string.format("LICENSE=\"%s\"", LICENSE))
add_defines(string.format("START_YEAR=%d", START_YEAR))
add_defines(string.format("AUTHOR=\"%s\"", AUTHOR))

set_languages("c++23")
set_warnings("allextra")

includes("scriptforge_lib/xmake.lua")
includes("test/xmake.lua")

task("t")
    set_category("plugin")
    set_menu {
        usage = "xmake t [filter]",
        description = "Quick run gtest cases for scriptforge_test",
        options = {
            {nil, "list", "k", nil, "List all gtest test cases."}
        }
    }
    on_run(function ()
        import("core.base.option")

        local do_list = option.get("list")
        local argv = option.get("argv")

        -- 直接shell调用编译，不去拿project target对象
        os.exec("xmake build scriptforge_test")

        local cmd = {"xmake run", "scriptforge_test", "--"}
        if do_list then
            table.insert(cmd, "--gtest_list_tests")
        elseif #argv > 0 then
            local filter = table.concat(argv, ":")
            table.insert(cmd, "--gtest_filter="..filter)
        end
        os.execv("cmd.exe", {"/c", table.concat(cmd, " ")})
    end)
task_end()
