target("scriptforge_lib")
	set_kind("static")
	add_cxxflags({
        "/permissive-",
        "/Zc:preprocessor",
        "/Zc:templateScope"
    }, {tools = {"cl"}})

    if IS_STATIC then
        add_defines("BUILD_STATIC")
    end
    if IS_MODULE then
        add_defines("SCRIPTFORGE_MODULE")
    else
        add_defines("SCRIPTFORGE_HEAD")
    end

    set_pcxxheader("Scriptforge.Pch.hpp")

    add_files("Scriptforge.ixx", {public = true})
    add_files("Scriptforge.*.ixx", {public = true})
    add_files("*.cpp")
target_end()