#pragma once

#include "../Utility/Singleton.h"
#include "Game.h"
#include "Register.h"
#include "../Audio/Audio.h"
#include <vector>
#include <stack>
#include <list>
#include <string>
#include "yaml-cpp/yaml.h"

enum class ConversationNodeType	
{
	NormalTalk,
	ChooseTalk,
    Optional,
    Conditional,
    GoToNode,
    
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
    CleanUp,

    SetImageSlide,

    PlayMusic,
    SetMusic,
    PlaySound,

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
        case ConversationNodeType::SetBool:
            Register::Get().SetValue(Action.first, Action.second);
            break;

        case ConversationNodeType::SetImage:
            if(!Text.empty())
                Game::Get().States.dialogState.LoadImage(Action.first, Text);                       // LoadImage
            Game::Get().States.dialogState.SetImageVisible( Action.first, Action.second);   
            break;

        case ConversationNodeType::SetImageVisibility:
            Game::Get().States.dialogState.SetImageVisible(Action.first, Action.second);            // Set it Visible or not
            break;

        case ConversationNodeType::SetImageSize:
            if(Action.second)
                Game::Get().States.dialogState.SetFullSize(Action.first, DialogState::EStrechProportion);    // Strech it along screen
            else
                Game::Get().States.dialogState.SetFullSize(Action.first, DialogState::EExtend);    // Else Extend tiles too 
            break;

        case ConversationNodeType::SetImageFade:
            if (Action.second == true) // Fade In else Out
                Game::Get().States.dialogState.SetEasing( Action.first, DialogState::EActionEasing::EFadeIn);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EFadeOut);
            break;


        case ConversationNodeType::SetImageLeft:
            if (Action.second == true) // Set inside Screen else set it Out
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionLeftOut);
            else
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionLeft);
            break;

        case ConversationNodeType::SetImageRight:
            if (Action.second == true) // Set inside Screen else set it Out
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionRightOut);
            else
                Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionRight);
            break;

        case ConversationNodeType::SetImageCenter:
            Game::Get().States.dialogState.SetPosition(Action.first, DialogState::EScreenPosition::EPositionCenter);
            break;


        case ConversationNodeType::MoveImageLeft:
            if (Action.second == true) // Move inside Screen else set it Out
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionLeftOut);
            else
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionLeft);
            break;

        case ConversationNodeType::MoveImageRight:
            if (Action.second == true) // Set inside Screen else set it Out
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionRightOut);
            else
                Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionRight);
            break;

        case ConversationNodeType::MoveImageCenter:
            Game::Get().States.dialogState.MovePosition(Action.first, DialogState::EScreenPosition::EPositionCenter);
            break;


        case ConversationNodeType::SetImageSlide:
            if (Action.second == true)      // Set Slide From Right else Slide From Left
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::ESlideFromRight);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::ESlideFromLeft);
            break;


        case ConversationNodeType::ScrollLeft:
            if (Action.second == true)      // Move inside Screen cap else leave it scroll free Out undefinedly
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollLeftCap);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollLeft);
            break;

        case ConversationNodeType::ScrollRight:
            if (Action.second == true)  // Move inside Screen cap else leave it free Out undefinedly
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollRightCap);
            else
                Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollRight);
            break;

        case ConversationNodeType::ScrollCycle:                   // Move inside Screen cap undefinedly
            Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EScrollCycle);
            break;

        case ConversationNodeType::ShakeImage:
            Game::Get().States.dialogState.SetEasing(Action.first, DialogState::EActionEasing::EShakeQuake);
            break;

        case ConversationNodeType::CleanUp:
            if (Action.second == true)  // Move inside Screen cap else leave it free Out undefinedly
                Game::Get().States.dialogState.UnLoadAll();
            else
                Game::Get().States.dialogState.Unload(Action.first);
            break;
        case ConversationNodeType::PlayMusic:   // PlayMusic with looping On/Off
                Audio::Get().PlayMusic(Action.first, Action.second);
            break;
        case ConversationNodeType::SetMusic:  // Stop music or just toggle pause/resume it
            if (Action.second == true)  
                Audio::Get().StopMusic();
            else
                Audio::Get().PauseMusic();
            break;
        case ConversationNodeType::PlaySound:   
                Audio::Get().PlaySound(Action.first);
            break;
        default:
            break;
        }
    }
};

struct Conversation                                         // SubConversation Root tree
{
    std::string  Name           = "";
	ConversationNode Root;
};

class ConversationManager : public Singleton<ConversationManager>
{
	friend class Singleton<ConversationManager>;

    Rectangle TextBoxArea;
    Rectangle TextArea;

    typedef std::vector<CharacterSpeaker> CharactersVector; //Type definitions to simplify the syntax
    typedef CharactersVector::iterator CharactersIt;
    typedef std::list<Conversation> ConversationsList;
    typedef ConversationsList::iterator ConversationsIt;
    typedef std::vector<ConversationNode> ConversationChildren;
    typedef ConversationChildren::iterator ConversationChildrenIt;

    CharactersVector Characters;                            //List of characters
    ConversationsList Conversations;                        //List of conversations 
    ConversationNode* CurrentConversationNode = nullptr;    //Current conversation node

    bool ConversationEnded          = false;
    float MessageTime               = 5.f;                  //Time to show the conversation
    unsigned ChooseOption           = 0;                    //Selected option in a choose conversation
    unsigned ChooseIndex            = 0;                    //Selected option in a choose conversation
    unsigned CharIndex              = 0;                    //Current frame char index for our wording printer FX
    unsigned CharShowDelay          = 1;                    //Current frame char index for our wording printer FX

    std::stack<ConversationNode*> TalkStack;
    std::vector<ConversationNode*> DisplayedChildren;

    void PreLoadImage(const YAML::const_iterator& images);
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