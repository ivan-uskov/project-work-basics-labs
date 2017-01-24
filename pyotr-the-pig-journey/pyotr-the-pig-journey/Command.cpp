#include "stdafx.h"
#include "Command.h"

Command::Command(unsigned category, Action && action)
    : action(move(action))
    , category(category)
{}