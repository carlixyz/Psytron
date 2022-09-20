#include "Register.h"
#include "../Utility/Utils.h"

bool Register::CheckNegation(const std::string& key)
{
    std::string keyStr = key;
    size_t negationPos = keyStr.find('!');
    bool isNegation = (negationPos != std::string::npos && negationPos == 0);
 
    return isNegation;
}

bool Register::GetValue(const std::string& key)
{
    CHECK(!CheckNegation(key)); // NO '!' Expressions allowed!

    bool result = Values[key];
    return result;
}

void Register::SetValue(const std::string& key, bool value)
{
    CHECK(!CheckNegation(key)); // NO '!' Expressions allowed!

    Values[key] = value;
}
