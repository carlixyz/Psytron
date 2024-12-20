#include "ApplicationProperties.h"

#include <yaml-cpp/yaml.h>
#include "../Utility/Utils.h"
#include <Windows.h>		

void ApplicationProperties::LoadConfiguration(const std::string filename)
{
	///...
	//YAML::Node configFile = YAML::LoadFile(filename); // load file

	YAML::Node configFile; // load file
	try
	{
		configFile = YAML::LoadFile(filename);
	}
	catch (std::exception const& e) // @throws {@link BadFile} if the file cannot be loaded.
	{
		MSG_BOX(e.what());
	}

	CHECK(!configFile.IsNull());  //assert(configFile.Type() == YAML::NodeType::Map);
		
	if (YAML::Node AppNode = configFile["Application"])		// as a Map;
	{
		if (YAML::Node WinNode = AppNode["Window"])			// .as<YAML::Node>();
		{
			YAML::Node properties = WinNode["Properties"];	// .as<YAML::Node>();

			ApplicationName = properties["_Name"].IsDefined() ? 
								properties["_Name"].as<std::string>() :
								"Unnamed Window";
			Width			= properties["_Width"].IsDefined() ?
								properties["_Width"].as<int>() :
								640;
			Height			= properties["_Height"].IsDefined() ?
								properties["_Height"].as<int>() :
								480;
			Bits			= properties["_Bits"].IsDefined() ?
								properties["_Bits"].as<int>() :
								32;
			FPS				= properties["_FPS"].IsDefined() ?
								properties["_FPS"].as<int>() :
								60;
			Fullscreen		= properties["_Fullscreen"].IsDefined() ?
								properties["_Fullscreen"].as<bool>() :
								false;

			DebugMode		= properties["_DebugMode"].IsDefined() ?
								properties["_DebugMode"].as<bool>() :
								false;

			SkipIntro		= properties["_SkipIntro"].IsDefined() ?
								properties["_SkipIntro"].as<bool>() :
								false;


			DEBUG_COUT("ApplicationName \t-> "<< ApplicationName << "\n");
			DEBUG_COUT("Width \t\t\t-> "	<< Width << "\n");
			DEBUG_COUT("Height \t\t\t-> "	<< Height << "\n");
			DEBUG_COUT("FPS \t\t\t-> "		<< FPS << "\n\n");

			/*
			// iterate over bar node to get Property nodes (Example)
			for (auto i = WinNode.begin(); i != WinNode.end(); ++i)
			{
				YAML::Node prop = i->second; // this node is a list of pairs
				std::cout << "	Win in: " << i->first << ": \n";

				for (const auto& j : prop)
					std::cout << "		" << j.first << ": " << j.second << "\n";
			}
			*/
		}

		if (YAML::Node Resources = AppNode["Resources"])
		{
			DialogFilePath = Resources["DialogFile"].IsDefined() ?
								Resources["DialogFile"].as<std::string>() :
								"";

			DEBUG_COUT("DialogFile \t-> " << DialogFilePath << "\n");
		}
	}
}
