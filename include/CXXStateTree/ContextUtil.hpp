#ifndef CXXSTATETREE_CONTEXTUTIL_HPP
#define CXXSTATETREE_CONTEXTUTIL_HPP

#include <string>
#include <optional>
#include <functional>
#include <any>

namespace CXXStateTree
{

    template <typename T>
    const T &get_context(const std::any &ctx)
    {
        if (!ctx.has_value())
            throw std::bad_any_cast();

        return std::any_cast<const T &>(ctx);
    }

    template <typename T>
    T get_context_or(const std::any &ctx, const T &default_value)
    {
        if (ctx.type() == typeid(T))
        {
            return std::any_cast<T>(ctx);
        }
        return default_value;
    }

} // namespace CXXStateTree

#endif // CXXSTATETREE_CONTEXTUTIL_H
