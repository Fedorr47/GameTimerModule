#include <gtest/gtest.h>

import gametimer;

TEST(GameTimer, ResetSetsZeroState)
{
    GameTimer timer;
    timer.Reset();
    auto state = timer.GetState();

    EXPECT_DOUBLE_EQ(state.totalSec, 0.0);
    EXPECT_DOUBLE_EQ(state.deltaSec, 0.0);
}

TEST(GameTimer, StartState)
{
    GameTimer timer;
    timer.Start();

    auto state = timer.GetState();

    EXPECT_DOUBLE_EQ(state.totalSec, 0.0);
	EXPECT_DOUBLE_EQ(state.deltaSec, 0.0);
}

TEST(GameTimer, PausedTimer)
{
    GameTimer timer;
    timer.Start();
    timer.Stop();


    EXPECT_DOUBLE_EQ(timer.IsStopped(), true);
}

TEST(GameTimer, UnPausedTimer)
{
    GameTimer timer;
	timer.Stop();
    timer.Start();


    EXPECT_DOUBLE_EQ(timer.IsStopped(), false);
}

TEST(GameTimer, AfterTick)
{
    GameTimer timer;
    timer.Start();
    timer.Tick();

    auto state = timer.GetState();

    EXPECT_GT(state.totalSec, 0.0);
	EXPECT_GT(state.deltaSec, 0.0);
}

TEST(FixedStepScheduler, BaseInvariants)
{
	FixedStepScheduler scheduler(FixedDeltaSec60);
	scheduler.Reset(123);
    FixedStepResult result = scheduler.Advance(0.0);

	EXPECT_EQ(result.tickToSimulate, 0);
    EXPECT_DOUBLE_EQ(result.alpha, 0.0);
	EXPECT_EQ(result.firstTickindex, 123);
}

TEST(FixedStepScheduler, AccumulateAnd0Ticks)
{
    FixedStepScheduler scheduler(FixedDeltaSec60);
	scheduler.SetFixedDeltaSec(0.02);
    FixedStepResult result = scheduler.Advance(0.01);

    EXPECT_EQ(result.tickToSimulate, 0);
    EXPECT_DOUBLE_EQ(result.alpha, 0.5);
    EXPECT_EQ(result.firstTickindex, 0);
}

TEST(FixedStepScheduler, AccumulateAnd1Tick)
{
    FixedStepScheduler scheduler(FixedDeltaSec60);
    scheduler.SetFixedDeltaSec(0.02);
    FixedStepResult result = scheduler.Advance(0.02);

    EXPECT_EQ(result.tickToSimulate, 1);
    EXPECT_DOUBLE_EQ(result.alpha, 0.0);
    EXPECT_EQ(result.firstTickindex, 0);
}

TEST(FixedStepScheduler, AccumulateAnd2Ticks)
{
    FixedStepScheduler scheduler(FixedDeltaSec60);
    scheduler.SetFixedDeltaSec(0.02);

    FixedStepResult result = scheduler.Advance(0.01);
    EXPECT_EQ(result.tickToSimulate, 0);
    EXPECT_DOUBLE_EQ(result.alpha, 0.5);
    EXPECT_EQ(result.firstTickindex, 0);

    result = scheduler.Advance(0.01);
    EXPECT_EQ(result.tickToSimulate, 1);
    EXPECT_DOUBLE_EQ(result.alpha, 0.0);
	EXPECT_EQ(result.firstTickindex, 0); // TODO : Should this be 1?
}