// File: examples/basic.cpp
#include <iostream>
#include "CXXStateTree/Builder.hpp"

using namespace CXXStateTree;

int main()
{
    auto machine = Builder()
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
