#pragma once

#include <string>
#include <optional>
#include <functional>

namespace CXXStateTree
{

    using Action = std::function<void()>;
    using Guard = std::function<bool()>;

    struct Transition
    {
        std::string target;
        std::optional<Guard> guard;
        std::optional<Action> action;
    };
} // namespace CXXStateTree
