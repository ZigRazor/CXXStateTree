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
            if (!current_)
                return;
            const auto &transitions = current_->transitions();
            auto it = transitions.find(event);
            if (it != transitions.end())
            {
                const auto &trans = it->second;
                if (trans.guard && !trans.guard())
                    return;
                if (trans.action)
                    trans.action();
                current_ = find_state(trans.target);
                if (current_ && current_->initial_substate())
                {
                    current_ = current_->find_substate(*current_->initial_substate());
                }
            }
            // try to send the event to parent states
            else if (current_->parent())
            {
                sendToParent(event, current_->parent());
            }
            else
            {
                throw std::runtime_error("Event '" + event + "' not handled in state '" + current_->name() + "'");
            }
        }

        void sendToParent(const std::string &event, const State *parent)
        {
            if (!parent)
                return;
            const auto &transitions = parent->transitions();
            auto it = transitions.find(event);
            if (it != transitions.end())
            {
                const auto &trans = it->second;
                if (trans.guard && !trans.guard())
                    return;
                if (trans.action)
                    trans.action();
                current_ = find_state(trans.target);
                if (current_ && current_->initial_substate())
                {
                    current_ = current_->find_substate(*current_->initial_substate());
                }
            }
            else if (parent->parent())
            {
                sendToParent(event, parent->parent());
            }
            else
            {
                throw std::runtime_error("Event '" + event + "' not handled in parent state '" + parent->name() + "'");
            }
        }

        const State &current_state() const
        {
            return *current_;
        }

    private:
        std::list<State> states_;
        const State *current_ = nullptr;

        const State *find_state(const std::string &name) const
        {

            if (!current_ || current_->parent() == nullptr)
            {
                for (const auto &s : states_)
                {
                    if (s.name() == name)
                        return &s;
                }
                return nullptr;
            }
            else
            {

                auto foundState = current_->parent()->find_substate(name);
                if (foundState)
                {
                    return foundState;
                }

                for (const auto &s : states_)
                {
                    if (s.name() == name)
                        return &s;
                }
            }

            return nullptr;
        }
    };

} // namespace CXXStateTree
