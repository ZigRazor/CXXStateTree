#ifndef CXXSTATETREE_IGUARD_H
#define CXXSTATETREE_IGUARD_H

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

#endif // CXXSTATETREE_IGUARD_H
