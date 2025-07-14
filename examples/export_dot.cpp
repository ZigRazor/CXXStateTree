#include "CXXStateTree/StateTree.h"
#include <iostream>
#include <fstream>

using namespace CXXStateTree;

int main()
{
    auto tree = StateTree::Builder()
                    .initial("App")
                    .state("App", [](State &app)
                           { app.initial_substate("Idle")
                                 .substate("Idle", [](State &idle)
                                           { idle.on("start", "Running"); })
                                 .substate("Running", [](State &running)
                                           { running.on("stop", "Idle"); }); })
                    .build();

    // Export to DOT format and print
    std::string dot = tree.export_dot();
    std::cout << dot;

    // Optionally write to file
    std::ofstream("example_state_tree.dot") << dot;

    return 0;
}
