#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.1415926535897932384626433832795

#ifndef BEZIER
#define BEZIER
template <typename T>
struct Bezier
{
    T p0, p1, p2, p3;
    Bezier(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}
    T operator()(float t)
    {
        return lerp(lerp(lerp(p0, p1, t), lerp(p1, p2, t), t), lerp(lerp(p1, p2, t), lerp(p2, p3, t), t), t);
    }
};
#endif

#ifndef BSPLINE
// ReSharper disable once IdentifierTypo
#define BSPLINE
template <typename T>
struct BSpline
{
    std::vector<std::tuple<float, Bezier<T>>> points;
    void add_point(float t, Bezier<T> curve)
    {
        points.push_back(std::tuple<float, Bezier<T>>(t, curve));
    }

    T operator()(float t)
    {
        for (int i = 0; i < points.size() - 1; i++)
        {
            if (std::get<0>(points[i]) <= t && t <= std::get<0>(points[i + 1]))
            {
                float t0 = std::get<0>(points[i]);
                float t1 = std::get<0>(points[i + 1]);
                float t_ = (t - t0) / (t1 - t0);
                return std::get<1>(points[i])(t_);
            }
        }
        return std::get<1>(points[points.size() - 1])(1);
    }
};
#endif
