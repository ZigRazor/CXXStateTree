// File: tests/state_tree_test.cpp
#include <gtest/gtest.h>
#include "CXXStateTree/StateTree.hpp"
#include "CXXStateTree/Builder.hpp"

using namespace CXXStateTree;

TEST(StateTreeTest, InitialStateIsSetCorrectly)
{
       auto machine = Builder()
                          .initial("Idle")
                          .state("Idle", [](State &s)
                                 { s.on("Start", "Running"); })
                          .state("Running", [](State &s)
                                 { s.on("Stop", "Idle"); })
                          .build();

       EXPECT_EQ(machine.current_state().name(), "Idle");
}

TEST(StateTreeTest, TransitionChangesState)
{
       auto machine = Builder()
                          .initial("Idle")
                          .state("Idle", [](State &s)
                                 { s.on("Start", "Running"); })
                          .state("Running", [](State &s)
                                 { s.on("Stop", "Idle"); })
                          .build();

       machine.send("Start");
       EXPECT_EQ(machine.current_state().name(), "Running");

       machine.send("Stop");
       EXPECT_EQ(machine.current_state().name(), "Idle");
}

TEST(StateTreeTest, GuardPreventsTransition)
{
       auto machine = Builder()
                          .initial("Idle")
                          .state("Idle", [](State &s)
                                 { s.on("Start", "Running", []()
                                        { return false; }); })
                          .state("Running", [](State &s)
                                 { s.on("Stop", "Idle"); })
                          .build();

       machine.send("Start");
       EXPECT_EQ(machine.current_state().name(), "Idle"); // Guard blocks transition
}

TEST(StateTreeTest, ActionIsCalled)
{
       bool actionCalled = false;

       auto machine = Builder()
                          .initial("Idle")
                          .state("Idle", [&](State &s)
                                 { s.on("Start", "Running", nullptr, [&]()
                                        { actionCalled = true; }); })
                          .state("Running", [](State &s)
                                 { s.on("Stop", "Idle"); })
                          .build();

       machine.send("Start");
       EXPECT_TRUE(actionCalled);
       EXPECT_EQ(machine.current_state().name(), "Running");
}
