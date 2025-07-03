#pragma once

#include <string>
#include <optional>
#include <functional>
#include <any>

namespace CXXStateTree
{

    class IGuard
    {
    public:
        virtual ~IGuard() = default;
        virtual bool evaluate(const std::any &context) const = 0;
    };

} // namespace CXXStateTree