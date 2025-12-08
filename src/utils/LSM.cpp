//
// Created by sun on 07.12.2025.
//

#include "LSM.h"

LSM::LSM(const StateVector& initial_guess, std::map<SpiceDouble, BodyState> &points) {
    this->state = initial_guess;
    this->covariance = Eigen::MatrixXd::Zero(NUM_PARAMS, NUM_PARAMS);
    this->doppler_computer = new DopplerComputer(points);
}

StateVector LSM::do_LSM(int max_iterations, double convergence_tol) {

    // obs_data - точки из csv
    // структура что-то типа recv_time, observable_doppler, dsn_id, ref_freq
    // пока хз где ее реализовать, скорее всего в ридере

    //t0 ???

    for (int iter = 0; iter < max_iterations; ++iter) {

        // A, b
        Eigen::MatrixXd A(obs_data.size(), NUM_PARAMS);
        Eigen::VectorXd b(obs_data.size());
        Eigen::MatrixXd W = Eigen::MatrixXd::Zero(obs_data.size(), obs_data.size());

        double total_residual_sq = 0.0;

        #pragma omp parallel for reduction(+:total_residual_sq)
        for (size_t i = 0; i < obs_data.size() - 1; ++i) {
            const /*obs_data_struct*/& obs1 = obs_data[i];
            const /*obs_data_struct*/& obs2 = obs_data[i+1];

            SpiceDouble full_time;
            double doppler_calc = doppler_computer->compute_doppler(obs1.recv_time,
                obs2.recv_time, obs1.ref_freq, obs1.dsn_id, full_time);

            // r
            double residual = obs1.observable_doppler - doppler_calc;
            b(i) = residual;

            // частные произдводные
            Eigen::VectorXd derivatives = compute_partial_derivatives(full_time);
            A.row(i) = derivatives;

            // матрица весов
            W(i, i) = obs1.weight; // узнать насчет веса

            total_residual_sq += residual * residual * obs1.weight;
        }

        double rms = sqrt(total_residual_sq / obs_data.size());

        // (A^T * W * A) * dx = A^T * W * b
        Eigen::MatrixXd AT = A.transpose();
        Eigen::MatrixXd ATWA = AT * W * A;
        Eigen::VectorXd ATWb = AT * W * b;

        // Регуляризация (добавление малого числа на диагональ)
        double lambda = 1e-10;
        for (int j = 0; j < NUM_PARAMS; ++j) {
            ATWA(j, j) += lambda;
        }

        // Решение системы
        Eigen::VectorXd dx = ATWA.ldlt().solve(ATWb);

        // Проверка сходимости
        double norm_dx = dx.norm();

        if (norm_dx < convergence_tol) {
            // Сохранение ковариационной матрицы
            covariance = ATWA.inverse();
            break;
        }

        state.update(dx);
    }
    return state;
}

Eigen::VectorXd LSM::compute_partial_derivatives(SpiceDouble& full_time, obs_data_struct& obs_data1, obs_data_struct& obs_data2) {
    Eigen::VectorXd derivatives(NUM_PARAMS);
    long double d_f_d_q_scalar = C2 / full_time * obs_data1.ref_freq;
    long double d_r_d_q = 0; // сделать
    Vec3d d_t2_d_q_1 = -1 / C * (this->doppler_computer->compute_doppler(obs_data1.recv_time, obs_data1.dsn_id).normalize()); // * на dr/dq / 1-ro/c
    Vec3d d_t2_d_q_2 = -1 / C * (this->doppler_computer->compute_doppler(obs_data2.recv_time, obs_data1.dsn_id).normalize()); // * на dr/dq / 1-ro/c

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