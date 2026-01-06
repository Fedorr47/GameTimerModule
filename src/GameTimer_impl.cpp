module;                 // global module fragment

#include <chrono>
#include <algorithm>
#include <cstdint>

module gametimer;

void GameTimer::Reset()
{
	const auto now = clock::now();

	baseTime_ = now;
	previousTime_ = now;
	currentTime_ = now;

	stopTime_ = time_point{};
	pausedTime_ = clock::duration::zero();

	stopped_ = false;
	deltaTime_ = 0.0;
}

void GameTimer::Start()
{
	if (!stopped_)
	{
		return;
	}

	const auto startTime = clock::now();
	pausedTime_ += startTime - stopTime_;

	previousTime_ = startTime;
	stopTime_ = time_point{};
	stopped_ = false;
}

void GameTimer::Stop()
{
	if (stopped_)
	{
		return;
	}

	stopTime_ = clock::now();
	stopped_ = true;
}

void GameTimer::Tick()
{
	if (stopped_)
	{
		deltaTime_ = 0.0;
		return;
	}

	currentTime_ = clock::now();
	const auto delta = currentTime_ - previousTime_;
	previousTime_ = currentTime_;

	deltaTime_ = std::chrono::duration<double>(delta).count();
	deltaTime_ = std::clamp(deltaTime_, 0.0, maxDeltaSec_);
}

double GameTimer::GetTotalTime() const
{
	const auto totalTime = stopped_ ? stopTime_ : currentTime_;
	const auto elapsed = (totalTime - baseTime_) - pausedTime_;
	return std::chrono::duration<double>(elapsed).count();
}

double GameTimer::GetDeltaTime() const
{
	return deltaTime_;
}

FixedStepResult FixedStepScheduler::Advance(double frameDeltaSec)
{
	if (frameDeltaSec < 0.0)
	{
		frameDeltaSec = 0.0;
	}

	accumulatedDeltaSec_ += frameDeltaSec;

	FixedStepResult result{};
	result.firstTickindex = tickIndex_;

	int tickCount = 0;
	while (accumulatedDeltaSec_ >= fixedDeltaSec_ && tickCount < maxCatchupTicks_)
	{
		accumulatedDeltaSec_ -= fixedDeltaSec_;
		++tickCount;
		++tickIndex_;
	}

	result.tickToSimulate = tickCount;

	result.alpha = std::clamp(accumulatedDeltaSec_ / fixedDeltaSec_, 0.0, 1.0);
	return result;
}
