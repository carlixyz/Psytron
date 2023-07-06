#pragma once

#include "../Utility/Singleton.h"
#include <vector>
#include <stack>
#include <list>
#include <string>
#include "yaml-cpp/yaml.h"
#include "Register.h"
#include "Game.h"

enum ConversationNodeType	
{
	NormalTalk,
	ChooseTalk,
    Optional,
    Conditional,
    
    //Actions
    SetBool,
    SetImage,
    SetImageVisibility,
    SetImageSize,
    SetImageFade,

    SetImageLeft,
    SetImageRight,
    SetImageCenter,

    MoveImageLeft,
    MoveImageRight,
    MoveImageCenter,

    ScrollLeft,
    ScrollRight,
    ScrollCycle,
    ShakeImage,

    SetImageSlide,

	JumpBack,
	EndConversation
};

struct CharacterSpeaker	
{
	int CharacterId             = 0;
	std::string CharacterName   = "";
};

struct ImageResource		
{
    std::string ImageID = "";
    std::string file = "";
};

struct ConversationNode     // Conversation SubNodes
{
    ConversationNodeType Type   = ConversationNodeType::EndConversation;
    std::string Text            = "";
	float Duration              = 5.f;
	int CharacterId             = 0;

	typedef std::vector<ConversationNode> ConversationChildren;
	ConversationChildren Children;
    std::map<const std::string, bool> ConditionsEntries;
    std::pair<std::string, bool> Action;

    bool EvalConditions()
    {
        bool result = true;
        for (auto& cond : ConditionsEntries)
        {
            bool RegisterValue = Register::Get().GetValue(cond.first);
            result = result && 
                (RegisterValue == ConditionsEntries[cond.first]);
        }

        return result;
    }

    void ExecuteAction()
    {
        switch (Type)
        {
        case SetBool:
            Register::Get().SetValue(Action.first, Action.second);
            break;

        case SetImage:
            if(!Text.empty())
                Game::Get().States.dialogState.LoadImage(Action.first, Text);                       // LoadImage
            Game::Get().States.dialogState.SetImageVisible( Action.first, Action.second);   
            break;

        case SetImageVisibility:
            Game::Get().States.dialogState.SetImageVisible(Action.first, Action.second);            // Set it Visible or not
            break;

        case SetImageSize:
            if(Action.second)
                Game::Get().States.dialogState.SetFullSize(Action.first, DialogState::EStrechProportion);    // Strech it along screen
            else
                Game::Get().States.dialogState.SetFullSize(Action.first, DialogState::EExtend);    // Else Extend tiles too 
            break;

        case SetImageFade:
            if (Action.second == true) // Fade In else Out
                Game::Get().States.dialogState.SetEasing( Action.first, DialogState::EActionEasing::EFadeIn);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EFadeOut);
            break;


        case SetImageLeft:
            if (Action.second == true) // Set inside Screen else set it Out
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionLeft);
            else
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionLeftOut);
            break;

        case SetImageRight:
            if (Action.second == true) // Set inside Screen else set it Out
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionRight);
            else
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionRightOut);
            break;

        case SetImageCenter:
            Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionCenter);
            break;


        case MoveImageLeft:
            if (Action.second == true) // Move inside Screen else set it Out
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionLeft);
            else
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionLeftOut);
            break;

        case MoveImageRight:
            if (Action.second == true) // Set inside Screen else set it Out
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionRight);
            else
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionRightOut);
            break;

        case MoveImageCenter:
            Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionCenter);
            break;


        case SetImageSlide:
            if (Action.second == true)      // Set Slide From Right else Slide From Left
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::ESlideFromRight);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::ESlideFromLeft);
            break;


        case ScrollLeft:
            if (Action.second == true)      // Move inside Screen cap else leave it scroll free Out undefinedly
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollLeftCap);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollLeft);
            break;

        case ScrollRight:
            if (Action.second == true)  // Move inside Screen cap else leave it free Out undefinedly
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollRightCap);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollRight);
            break;

        case ScrollCycle:                   // Move inside Screen cap undefinedly
            Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollCycle);
            break;

        case ShakeImage:
            Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EShakeQuake);
            break;

        default:
            break;
        }
    }
};

struct Conversation         // SubConversation Root tree, In our example We have a pair
{
    std::string  Name           = "";
	ConversationNode Root;
};


class ConversationManager : public Singleton<ConversationManager>
{
	friend class Singleton<ConversationManager>;

    //Type definition to simplify the syntax
    typedef std::vector<CharacterSpeaker> CharactersVector;
    typedef CharactersVector::iterator CharactersIt;
    typedef std::list<Conversation> ConversationsList;
    typedef ConversationsList::iterator ConversationsIt;
    typedef std::vector<ConversationNode> ConversationChildren;
    typedef ConversationChildren::iterator ConversationChildrenIt;

    //List of characters
    CharactersVector Characters;
    //List of conversations 
    ConversationsList Conversations;
    //Current conversation node
    ConversationNode* CurrentConversationNode = nullptr;
    //Time to show the conversation
    float MessageTime = 5.f;
    //Selected option in a choose conversation
    unsigned ChooseOption = 0;

    std::stack<ConversationNode*> TalkStack;
    std::vector<ConversationNode*> DisplayedChildren;

    void ParseCharacter(const YAML::const_iterator& element);
    void ParseConversation(const YAML::const_iterator& element);
    void ParseNode(YAML::const_iterator& iterator, YAML::const_iterator& end, ConversationNode* currentNode);
    void ParseCondition(const std::string& cond, ConversationNode* currentNode);

    void NextMessage(unsigned nextMessageIndex);
    void ProcessDisplayedOptions();

public:
    void Init(const std::string& conversationFile);
    void Deinit(void);
    void Update();
    void Render();
    void StartConversation(const std::string& lconversationId);
    bool IsInConversation();
};