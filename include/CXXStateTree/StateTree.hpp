// File: include/libstatetree/state_tree.hpp
#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <stdexcept>
#include "State.hpp"

namespace CXXStateTree
{
    // Forward declaration of Builder class
    // This allows us to use Builder in StateTree without including its full definition here.
    // The full definition of Builder will be in Builder.hpp, which will be included in the
    // implementation file or wherever the Builder is actually used.
    class Builder;

    class StateTree
    {
    public:
        // friend class declaration
        friend class Builder;

        void send(const std::string &event)
        {
            const auto &current = states_.at(current_state_);
            auto trans = current.get_transition(event);
            if (trans && (!trans->guard.has_value() || (trans->guard.has_value() && !trans->guard.value()) || (*trans->guard)()))
            {
                if (trans->action.has_value() && trans->action.value() != nullptr)
                {
                    (*trans->action)();
                }
                current_state_ = trans->target;
            }
        }

        const std::string &current_state() const
        {
            return current_state_;
        }

    private:
        StateTree(std::unordered_map<std::string, State> states, std::string initial)
            : states_(std::move(states)), current_state_(std::move(initial)) {}

        std::unordered_map<std::string, State> states_;
        std::string current_state_;
    };

} // namespace CXXStateTree
