#include "ConversationManager.h"
#include "../Utility/Utils.h"
#include "raylib-cpp.hpp"

using namespace std;

void ConversationManager::Init(const std::string& conversationFile)
{
	Characters.reserve(5);
	DisplayedChildren.reserve(5);

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

void ConversationManager::ProcessDisplayedOptions()
{
	DisplayedChildren.clear();

	for (ConversationNode& child : CurrentConversationNode->Children)
		if (child.EvalConditions())
			DisplayedChildren.push_back(&child);
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

	//Preparare the next node
	ConversationNode node;

	//Get the type of node from the YML
	if (!strKey.empty() && strKey.find_first_not_of("-.0123456789") == std::string::npos) 	//if (strKey[0] == '0' || (atoi(strKey.c_str())))
	{
		//Set the type
		if (currentNode->Type != Conditional) // If true this means We are carring some conditionals already
			currentNode->Type = NormalTalk;
		//Get the text
		currentNode->Text = talkIt->second.as<string>();

		//Get the speaker id
		currentNode->CharacterId = talkIt->first.as<int>();
		CHECK(currentNode->CharacterId >= 0);
		CHECK(currentNode->CharacterId < (int)Characters.size());

		//DEBUG_COUT("CharacterId: " << currentNode->CharacterId << " value:  " << currentNode->Text << "\n ");

		if (talkNode["time"]) 		//Get the duration (optional parametter)
			currentNode->Duration = talkNode["time"].as<float>();
	}
	else if (strKey == "Option")
	{
		//Set the type
		currentNode->Type = Optional;

		//Get all options
		if (talkIt->second.IsSequence())
		{
			//Add node to the childrens vector
			ConversationNode node;

			YAML::const_iterator subTalkIt = talkIt->second.begin();
			YAML::const_iterator subTalkEnd = talkIt->second.end();

			/// We're going to group all sucessive conditionals into the first talk element
			while (subTalkIt != subTalkEnd)
			{
				string nextKey = subTalkIt->begin()->first.as<string>();
				string nextValue = subTalkIt->begin()->second.as<string>();

				//DEBUG_COUT("nextKey: " << nextKey << " value:  " << nextValue << "\n ");

				if (nextKey != "Condition")
					break;

				node.Type = Conditional;
				ParseCondition(nextValue, &node);
				subTalkIt++;
				//DEBUG_COUT("nextKey: " << nextKey << " value:  " << nextValue << "\n ");
			}

			currentNode->Children.push_back(node);

			//Recursive call
			ParseNode(subTalkIt, subTalkEnd, &(currentNode->Children[0]));
		}

		//Preparare the next node
		ConversationNode nextNode;
		currentNode->Children.push_back(nextNode);

		//Recursive call
		ParseNode(++iterator, end, &(currentNode->Children[1]));
		return;
	}
	else if (strKey == "ChooseTalk")
	{
		//Set the type
		currentNode->Type = ChooseTalk;

		//Get all options
		for (auto optionIt : talkIt->second) // inside ChooseTalk: list of Options
		{
			auto subOptionIt = optionIt.begin(); // First option item 
			CHECK(subOptionIt->first.as<string>() == "Option");

			if (subOptionIt->second.IsSequence())
			{
				//Add node to the childrens vector
				ConversationNode node;

				YAML::const_iterator subTalkIt = subOptionIt->second.begin();
				YAML::const_iterator subTalkEnd = subOptionIt->second.end();

				/// We're going to group all sucessive conditionals into the first talk element
				while (subTalkIt != subTalkEnd)
				{
					string nextKey = subTalkIt->begin()->first.as<string>();
					string nextValue = subTalkIt->begin()->second.as<string>();

					//DEBUG_COUT("nextKey: " << nextKey << " value:  " << nextValue << "\n ");

					if (nextKey != "Condition")
						break;

					node.Type = Conditional;
					ParseCondition(nextValue, &node);
					subTalkIt++;
				}

				// --------------------------------------------------------------------------- //

				currentNode->Children.push_back(node);

				//Recursive call
				size_t lastIndex = currentNode->Children.size() - 1;
				ParseNode(subTalkIt, subTalkEnd, &(currentNode->Children[lastIndex]));
			}
		}
		return;
	}
	else if (strKey == "JumpBack")
	{
		//Set the type
		currentNode->Type = JumpBack;

		// Keep as Param for JumpLevels
		currentNode->Text = !talkIt->second.IsNull() ? talkIt->second.as<string>() : "1";
	}
	else if (strKey == "SetTrue" || strKey == "SetFalse")
	{
		//Set the type
		if (currentNode->Type != Conditional) // If true this means We are carring some conditionals already
			currentNode->Type = SetBool;

		currentNode->Action.first = talkIt->second.as<string>();
		currentNode->Action.second = (strKey == "SetTrue" ? true : false);
	}
	else if (strKey == "SetImage")
	{
		//Set the type (If true this means We are carring some conditionals already)
		if (currentNode->Type != Conditional) 
			currentNode->Type = SetImage;

		// Get the Image ID
		currentNode->Action.first = talkIt->second.as<string>();	 

		// Optional 1: Load this file  (I know it's an ugly approach but let's roll with it)
		currentNode->Text = talkNode["file"] ? talkNode["file"].as<string>() : "";

		// Optional 2: If there's a visibility property use it
		if (talkNode["visible"])									
		{
			currentNode->Action.second = talkNode["visible"].as<string>() == "true";
		}
		else if (Game::Get().States.dialogState.ImagesMap.contains(currentNode->Action.first))	
		{
			// else use the previous value if We had one
			currentNode->Action.second = Game::Get().States.dialogState.ImagesMap[currentNode->Action.first]->GetIsVisible();
		}
	}
	else if (strKey == "ShowImage" || strKey == "HideImage")
	{
		//Set the type  (If true this means We are carring some conditionals already)
		if (currentNode->Type != Conditional) 
			currentNode->Type = SetImageVisibility;

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to show/hide
		currentNode->Action.second = strKey == "ShowImage" ? true : false;
	}
	else if (strKey == "SetFullHeight" || strKey == "SetSizeExtend" /*|| strKey == "SetImageSizeNormal"*/)
	{
		//Set the type  (If true this means We are carring some conditionals already)
		if (currentNode->Type != Conditional)
			currentNode->Type = SetImageSize;

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to show/hide
		currentNode->Action.second = strKey == "SetSizeExtend" ? true : false;
	}
	else if (strKey == "FadeImageIn" || strKey == "FadeImageOut")
	{
		//Set the type  (If true this means We are carring some conditionals already)
		if (currentNode->Type != Conditional)
			currentNode->Type = SetImageFade;

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to fade in/out
		currentNode->Action.second = strKey == "FadeImageIn" ? true : false;
	}
	else if (strKey.find("SetImageLeft") != string::npos || strKey == "SetImageCenter" || strKey.find("SetImageRight") != string::npos)
	{
		//Set the type  (If true this means We are carring some conditionals already)
		if (currentNode->Type != Conditional)
		{
			currentNode->Type = SetImageCenter;

			if (strKey.find("Left") != string::npos)
				currentNode->Type = SetImageLeft;
			else if (strKey.find("Right") != string::npos)
				currentNode->Type = SetImageRight;
		}

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to be inside screen or else out
		currentNode->Action.second = strKey.find("Out") == string::npos ? true : false;
	}
	else if (strKey.find("MoveImageLeft") != string::npos || strKey == "MoveImageCenter" || strKey.find("MoveImageRight") != string::npos)
	{
		//Set the type  (If true this means We are carring some conditionals already)
		if (currentNode->Type != Conditional)
		{
			currentNode->Type = MoveImageCenter;

			if (strKey.find("Left") != string::npos)
				currentNode->Type = MoveImageLeft;
			else if (strKey.find("Right") != string::npos)
				currentNode->Type = MoveImageRight;
		}

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to be inside screen or else out
		currentNode->Action.second = strKey.find("Out") == string::npos ? true : false;
	}
	else if (strKey.find("ScrollLeft") != string::npos || strKey == "ScrollLoop" || strKey.find("ScrollRight") != string::npos)
	{
		if (currentNode->Type != Conditional)
		{
			currentNode->Type = ScrollCycle;

			if (strKey.find("Left") != string::npos)
				currentNode->Type = ScrollLeft;
			else if (strKey.find("Right") != string::npos)
				currentNode->Type = ScrollRight;
		}

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to be inside screen or else out
		currentNode->Action.second = strKey.find("End") == string::npos ? true : false;
	}
	else if (strKey == "ShakeImage" )
	{
		if (currentNode->Type != Conditional)
			currentNode->Type = ShakeImage;

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to fade in/out
		currentNode->Action.second = true; // it's Right else is Left
	}
	else if (strKey == "SlideImageRight" || strKey == "SlideImageLeft")
	{
		//Set the type  (If true this means We are carring some conditionals already)
		if (currentNode->Type != Conditional)
			currentNode->Type =  SetImageSlide;

		currentNode->Action.first = talkIt->second.as<string>();	/// Set the Image ID to fade in/out
		currentNode->Action.second = strKey.find("Right") != string::npos ? true : false; // it's Right else is Left
	}
	else
	{
		//Invalid tag found
		CHECK(0 && "Wrong character tag ID\n");
	}

	currentNode->Children.push_back(node);

	//Recursive call
	ParseNode(++iterator, end, &(currentNode->Children[0]));
}

// Parse yml to  group conditions
void ConversationManager::ParseCondition(const std::string& cond, ConversationNode* currentNode)
{
	std::string keyStr = cond;
	size_t negationPos = keyStr.find('!');
	bool isNegated = false;

	if (negationPos != std::string::npos && negationPos == 0)
	{
		keyStr = keyStr.substr(1, keyStr.size() - 1);
		isNegated = true;
	}

	currentNode->ConditionsEntries[keyStr] = (isNegated == false);
}


// Jump to the next Node
void ConversationManager::NextMessage(unsigned nextMessageIndex)
{
	CHECK(CurrentConversationNode);
	CHECK(nextMessageIndex < CurrentConversationNode->Children.size());

	//Change to the next node
	bool IsChooseNode = (CurrentConversationNode->Type == ChooseTalk);

	CurrentConversationNode = (IsChooseNode ?
							   DisplayedChildren[nextMessageIndex] :
							   &(CurrentConversationNode->Children[nextMessageIndex]));

	if (CurrentConversationNode->Type == JumpBack)
	{
		//JumpLevel is the amount of jumps backs We want
		int levelJumps = stoi(CurrentConversationNode->Text);
		while (TalkStack.size() > 1 && --levelJumps > 0)
		{
			TalkStack.pop();
		}

		CurrentConversationNode = TalkStack.top();
		return;
	}

	//Finish the conversation if we reach to the leaf node
	if (CurrentConversationNode->Type == EndConversation)
	{
		TalkStack = std::stack<ConversationNode*>();
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

	ProcessDisplayedOptions();

	switch (CurrentConversationNode->Type)
	{
	//case Action:
	case SetBool:
	case SetImage:
	case SetImageVisibility:
	case SetImageSize:
	case SetImageFade:
	case SetImageLeft:
	case SetImageRight:
	case SetImageCenter:
	case MoveImageLeft:
	case MoveImageRight:
	case MoveImageCenter:
	case ScrollLeft:
	case ScrollRight:
	case ScrollCycle:
	case ShakeImage:
	case SetImageSlide:
		if (!CurrentConversationNode->Action.first.empty())
		{
			CurrentConversationNode->ExecuteAction();
			NextMessage(0);
		}
		break;
	case Conditional:
	case NormalTalk:
		//Reduce show time
		MessageTime -= GetFrameTime();

		//If time has finished or Accept action has been selected change to the next message
		if (MessageTime <= 0.0f || IsKeyPressed(KEY_ENTER))
			NextMessage(0);
		break;

	case Optional:
	{
		ConversationNode& childNode = CurrentConversationNode->Children[0];
		if (childNode.Type == Conditional && childNode.EvalConditions())
		{
			if (childNode.Action.second || !childNode.Action.first.empty())
				childNode.ExecuteAction();

			if (TalkStack.empty() || CurrentConversationNode != TalkStack.top())
				TalkStack.push(&(CurrentConversationNode->Children[1]));
			NextMessage(0);
		}
		else
			NextMessage(1);
	}
	break;

	case ChooseTalk:
		//Only if the user has accepted an option, change to that branch
		if (TalkStack.empty() || CurrentConversationNode != TalkStack.top())
			TalkStack.push(CurrentConversationNode);

		if (IsKeyPressed(KEY_ENTER))
			NextMessage(ChooseOption);
		if (IsKeyPressed(KEY_UP) && ChooseOption > 0)
			ChooseOption--;
		if (IsKeyPressed(KEY_DOWN) && ChooseOption < (DisplayedChildren.size() - 1))
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
	case Conditional:
		//Write the name
		characterName = Characters[(CurrentConversationNode->CharacterId)].CharacterName + ": ";
		DrawText(characterName.c_str(), 50, 100, 20, SKYBLUE);

		//Draw the text
		message = CurrentConversationNode->Text;

		//if (CurrentConversationNode->ConditionsEntries.size() > 0)	// Show me which entries has conditionals beforehand
		//	DrawText(message.c_str(), 100, 120, 20, RED);
		//else
			DrawText(message.c_str(), 100, 120, 20, SKYBLUE);
		break;

	case ChooseTalk:
		//Draw the options
		for (ConversationNode* child : DisplayedChildren)
		{
			characterName = Characters[child->CharacterId].CharacterName + ": ";
			DrawText(characterName.c_str(), 50, 100, 20, SKYBLUE);

			//Set the propper color to the selected option
			Color color = (counter == ChooseOption) ? VIOLET : SKYBLUE;

			//if (child->Type == Conditional)
			//	color = RED;

			//Draw the child
			message = child->Text;
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
