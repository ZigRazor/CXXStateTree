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
        Guard guard = nullptr;
        Action action = nullptr;
    };
} // namespace CXXStateTree
