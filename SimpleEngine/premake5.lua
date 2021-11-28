workspace "SimpleEngine" --解决方案名称
    architecture "x86" --编译平台 只编64位--(x86,x86_64,ARM)

    configurations 
    {
        "Debug",
        "Release",
        --"Dist"
    }
--临时变量 定义 输出目录
--详细的所有支持的tokens 可参考 [https://github.com/premake/premake-core/wiki/Tokens]
--outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--添加GLFW的引用
--Includedirs = {}
--Includedirs["GLFW"] = "VEngine/vendor/GLFW/include"

function includeGLFW()
    includedirs "include"
end

function linkGLFW()
    libdirs "lib"
    links "glfw3"
    links "opengl32"
end

--include "VEngine/vendor/GLFW"

project "SimpleEngine" --项目名称
    location "SimpleEngine" --相对路径
    kind "ConsoleApp" --表明该项目是exe
    language "c++"

    --targetdir ("bin/" .. outputdir .. "/%{prj.name}")--输出目录
    --objdir ("bin-int/" .. outputdir .. "/%{prj.name}")--中间临时文件的目录

    includeGLFW()
    linkGLFW()

	--pchheader "VEnginePCH.h"
	--pchsource "VEngine/src/VEnginePCH.cpp"

    files--该项目的文件
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs--附加包含目录
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
		--"%{Includedirs.GLFW}"
    }

	links
	{
		"glfw3",
		"opengl32.lib"
	}

    filter "system:windows"--windows平台的配置
        cppdialect "c++17"
        staticruntime "On"
        systemversion "10.0.17134.0"

        defines --预编译宏
        {
            "VENGINE_BUILD_DLL",
            "VENGINE_PLATFORM_WINDOWS"
        }

        --postbuildcommands -- build后的自定义命令
        --{
            --("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --拷贝引擎dll库到sanbox.exe的同一目录下去
        --}

    filter "configurations:Debug"
        defines "VENGINE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "VENGINE_RELEASE"
        runtime "Release"
        optimize "on"

    --filter "configurations:Dist"
        --defines "VENGINE_DIST"
        --runtime "Release"
        --optimize "on"
