workspace "Game"
	configurations { "Debug", "Release" }
	startproject "GameCode"
	architecture "x64"
	characterset "MBCS"
	language "C++"
	buildoptions { "/Gm" } -- minimal rebuild
	
-- DEngine library
project "DEngine"
	location "DEngine"
	kind "StaticLib"
	includedirs "$(ProjectDir)"
	defines {"_CRT_SECURE_NO_WARNINGS"}
	buildoptions { "/sdl" } -- code generation for uninitialized variable
	
	files 
	{ 
		"DEngine/**.h", 
		"DEngine/**.cpp",
		"DEngine/**.hlsl" 
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		targetdir "Build/DEngine/Debug"
		objdir "Intermediate/DEngine/Debug"
		flags { "Symbols" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "Full"
		targetdir "Build/DEngine/Release"
		objdir "Intermediate/DEngine/Release"

-- Actual game code	  
project "GameCode"
	location "GameCode"
	kind "WindowedApp"
	links { "DEngine", "DXGI", "dxguid" }
	includedirs "$(ProjectDir);$(SolutionDir);$(SolutionDir)DEngine"
	flags { "WinMain" }
	defines {"_CRT_SECURE_NO_WARNINGS"}
	
	files 
	{ 
		"GameCode/**.h", 
		"GameCode/**.cpp",
		"GameCode/**.hlsl"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		targetdir "Build/GameCode/Debug"
		objdir "Intermediate/GameCode/Debug"
		flags { "Symbols" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "Full"
		targetdir "Build/GameCode/Release"
		objdir "Intermediate/GameCode/Release"