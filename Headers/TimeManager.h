#pragma once
#include <chrono>


class TimeManager
{
	public:
		TimeManager() {
			ChronoStartFrame = chrono::high_resolution_clock::now();
			ChronoLastFrame = chrono::high_resolution_clock::now();
			ChronoTime = ChronoStartFrame - ChronoLastFrame;
		}
		inline void Update() {
			ChronoStartFrame = chrono::high_resolution_clock::now();
			ChronoTime = ChronoStartFrame - ChronoLastFrame;
			ChronoLastFrame = chrono::high_resolution_clock::now();
			using ms = chrono::duration<float, milli>;
			ChronoDT = chrono::duration_cast<ms>(ChronoTime).count();
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

	private:
		chrono::time_point<chrono::high_resolution_clock> ChronoStartFrame;
		chrono::time_point<chrono::high_resolution_clock> ChronoLastFrame;
		chrono::duration<float> ChronoTime;
		float ChronoDT = 0;
		float elapsedTime = 0;
		float timeBetweenFPSUpdates = 0.0f;
		int framesBetweenFPSUpdates = 0;
		float FPS = 0.0f;
};

