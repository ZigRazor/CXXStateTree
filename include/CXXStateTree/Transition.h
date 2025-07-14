#ifndef CXXSTATETREE_TRANSITION_H
#define CXXSTATETREE_TRANSITION_H

#include <string>
#include <optional>
#include <functional>
#include "IGuard.h"

namespace CXXStateTree
{

    using Action = std::function<void(const std::any &)>;

    struct Transition
    {
        std::string target;
        const IGuard *guard = nullptr;
        Action action = nullptr;
    };
} // namespace CXXStateTree
#endif // CXXSTATETREE_TRANSITION_H