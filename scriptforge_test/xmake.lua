target(TEST_NAME)
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

    set_pcxxheader(path.join(LIB_DIR, "Scriptforge.Pch.hpp"))

    add_extrafiles(LANG_DIR)

    add_packages("gtest")
    add_packages("nlohmann_json")
    add_packages("utfcpp")

    

    local local_ANTIDEB_RAND_DIR = ANTIDEB_RAND_DIR

    on_load(function (target)
        import("pre", {always_build = true})
        pre.run(local_ANTIDEB_RAND_DIR)
    end)

    add_includedirs(LIB_DIR)
    add_includedirs(DETAIL_DIR)
    
    add_files(path.join(LIB_DIR, "*.ixx"))
    add_files(path.join(LIB_DIR, "*.cpp"))
    add_files(path.join(DETAIL_DIR, "*.ixx"))
    add_files(path.join(DETAIL_DIR, "*.cpp"))
    add_files(path.join(TEST_DIR, "*.ixx"))
    add_files(path.join(TEST_DIR, "*.cpp"))
target_end()

