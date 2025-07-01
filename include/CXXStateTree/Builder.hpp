#pragma once

#include <list>
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
            states_.emplace_back(name);
            config(states_.back());
            return *this;
        }

        StateTree build()
        {
            StateTree tree;
            tree.states_ = std::move(states_);
            tree.current_ = tree.find_state(initial_state_);
            if (tree.current_ && tree.current_->initial_substate())
            {
                tree.current_ = tree.current_->find_substate(*tree.current_->initial_substate());
            }
            return tree;
        }

    private:
        std::list<State> states_;
        std::string initial_state_;
    };
} // namespace CXXStateTree
