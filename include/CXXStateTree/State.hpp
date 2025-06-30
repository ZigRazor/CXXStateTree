#pragma once

#include "Transition.hpp"

namespace CXXStateTree
{

    class State
    {
    public:
        explicit State(std::string name) : name_(std::move(name)) {}

        void on(const std::string &event, const std::string &target,
                Guard guard = nullptr, Action action = nullptr)
        {
            transitions_[event] = Transition{target, guard, action};
        }

        std::optional<Transition> get_transition(const std::string &event) const
        {
            auto it = transitions_.find(event);
            if (it != transitions_.end())
                return it->second;
            return std::nullopt;
        }

        const std::string &name() const { return name_; }

    private:
        std::string name_;
        std::unordered_map<std::string, Transition> transitions_;
    };
} // namespace CXXStateTree
