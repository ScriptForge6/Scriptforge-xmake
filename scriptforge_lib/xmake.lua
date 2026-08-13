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
    
    add_deps(PRE_NAME)

    add_packages("nlohmann_json")
    add_packages("utfcpp")

    set_pcxxheader("Scriptforge.Pch.hpp")

    local local_PRE_NAME = PRE_NAME

    before_build(function (target)
        linkdir = target._INFO._INFO.linkdirs
        os.execv(linkdir .. "/" .. local_PRE_NAME, {})
    end)
    
    add_files(string.vformat("%s/*.ixx", LIB_DIR))
    add_files(string.vformat("%s/*.cpp", LIB_DIR))
target_end()