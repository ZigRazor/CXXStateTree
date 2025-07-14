#include "CXXStateTree/StateTree.h"
#include <iostream>
#include <string>
#include <any>

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

    auto sm = StateTree::Builder()
                  .initial("Idle")
                  .state("Idle", [&](State &s)
                         { s.on("login", "Dashboard", &auth_guard, [](const std::any &ctx)
                                { std::cout << "Login accepted for user: " << std::any_cast<std::string>(ctx) << "\n"; }); })
                  .state("Dashboard", [&](State &s)
                         { s.on("logout", "Idle", nullptr, [](const std::any &ctx)
                                { std::cout << "User logged out.\n"; }); })
                  .build();

    std::cout << "Sending login with unauthorized user:\n";
    sm.send("login", std::string("guest")); // Guard will reject

    std::cout << "Sending login with admin:\n";
    sm.send("login", std::string("admin")); // Guard will accept

    std::cout << "Sending logout:\n";
    sm.send("logout");
}
