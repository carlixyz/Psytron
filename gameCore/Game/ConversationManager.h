#pragma once

#include "../Utility/Singleton.h"
#include <vector>
#include <stack>
#include <list>
#include <string>
#include "yaml-cpp/yaml.h"
#include "Register.h"

enum ConversationNodeType	//tipo de conversación
{
	NormalTalk,
	ChooseTalk,
    Optional,
    Conditional,
    Action,
	JumpBack,
	EndConversation
};

struct CharacterSpeaker		// Personajes que Hablan
{
	int CharacterId             = 0;
	std::string CharacterName   = "";
};

struct ConversationNode     // SubNodos de la conversación
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
        for (auto cond : ConditionsEntries)
        {
            bool RegisterValue = Register::Get().GetValue(cond.first);
            result = result && 
                (RegisterValue == ConditionsEntries[cond.first]);
        }

        return result;
    }

    void ExecuteAction()
    {
        Register::Get().SetValue(Action.first, Action.second);
    }
};

struct Conversation         // Raíz del arbol de subconversaciones, en nuestro ejemplo tenemos un par
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
    ConversationNode* CurrentConversationNode;
    //Time to show the conversation
    float MessageTime;
    //Selected option in a choose conversation
    unsigned ChooseOption;

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

