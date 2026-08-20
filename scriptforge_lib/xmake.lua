add_moduledirs(PROJECTDIR)

target(LIB_NAME)
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
    
    --add_deps(PRE_NAME)
    

    
    add_packages("nlohmann_json")
    add_packages("utfcpp")

    set_pcxxheader(path.join(LIB_DIR, "Scriptforge.Pch.hpp"))

    local local_ANTIDEB_RAND_PATH = ANTIDEB_RAND_PATH

    on_load(function (target)
        import("pre", {always_build = true})
        pre.run(local_ANTIDEB_RAND_PATH)
    end)
    
    add_files(path.join(LIB_DIR, "*.ixx"))
    add_files(path.join(LIB_DIR, "*.cpp"))
target_end()