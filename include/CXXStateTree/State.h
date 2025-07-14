#pragma once

#include <list>
#include "Transition.h"

namespace CXXStateTree
{

    class State
    {
    public:
        explicit State(std::string name, State *parent = nullptr);

        State &on(const std::string &event, const std::string &target,
                  IGuard *guard = nullptr, Action action = nullptr);
        State &initial_substate(const std::string &name);
        State &substate(const std::string &name, std::function<void(State &)> config);
        const std::string &name() const;
        std::string fullName() const;
        std::string baseName() const;
        const std::list<State> &substates() const;
        const std::unordered_map<std::string, Transition> &transitions() const;
        const std::optional<std::string> &initial_substate() const;
        const State *parent() const;
        const State *find_substate(const std::string &name) const;
        void collect_transitions(std::vector<std::tuple<std::string, std::string, std::string, bool>> &all_transitions, const std::string &full_name, const std::string &base_name) const;
        void collect_states(std::ostream &os, const std::string &prefix = "") const;

    private:
        std::string name_;
        State *parent_ = nullptr;
        std::list<State> substates_;
        std::unordered_map<std::string, Transition> transitions_;
        std::optional<std::string> initial_substate_;
    };
} // namespace CXXStateTree
