#pragma once

#include <chrono>

class Timer {
protected:

	std::chrono::steady_clock::time_point startingTime;
	std::chrono::steady_clock::time_point prevTime;

	int frame;
	float frameRate;

	float time;
	float timeDelta;

public:

	Timer() {
		Reset();
	}

	void Step() {
		auto now = std::chrono::steady_clock::now();

		time = std::chrono::duration<float>(now - startingTime).count();

		timeDelta = std::chrono::duration<float>(now - prevTime).count();
		prevTime = now;

		frameRate = 1.0f / timeDelta;

		frame++;
	}

	float GetTime() const {
		return time;
	}

	float GetTimeDelta() const {
		return timeDelta;
	}

	int GetFrame() const {
		return frame;
	}

	float GetFramerate() const {
		return frameRate;
	}

	void Reset() {
		startingTime = std::chrono::steady_clock::now();
		prevTime = startingTime;

		frame = 0;
		frameRate = 0.0f;

		time = 0.0f;
		timeDelta = 0.0f;
	}
};
