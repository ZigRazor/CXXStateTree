#pragma once

#include <list>
#include "Transition.hpp"

namespace CXXStateTree
{

    class State
    {
    public:
        explicit State(std::string name, State *parent = nullptr) : name_(std::move(name)), parent_(parent) {}

        State &on(const std::string &event, const std::string &target,
                  Guard guard = nullptr, Action action = nullptr)
        {
            transitions_[event] = {std::move(target), guard, std::move(action)};
            return *this;
        }

        State &initial_substate(const std::string &name)
        {
            initial_substate_ = name;
            return *this;
        }

        State &substate(const std::string &name, std::function<void(State &)> config)
        {
            substates_.emplace_back(name, this);
            State &new_substate = substates_.back();
            config(new_substate);
            return *this;
        }

        const std::string &name() const { return name_; }
        std::string fullName() const
        {
            if (parent_ != nullptr)
            {
                std::string result = parent_->fullName() + "." + name_;
                return result;
            }
            else
            {
                return name_;
            }
        }
        const std::list<State> &substates() const { return substates_; }
        const std::unordered_map<std::string, Transition> &transitions() const { return transitions_; }
        const std::optional<std::string> &initial_substate() const { return initial_substate_; }
        const State *parent() const { return parent_; }

        const State *find_substate(const std::string &name) const
        {
            for (const auto &s : substates_)
            {
                if (s.name() == name)
                    return &s;
            }
            if (parent_ != nullptr)
            {
                return parent_->find_substate(name);
            }
            return nullptr;
        }

    private:
        std::string name_;
        State *parent_ = nullptr;
        std::list<State> substates_;
        std::unordered_map<std::string, Transition> transitions_;
        std::optional<std::string> initial_substate_;
    };
} // namespace CXXStateTree
