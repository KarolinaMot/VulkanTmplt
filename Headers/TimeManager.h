#pragma once
#include <chrono>

class TimeManager
{
	public:
		TimeManager() {
			ChronoStartFrame = std::chrono::high_resolution_clock::now();
			ChronoLastFrame = std::chrono::high_resolution_clock::now();
			ChronoTime = ChronoStartFrame - ChronoLastFrame;
		}
		inline void Update() {
			ChronoStartFrame = std::chrono::high_resolution_clock::now();
			ChronoTime = ChronoStartFrame - ChronoLastFrame;
			ChronoLastFrame = std::chrono::high_resolution_clock::now();
			using ms = std::chrono::duration<float, std::milli>;
			ChronoDT = std::chrono::duration_cast<ms>(ChronoTime).count();
			ChronoDT /= 1000;
			elapsedTime += ChronoDT;
			timeBetweenFPSUpdates += ChronoDT;
			framesBetweenFPSUpdates++;
			if (timeBetweenFPSUpdates > 1.0f)
			{
				FPS = (float)framesBetweenFPSUpdates / timeBetweenFPSUpdates;
				timeBetweenFPSUpdates -= 1.0f;
				framesBetweenFPSUpdates = 0;
			}
		}
		inline const float GetDeltaTime() {	return ChronoDT;}
		inline const float GetElapsedTime() { return elapsedTime; }
		inline const float GetFPS() { return FPS; }

	protected:

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> ChronoStartFrame;
		std::chrono::time_point<std::chrono::high_resolution_clock> ChronoLastFrame;
		std::chrono::duration<float> ChronoTime;
		float ChronoDT = 0;
		float elapsedTime = 0;
		float timeBetweenFPSUpdates = 0.0f;
		int framesBetweenFPSUpdates = 0;
		float FPS = 0.0f;
};

