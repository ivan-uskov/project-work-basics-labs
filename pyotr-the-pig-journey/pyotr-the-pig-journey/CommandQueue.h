#pragma once

#include "Command.h"

#include <queue>

class CommandQueue
{
public:
    void push(const Command & command);
    Command pop();
    bool isEmpty() const;

    template<class... Types>
    void emplace(Types &&... args)
    {
        mQueue.emplace(std::forward<Types>(args)...);
    }

private:
    std::queue<Command> mQueue;
};
