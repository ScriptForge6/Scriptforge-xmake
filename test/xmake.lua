target("scriptforge_test")
	add_cxxflags({
        "/permissive-",
        "/Zc:preprocessor",
        "/Zc:templateScope"
    }, {tools = {"cl"}})

    add_deps("scriptforge_lib")
    add_includedirs("../scriptforge_lib")

	add_files("*.cpp")
target_end()