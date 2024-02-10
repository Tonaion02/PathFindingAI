workspace "PathFindingAI"
	architecture "x64"

	-- set the language
	language "C++"

	-- set the cpp version
	cppdialect "C++17"

	-- all the configurations
    configurations { "Debug", "Release" }

	-- create src directory if don't exist
	if not os.isdir("src") then os.mkdir("src") end

	-- template main code
	mainFile = "int main()\n{\n	return 0;\n}"
	-- template main code

	-- create main.cpp if doesn't exist
	if not os.isfile("src/Main.cpp") and not os.isfile("src/main.cpp") then io.writefile("src/Main.cpp", mainFile) end

	filter "system:Windows"
		systemversion "10.0.17763.0"

	filter {}



project "PathFindingAI"
    kind "ConsoleApp"



	-- filtering for the different systems
	filter { "system:windows" }
		defines {"WINDOWS"}
		includedirs ("%{wks.location}/src")
		
	filter { "system:linux" }
		defines {"LINUX"}
		includedirs ("%{wks.location}/src")
		
	filter { }



	includedirs 
	{ 
		"C:/SDL_Lib/SDL2/include",			--basic for SDL
		"C:/SDL_Lib/SDL2_image/include" ,	--basic for SDL
		"C:/SDL_Lib/SDL2_ttf/include",		--basic for SDL
	}
    
	libdirs --directory to link SDL library
	{ 		
		"C:/SDL_Lib/SDL2/lib/x64",	
		"C:/SDL_Lib/SDL2_image/lib/x64", 
		"C:/SDL_Lib/SDL2_ttf/lib/x64" 
	}

	links --SDL library to link
	{ 
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib"
	}

	files { "%{wks.location}/src/**.h", "%{wks.location}/src/**.cpp" } -- source files
	removefiles { "%{wks.location}/src/ToDo.h" }       -- files to exclude from compilation

	objdir ( "%{wks.location}/output/obj" ) -- directory where object and other intermediate files should be placed when building a project



	-- filtering for the different configurations
    filter { "configurations:Debug" }		-- debug configuration
        defines { "DEBUG" }
        symbols "On"
		targetdir ( "%{wks.location}/output/bin/debug" )

    filter { "configurations:Release" }		-- release configuration
        defines { "NDEBUG" }
        optimize "On"
		targetdir ( "%{wks.location}/output/bin/release" )

	filter { }



-- Clean Function --
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
		
		os.rmdir("./vs")

		-- Clean the directory and files for Windows build --
		os.remove("./**.sln")
		os.remove("./**.vcxproj")
		os.remove("./**.vcxproj.filters")		
		os.remove("./**.vcxproj.user")
		
		print("done.")
	end
}