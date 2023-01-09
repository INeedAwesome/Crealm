project "Crealm"
   kind "WindowedApp"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"
   dpiawareness "HighPerMonitor"

   files { "src/**.h", "src/**.c" }

   includedirs
   {
   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "CR_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "CR_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "CR_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "CR_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"