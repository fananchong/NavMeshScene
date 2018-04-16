workspace "NavMeshScene"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    targetdir "../bin/%{cfg.buildcfg}"
    language "C++"
    includedirs {
        "..",
        "../Detour/Include",
    }
    flags {
        "C++11",
        "StaticRuntime",
    }

    filter "configurations:Debug"
    defines { "_DEBUG" }
    flags { "Symbols" }
    libdirs { }
    filter "configurations:Release"
    defines { "NDEBUG" }
    libdirs { }
    optimize "On"
    filter { }  
    
    
    
project "NavMeshScene"
    kind "ConsoleApp"
    targetname "NavMeshScene"
    files {
        "../*.h",
        "../*.cpp",
        "../Detour/**",
    }
    