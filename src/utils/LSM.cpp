//
// Created by sun on 07.12.2025.
//

#include "LSM.h"

LSM::LSM(const StateVector& initial_guess, std::map<SpiceDouble, ExtendedBodyState> &points,
        std::string file_name, double start_time, double end_time) {
    this->state = initial_guess;
    this->covariance = Eigen::MatrixXd::Zero(NUM_PARAMS, NUM_PARAMS);
    this->doppler_computer = new DopplerComputer(points);
    this->observation_data = CSVReader::read_csv(file_name, start_time, end_time);
}

StateVector LSM::do_LSM(int max_iterations, double convergence_tol) {

    // obs_data - точки из csv
    // структура что-то типа recv_time, observable_doppler, dsn_id, ref_freq
    // пока хз где ее реализовать, скорее всего в ридере

    //t0 ???

    for (int iter = 0; iter < max_iterations; ++iter) {
        std::cout << "LSM Iteration: " << iter << std::endl;

        // A, b
        Eigen::MatrixXd A(observation_data.size(), NUM_PARAMS);
        Eigen::VectorXd b(observation_data.size());
        double total_residual_sq = 0.0;

        #pragma omp parallel for reduction(+:total_residual_sq)
        for (size_t i = 0; i < observation_data.size(); ++i) {
            ObservationData& obs1 = observation_data[i];

            double doppler_calc = doppler_computer->compute_doppler(obs1.time_tag_seconds,
                obs1.full_ref_freq, obs1.receiving_station_id, obs1.Tc);

            // r
            double residual = obs1.full_observable - doppler_calc;
            std::cout << "o: " << obs1.full_observable << " c: " << doppler_calc << " r: " << residual << std::endl;
            b(i) = residual;

            // частные произдводные
            Eigen::VectorXd derivatives = compute_partial_derivatives(obs1);
            A.row(i) = derivatives;

            total_residual_sq += residual * residual;
        }

        double rms = sqrt(total_residual_sq / observation_data.size());

        // (A^T * A) * dx = A^T * b
        Eigen::MatrixXd AT = A.transpose();
        Eigen::MatrixXd ATA = AT * A;
        Eigen::VectorXd ATb = AT * b;

        // Регуляризация (добавление малого числа на диагональ)
        /*double lambda = 1e-10;
        for (int j = 0; j < NUM_PARAMS; ++j) {
            ATA(j, j) += lambda;
        }*/

        // Решение системы
        Eigen::VectorXd dx = ATA.ldlt().solve(ATb);

        // Проверка сходимости
        /*double norm_dx = dx.norm();

        if (norm_dx < convergence_tol) {
            // Сохранение ковариационной матрицы
            covariance = ATA.inverse();
            break;
        }*/

        state.update(dx);
    }
    return state;
}

Eigen::VectorXd LSM::compute_partial_derivatives(ObservationData& obs_data1) {
    Eigen::VectorXd derivatives(NUM_PARAMS);
    long double d_f_d_q_scalar = C2 / obs_data1.Tc * obs_data1.full_ref_freq;
    LightTimeSolver* light_time = this->doppler_computer->get_light_time_solver();

    SpiceDouble t_recv_first = obs_data1.time_tag_seconds - obs_data1.Tc / 2;
    SpiceDouble t_recv_second = obs_data1.time_tag_seconds + obs_data1.Tc / 2;

    Mat3d d_r_d_q1 = light_time->get_spline_mat3d(t_recv_first);
    Mat3d d_r_d_q2 = light_time->get_spline_mat3d(t_recv_second);



    Vec3d r_norm_c_1 = (-1 / C) * (light_time->get_vec_2_3(t_recv_first, obs_data1.receiving_station_id).normalized());
    Vec3d r_norm_c_2 = (-1 / C) * (light_time->get_vec_2_3(t_recv_second, obs_data1.receiving_station_id).normalized());

    long double d_p_2_3_t1 = 1 + r_norm_c_1.dot(light_time->get_vec_r_C(light_time->light_time_solve(t_recv_first, obs_data1.receiving_station_id)));
    long double d_p_2_3_t2 = 1 + r_norm_c_2.dot(light_time->get_vec_r_C(light_time->light_time_solve(t_recv_second, obs_data1.receiving_station_id)));

    Vec3d d_t2_d_q_1 = (r_norm_c_1 / d_p_2_3_t1) * d_r_d_q1; // * матрицу
    Vec3d d_t2_d_q_2 = (r_norm_c_2 / d_p_2_3_t2) * d_r_d_q2; // * матрицу

    Vec3d d_f_d_q = (d_t2_d_q_1 - d_t2_d_q_2) * d_f_d_q_scalar;
    derivatives(0) = d_f_d_q.x;
    derivatives(1) = d_f_d_q.y;
    derivatives(2) = d_f_d_q.z;



    // long double d_delta_f_t0 = - C2 * d_full_time / full_time;
    // long double d_f_t1 = - C2 * (middle_time - t0) * d_full_time / full_time;
    // long double d_f_t2 = - C2 * ((middle_time - t0) * (middle_time - t0) + d_full_time * d_full_time / 12) * d_full_time / full_time;
    //
    // derivatives(6) = d_delta_f_t0;
    // derivatives(7) = d_f_t1;
    // derivatives(8) = d_f_t2;

    return derivatives;
}

LSM::~LSM() {
    delete doppler_computer;
    observation_data.clear();
}