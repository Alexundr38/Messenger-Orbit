//
// Created by magofrays on 11/3/25.
//

#ifndef MESSENGERORBIT_CONSTANTS_H
#define MESSENGERORBIT_CONSTANTS_H

#define DP_TOL      1e-13f
#define TOL     1e-4f;
#define GRAVITY_CONSTANT 6.67430e-11f
constexpr inline long double au  = 149597870.7L;
constexpr inline long double day = 86400.0L;
constexpr inline long double color_max = 255;
constexpr inline double C = (299792.458 / au) * day; // au/d
constexpr inline double C2 = 880.0 / 240.0;

#endif //MESSENGERORBIT_CONSTANTS_H