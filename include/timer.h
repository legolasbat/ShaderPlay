#pragma once

#include <chrono>

class Timer {
protected:

	std::chrono::steady_clock::time_point startingTime;
	std::chrono::steady_clock::time_point prevTime;

	std::chrono::steady_clock::time_point pauseStart;

	bool pause;
	std::chrono::duration<float> pauseDuration;

	int frame;
	float frameRate;

	float time;
	float timeDelta;

public:

	Timer() {
		pause = false;
	}

	void Start() {
		Reset();
	}

	void Step() {
		if (pause) {
			timeDelta = 0.0f;
			return;
		}

		auto now = std::chrono::steady_clock::now();

		time = std::chrono::duration<float>(now - startingTime - pauseDuration).count();

		timeDelta = std::chrono::duration<float>(now - prevTime).count();
		prevTime = now;

		frameRate = 1.0f / timeDelta;

		frame++;
	}

	void Pause() {
		pause = !pause;

		if (pause) {
			pauseStart = std::chrono::steady_clock::now();
		}
		else {
			auto now = std::chrono::steady_clock::now();
			pauseDuration += (now - pauseStart);
			prevTime = now;
		}
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
		pauseStart = startingTime;

		pauseDuration = std::chrono::duration<float>(0);

		frame = 0;
		frameRate = 0.0f;

		time = 0.0f;
		timeDelta = 0.0f;
	}
};
