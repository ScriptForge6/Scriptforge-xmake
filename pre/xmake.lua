target(PRE_NAME)
	set_kind("binary")

	add_defines(string.vformat("RANDOM_FILE=R\"(%s/%s)\"", LIB_DIR, ANTIDEB_RAND_FILE))

    add_files(string.vformat("%s/main.cpp", PRE_DIR))
target_end()