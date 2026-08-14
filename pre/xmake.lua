includes("../define.lua")

target("pre")
	set_kind("binary")

	add_rules("module.shared")

    add_files("main.cpp")
target_end()