module;

#include <cstdint>
#include <algorithm>
#include <chrono>

export module gametimer;

export inline constexpr double MaxDeltaSec = 1e-9;
export inline constexpr double FixedDeltaSec60 = 1.0 / 60.0;

export struct TimeState
{
	double totalSec{};
	double deltaSec{};
};

export class GameTimer
{
public:
	GameTimer(){ Reset(); }
	~GameTimer(){}
	
	GameTimer(const GameTimer& other) : GameTimer() {};
	GameTimer(GameTimer&& other) noexcept {};
	
	GameTimer& operator=(const GameTimer& other)
	{
		return *this = GameTimer(other);
	}
	GameTimer& operator=(GameTimer&& other) noexcept
	{
		// Swap here
		return *this;
	}
	
	void Reset();
	void Start();
	void Stop();
	void Tick();
	
	double GetTotalTime() const;
	double GetDeltaTime() const;
	
	bool IsStopped() const { return stopped_; };
	
	void SetMaxDelta(double sec) { maxDeltaSec_ = std::max(0.0, sec); };
	double GetMaxDelta() const { return maxDeltaSec_; };

	TimeState GetState() const { return { GetTotalTime(), GetDeltaTime() }; } 
	
private:
	
	using clock = std::chrono::steady_clock;
	using time_point = clock::time_point;
	
	double deltaTime_{0.0};
	
	time_point baseTime_{};
	time_point previousTime_{};
	time_point currentTime_{};
	time_point stopTime_{};
	
	clock::duration pausedTime_{clock::duration::zero()};
	
	bool stopped_{false};
	double maxDeltaSec_{0.1};
};

export struct FixedStepResult
{
	int tickToSimulate{0};
	int64_t firstTickindex{0};
	double alpha{0.0};
};

export class FixedStepScheduler
{
public:
	explicit FixedStepScheduler(double fixedDeltaSec = FixedDeltaSec60) :
		fixedDeltaSec_{fixedDeltaSec} {}
	
	void SetFixedDeltaSec(double sec) { fixedDeltaSec_ = std::max(sec, MaxDeltaSec); };
	double GetFixedDeltaSec() { return fixedDeltaSec_; };
	
	void SetMaxCatchupTicks(int numberOfTicks) { maxCatchupTicks_ = std::max(0, numberOfTicks); };
	int GetMaxCatchupTicks() { return maxCatchupTicks_; };
	
	void Reset(int64_t tickIndex = 0)
	{
		accumulatedDeltaSec_ = 0.0;
		tickIndex_ = tickIndex;
	}

	FixedStepResult Advance(double frameDeltaSec);
	
private:
	double fixedDeltaSec_{FixedDeltaSec60};
	double accumulatedDeltaSec_{0.0};
	int64_t tickIndex_{0};
	int maxCatchupTicks_{8};
};