#pragma once

class TimeManager
{
public:
    static double get_delta_time();
    static double get_camera_lerp();
    static double get_last_frame();
    static double get_current_frame();
    static void set_camera_lerp(const double lerp);
    static void set_delta_time(const double time);
    static void set_last_frame(const double time);
    static void set_current_frame(const double time);
};
