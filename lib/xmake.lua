

target("scriptforge_lib")
	set_kind("static")
	add_cxxflags({
        "/permissive-",
        "/Zc:preprocessor",
        "/Zc:templateScope"
    }, {tools = {"cl"}})

    add_packages("nlohmann_json")
    add_packages("utfcpp")

    add_includedirs("../thirdParty")

    set_pcxxheader("Scriptforge.Pch.hpp")

    add_files("Scriptforge.*.ixx")
target_end()