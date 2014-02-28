solution "cutf_test"
	configurations { "Debug", "Release" }
	platforms {"Native", "x32", "x64"}
	flags {"NoPCH", "EnableSSE2", "ExtraWarnings", "NoMinimalRebuild"}
	location  ("build/" .. _ACTION)
	objdir    "build/obj"
	targetdir "bin"
	debugdir  "bin"

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix (".debug")
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
	configuration {"vs*"}
		defines {"_CRT_SECURE_NO_WARNINGS"}

	project "cutf_test"
		uuid     "3d90ce08-b1c7-4423-99db-9a8201221910"
		kind     "ConsoleApp"
		language "C"
		files {
			"cutf.h",
			"test.c"
		}
