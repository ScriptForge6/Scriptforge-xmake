target("scriptforge_test")
	add_cxxflags({
        "/permissive-",
        "/Zc:preprocessor",
        "/Zc:templateScope"
    }, {tools = {"cl"}})

	add_files("*.cpp")
target_end()