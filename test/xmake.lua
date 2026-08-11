

target("scriptforge_test")
	add_cxxflags({
        "/permissive-",
        "/Zc:preprocessor",
        "/Zc:templateScope"
        
    }, {tools = {"cl"}})

    if is_plat("windows") then
        add_ldflags("/WHOLEARCHIVE:gtest_main.lib", {tools={"link"}, force = true})
    end



    add_deps("scriptforge_lib")
    add_includedirs("../scriptforge_lib")

    add_packages("gtest", {components = {"gtest","gtest_main"}})

	add_files("*.cpp")
    add_files("*.ixx")
    --after_build(function (target)
    --    print("$(builddir)/windows/x64/release")
    --    os.execv("./$(builddir)/windows/x64/release/scriptforge_test.exe", "")
    --end)
    
target_end()

add_tests("gtest_suite", {
    target = "scriptforge_test",       -- 指定测试可执行目标
    -- 传给gtest的命令行参数
    args = {
        "--gtest_color=yes",
        "--gtest_output=xml:$(buildir)/gtest-report.xml" -- gtest原生xml报告输出
    },
    -- 工作目录，很多gtest读取测试资源需要
    workingdir = "$(projectdir)",
    -- 开启解析gtest的xml报告，xmake会读取xml，同步显示每个case成功失败
    parse_output = "gtest",
})