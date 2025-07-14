// File: examples/basic.cpp
#include <iostream>
#include "CXXStateTree/StateTree.h"

using namespace CXXStateTree;

int main()
{
       auto machine = StateTree::Builder()
                          .initial("Main")
                          .state("Main", [](State &s)
                                 { s.initial_substate("Idle")
                                       .substate("Idle", [](State &s)
                                                 { s.on("Start", "Running", nullptr, [](const std::any &)
                                                        { std::cout << "Transition: Idle -> Running" << std::endl; }); })

                                       .substate("Running", [](State &s)
                                                 { s.on("Stop", "Idle", nullptr, [](const std::any &)
                                                        { std::cout << "Transition: Running -> idle" << std::endl; }); })

                                       .on("Switch", "Alternate", nullptr, [](const std::any &)
                                           { std::cout << "Transition: Main -> Alternate" << std::endl; }); })
                          .state("Alternate", [](State &s)
                                 { s.initial_substate("Idle")
                                       .substate("Idle", [](State &s)
                                                 { s.on("Start", "Running", nullptr, [](const std::any &)
                                                        { std::cout << "Transition: Idle -> Running" << std::endl; }); })

                                       .substate("Running", [](State &s)
                                                 { s.on("Stop", "Idle", nullptr, [](const std::any &)
                                                        { std::cout << "Transition: Running -> idle" << std::endl; }); })
                                       .on("Switch", "Main", nullptr, [](const std::any &)
                                           { std::cout << "Transition: Alternate -> Main" << std::endl; }); })
                          .build();

       std::cout << "Initial state: " << machine.current_state().fullName() << std::endl;
       machine.send("Start");
       std::cout << "Current state: " << machine.current_state().fullName() << std::endl;
       machine.send("Stop");
       std::cout << "Current state: " << machine.current_state().fullName() << std::endl;
       machine.send("Switch");
       std::cout << "Current state: " << machine.current_state().fullName() << std::endl;
       machine.send("Start");
       std::cout << "Current state: " << machine.current_state().fullName() << std::endl;
       machine.send("Stop");
       std::cout << "Current state: " << machine.current_state().fullName() << std::endl;

       return 0;
}
