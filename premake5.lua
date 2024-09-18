require "cmake"

workspace "MitoEngine2D"
    platforms { "Windows" }
    configurations { "Debug", "Release" }
    flags "MultiProcessorCompile"

project "MitoEngine2D"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    
    targetdir "bin/%{cfg.system}-%{cfg.buildcfg}/%{cfg.architecture}"
    objdir "bin-int/%{cfg.system}-%{cfg.buildcfg}/%{cfg.architecture}"

    files 
	{ 	
		"src/**.cpp",
		"src/**.h",
		"src/**.hpp",
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		architecture "x86_64"
		
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
		architecture "x86_64"
    
    filter "platforms:Windows"
        system "Windows"
        architecture "x86_64"

    filter "system:windows"
        system "Windows"

        includedirs
        { 
            "libs/SDL2/include",
			"libs/SDL2_image/include",
			"libs/SDL2_mixer/include",
			"libs/SDL2_ttf/include",
			"libs/glm",
			"libs/lua",
            "libs/sol"
        }

        libdirs
        {
            "libs/SDL2/lib/x64",
			"libs/SDL2_image/lib/x64",
			"libs/SDL2_mixer/lib/x64",
			"libs/SDL2_ttf/lib/x64",
			"libs/lua"
        }

        links 
        { 
            "SDL2main", 
            "SDL2",
			"SDL2_image",
			"SDL2_mixer",
			"SDL2_ttf",
			"liblua53.a"
        }
        
        postbuildcommands {
            "{COPY} %{path.getabsolute('libs/SDL2/lib/x64/SDL2.dll')} %{cfg.targetdir}",
			"{COPY} %{path.getabsolute('libs/SDL2_image/lib/x64/SDL2_image.dll')} %{cfg.targetdir}",
			"{COPY} %{path.getabsolute('libs/SDL2_mixer/lib/x64/SDL2_mixer.dll')} %{cfg.targetdir}",
			"{COPY} %{path.getabsolute('libs/SDL2_ttf/lib/x64/SDL2_ttf.dll')} %{cfg.targetdir}",
            "{COPY} %{path.getabsolute('libs/lua/lua53.dll')} %{cfg.targetdir}",
			"{COPY} %{path.getabsolute('libs/lua/liblua53.a.lib')} %{cfg.targetdir}"
        }