#ifndef CXXSTATETREE_STATETREE_H
#define CXXSTATETREE_STATETREE_H

#include <string>

#include <optional>
#include "State.h"

namespace CXXStateTree
{
    class StateTree
    {
    public:
        class Builder
        {
        public:
            Builder &initial(const std::string &name);
            Builder &state(const std::string &name, std::function<void(State &)> config);
            StateTree build();

        private:
            std::list<State> states_;
            std::string initial_state_;
        };

        void send(const std::string &event, const std::any &context = {});
        const State &current_state() const;
        std::string export_dot() const;

    private:
        std::list<State> states_;
        const State *current_ = nullptr;

        const State *find_state(const std::string &name) const;
        void sendToParent(const std::string &event, const std::any &context, const State *parent);
    };

} // namespace CXXStateTree
#endif // CXXSTATETREE_STATETREE_H