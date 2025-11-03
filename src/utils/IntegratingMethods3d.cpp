//
// Created by magofrays on 11/3/25.
//

#include "IntegratingMethods3d.h"

#include <cmath>
#include <functional>

Vec3d IntegratingMethods3d::dopri8_method(double h, const std::function<Vec3d(Vec3d)>& vec_function, const Vec3d& pos)
{
    double tol = 0.0001;
    double min_h = 0.001;
    Vec3d k1 = vec_function(pos);
    Vec3d k2 = vec_function(pos + k1 * (h / 18.0));
    Vec3d k3 = vec_function(pos + k1 * (h / 48.0) + k2 * (h / 16.0));
    Vec3d k4 = vec_function(pos + k1 * (h / 32.0) + k3 * (h * 3.0 / 32.0));
    Vec3d k5 = vec_function(pos + k1 * (h * 5.0 / 16.0) - k3 * (h * 75.0 / 64.0) + k4 * (h * 75.0 / 64.0));
    Vec3d k6 = vec_function(pos + k1 * (h * 3.0 / 80.0) + k4 * (h * 3.0 / 16.0) + k5 * (h * 3.0 / 20.0));
    Vec3d k7 = vec_function(pos + k1 * (h * 29443841.0 / 614563906.0) + k4 * (h * 77736538.0 / 692538347.0) - k5 * (h * 28693883.0 / 1125000000.0) + k6 * (h * 23124283.0 / 1800000000.0));
    Vec3d k8 = vec_function(pos + k1 * (h * 16016141.0 / 946692911.0) + k4 * (h * 61564180.0 / 158732637.0) + k5 * (h * 22789713.0 / 633445777.0) + k6 * (h * 545815736.0 / 2771057229.0) - k7 * (h * 180193667.0 / 1043307555.0));
    Vec3d k9 = vec_function(pos + k1 * (h * 39632708.0 / 573591083.0) - k4 * (h * 433636366.0 / 683701615.0) - k5 * (h * 421739975.0 / 2616292301.0) + k6 * (h * 100302831.0 / 723423059.0) + k7 * (h * 790204164.0 / 839813087.0) + k8 * (h * 800635310.0 / 3783071287.0));
    Vec3d k10 = vec_function(pos + k1 * (h * 246121993.0 / 1340847787.0) - k4 * (h * 37695042795.0 / 15268766246.0) - k5 * (h * 309121744.0 / 1061227803.0) - k6 * (h * 12992083.0 / 490766935.0) + k7 * (h * 6005943493.0 / 2108947869.0) + k8 * (h * 393006217.0 / 1396673457.0) + k9 * (h * 123872331.0 / 1001029789.0));
    Vec3d k11 = vec_function(pos - k1 * (h * 1028468189.0 / 846180014.0) + k4 * (h * 8478235783.0 / 508512852.0) + k5 * (h * 1311729495.0 / 1432422823.0) - k6 * (h * 10304129995.0 / 1701304382.0) - k7 * (h * 48777925059.0 / 3047939560.0) + k8 * (h * 15336726248.0 / 1032824649.0) - k9 * (h * 45442868181.0 / 3398467696.0) + k10 * (h * 3065993473.0 / 597172653.0));
    Vec3d k12 = vec_function(pos + k1 * (h * 185892177.0 / 718116043.0) - k4 * (h * 3185094517.0 / 667107341.0) - k5 * (h * 477755414.0 / 1098053517.0) - k6 * (h * 703635378.0 / 230739211.0) + k7 * (h * 5731566787.0 / 1027545527.0) + k8 * (h * 5232866602.0 / 850066563.0) - k9 * (h * 4093664535.0 / 808688257.0) + k10 * (h * 3962137247.0 / 1805957418.0) + k11 * (h * 65686358.0 / 487910083.0));
    Vec3d k13 = vec_function(pos + k1 * (h * 403863854.0 / 491063109) - k4 * (h * 5068492393.0 / 434740067.0) - k5 * (h * 411421997.0 / 543043805.0) + k6 * (h * 652783627.0 / 914296604.0) + k7 * (h * 11173962825.0 / 925320556.0) - k8 * (h * 13158990841.0 / 6184727024.0) + k9 * (h * 3936647629.0 / 1978049680.0) - k10 * (h * 160528059.0 / 685178525.0) + k11 * (h * 248638103.0 / 1413531060.0));
    Vec3d add_point1 = k1 * (h * 14005451.0 / 335480064.0) - k6 * (h * 59238493.0 / 1068277825.0) + k7 * (h * 181606767.0 / 758867731.0) + k8 * (h * 561292985.0 / 797845732.0) - k9 * (h * 1041891430.0 / 1371343529.0) + k10 * (h * 760417239.0 / 1151165299.0) + k11 * (h * 118820643.0 / 751138087.0) - k12 * (h * 528747749.0 / 2220607170.0);
    Vec3d add_point2 = k1 * (h * 13451932.0 / 455176623.0) - k6 * (h * 808719846.0 / 976000145.0) + k7 * (h * 1757004468.0 / 5645159321.0) + k8 * (h * 656045339.0 / 265891186.0) - k9 * (h * 3867574721.0 / 1518517206.0) + k10 * (h * 465885868.0 / 322736535.0) + k11 * (h * 53011238.0 / 667516719.0) + k12 * (h * 2.0 / 45.0);

    Vec3d error_vec = add_point1 - add_point2;
    double error = error_vec.norm();

    if (error < tol || h * pow(tol / error, 1.0 / 8.0) < min_h)
    {
        h = std::max(min_h, h * pow(tol / error, 1.0 / 9.0));
        return add_point1;
    }
    h *= std::pow(tol / error, 1.0 / 8.0);
    return dopri8_method(h, vec_function, pos);
}

Vec3d IntegratingMethods3d::rk4_method(double h, const std::function<Vec3d(Vec3d, double)>& vec_function, const Vec3d& pos)
{
    Vec3d k1 = vec_function(pos, h);
    Vec3d k2 = vec_function(pos + k1 * (h * 0.5f), h);
    Vec3d k3 = vec_function(pos + k2 * (h * 0.5f), h);
    Vec3d k4 = vec_function(pos + k3 * h, h);
    return (k1 * 0.16667f + k2 * 0.33333f + k3 * 0.33333f + k4 * 0.16667f) * h;
}

Vec3d IntegratingMethods3d::euler_method(double h, const std::function<Vec3d(Vec3d, double)>& vec_function, const Vec3d& pos)
{
    Vec3d k1 = vec_function(pos, h);
    return k1 * h;
}
