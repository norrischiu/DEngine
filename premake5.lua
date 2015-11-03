workspace "Game"
   configurations { "Debug", "Release" }
   location "../Game"

project "Game"
   kind "WindowedApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   includedirs "DirectXTK"

   files { "**.h", "**.cpp","**.hlsl" }

   filter "configurations:Debug"
      defines { "DEBUG" }

   filter "configurations:Release"
      optimize "On"