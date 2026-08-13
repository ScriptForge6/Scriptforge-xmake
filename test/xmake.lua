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
    
target_end()

