workspace "NavMeshScene"
    configurations { "Debug", "Release" }
    platforms { "x32", "x64" }
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
    kind "StaticLib"
    targetname "NavMeshScene"
    files {
        "../*.h",
        "../*.cpp",
        "../Detour/**",
    }
    
project "example"
    kind "ConsoleApp"
    targetname "example"
    includedirs { "../*.h" }
    libdirs { "../bin" }
    links { "NavMeshScene" }
    files {
        "../example/*.h",
        "../example/*.cpp",
    }
    