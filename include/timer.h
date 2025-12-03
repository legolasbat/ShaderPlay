#pragma once

#include <chrono>

struct Date {
	float year;
	float month;
	float day;
	float time;
};

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

		frame = 0;
		frameRate = 0.0f;

		time = 0.0f;
		timeDelta = 0.0f;
	}

	void Start() {
		Reset();
	}

	void Step() {
		if (pause) {
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

	Date GetDate() {
		// Get current time as time_t
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);

		// Convert to local time
		std::tm* local = std::localtime(&t);

		Date date{};

		date.year = local->tm_year + 1900;  // Years since 1900
		date.month = local->tm_mon + 1;      // Months since January (0–11)
		date.day = local->tm_mday;
		int hour = local->tm_hour;
		int minute = local->tm_min;
		int second = local->tm_sec;
		auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
		float milliseconds = (float)(ms.time_since_epoch().count() % 1000) / 1000;
		date.time = hour * 3600 + minute * 60 + second + milliseconds;

		return date;
	}
};
