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

        std::string baseName() const
        {
            if (parent_ != nullptr)
            {
                std::string result = parent_->baseName() + "." + name_;
                return result;
            }
            else
            {
                return "";
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

        void collect_transitions(std::vector<std::tuple<std::string, std::string, std::string>> &all_transitions, const std::string &full_name, const std::string &base_name) const
        {
            for (const auto &[event, t] : transitions_)
            {
                all_transitions.emplace_back(full_name, base_name != "" ? base_name + "." + t.target : t.target, event);
            }
            for (const auto &sub : substates_)
            {
                std::string sub_full_name = full_name + "." + sub.name();
                std::string sub_base_name = full_name;
                ;
                sub.collect_transitions(all_transitions, sub_full_name, sub_base_name);
            }
        }

        void collect_states(std::ostream &os, const std::string &prefix = "") const
        {
            std::string full_name = prefix.empty() ? name_ : prefix + "." + name_;

            if (!substates_.empty())
            {
                os << "\tsubgraph cluster_" << full_name << " {\n";
                os << "\t\tlabel = \"" << full_name << "\";\n";
                for (const auto &sub : substates_)
                {
                    sub.collect_states(os, full_name);
                }
                // Add virtual entry/exit nodes for clusters
                os << "\t\"" << full_name << "_entry\" [label=\"\", shape=point, style=invis];\n";
                os << "\t\"" << full_name << "_exit\" [label=\"\", shape=point, style=invis];\n";

                os << "\t}\n";
            }
            else
            {
                os << "\t\"" << full_name << "\";\n";
            }
        }

    private:
        std::string name_;
        State *parent_ = nullptr;
        std::list<State> substates_;
        std::unordered_map<std::string, Transition> transitions_;
        std::optional<std::string> initial_substate_;
    };
} // namespace CXXStateTree
