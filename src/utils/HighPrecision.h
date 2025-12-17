#ifndef MESSENGERORBIT_HIGHPRECISION_H
#define MESSENGERORBIT_HIGHPRECISION_H

#include <boost/multiprecision/cpp_dec_float.hpp>

namespace hp {
    using real = boost::multiprecision::number<
        boost::multiprecision::cpp_dec_float<100>
    >;

    inline real from_double(double x) {
        return real(x);
    }

    inline double to_double(const real& x) {
        return x.convert_to<double>();
    }

    const real GM_SUN_KM3_S2(from_double(132712440041.27939));
    const real GM_SUN_AU3_DAY2(from_double(0.00029591220828832));

    inline real get_conversion_factor() {
        static const real factor = GM_SUN_AU3_DAY2 / GM_SUN_KM3_S2;
        return factor;
    }
}

#endif // MESSENGERORBIT_HIGHPRECISION_H