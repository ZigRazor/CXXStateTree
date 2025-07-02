#include "CXXStateTree/Builder.hpp"
#include "CXXStateTree/StateTree.hpp"
#include <iostream>
#include <fstream>

using namespace CXXStateTree;

int main()
{
    auto tree = Builder()
                    .initial("Main")
                    .state("Main", [](State &s)
                           { s.initial_substate("Idle")
                                 .substate("Idle", [](State &s)
                                           { s.on("Start", "Running", nullptr, []()
                                                  { std::cout << "Transition: Idle -> Running" << std::endl; }); })

                                 .substate("Running", [](State &s)
                                           { s.on("Stop", "Idle", nullptr, []()
                                                  { std::cout << "Transition: Running -> idle" << std::endl; }); })

                                 .on("Switch", "Alternate", nullptr, []()
                                     { std::cout << "Transition: Main -> Alternate" << std::endl; }); })
                    .state("Alternate", [](State &s)
                           { s.initial_substate("Idle")
                                 .substate("Idle", [](State &s)
                                           { s.on("Start", "Running", nullptr, []()
                                                  { std::cout << "Transition: Idle -> Running" << std::endl; }); })

                                 .substate("Running", [](State &s)
                                           { s.on("Stop", "Idle", nullptr, []()
                                                  { std::cout << "Transition: Running -> idle" << std::endl; }); })
                                 .on("Switch", "Main", nullptr, []()
                                     { std::cout << "Transition: Alternate -> Main" << std::endl; }); })
                    .build();

    // Export to DOT format and print
    std::string dot = tree.export_dot();
    std::cout << dot;

    // Optionally write to file
    std::ofstream("example_state_tree_nested.dot") << dot;

    return 0;
}
