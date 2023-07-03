#pragma once

#include <string>
#include "InputActionType.h"

struct InputAction
{
public:
	std::string* name;
	InputActionType actionType;

};