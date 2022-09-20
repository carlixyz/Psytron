#pragma once

#include "../Utility/Singleton.h"
#include <iostream>
#include <map>

class Register : public Singleton<Register>
{
	friend class Singleton<Register>;

	std::map<std::string, bool> Values;

	bool CheckNegation(const std::string& key);

public:
	// you can check a simple expression like "!var_is_enabled"

	// Default Getter
	bool GetValue(const std::string& key);

	// Default Setter
	void SetValue(const std::string& key, bool value);

};

