// File: examples/basic.cpp
#include <iostream>
#include "CXXStateTree/StateTree.hpp"
#include "CXXStateTree/Builder.hpp"

using namespace CXXStateTree;

int main()
{
    auto machine = Builder()
                       .initial("Idle")
                       .state("Idle", [](State &s)
                              { s.on("Start", "Running", nullptr, [](const std::any &)
                                     { std::cout << "Transition: Idle -> Running" << std::endl; }); })
                       .state("Running", [](State &s)
                              {
            s.on("Pause", "Paused", nullptr, [](const std::any &) {
                std::cout << "Transition: Running -> Paused" << std::endl;
            });
            s.on("Stop", "Idle", nullptr, [](const std::any &) {
                std::cout << "Transition: Running -> Idle" << std::endl;
            }); })
                       .state("Paused", [](State &s)
                              { s.on("Resume", "Running", nullptr, [](const std::any &)
                                     { std::cout << "Transition: Paused -> Running" << std::endl; }); })
                       .build();

    std::cout << "Initial state: " << machine.current_state().name() << std::endl;
    machine.send("Start");
    std::cout << "Current state: " << machine.current_state().name() << std::endl;
    machine.send("Pause");
    std::cout << "Current state: " << machine.current_state().name() << std::endl;
    machine.send("Resume");
    std::cout << "Current state: " << machine.current_state().name() << std::endl;
    machine.send("Stop");
    std::cout << "Current state: " << machine.current_state().name() << std::endl;

    return 0;
}
