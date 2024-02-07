workspace "PathFindingAI"
    architecture "x64"

    configurations { "Debug", "Release" }

    filter { "system:windows" }
        systemversion "10.0.17763.0"

    filter {}

    filter { "system:windows" }
        defines { "WINDOWS" }

    filter { "system:linux" }
        defines { "LINUX" }

    filter {}

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "RELEASE" }
		optimize "On"

	filter {}

project "PathFindingAI"
    kind "ConsoleApp"

    cppdialect "C++11"

    SDL_Lib = "C:/SDL_Lib/"
	SDL_Librev = "C:\\SDL_Lib\\"

    includedirs
    {
        "src",
        SDL_Lib .. "SDL2/include",
        SDL_Lib .. "SDL2_image/include",
        SDL_Lib .. "SDL2_ttf/include",
    }

    libdirs
    {
        SDL_Lib .. "SDL2/lib/x64",
        SDL_Lib .. "SDL2_image/lib/x64",
        SDL_Lib .. "SDL2_ttf/lib/x64",
    }

    links
    {
        "SDL2.lib",
        "SDL2_image.lib",
        "SDL2_ttf.lib",
    }
    

    debugenvs
    {
		"PATH=",
        SDL_Librev .. "SDL2\\lib\\x64;",
        SDL_Librev .. "SDL2_image\\lib\\x64;",
        --SDL_Lib .. "SDL2_ttf/lib/x64",
    }

	-- PATH=C:/SDL_Lib/SDL2/lib/x64;C:/SDL_Lib/SDL2_image/lib/x64;C:/SDL_Lib/SDL2_ttf/lib/x64

    files { "src/**.cpp", "src/**.h", }

	filter { "configurations:Debug" }
		targetdir "output/bin/debug"
		objdir "output/obj/debug"

	filter { "configurations:Release" }
		targetdir "output/bin/release"
		objdir "output/obj/release"

	filter {}

    print(_OPTIONS["SDL_dir"])




newoption {
    trigger = "SDL_dir",
    description = "ma che ne sacciu"
}

newaction {
	trigger = "clean",
	description = "clean the software",
	execute = function()
		print("clean the build...")
		
		-- Clean the directory and files for Linux build --
		os.remove("./**.mk")
		os.remove("./**.project")
		os.remove("./**.txt")
		os.remove("./**.workspace")
		os.remove("./Makefile")
		os.rmdir("./output")
		
		-- Clean the directory and files for Windows build --
		os.remove("**.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")		
		os.remove("**.vcxproj.user")		
		
		print("done.")
	end
}