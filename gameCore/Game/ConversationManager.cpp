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

void ConversationManager::ProcessChildOptions()
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

		//Preparare the next node
		ConversationNode node;
		currentNode->Children.push_back(node);

		//Recursive call
		ParseNode(++iterator, end, &(currentNode->Children[0]));

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
	}
	else if (strKey == "JumpBack")
	{
		//Set the type
		currentNode->Type = JumpBack;

		// Keep as Param for JumpLevels
		currentNode->Text = !talkIt->second.IsNull() ? talkIt->second.as<string>() : "1";

		//Preparare the next node
		ConversationNode node;
		currentNode->Children.push_back(node);

		//Recursive call
		ParseNode(++iterator, end, &(currentNode->Children[0]));
	}
	else if (strKey == "SetTrue" || strKey == "SetFalse")
	{
		//Set the type
		if (currentNode->Type != Conditional) // If true this means We are carring some conditionals already
			currentNode->Type = Action;

		currentNode->Action.first = talkIt->second.as<string>();
		currentNode->Action.second = (strKey == "SetTrue" ? true : false);

		//Preparare the next node
		ConversationNode node;
		currentNode->Children.push_back(node);

		//Recursive call
		ParseNode(++iterator, end, &(currentNode->Children[0]));
	}
	else
	{
		//Invalid tag found
		CHECK(0 && "Wrong character tag ID\n");
	}
}

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

	if (!IsChooseNode)
		CurrentConversationNode = &(CurrentConversationNode->Children[nextMessageIndex]);
	else
		CurrentConversationNode = (DisplayedChildren[nextMessageIndex]);

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

	ProcessChildOptions();

	switch (CurrentConversationNode->Type)
	{
	case Action:
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
		if (CurrentConversationNode->ConditionsEntries.size() > 0)
			DrawText(message.c_str(), 100, 120, 20, RED);
		else
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
