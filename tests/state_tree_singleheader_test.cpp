// File: tests/state_tree_singleheader_test.cpp
#include <gtest/gtest.h>
#include "CXXStateTree.hpp"

using namespace CXXStateTree;

TEST(StateTreeSingleHeaderTest, InitialStateIsSetCorrectly)
{
    auto machine = StateTree::Builder()
                       .initial("Idle")
                       .state("Idle", [](State &s)
                              { s.on("Start", "Running"); })
                       .state("Running", [](State &s)
                              { s.on("Stop", "Idle"); })
                       .build();

    EXPECT_EQ(machine.current_state().name(), "Idle");
}

TEST(StateTreeSingleHeaderTest, TransitionChangesState)
{
    auto machine = StateTree::Builder()
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

TEST(StateTreeSingleHeaderTest, GuardPreventsTransition)
{
    class NotPassGuard : public IGuard
    {
    public:
        bool evaluate(const std::any &context) const override
        {
            return false;
        }
    };
    auto machine = StateTree::Builder()
                       .initial("Idle")
                       .state("Idle", [](State &s)
                              { s.on("Start", "Running", new NotPassGuard()); })
                       .state("Running", [](State &s)
                              { s.on("Stop", "Idle"); })
                       .build();

    machine.send("Start");
    EXPECT_EQ(machine.current_state().name(), "Idle"); // Guard blocks transition
}

TEST(StateTreeSingleHeaderTest, ActionIsCalled)
{
    bool actionCalled = false;

    auto machine = StateTree::Builder()
                       .initial("Idle")
                       .state("Idle", [&](State &s)
                              { s.on("Start", "Running", nullptr, [&](const std::any &)
                                     { actionCalled = true; }); })
                       .state("Running", [](State &s)
                              { s.on("Stop", "Idle"); })
                       .build();

    machine.send("Start");
    EXPECT_TRUE(actionCalled);
    EXPECT_EQ(machine.current_state().name(), "Running");
}
