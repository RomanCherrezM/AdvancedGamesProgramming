
-- https://github.com/jpcy/bgfx-minimal-example/blob/master/premake5.lua

local BUILD_DIR = path.join("build", _ACTION)
if _OPTIONS["cc"] ~= nil then
	BUILD_DIR = BUILD_DIR .. "_" .. _OPTIONS["cc"]
end

local GLFW_DIR = "deps/glfw"
local GLAD_DIR = "deps/glad"
local GLM_DIR = "deps/glm"
local RF3_DIR = "deps/reactphysics3d"
local PX_DIR = "deps/px"
local TOBJ_DIR = "deps/TinyOBJ"
local STB_DIR = "deps/stb"
local ENET_DIR = "deps/enet"
local LUA_DIR = "deps/lua"
local SOLOUD_DIR = "deps/soloud"

solution "AloeEngine"
	location(BUILD_DIR)
	startproject "AloeEngine"
	configurations { "Release", "Debug" }
	if os.is64bit() and not os.istarget("windows") then
		platforms "x86_64"
	else
		platforms { "x86", "x86_64" }
	end
	filter "configurations:Release"
		defines "NDEBUG"
		optimize "Full"
		targetdir ("bin/Release")
		objdir ("build/Release")
	filter "configurations:Debug*"
		defines "_DEBUG"
		optimize "Debug"
		targetdir ("bin/Debug")
    	objdir ("build/Debug")
		symbols "On"
	filter "platforms:x86"
		architecture "x86"
	filter "platforms:x86_64"
		architecture "x86_64"
	filter "system:macosx"
		xcodebuildsettings {
			["MACOSX_DEPLOYMENT_TARGET"] = "10.9",
			["ALWAYS_SEARCH_USER_PATHS"] = "YES", -- This is the minimum version of macos we'll be able to run on
		};
	
project "AloeServer"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	includedirs
	{
		"include",
		
		GLM_DIR,

		path.join(TOBJ_DIR,"include/"),
		path.join(GLFW_DIR,"include/"),
		path.join(GLAD_DIR,"include/"),
		path.join(RF3_DIR,"include/"),
		path.join(STB_DIR),
		path.join(PX_DIR),
		path.join(ENET_DIR,"include/"),
		path.join(LUA_DIR,"src/"),
		path.join(SOLOUD_DIR,"include/"),

	}

	files 
	{		
		"src/server_user_main.cpp",
		
		"src/Core/**.cpp",
		"src/Game/**.cpp",
		"src/ECS/**.cpp",
		"src/Graphics/**.cpp",
		"include/**.h",
		"src/ECS/Systems/**.cpp",

		path.join(GLM_DIR,"*.hpp"),
		path.join(STB_DIR,"*.h"),
		path.join(PX_DIR,"*.h"),
		path.join(TOBJ_DIR,"include/*.h"),
		path.join(TOBJ_DIR,"src/*.cc"),
		path.join(ENET_DIR,"include/enet/*.h"),
		path.join(LUA_DIR,"src/*.c"),
		path.join(LUA_DIR,"src/*.h"),
		path.join(SOLOUD_DIR,"include/*.h"),
		path.join(SOLOUD_DIR,"src/**.cpp"),
		path.join(SOLOUD_DIR,"src/**.c"),
		path.join(SOLOUD_DIR,"src/**.h"),

	}

	links { "glfw","glad","reactphysics3d","enet"}
	defines {"WITH_MINIAUDIO"}


project "AloeClient"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	includedirs
	{
		"include",
		
		GLM_DIR,

		path.join(TOBJ_DIR,"include/"),
		path.join(GLFW_DIR,"include/"),
		path.join(GLAD_DIR,"include/"),
		path.join(RF3_DIR,"include/"),
		path.join(STB_DIR),
		path.join(PX_DIR),
		path.join(ENET_DIR,"include/"),
		path.join(LUA_DIR,"src/"),
		path.join(SOLOUD_DIR,"include/"),

	}

	files 
	{		
		"src/client_user_main.cpp",
		
		"src/Core/**.cpp",
		"src/ECS/**.cpp",
		"src/Graphics/**.cpp",
		"include/**.h",
		"src/ECS/Systems/**.cpp",

		path.join(GLM_DIR,"*.hpp"),
		path.join(STB_DIR,"*.h"),
		path.join(PX_DIR,"*.h"),
		path.join(TOBJ_DIR,"include/*.h"),
		path.join(TOBJ_DIR,"src/*.cc"),
		path.join(ENET_DIR,"include/enet/*.h"),
		path.join(LUA_DIR,"src/*.c"),
		path.join(LUA_DIR,"src/*.h"),
		path.join(SOLOUD_DIR,"include/*.h"),
		path.join(SOLOUD_DIR,"src/**.cpp"),
		path.join(SOLOUD_DIR,"src/**.c"),
		path.join(SOLOUD_DIR,"src/**.h"),
	}

	links { "glfw","glad","reactphysics3d","enet"}
	defines {"WITH_MINIAUDIO"}

project "GraphicsProgramming"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	includedirs
	{
		"include",
		
		GLM_DIR,

		path.join(TOBJ_DIR,"include/"),
		path.join(GLFW_DIR,"include/"),
		path.join(GLAD_DIR,"include/"),
		path.join(RF3_DIR,"include/"),
		path.join(STB_DIR),
		path.join(PX_DIR),
		path.join(ENET_DIR,"include/"),
		path.join(LUA_DIR,"src/"),
		path.join(SOLOUD_DIR,"include"),

	}

	files 
	{		
		"src/graphics_user_main.cpp",
		
		"src/Core/**.cpp",
		"src/ECS/**.cpp",
		"src/Graphics/**.cpp",
		"include/**.h",
		"src/ECS/Systems/**.cpp",

		path.join(GLM_DIR,"*.hpp"),
		path.join(STB_DIR,"*.h"),
		path.join(PX_DIR,"*.h"),
		path.join(TOBJ_DIR,"include/*.h"),
		path.join(TOBJ_DIR,"src/*.cc"),
		path.join(ENET_DIR,"include/enet/*.h"),
		path.join(LUA_DIR,"src/*.c"),
		path.join(LUA_DIR,"src/*.h"),
		path.join(SOLOUD_DIR,"include/*.h"),
		path.join(SOLOUD_DIR,"src/**.cpp"),
		path.join(SOLOUD_DIR,"src/**.c"),
		path.join(SOLOUD_DIR,"src/**.h"),
	}

	links { "glfw","glad","reactphysics3d","enet"}
	defines {"WITH_MINIAUDIO"}

project "glad"

	kind "StaticLib"
	language "C"

	files {
		path.join(GLAD_DIR,"include/glad/*.h"),
		path.join(GLAD_DIR,"include/KHR/*.h"),
		path.join(GLAD_DIR,"src/*.c")
	}

	includedirs {
		path.join(GLAD_DIR,"include")
	}

project "enet"

	kind "StaticLib"
	language "C"

	files {
		path.join(ENET_DIR,"*.c"),
		path.join(ENET_DIR,"include/enet/*.h")
	}

	includedirs {
		path.join(ENET_DIR,"include/")
	}

	links {"ws2_32","winmm"}

project "glm"

	kind "StaticLib"
	language "C"

	files {
		path.join(GLM_DIR,"**.hpp"),
	}

	includedirs {
		GLM_DIR
	}

project "reactphysics3d"

		kind "StaticLib"
		language "C"

		files {
			path.join(RF3_DIR,"src/**.cpp"),
			path.join(RF3_DIR,"include/**.h"),
		}

		includedirs {
			path.join(RF3_DIR,"include/"),
		}
	

project "glfw"
	kind "StaticLib"
	language "C"
	files
	{
		path.join(GLFW_DIR, "include/GLFW/*.h"),
		path.join(GLFW_DIR, "src/context.c"),
		path.join(GLFW_DIR, "src/egl_context.*"),
		path.join(GLFW_DIR, "src/init.c"),
		path.join(GLFW_DIR, "src/input.c"),
		path.join(GLFW_DIR, "src/internal.h"),
		path.join(GLFW_DIR, "src/monitor.c"),
		path.join(GLFW_DIR, "src/osmesa_context.*"),
		path.join(GLFW_DIR, "src/vulkan.c"),
		path.join(GLFW_DIR, "src/window.c"),
	}
	includedirs { path.join(GLFW_DIR, "include") }
	filter "system:windows"
		defines "_GLFW_WIN32"
		files
		{
			path.join(GLFW_DIR, "src/win32_*.*"),
			path.join(GLFW_DIR, "src/wgl_context.*")
		}
	filter "system:linux"
		defines "_GLFW_X11"
		files
		{
			path.join(GLFW_DIR, "src/glx_context.*"),
			path.join(GLFW_DIR, "src/linux*.*"),
			path.join(GLFW_DIR, "src/posix*.*"),
			path.join(GLFW_DIR, "src/x11*.*"),
			path.join(GLFW_DIR, "src/xkb*.*")
		}
	filter "system:macosx"
		defines "_GLFW_COCOA"
		files
		{
			path.join(GLFW_DIR, "src/cocoa_*.*"),
			path.join(GLFW_DIR, "src/posix_thread.h"),
			path.join(GLFW_DIR, "src/nsgl_context.h"),
			path.join(GLFW_DIR, "src/egl_context.h"),
			path.join(GLFW_DIR, "src/osmesa_context.h"),

			path.join(GLFW_DIR, "src/posix_thread.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/egl_context.c"),
			path.join(GLFW_DIR, "src/nsgl_context.m"),
			path.join(GLFW_DIR, "src/osmesa_context.c"),                       
		}

	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"