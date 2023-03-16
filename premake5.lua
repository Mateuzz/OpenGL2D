workspace "Batch"

    configurations  {"Debug", "Release"}
    platforms {"Win64", "Win32", "linux32", "linux64"}

    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.architecturecfg}-%{cfg.buildcfg}"

    filter "configurations:Release"
        kind "WindowedApp"
        defines {"RELEASE"}
        optimize "On"

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines {"DEBUG"}
        symbols "Speed"

    filter {"platforms:Win64"}
        system "Windows"
        architecture "x86_64"

    filter {"platforms:Win32"}
        system "Windows"
        architecture "x86"

    filter {"platforms:linux32"}
        system "linux"
        architecture "x86"

    filter {"platforms:linux64"}
        system "linux"
        architecture "x86_64"

    filter {}
        links {"SDL2", "SDL2main", "GLEW", "OpenGL"} 

    project "Batch"
        files {"src/**.cpp", "src/**.h"}
        includedirs {"src/"}
        includedirs {"src/vendor"}
        targetname "Main"
        pchheader "src/vendor/glm/glm.hpp"
