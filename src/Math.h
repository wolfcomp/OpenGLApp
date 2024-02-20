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
    T operator()(const float t)
    {
        const auto u = 1 - t;
        return powf(u, 3) * p0 +
            3 * t * powf(u, 2) * p1 +
            3 * powf(t, 2) * u * p2 +
            powf(t, 3) * p3;
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
        if (points.size() == 0)
            return T();
        if (points.size() == 1)
            return std::get<1>(points[0])(t);
        auto it = points.begin();
        auto cur = *it;
        auto next = *(++it);
        while (t >= std::get<0>(cur) && next != points.end())
        {
            cur = next;
            next = *(++it);
        }
        if(next == points.end())
            return std::get<1>(cur)(1);
        auto u = (t - std::get<0>(cur)) / (std::get<0>(next) - std::get<0>(cur));
        return std::get<1>(cur)(u);
    }
};
#endif