#include "TimeManager.h"

double deltaTime;
double cameraLerp;
double lastFrame;
double currentFrame;

double TimeManager::get_camera_lerp()
{
    return cameraLerp;
}

double TimeManager::get_current_frame()
{
    return currentFrame;
}

double TimeManager::get_delta_time()
{
    return deltaTime;
}

double TimeManager::get_last_frame()
{
    return lastFrame;
}

void TimeManager::set_camera_lerp(const double lerp)
{
    cameraLerp = lerp;
}

void TimeManager::set_current_frame(const double time)
{
    currentFrame = time;
}

void TimeManager::set_delta_time(const double time)
{
    deltaTime = time;
}

void TimeManager::set_last_frame(const double time)
{
    lastFrame = time;
}