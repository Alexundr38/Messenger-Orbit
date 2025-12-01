#include "NewtonFormula.h"
#include <cmath>
#include <iostream>
#include <utility>

// Константы метода DOPRI8
const double a2 = 1.0/18.0, a3 = 1.0/12.0, a4 = 1.0/8.0, a5 = 5.0/16.0, a6 = 3.0/8.0, a7 = 59.0/400.0,
             a8 = 93.0/200.0, a9 = 5490023248.0/9719169821.0, a10 = 13.0/20.0, a11 = 1201146811.0/1299019798.0,
             a12 = 1.0, a13 = 1.0;

const double b21 = 1.0/18.0;
const double b31 = 1.0/48.0, b32 = 1.0/16.0;
const double b41 = 1.0/32.0, b43 = 3.0/32.0;
const double b51 = 5.0/16.0, b53 = -75.0/64.0, b54 = 75.0/64.0;
const double b61 = 3.0/80.0, b64 = 3.0/16.0, b65 = 3.0/20.0;
const double b71 = 29443841.0/614563906.0, b74 = 77736538.0/692538347.0,
             b75 = -28693883.0/1125000000.0, b76 = 23124283.0/1800000000.0;
const double b81 = 16016141.0/946692911.0, b84 = 61564180.0/158732637.0,
             b85 = 22789713.0/633445777.0, b86 = 545815736.0/2771057229.0, b87 = -180193667.0/1043307555.0;
const double b91 = 39632708.0/573591083.0, b94 = -433636366.0/683701615.0,
             b95 = -421739975.0/2616292301.0, b96 = 100302831.0/723423059.0,
             b97 = 790204164.0/839813087.0, b98 = 800635310.0/3783071287.0;
const double b101 = 246121993.0/1340847787.0, b104 = -37695042795.0/15268766246.0,
              b105 = -309121744.0/1061227803.0, b106 = -12992083.0/490766935.0,
              b107 = 6005943493.0/2108947869.0, b108 = 393006217.0/1396673457.0,
              b109 = 123872331.0/1001029789.0;
const double b111 = -1028468189.0/846180014.0, b114 = 8478235783.0/508512852.0,
              b115 = 1311729495.0/1432422823.0, b116 = -10304129995.0/1701304382.0,
              b117 = -48777925059.0/3047939560.0, b118 = 15336726248.0/1032824649.0,
              b119 = -45442868181.0/3398467696.0, b1110 = 3065993473.0/597172653.0;
const double b121 = 185892177.0/718116043.0, b124 = -3185094517.0/667107341.0,
              b125 = -477755414.0/1098053517.0, b126 = -703635378.0/230739211.0,
              b127 = 5731566787.0/1027545527.0, b128 = 5232866602.0/850066563.0,
              b129 = -4093664535.0/808688257.0, b1210 = 3962137247.0/1805957418.0,
              b1211 = 65686358.0/487910083.0;
const double b131 = 403863854.0/491063109.0, b134 = -5068492393.0/434740067.0,
              b135 = -411421997.0/543043805.0, b136 = 652783627.0/914296604.0,
              b137 = 11173962825.0/925320556.0, b138 = -13158990841.0/6184727034.0,
              b139 = 3936647629.0/1978049680.0, b1310 = -160528059.0/685178525.0,
              b1311 = 248638103.0/1413531060.0;

// Веса для 8-го порядка
const double c1 = 14005451.0/335480064.0, c6 = -59238493.0/1068277825.0,
             c7 = 181606767.0/758867731.0, c8 = 561292985.0/797845732.0,
             c9 = -1041891430.0/1371343529.0, c10 = 760417239.0/1151165299.0,
             c11 = 118820643.0/751138087.0, c12 = -528747749.0/2220607170.0,
             c13 = 1.0/4.0;

NewtonFormula::NewtonFormula(
    std::vector<SpaceObject*> force_bodies,
    const std::string & object_name,
    const BodyState & start_state,
    const SpiceDouble step
) : force_bodies(std::move(force_bodies)), step(step), start_state(start_state),
    use_implicit_methods(false), use_corrector(false),
    use_newton_methods(false), stiffness_threshold(1.0)
{
    this->size = 100;
    SpaceObject::set_object_name(object_name);
    SpaceObject::add_body_state(start_state);
    SpaceObject::set_current_body_state(start_state);
    previous_states.push_back(start_state);
}

void NewtonFormula::set_object_name(const std::string& object_name) {
    SpaceObject::set_object_name(object_name);
}

BodyState NewtonFormula::get_body_state(const SpiceDouble tdb) {
    auto current = get_current_body_state();
    if (tdb == current.time) {
        std::cout << current.position.x << " " << current.position.y << " " << current.position.z << " " << std::endl;
        return current;
    }
    auto exact = body_states.find(tdb);
    if (exact != body_states.end()) {
        set_current_body_state(exact->second);
        std::cout << exact->second.position.x << " " << exact->second.position.y << " " << exact->second.position.z << " " << std::endl;
        return exact->second;
    }

    auto upper = body_states.upper_bound(tdb);
    if (upper == body_states.end()) {
        if (body_states.empty()) {
            throw std::runtime_error("No body states available");
        }
        auto last = body_states.rbegin();
        calculate_to_target(last->second, tdb);

        exact = body_states.find(tdb);
        if (exact != body_states.end()) {
            set_current_body_state(exact->second);
            std::cout << exact->second.position.x << " " << exact->second.position.y << " " << exact->second.position.z << " " << std::endl;
            return exact->second;
        }
        upper = body_states.upper_bound(tdb);
    }
    BodyState body = interpolate(
        std::prev(upper)->second,
        upper->second, tdb
    );
    set_current_body_state(body);
    std::cout << body.position.x << " " << body.position.y << " " << body.position.z << " " << std::endl;

    return body;
}

void NewtonFormula::set_current_body_state(const BodyState& state) {
    current_body_state = state;
}

BodyState NewtonFormula::get_current_body_state() const {
    return current_body_state;
}

BodyState NewtonFormula::next_step(const BodyState& current_state) const {
    if (use_implicit_methods) {
        return next_step_implicit_newton(current_state);
    }

    const double h = step;
    const Vec3d& y0_pos = current_state.position;
    const Vec3d& y0_vel = current_state.velocity;
    const double t0 = current_state.time;

    std::vector<Vec3d> k_pos(13), k_vel(13);

    k_vel[0] = calculate_acceleration(t0, y0_pos);
    k_pos[0] = y0_vel;

    Vec3d pos2 = y0_pos + k_pos[0] * (h * b21);
    Vec3d vel2 = y0_vel + k_vel[0] * (h * b21);
    k_vel[1] = calculate_acceleration(t0 + h * a2, pos2);
    k_pos[1] = vel2;

    Vec3d pos3 = y0_pos + k_pos[0] * (h * b31) + k_pos[1] * (h * b32);
    Vec3d vel3 = y0_vel + k_vel[0] * (h * b31) + k_vel[1] * (h * b32);
    k_vel[2] = calculate_acceleration(t0 + h * a3, pos3);
    k_pos[2] = vel3;

    Vec3d pos4 = y0_pos + k_pos[0] * (h * b41) + k_pos[2] * (h * b43);
    Vec3d vel4 = y0_vel + k_vel[0] * (h * b41) + k_vel[2] * (h * b43);
    k_vel[3] = calculate_acceleration(t0 + h * a4, pos4);
    k_pos[3] = vel4;

    Vec3d pos5 = y0_pos + k_pos[0] * (h * b51) + k_pos[2] * (h * b53) + k_pos[3] * (h * b54);
    Vec3d vel5 = y0_vel + k_vel[0] * (h * b51) + k_vel[2] * (h * b53) + k_vel[3] * (h * b54);
    k_vel[4] = calculate_acceleration(t0 + h * a5, pos5);
    k_pos[4] = vel5;

    Vec3d pos6 = y0_pos + k_pos[0] * (h * b61) + k_pos[3] * (h * b64) + k_pos[4] * (h * b65);
    Vec3d vel6 = y0_vel + k_vel[0] * (h * b61) + k_vel[3] * (h * b64) + k_vel[4] * (h * b65);
    k_vel[5] = calculate_acceleration(t0 + h * a6, pos6);
    k_pos[5] = vel6;

    Vec3d pos7 = y0_pos + k_pos[0] * (h * b71) + k_pos[3] * (h * b74) + k_pos[4] * (h * b75) + k_pos[5] * (h * b76);
    Vec3d vel7 = y0_vel + k_vel[0] * (h * b71) + k_vel[3] * (h * b74) + k_vel[4] * (h * b75) + k_vel[5] * (h * b76);
    k_vel[6] = calculate_acceleration(t0 + h * a7, pos7);
    k_pos[6] = vel7;

    Vec3d pos8 = y0_pos + k_pos[0] * (h * b81) + k_pos[3] * (h * b84) + k_pos[4] * (h * b85) +
                k_pos[5] * (h * b86) + k_pos[6] * (h * b87);
    Vec3d vel8 = y0_vel + k_vel[0] * (h * b81) + k_vel[3] * (h * b84) + k_vel[4] * (h * b85) +
                k_vel[5] * (h * b86) + k_vel[6] * (h * b87);
    k_vel[7] = calculate_acceleration(t0 + h * a8, pos8);
    k_pos[7] = vel8;

    Vec3d pos9 = y0_pos + k_pos[0] * (h * b91) + k_pos[3] * (h * b94) + k_pos[4] * (h * b95) +
                k_pos[5] * (h * b96) + k_pos[6] * (h * b97) + k_pos[7] * (h * b98);
    Vec3d vel9 = y0_vel + k_vel[0] * (h * b91) + k_vel[3] * (h * b94) + k_vel[4] * (h * b95) +
                k_vel[5] * (h * b96) + k_vel[6] * (h * b97) + k_vel[7] * (h * b98);
    k_vel[8] = calculate_acceleration(t0 + h * a9, pos9);
    k_pos[8] = vel9;

    Vec3d pos10 = y0_pos + k_pos[0] * (h * b101) + k_pos[3] * (h * b104) + k_pos[4] * (h * b105) +
                 k_pos[5] * (h * b106) + k_pos[6] * (h * b107) + k_pos[7] * (h * b108) + k_pos[8] * (h * b109);
    Vec3d vel10 = y0_vel + k_vel[0] * (h * b101) + k_vel[3] * (h * b104) + k_vel[4] * (h * b105) +
                 k_vel[5] * (h * b106) + k_vel[6] * (h * b107) + k_vel[7] * (h * b108) + k_vel[8] * (h * b109);
    k_vel[9] = calculate_acceleration(t0 + h * a10, pos10);
    k_pos[9] = vel10;

    Vec3d pos11 = y0_pos + k_pos[0] * (h * b111) + k_pos[3] * (h * b114) + k_pos[4] * (h * b115) +
                 k_pos[5] * (h * b116) + k_pos[6] * (h * b117) + k_pos[7] * (h * b118) + k_pos[8] * (h * b119) +
                 k_pos[9] * (h * b1110);
    Vec3d vel11 = y0_vel + k_vel[0] * (h * b111) + k_vel[3] * (h * b114) + k_vel[4] * (h * b115) +
                 k_vel[5] * (h * b116) + k_vel[6] * (h * b117) + k_vel[7] * (h * b118) + k_vel[8] * (h * b119) +
                 k_vel[9] * (h * b1110);
    k_vel[10] = calculate_acceleration(t0 + h * a11, pos11);
    k_pos[10] = vel11;

    Vec3d pos12 = y0_pos + k_pos[0] * (h * b121) + k_pos[3] * (h * b124) + k_pos[4] * (h * b125) +
                 k_pos[5] * (h * b126) + k_pos[6] * (h * b127) + k_pos[7] * (h * b128) + k_pos[8] * (h * b129) +
                 k_pos[9] * (h * b1210) + k_pos[10] * (h * b1211);
    Vec3d vel12 = y0_vel + k_vel[0] * (h * b121) + k_vel[3] * (h * b124) + k_vel[4] * (h * b125) +
                 k_vel[5] * (h * b126) + k_vel[6] * (h * b127) + k_vel[7] * (h * b128) + k_vel[8] * (h * b129) +
                 k_vel[9] * (h * b1210) + k_vel[10] * (h * b1211);
    k_vel[11] = calculate_acceleration(t0 + h * a12, pos12);
    k_pos[11] = vel12;

    Vec3d pos13 = y0_pos + k_pos[0] * (h * b131) + k_pos[3] * (h * b134) + k_pos[4] * (h * b135) +
                 k_pos[5] * (h * b136) + k_pos[6] * (h * b137) + k_pos[7] * (h * b138) + k_pos[8] * (h * b139) +
                 k_pos[9] * (h * b1310) + k_pos[10] * (h * b1311);
    Vec3d vel13 = y0_vel + k_vel[0] * (h * b131) + k_vel[3] * (h * b134) + k_vel[4] * (h * b135) +
                 k_vel[5] * (h * b136) + k_vel[6] * (h * b137) + k_vel[7] * (h * b138) + k_vel[8] * (h * b139) +
                 k_vel[9] * (h * b1310) + k_vel[10] * (h * b1311);
    k_vel[12] = calculate_acceleration(t0 + h * a13, pos13);
    k_pos[12] = vel13;

    Vec3d new_pos = y0_pos + h * (
        k_pos[0] * c1 + k_pos[5] * c6 + k_pos[6] * c7 + k_pos[7] * c8 +
        k_pos[8] * c9 + k_pos[9] * c10 + k_pos[10] * c11 + k_pos[11] * c12 + k_pos[12] * c13
    );

    Vec3d new_vel = y0_vel + h * (
        k_vel[0] * c1 + k_vel[5] * c6 + k_vel[6] * c7 + k_vel[7] * c8 +
        k_vel[8] * c9 + k_vel[9] * c10 + k_vel[10] * c11 + k_vel[11] * c12 + k_vel[12] * c13
    );

    BodyState predictor_state(new_pos, new_vel, t0 + h);

    if (use_corrector) {
        return trapezoidal_corrector_newton(current_state, predictor_state);
    }
    std::cout << predictor_state.position.x << " " << predictor_state.position.y << " " << predictor_state.position.z << " " << std::endl;
    return predictor_state;
}

BodyState NewtonFormula::next_step_implicit_newton(const BodyState& current_state) const {
    const double h = step;
    const double t_next = current_state.time + h;

    BodyState x = next_step_explicit_euler(current_state);

    const int max_iterations = 10;
    const double tolerance = 1e-12;

    for (int iter = 0; iter < max_iterations; ++iter) {
        Vec3d F_pos = x.position - current_state.position - h * x.velocity;
        Vec3d F_vel = x.velocity - current_state.velocity - h * calculate_acceleration(t_next, x.position);

        Mat3d J_accel = calculate_acceleration_jacobian(t_next, x.position);
        Vec3d delta_pos = -F_pos + h * F_vel;
        Vec3d delta_vel = -F_vel + h * J_accel * F_pos;

        x.position += delta_pos;
        x.velocity += delta_vel;

        double norm_F = std::sqrt(F_pos.squaredNorm() + F_vel.squaredNorm());
        double norm_delta = std::sqrt(delta_pos.squaredNorm() + delta_vel.squaredNorm());

        if (norm_F < tolerance && norm_delta < tolerance) {
            break;
        }

        if (iter == max_iterations - 1) {
            std::cout << "Метод Ньютона не сошелся за " << max_iterations
                      << " итераций. Норма невязки: " << norm_F << std::endl;
        }
    }

    return BodyState(x.position, x.velocity, t_next);
}

BodyState NewtonFormula::trapezoidal_corrector_newton(const BodyState& current_state,
                                                     const BodyState& predictor_state) const {
    const double h = step;
    const double t_next = current_state.time + h;

    BodyState x = predictor_state;

    const int max_iterations = 5;
    const double tolerance = 1e-12;

    for (int iter = 0; iter < max_iterations; ++iter) {
        Vec3d accel_current = calculate_acceleration(current_state.time, current_state.position);
        Vec3d accel_next = calculate_acceleration(t_next, x.position);

        Vec3d F_pos = x.position - current_state.position - (h/2.0) * (current_state.velocity + x.velocity);
        Vec3d F_vel = x.velocity - current_state.velocity - (h/2.0) * (accel_current + accel_next);

        Mat3d J_accel = calculate_acceleration_jacobian(t_next, x.position);

        Vec3d delta_pos = -F_pos + (h/2.0) * F_vel;
        Vec3d delta_vel = -F_vel + (h/2.0) * J_accel * F_pos;

        x.position += delta_pos;
        x.velocity += delta_vel;

        double norm_F = std::sqrt(F_pos.squaredNorm() + F_vel.squaredNorm());

        if (norm_F < tolerance) {
            break;
        }
    }

    return BodyState(x.position, x.velocity, t_next);
}

Mat3d NewtonFormula::calculate_acceleration_jacobian(SpiceDouble time, const Vec3d& position) const {
    Mat3d jacobian = Mat3d::Zero();

    for (SpaceObject* body : force_bodies) {
        BodyState body_state = body->get_body_state(time);
        Vec3d r_vec = body_state.position - position;
        double r = r_vec.norm();

        if (r > 1e-10) {
            double mu = body->get_gravitational_parameter();
            double r3 = r * r * r;
            double r5 = r3 * r * r;

            Mat3d term1 = -mu / r3 * Mat3d::Identity();

            Mat3d outer_product(
                r_vec.x * r_vec.x, r_vec.x * r_vec.y, r_vec.x * r_vec.z,
                r_vec.y * r_vec.x, r_vec.y * r_vec.y, r_vec.y * r_vec.z,
                r_vec.z * r_vec.x, r_vec.z * r_vec.y, r_vec.z * r_vec.z
            );
            Mat3d term2 = (3.0 * mu / r5) * outer_product;

            jacobian += term1 + term2;
        }
    }

    return jacobian;
}

BodyState NewtonFormula::implicit_midpoint_newton(const BodyState& current_state) const {
    const double h = step;
    const double t_mid = current_state.time + h/2.0;

    BodyState x_mid = current_state;

    const int max_iterations = 8;
    const double tolerance = 1e-12;

    for (int iter = 0; iter < max_iterations; ++iter) {
        Vec3d F_pos = x_mid.position - current_state.position - (h/2.0) * x_mid.velocity;
        Vec3d F_vel = x_mid.velocity - current_state.velocity - (h/2.0) * calculate_acceleration(t_mid, x_mid.position);

        Mat3d J_accel = calculate_acceleration_jacobian(t_mid, x_mid.position);

        Vec3d delta_pos = -F_pos - (h/2.0) * F_vel;
        Vec3d delta_vel = -F_vel - (h/2.0) * J_accel * F_pos;

        x_mid.position += delta_pos;
        x_mid.velocity += delta_vel;

        double norm_F = std::sqrt(F_pos.squaredNorm() + F_vel.squaredNorm());
        if (norm_F < tolerance) {
            break;
        }
    }

    Vec3d final_pos = 2.0 * x_mid.position - current_state.position;
    Vec3d final_vel = 2.0 * x_mid.velocity - current_state.velocity;

    return BodyState(final_pos, final_vel, current_state.time + h);
}

BodyState NewtonFormula::dopri8_newton_corrector(const BodyState& current_state) const {
    BodyState predictor = next_step(current_state);
    return trapezoidal_corrector_newton(current_state, predictor);
}

BodyState NewtonFormula::adaptive_newton_method(const BodyState& current_state) const {
    double stiffness_estimate = estimate_stiffness(current_state);
    if (stiffness_estimate > stiffness_threshold) {
        return next_step_implicit_newton(current_state);
    } else {
        return implicit_midpoint_newton(current_state);
    }
}
double NewtonFormula::estimate_stiffness(const BodyState& state) const {
    Mat3d J = calculate_acceleration_jacobian(state.time, state.position);
    double frobenius_norm = J.norm();
    return frobenius_norm * step;
}

BodyState NewtonFormula::next_step_explicit_euler(const BodyState& current_state) const {
    const double h = step;
    Vec3d accel = calculate_acceleration(current_state.time, current_state.position);

    Vec3d new_pos = current_state.position + h * current_state.velocity;
    Vec3d new_vel = current_state.velocity + h * accel;

    return BodyState(new_pos, new_vel, current_state.time + h);
}

Vec3d NewtonFormula::calculate_acceleration(SpiceDouble time, const Vec3d& current_position) const {
    Vec3d acceleration;
    for (SpaceObject* body : force_bodies) {
        BodyState body_state = body->get_body_state(time);
        Vec3d r_vec = body_state.position - current_position;
        double r = r_vec.norm();

        if (r > 1e-10) {
            double mu = body->get_gravitational_parameter();
            double r3 = r * r * r;
            acceleration += mu * r_vec / r3;
        }
    }
    return acceleration;
}

Vec3d NewtonFormula::integrate(const Vec3d& derivative, const SpiceDouble step) {
    return derivative * step;
}

BodyState NewtonFormula::calculate_to_target(BodyState current_state, SpiceDouble target_time) {
    while (current_state.time < target_time) {
        if (use_newton_methods) {
            current_state = adaptive_newton_method(current_state);
        } else {
            current_state = next_step(current_state);
        }
        add_body_state(current_state);

        previous_states.push_back(current_state);
        if (previous_states.size() > 4) {
            previous_states.pop_front();
        }
    }

    return current_state;
}

BodyState NewtonFormula::interpolate(const BodyState& first, const BodyState& second, const SpiceDouble current_time) {
    const double factor = (current_time - first.time) / (second.time - first.time);
    BodyState result;
    result.time = current_time;
    result.position = first.position + factor * (second.position - first.position);
    result.velocity = first.velocity + factor * (second.velocity - first.velocity);
    return result;
}

void NewtonFormula::set_start_state(const BodyState& start_state) {
    this->start_state = start_state;
    set_current_body_state(start_state);
    body_states.clear();
    add_body_state(start_state);
    previous_states.clear();
    previous_states.push_back(start_state);
}

void NewtonFormula::add_force_body(SpaceObject* force_body) {
    force_bodies.push_back(force_body);
}

void NewtonFormula::set_implicit_methods(bool use_implicit) {
    use_implicit_methods = use_implicit;
}

void NewtonFormula::set_use_corrector(bool use_corr) {
    use_corrector = use_corr;
}

void NewtonFormula::set_newton_methods(bool use_newton) {
    use_newton_methods = use_newton;
}

void NewtonFormula::set_stiffness_threshold(double threshold) {
    stiffness_threshold = threshold;
}