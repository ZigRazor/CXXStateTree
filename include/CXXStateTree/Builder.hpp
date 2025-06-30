#pragma once

#include "StateTree.hpp"

namespace CXXStateTree
{

    class Builder
    {
    public:
        Builder &initial(const std::string &name)
        {
            initial_state_ = name;
            return *this;
        }

        Builder &state(const std::string &name, std::function<void(State &)> config)
        {
            State s(name);
            config(s);
            states_.insert({name, std::move(s)});
            return *this;
        }

        StateTree build()
        {
            if (initial_state_.empty())
            {
                throw std::runtime_error("Initial state not set");
            }
            return StateTree(states_, initial_state_);
        }

    private:
        std::unordered_map<std::string, State> states_;
        std::string initial_state_;
    };
} // namespace CXXStateTree
