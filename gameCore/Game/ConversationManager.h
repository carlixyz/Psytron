#pragma once

#include "../Utility/Singleton.h"
#include "VisualDialogManager.h"
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
    ScrollCycleVertical,
    ScrollTop,
    ScrollBottom,
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

    void ExecuteAction() const
    {
        switch (Type)
        {
        case ConversationNodeType::SetBool:
            Register::Get().SetValue(Action.first, Action.second);
            break;

        case ConversationNodeType::SetImage:
            if(!Text.empty())
                VisualDialogManager::Get().LoadImage(Action.first, Text);                       // LoadImage
            VisualDialogManager::Get().SetImageVisible( Action.first, Action.second);   
            break;

        case ConversationNodeType::SetImageVisibility:
            VisualDialogManager::Get().SetImageVisible(Action.first, Action.second);            // Set it Visible or not
            break;

        case ConversationNodeType::SetImageSize:
            if(Action.second)
                VisualDialogManager::Get().SetFullSize(Action.first, VisualDialogManager::EStrechProportion);    // Strech it along screen
            else
                VisualDialogManager::Get().SetFullSize(Action.first, VisualDialogManager::EExtend);    // Else Extend tiles too 
            break;

        case ConversationNodeType::SetImageFade:
            if (Action.second == true) // Fade In else Out
                VisualDialogManager::Get().SetEasing( Action.first, VisualDialogManager::EActionEasing::EFadeIn);
            else
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EFadeOut);
            break;


        case ConversationNodeType::SetImageLeft:
            if (Action.second == true) // Set inside Screen else set it Out
                VisualDialogManager::Get().SetPosition(Action.first, VisualDialogManager::EScreenPosition::EPositionLeftOut);
            else
                VisualDialogManager::Get().SetPosition(Action.first, VisualDialogManager::EScreenPosition::EPositionLeft);
            break;

        case ConversationNodeType::SetImageRight:
            if (Action.second == true) // Set inside Screen else set it Out
                VisualDialogManager::Get().SetPosition(Action.first, VisualDialogManager::EScreenPosition::EPositionRightOut);
            else
                VisualDialogManager::Get().SetPosition(Action.first, VisualDialogManager::EScreenPosition::EPositionRight);
            break;

        case ConversationNodeType::SetImageCenter:
            VisualDialogManager::Get().SetPosition(Action.first, VisualDialogManager::EScreenPosition::EPositionCenter);
            break;


        case ConversationNodeType::MoveImageLeft:
            if (Action.second == true) // Move inside Screen else set it Out
                VisualDialogManager::Get().MovePosition(Action.first, VisualDialogManager::EScreenPosition::EPositionLeftOut);
            else
                VisualDialogManager::Get().MovePosition(Action.first, VisualDialogManager::EScreenPosition::EPositionLeft);
            break;

        case ConversationNodeType::MoveImageRight:
            if (Action.second == true) // Set inside Screen else set it Out
                VisualDialogManager::Get().MovePosition(Action.first, VisualDialogManager::EScreenPosition::EPositionRightOut);
            else
                VisualDialogManager::Get().MovePosition(Action.first, VisualDialogManager::EScreenPosition::EPositionRight);
            break;

        case ConversationNodeType::MoveImageCenter:
            VisualDialogManager::Get().MovePosition(Action.first, VisualDialogManager::EScreenPosition::EPositionCenter);
            break;


        case ConversationNodeType::SetImageSlide:
            if (Action.second == true)      // Set Slide From Right else Slide From Left
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::ESlideFromRight);
            else
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::ESlideFromLeft);
            break;


        case ConversationNodeType::ScrollLeft:
            if (Action.second == true)      // Move inside Screen cap else leave it scroll free Out undefinedly
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollLeftCap);
            else
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollLeft);
            break;

        case ConversationNodeType::ScrollRight:
            if (Action.second == true)  // Move inside Screen cap else leave it free Out undefinedly
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollRightCap);
            else
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollRight);
            break;

        case ConversationNodeType::ScrollCycle:                   // Move inside Screen cap undefinedly
            VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollCycle);
            break;


        case ConversationNodeType::ScrollTop:
            //if (Action.second == true)  // Move inside Screen cap else leave it free Out undefinedly
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollTopCap);
            //else
            //    VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollTop);
            break;

        case ConversationNodeType::ScrollBottom:
            //if (Action.second == true)  // Move inside Screen cap else leave it free Out undefinedly
                VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollBottomCap);
            //else
            //    VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollBottom);
            break;

        case ConversationNodeType::ScrollCycleVertical:                   // Move inside Screen cap undefinedly
            VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EScrollCycleVertical);
            break;


        case ConversationNodeType::ShakeImage:
            VisualDialogManager::Get().SetEasing(Action.first, VisualDialogManager::EActionEasing::EShakeQuake);
            break;

        case ConversationNodeType::CleanUp:
            if (Action.second == true)  // Move inside Screen cap else leave it free Out undefinedly
                VisualDialogManager::Get().UnLoadAll();
            else
                VisualDialogManager::Get().Unload(Action.first);
            break;
        case ConversationNodeType::PlayMusic:   // PlayMusic with looping On/Off
                Audio::Get().PlayMusic(Action.first, Action.second);
            break;
        case ConversationNodeType::SetMusic:    // Stop music or just toggle pause/resume it
            if (Action.first == "StopMusic")
            {
                if(Action.second == true)       // Do Fade?
                    Audio::Get().FadeMusicOut();
                else
                    Audio::Get().StopMusic();
            }
            else
            {
                bool SetPausedMusic = (Audio::Get().IsPlayingMusic() && Action.first == "PauseMusic" ||
                                        (Action.first == "ToggleMusic" && Action.second == false));
                bool SetResumedMusic = (!Audio::Get().IsPlayingMusic() && Action.first == "ResumeMusic" ||
                                       (Action.first == "ToggleMusic" && Action.second == true));

                if (SetResumedMusic || SetPausedMusic)
                    Audio::Get().ToggleMusic();
            }

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
    Font FontResource;
    Color FontColor                 = SKYBLUE;
    const float FontSize            = 24 *Graphics::Get().GetFactorArea().y;
    const float FontOffset          = FontSize * 0.7f;

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
    bool HasEndedConversation();

};