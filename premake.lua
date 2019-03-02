-- These are only used on Windows + Visual Studio ------------------------------
SDL_ROOT          = "/dev/SDL/"
SDL_LIB_ROOT      = SDL_ROOT .. "VisualC/SDL/"
SDLMAIN_LIB_ROOT  = SDL_ROOT .. "VisualC/SDLmain/"
--------------------------------------------------------------------------------

BUILD_DIR         = "build"

if _ACTION == "clean" then
	os.rmdir(BUILD_DIR)
end

solution "MonsterDefense"
	configurations { "Debug", "Release" }
	location (BUILD_DIR .. "/" .. _ACTION)
	
project "MonsterDefense"
	kind "WindowedApp"
	language "C++"
	location (BUILD_DIR .. "/" .. _ACTION)
	files {
		"./src/**.c*",
		"./src/**.h",
		"./lib/**.c*",
		"./lib/**.h",
	}
	includedirs {
		"./lib/stl/include",
		"./lib/eastl/include",
		"./lib/glew",
		"./lib/gwen",
		"./lib/stb",
	}
	defines {
		"DESKTOP",
		"SDL",
		"GLEW_STATIC",
	}
	debugdir "."
	
	---- PLATFORM SPECIFICS ----------------------------------------------------
	configuration "vs*"
		flags {
			"NoPCH",
			"NoMinimalRebuild"
		}
		buildoptions { "/MP" }
		links {
			"SDLmain",
			"SDL",
			"opengl32",
			"glu32",
		}
		defines {
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS"
		}
		includedirs {
			(SDL_ROOT .. "/include"),
		}
		
	configuration { "vs*", "Debug" }
		libdirs {
			(SDL_LIB_ROOT .. "Debug/"),
			(SDLMAIN_LIB_ROOT .. "Debug/"),
		}
		
	configuration { "vs*", "Release" }
		libdirs {
			(SDL_LIB_ROOT .. "Release/"),
			(SDLMAIN_LIB_ROOT .. "Release/"),
		}
		
	configuration "gmake"
		kind "ConsoleApp"
		buildoptions { "-Wall" }
		
	configuration { "windows", "gmake" }
		kind "WindowedApp"
		defines {
			"_GNU_SOURCE=1",
			"main=SDL_main",
		}
		links {
			"mingw32",
			"SDLmain",
			"SDL",
			"opengl32",
			"glu32",
		}
		linkoptions {
			"-static-libgcc",
			"-static-libstdc++",
		}
		
	configuration "macosx"
		files {
			"./lib/sdl-osx/**.m",
			"./lib/sdl-osx/**.c*",
			"./lib/sdl-osx/**.h",
		}
		links {
			"OpenGL.framework",
			"SDL.framework",
			"Cocoa.framework",
		}
		includedirs {
			"/Library/Frameworks/SDL.framework/Headers",
			"/System/Library/Frameworks/OpenGL.framework/Headers",
		}
		
	configuration "linux"
		links {
			"SDL",
			"GL",
			"GLU",
		}
	----------------------------------------------------------------------------
			
	configuration "Debug"
		defines {
			"DEBUG",
			"DEBUG_ASSERT_BREAK",
		}
		flags { "Symbols" }
	
	configuration "Release"
		defines {
			"NDEBUG",
		}
		flags { "Optimize" }
