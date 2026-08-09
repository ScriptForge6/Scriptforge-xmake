target("scriptforge_lib")
	-- set_kind("static")
    set_kind("binary")
	add_cxxflags({
        "/permissive-",
        "/Zc:preprocessor",
        "/Zc:templateScope"
    }, {tools = {"cl"}})



    add_packages("nlohmann_json")
    add_packages("utfcpp")

    set_pcxxheader("Scriptforge.Pch.hpp")

    add_files("Scriptforge.ixx")
    add_files("Scriptforge.*.ixx")
    add_files("*.cpp")
target_end()