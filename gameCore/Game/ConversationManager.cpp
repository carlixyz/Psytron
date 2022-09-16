#include "ConversationManager.h"
#include "../Utility/Utils.h"
#include "raylib-cpp.hpp"

using namespace std;

//Initializate Conversation Manager
void ConversationManager::Init(const std::string& conversationFile)
{
	YAML::Node dialogFile = YAML::LoadFile(conversationFile); // load file

	CHECK(!dialogFile.IsNull());  // the file is wrongly loaded!

	//Get the root node
	YAML::Node Root = dialogFile["Dialogs"];	// as a Map;
	YAML::Node ElementNode;
	YAML::const_iterator ElementIt;

	//Read characters -  iterate over bar node to get Property nodes (Example)
	ElementNode = Root["Characters"];
	for (ElementIt = ElementNode.begin(); ElementIt != ElementNode.end(); ElementIt++)
	{
		ParseCharacter(ElementIt);
	}

	ElementNode = Root["Conversations"];
	for (ElementIt = ElementNode.begin(); ElementIt != ElementNode.end(); ElementIt++)
	{
		ParseConversation(ElementIt);
	}

	CurrentConversationNode = nullptr;
	ChooseOption = 0;
}

void ConversationManager::Deinit(void)
{
	//Free characters
	Characters.clear();

	//Free conversations
	Conversations.clear();
}

// Parse yml to process a character
void ConversationManager::ParseCharacter(const YAML::const_iterator& element)
{
	CharacterSpeaker character;

	//DEBUG_COUT(element->first.as<int>() << ": " << element->second << "\n");

	//Get character Id
	character.CharacterId = element->first.as<int>();
	CHECK(character.CharacterId >= 0);

	//Get character name
	character.CharacterName = element->second.as<std::string>();

	//Push back into character list
	Characters.push_back(character);
}

// Parse yml to process a conversation
void ConversationManager::ParseConversation(const YAML::const_iterator& element)
{
	Conversation conversation;

	//Get the conversation ID
	conversation.Name = element->first.as<std::string>();
	//DEBUG_COUT("\n conversation ID-> " << conversation.Name << " : " << "\n\n");

	//Get the conversation tree
	if (element->second.IsSequence())			// element.second has inside a sequence of maps!
	{
		YAML::const_iterator elem = element->second.begin();
		YAML::const_iterator end = element->second.end();
		ParseNode(elem, end, &conversation.Root); //ParseNode(element->second.begin(), &conversation.Root);
	}

	//Push back into conversations list
	Conversations.push_back(conversation);
}

// Parse yml to process a node of the conversation tree
void ConversationManager::ParseNode(YAML::const_iterator& iterator, YAML::const_iterator& end, ConversationNode* currentNode)
{
	//Base case
	if (iterator == end)
	{
		currentNode->Type = EndConversation;
		return;
	}

	const auto& talkNode = iterator->as<YAML::Node>();
	const auto& talkIt = iterator->begin();
	string strKey = talkIt->first.as<string>();

	//Get the type of node from the YML
	if (!strKey.empty() && strKey.find_first_not_of("-.0123456789") == std::string::npos) 	//if (strKey[0] == '0' || (atoi(strKey.c_str())))
	{
		//DEBUG_COUT(" >: " << talkIt->first << " " << talkIt->second << "\n");

		//Set the type
		currentNode->Type = NormalTalk;
		//Get the text
		currentNode->Text = talkIt->second.as<string>();

		//Get the speaker id
		currentNode->CharacterId = talkIt->first.as<int>();
		CHECK(currentNode->CharacterId >= 0);
		CHECK(currentNode->CharacterId < (int)Characters.size());

		if (talkNode["time"]) 		//Get the duration (optional parametter)
		{
			currentNode->Duration = talkNode["time"].as<float>();
			//DEBUG_COUT("	Additional Prop: " << std::fixed << currentNode->Duration << "\n");
		}

		//Preparare the next node
		ConversationNode node;
		currentNode->Children.push_back(node);

		//Recursive call
		ParseNode(++iterator, end, &(currentNode->Children[0]));

	}
	else if (talkIt->first.as<string>() == "ChooseTalk")
	{
		//DEBUG_COUT("\n >: ChooseTalk \n");

		//Set the type
		currentNode->Type = ChooseTalk;

		//Get all options
		for (auto optionIt : talkIt->second)
		{
			auto subOptionIt = optionIt.begin();
			//DEBUG_COUT("	->: " << subOptionIt->first << " \n");	// OPTION TAG
			CHECK(subOptionIt->first.as<string>() == "Option");

			if (subOptionIt->second.IsSequence())
			{
				//Add node to the childrens vector
				ConversationNode node;
				currentNode->Children.push_back(node);

				YAML::const_iterator subTalkIt = subOptionIt->second.begin();
				YAML::const_iterator subTalkEnd = subOptionIt->second.end();

				//Recursive call
				size_t lastIndex = currentNode->Children.size() - 1;
				ParseNode(subTalkIt, subTalkEnd, &(currentNode->Children[lastIndex]));

				//DEBUG_COUT(" ->: " << subTalkIt->begin()->first << " " << subTalkIt->begin()->second << " \n");	// TALK NODE
			}
		}
	}
	else
	{
		//Invalid tag found
		CHECK(0 && "Wrong character tag ID\n");
	}
}

// Jump to the next Node
void ConversationManager::NextMessage(unsigned nextMessageIndex)
{
	CHECK(CurrentConversationNode);
	CHECK(nextMessageIndex < CurrentConversationNode->Children.size());

	//Change to the next node
	bool IsChooseNode = (CurrentConversationNode->Type == ChooseTalk);
	CurrentConversationNode = &(CurrentConversationNode->Children[nextMessageIndex]);

	//Finish the conversation if we reach to the leaf node
	if (CurrentConversationNode->Type == EndConversation)
	{
		CurrentConversationNode = nullptr;
		return;
	}

	//If the node is a choose node skip the option selected node
	if (IsChooseNode)
		NextMessage(0);
	else
	{
		//Establish the actual node values
		ChooseOption = 0;
		MessageTime = CurrentConversationNode->Duration;
	}
}

void ConversationManager::Update()
{
	//If not in conversation goes out
	if (!IsInConversation())
		return;

	switch (CurrentConversationNode->Type)
	{
	case NormalTalk:
		//Reduce show time
		MessageTime -= GetFrameTime();

		//If time has finished or Accept action has been selected change to the next message
		if (MessageTime <= 0.0f || IsKeyPressed(KEY_ENTER))
			NextMessage(0);
		break;

	case ChooseTalk:
		//Only if the user has accepted an option, change to that branch
		if (IsKeyPressed(KEY_ENTER))
			NextMessage(ChooseOption);
		if (IsKeyPressed(KEY_UP) && ChooseOption > 0)
			ChooseOption--;
		if (IsKeyPressed(KEY_DOWN) && ChooseOption < (CurrentConversationNode->Children.size() - 1))
			ChooseOption++;
		return;

	default:
		break;
	}
}

void ConversationManager::Render()
{
	//If not in conversation skip the render method
	if (!IsInConversation()) return;

	//Variable that will hold the message
	std::string message;
	//Variable taht will hold the name of the character talking
	std::string characterName;
	//Counter for children
	unsigned counter = 0;


	switch (CurrentConversationNode->Type)
	{
	case NormalTalk:
		//Write the name
		characterName = Characters[(CurrentConversationNode->CharacterId)].CharacterName + ": ";
		DrawText(characterName.c_str(), 50, 100, 20, SKYBLUE);

		//Draw the text
		message = CurrentConversationNode->Text;
		DrawText(message.c_str(), 100, 120, 20, SKYBLUE);
		break;

	case ChooseTalk:
		//Draw the options
		for (ConversationChildrenIt cIt = CurrentConversationNode->Children.begin();
			 cIt != CurrentConversationNode->Children.end(); cIt++)
		{
			characterName = Characters[(cIt->CharacterId)].CharacterName + ": ";
			DrawText(characterName.c_str(), 50, 100, 20, SKYBLUE);

			//Set the propper color to the selected option
			Color color = (counter == ChooseOption) ? VIOLET : SKYBLUE;

			//Draw the child
			message = cIt->Text;
			DrawText(message.c_str(), 100, 120 + (counter * 20), 20, color);

			counter++;
		}
		break;

	default:
		break;
	}
}

void ConversationManager::StartConversation(const std::string& lacConversationId)
{
	//Seek for conversation
	for (ConversationsIt cIt = Conversations.begin(); cIt != Conversations.end(); cIt++)
	{
		//If the conversation is found assign the root node
		if ((*cIt).Name == lacConversationId)
		{
			CurrentConversationNode = &((*cIt).Root);
			//Also set the message time and the option
			MessageTime = (*cIt).Root.Duration;
			ChooseOption = 0;
			break;
		}
	}
}

bool ConversationManager::IsInConversation()
{
	return (CurrentConversationNode ? true : false);
}
