
#include <stdexcept>
#include <sstream>
#include <set>

#include "../../include/CXXStateTree/StateTree.h"

namespace CXXStateTree
{
    void StateTree::send(const std::string &event, const std::any &context)
    {
        if (!current_)
            return;
        const auto &transitions = current_->transitions();
        auto it = transitions.find(event);
        if (it != transitions.end())
        {
            const auto &trans = it->second;
            if (trans.guard && !trans.guard->evaluate(context))
                return;
            if (trans.action)
                trans.action(context);
            current_ = find_state(trans.target);
            if (current_ && current_->initial_substate())
            {
                current_ = current_->find_substate(*current_->initial_substate());
            }
        }
        // try to send the event to parent states
        else if (current_->parent())
        {
            sendToParent(event, context, current_->parent());
        }
        else
        {
            throw std::runtime_error("Event '" + event + "' not handled in state '" + current_->name() + "'");
        }
    }

    const State &StateTree::current_state() const
    {
        return *current_;
    }

    std::string StateTree::export_dot() const
    {
        std::ostringstream os;
        os << "digraph StateTree {\n";
        os << "\trankdir=LR;\n";
        os << "\tnode [shape=box];\n";

        // Collect cluster structure
        for (const auto &s : states_)
        {
            s.collect_states(os);
        }

        std::vector<std::tuple<std::string, std::string, std::string, bool>> transitions;
        for (const auto &s : states_)
        {
            std::string name = s.name();
            s.collect_transitions(transitions, name, s.baseName());
        }

        std::set<std::string> cluster_roots;
        for (const auto &s : states_)
        {
            if (!s.substates().empty())
            {
                cluster_roots.insert(s.name());
            }
        }

        for (const auto &[from, to, event, has_guard] : transitions)
        {
            std::string label = event;
            if (has_guard)
            {
                label += " [guard]";
            }

            bool from_is_cluster = cluster_roots.count(from);
            bool to_is_cluster = cluster_roots.count(to);

            std::string from_node = from_is_cluster ? from + "_exit" : "\"" + from + "\"";
            std::string to_node = to_is_cluster ? to + "_entry" : "\"" + to + "\"";

            os << "\t" << from_node << " -> " << to_node;

            if (from_is_cluster || to_is_cluster)
            {
                os << " [label=\"" << label << "\"";
                if (from_is_cluster)
                    os << ", ltail=cluster_" << from;
                if (to_is_cluster)
                    os << ", lhead=cluster_" << to;
                os << "]";
            }
            else
            {
                os << " [label=\"" << label << "\"]";
            }

            os << ";\n";
        }

        os << "}\n";
        return os.str();
    }

    const State *StateTree::find_state(const std::string &name) const
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

    void StateTree::sendToParent(const std::string &event, const std::any &context, const State *parent)
    {
        if (!parent)
            return;
        const auto &transitions = parent->transitions();
        auto it = transitions.find(event);
        if (it != transitions.end())
        {
            const auto &trans = it->second;
            if (trans.guard && !trans.guard->evaluate(context))
                return;
            if (trans.action)
                trans.action(context);
            current_ = find_state(trans.target);
            if (current_ && current_->initial_substate())
            {
                current_ = current_->find_substate(*current_->initial_substate());
            }
        }
        else if (parent->parent())
        {
            sendToParent(event, context, parent->parent());
        }
        else
        {
            throw std::runtime_error("Event '" + event + "' not handled in parent state '" + parent->name() + "'");
        }
    }

    StateTree::Builder &StateTree::Builder::initial(const std::string &name)
    {
        initial_state_ = name;
        return *this;
    }

    StateTree::Builder &StateTree::Builder::state(const std::string &name, std::function<void(State &)> config)
    {
        states_.emplace_back(name);
        config(states_.back());
        return *this;
    }

    StateTree StateTree::Builder::build()
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
}