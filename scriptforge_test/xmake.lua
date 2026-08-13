target(TEST_NAME)
	add_cxxflags({
        "/permissive-",
        "/Zc:preprocessor",
        "/Zc:templateScope"
        
    }, {tools = {"cl"}})

    if is_plat("windows") then
        add_ldflags("/WHOLEARCHIVE:gtest_main.lib", {tools={"link"}, force = true})
    end

    add_includedirs(LIB_DIR)

    add_packages("gtest", {components = {"gtest","gtest_main"}})
    add_packages("nlohmann_json")
    add_packages("utfcpp")

    add_files(string.vformat("%s/*.cpp", LIB_DIR))
	add_files(string.vformat("%s/*.ixx", LIB_DIR))
    add_files(string.vformat("%s/*.cpp", TEST_DIR))
    add_files(string.vformat("%s/*.ixx", TEST_DIR))
target_end()

