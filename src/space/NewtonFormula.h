//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_NEWTONFORMULA_H
#define MESSENGERORBIT_NEWTONFORMULA_H
#include "SpaceObject.h"


class NewtonFormula : public SpaceObject
{
    std::vector<SpaceObject*> force_bodies;
    SpiceDouble step;
    BodyState start_state;

    static constexpr double a2 = 1.0/18.0, a3 = 1.0/12.0, a4 = 1.0/8.0, a5 = 5.0/16.0;
    static constexpr double a6 = 3.0/8.0, a7 = 59.0/400.0, a8 = 93.0/200.0, a9 = 5490023248.0/9719169821.0;
    static constexpr double a10 = 13.0/20.0, a11 = 1201146811.0/1299019798.0, a12 = 1.0, a13 = 1.0;

    static constexpr double b21 = 1.0/18.0;
    static constexpr double b31 = 1.0/48.0, b32 = 1.0/16.0;
    static constexpr double b41 = 1.0/32.0, b42 = 0.0, b43 = 3.0/32.0;
    static constexpr double b51 = 5.0/16.0, b52 = 0.0, b53 = -75.0/64.0, b54 = 75.0/64.0;
    static constexpr double b61 = 3.0/80.0, b62 = 0.0, b63 = 0.0, b64 = 3.0/16.0, b65 = 3.0/20.0;
    static constexpr double b71 = 29443841.0/614563906.0, b72 = 0.0, b73 = 0.0, b74 = 77736538.0/692538347.0;
    static constexpr double b75 = -28693883.0/1125000000.0, b76 = 23124283.0/1800000000.0;
    static constexpr double b81 = 16016141.0/946692911.0, b82 = 0.0, b83 = 0.0, b84 = 61564180.0/158732637.0;
    static constexpr double b85 = 22789713.0/633445777.0, b86 = 545815736.0/2771057229.0, b87 = -180193667.0/1043307555.0;
    static constexpr double b91 = 39632708.0/573591083.0, b92 = 0.0, b93 = 0.0, b94 = -433636366.0/683701615.0;
    static constexpr double b95 = -421739975.0/2616292301.0, b96 = 100302831.0/723423059.0;
    static constexpr double b97 = 790204164.0/839813087.0, b98 = 800635310.0/3783071287.0;
    static constexpr double b101 = 246121993.0/1340847787.0, b102 = 0.0, b103 = 0.0, b104 = -37695042795.0/15268766246.0;
    static constexpr double b105 = -309121744.0/1061227803.0, b106 = -12992083.0/490766935.0;
    static constexpr double b107 = 6005943493.0/2108947869.0, b108 = 393006217.0/1396673457.0, b109 = 123872331.0/1001029789.0;
    static constexpr double b111 = -1028468189.0/846180014.0, b112 = 0.0, b113 = 0.0, b114 = 8478235783.0/508512852.0;
    static constexpr double b115 = 1311729495.0/1432422823.0, b116 = -10304129995.0/1701304382.0;
    static constexpr double b117 = -48777925059.0/3047939560.0, b118 = 15336726248.0/1032824649.0;
    static constexpr double b119 = -45442868181.0/3398467696.0, b1110 = 3065993473.0/597172653.0;
    static constexpr double b121 = 185892177.0/718116043.0, b122 = 0.0, b123 = 0.0, b124 = -3185094517.0/667107341.0;
    static constexpr double b125 = -477755414.0/1098053517.0, b126 = -703635378.0/230739211.0;
    static constexpr double b127 = 5731566787.0/1027545527.0, b128 = 5232866602.0/850066563.0;
    static constexpr double b129 = -4093664535.0/808688257.0, b1210 = 3962137247.0/1805957418.0, b1211 = 65686358.0/487910083.0;
    static constexpr double b131 = 403863854.0/491063109.0, b132 = 0.0, b133 = 0.0, b134 = -5068492393.0/434740067.0;
    static constexpr double b135 = -411421997.0/543043805.0, b136 = 652783627.0/914296604.0;
    static constexpr double b137 = 11173962825.0/925320556.0, b138 = -13158990841.0/6184727034.0;
    static constexpr double b139 = 3936647629.0/1978049680.0, b1310 = -160528059.0/685178525.0, b1311 = 248638103.0/1413531060.0;

    // Веса для 8-го порядка
    static constexpr double c1 = 14005451.0/335480064.0, c2 = 0.0, c3 = 0.0, c4 = 0.0, c5 = 0.0;
    static constexpr double c6 = -59238493.0/1068277825.0, c7 = 181606767.0/758867731.0, c8 = 561292985.0/797845732.0;
    static constexpr double c9 = -1041891430.0/1371343529.0, c10 = 760417239.0/1151165299.0, c11 = 118820643.0/751138087.0;
    static constexpr double c12 = -528747749.0/2220607170.0, c13 = 1.0/4.0;


    [[nodiscard]] BodyState next_step(const BodyState & current_state) const;
    [[nodiscard]] Vec3d calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const;
    static Vec3d integrate(const Vec3d& derivative, SpiceDouble step);
    BodyState calculate_to_target(BodyState current_state, SpiceDouble target_time);
    static BodyState interpolate(const BodyState& first, const BodyState& second, SpiceDouble current_time);

public:
    NewtonFormula(std::vector<SpaceObject*> force_bodies, const std::string& object_name, const BodyState& start_state,
                  SpiceDouble step);
    void add_force_body(SpaceObject* force_body);
    void set_start_state(const BodyState& start_state);

    BodyState get_body_state(SpiceDouble tdb) override;
};


#endif //MESSENGERORBIT_NEWTONFORMULA_H