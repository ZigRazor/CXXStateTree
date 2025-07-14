
#include "CXXStateTree/StateTree.h"
#include <iostream>
#include <fstream>

using namespace CXXStateTree;
// A custom guard that checks if the context contains an authorized user
class UserAuthorizedGuard : public IGuard
{
public:
    bool evaluate(const std::any &context) const override
    {
        if (context.type() == typeid(std::string))
        {
            std::string user = std::any_cast<std::string>(context);
            return user == "admin";
        }
        return false;
    }
};
int main()
{
    UserAuthorizedGuard auth_guard;
    auto tree = StateTree::Builder()
                    .initial("Idle")
                    .state("Idle", [&](State &s)
                           { s.on("login", "Dashboard", &auth_guard, [](const std::any &ctx)
                                  { std::cout << "Login accepted for user: " << std::any_cast<std::string>(ctx) << "\n"; }); })
                    .state("Dashboard", [&](State &s)
                           { s.on("logout", "Idle", nullptr, [](const std::any &ctx)
                                  { std::cout << "User logged out.\n"; }); })
                    .build();

    // Export to DOT format and print
    std::string dot = tree.export_dot();
    std::cout << dot;

    // Optionally write to file
    std::ofstream("example_state_tree.dot") << dot;

    return 0;
}
