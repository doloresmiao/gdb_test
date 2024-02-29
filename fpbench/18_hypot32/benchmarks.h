#include <fenv.h>
#include <math.h>
#include <stdint.h>
#define TRUE 1
#define FALSE 0

#ifdef __CUDACC__
#define FPBENCH_INLINE __host__ __device__
#else
#define FPBENCH_INLINE inline
#endif

FPBENCH_INLINE double carthesianToPolar_radius(double x, double y) {
	return sqrt(((x * x) + (y * y)));
}

FPBENCH_INLINE double carthesianToPolar_theta(double x, double y) {
	double pi = 3.14159265359;
	double radiant = atan((y / x));
	return radiant * (180.0 / pi);
}

FPBENCH_INLINE double polarToCarthesian_x(double radius, double theta) {
	double pi = 3.14159265359;
	double radiant = theta * (pi / 180.0);
	return radius * cos(radiant);
}

FPBENCH_INLINE double polarToCarthesian_y(double radius, double theta) {
	double pi = 3.14159265359;
	double radiant = theta * (pi / 180.0);
	return radius * sin(radiant);
}

FPBENCH_INLINE double instantaneousCurrent(double t, double resistance, double frequency, double inductance, double maxVoltage) {
	double pi = 3.14159265359;
	double impedance_re = resistance;
	double impedance_im = ((2.0 * pi) * frequency) * inductance;
	double denom = (impedance_re * impedance_re) + (impedance_im * impedance_im);
	double current_re = (maxVoltage * impedance_re) / denom;
	double current_im = -(maxVoltage * impedance_im) / denom;
	double maxCurrent = sqrt(((current_re * current_re) + (current_im * current_im)));
	double theta = atan((current_im / current_re));
	return maxCurrent * cos(((((2.0 * pi) * frequency) * t) + theta));
}

FPBENCH_INLINE double matrixDeterminant(double a, double b, double c, double d, double e, double f, double g, double h, double i) {
	return ((((a * e) * i) + ((b * f) * g)) + ((c * d) * h)) - ((((c * e) * g) + ((b * d) * i)) + ((a * f) * h));
}

FPBENCH_INLINE double matrixDeterminant2(double a, double b, double c, double d, double e, double f, double g, double h, double i) {
	return ((a * (e * i)) + ((g * (b * f)) + (c * (d * h)))) - ((e * (c * g)) + ((i * (b * d)) + (a * (f * h))));
}

FPBENCH_INLINE float intro_example_mixed(float t) {
	float tmp_2 = t + 1.0f;
	double tmp_1 = ((double) t) / ((double) tmp_2);
	return (float) tmp_1;
}

FPBENCH_INLINE double delta4(double x1, double x2, double x3, double x4, double x5, double x6) {
	return (((((-x2 * x3) - (x1 * x4)) + (x2 * x5)) + (x3 * x6)) - (x5 * x6)) + (x1 * (((((-x1 + x2) + x3) - x4) + x5) + x6));
}

FPBENCH_INLINE double delta(double x1, double x2, double x3, double x4, double x5, double x6) {
	return (((((((x1 * x4) * (((((-x1 + x2) + x3) - x4) + x5) + x6)) + ((x2 * x5) * (((((x1 - x2) + x3) + x4) - x5) + x6))) + ((x3 * x6) * (((((x1 + x2) - x3) + x4) + x5) - x6))) + ((-x2 * x3) * x4)) + ((-x1 * x3) * x5)) + ((-x1 * x2) * x6)) + ((-x4 * x5) * x6);
}

FPBENCH_INLINE double sqrt_add(double x) {
	return 1.0 / (sqrt((x + 1.0)) + sqrt(x));
}

FPBENCH_INLINE double exp1x(double x) {
	return (exp(x) - 1.0) / x;
}

FPBENCH_INLINE float exp1x_32(float x) {
	return (expf(x) - 1.0f) / x;
}

FPBENCH_INLINE double floudas(double x1, double x2) {
	return x1 + x2;
}

FPBENCH_INLINE double exp1x_log(double x) {
	return (exp(x) - 1.0) / log(exp(x));
}

FPBENCH_INLINE float x_by_xy(float x, float y) {
	return x / (x + y);
}

FPBENCH_INLINE double hypot(double x1, double x2) {
	return sqrt(((x1 * x1) + (x2 * x2)));
}

FPBENCH_INLINE float hypot32(float x1, float x2) {
	return sqrtf(((x1 * x1) + (x2 * x2)));
}

FPBENCH_INLINE double logexp1(double x) {
	return log((1.0 + exp(x)));
}

FPBENCH_INLINE double sum(double x0, double x1, double x2) {
	double p0 = (x0 + x1) - x2;
	double p1 = (x1 + x2) - x0;
	double p2 = (x2 + x0) - x1;
	return (p0 + p1) + p2;
}

FPBENCH_INLINE double nonlin1(double z) {
	return z / (z + 1.0);
}

FPBENCH_INLINE double nonlin2(double x, double y) {
	double t = x * y;
	return (t - 1.0) / ((t * t) - 1.0);
}

FPBENCH_INLINE float i4(float x, float y) {
	return sqrtf((x + (y * y)));
}

FPBENCH_INLINE float i6(float x, float y) {
	return sinf((x * y));
}

FPBENCH_INLINE double himmilbeau(double x1, double x2) {
	double a = ((x1 * x1) + x2) - 11.0;
	double b = (x1 + (x2 * x2)) - 7.0;
	return (a * a) + (b * b);
}

FPBENCH_INLINE double logexp2(double x) {
	double e = exp(x);
	return log((1.0 + e));
}

FPBENCH_INLINE double sphere(double x, double r, double lat, double lon) {
	double sinLat = sin(lat);
	double cosLon = cos(lon);
	return x + ((r * sinLat) * cosLon);
}

FPBENCH_INLINE double azimuth(double lat1, double lat2, double lon1, double lon2) {
	double dLon = lon2 - lon1;
	double s_lat1 = sin(lat1);
	double c_lat1 = cos(lat1);
	double s_lat2 = sin(lat2);
	double c_lat2 = cos(lat2);
	double s_dLon = sin(dLon);
	double c_dLon = cos(dLon);
	return atan(((c_lat2 * s_dLon) / ((c_lat1 * s_lat2) - ((s_lat1 * c_lat2) * c_dLon))));
}

FPBENCH_INLINE double floudas1(double x1, double x2, double x3, double x4, double x5, double x6) {
	return (((((-25.0 * ((x1 - 2.0) * (x1 - 2.0))) - ((x2 - 2.0) * (x2 - 2.0))) - ((x3 - 1.0) * (x3 - 1.0))) - ((x4 - 4.0) * (x4 - 4.0))) - ((x5 - 1.0) * (x5 - 1.0))) - ((x6 - 4.0) * (x6 - 4.0));
}

FPBENCH_INLINE double floudas2(double x1, double x2) {
	return -x1 - x2;
}

FPBENCH_INLINE double floudas3(double x1, double x2) {
	return ((-12.0 * x1) - (7.0 * x2)) + (x2 * x2);
}

FPBENCH_INLINE double hartman3(double x1, double x2, double x3) {
	double e1 = ((3.0 * ((x1 - 0.3689) * (x1 - 0.3689))) + (10.0 * ((x2 - 0.117) * (x2 - 0.117)))) + (30.0 * ((x3 - 0.2673) * (x3 - 0.2673)));
	double e2 = ((0.1 * ((x1 - 0.4699) * (x1 - 0.4699))) + (10.0 * ((x2 - 0.4387) * (x2 - 0.4387)))) + (35.0 * ((x3 - 0.747) * (x3 - 0.747)));
	double e3 = ((3.0 * ((x1 - 0.1091) * (x1 - 0.1091))) + (10.0 * ((x2 - 0.8732) * (x2 - 0.8732)))) + (30.0 * ((x3 - 0.5547) * (x3 - 0.5547)));
	double e4 = ((0.1 * ((x1 - 0.03815) * (x1 - 0.03815))) + (10.0 * ((x2 - 0.5743) * (x2 - 0.5743)))) + (35.0 * ((x3 - 0.8828) * (x3 - 0.8828)));
	double exp1 = exp(-e1);
	double exp2 = exp(-e2);
	double exp3 = exp(-e3);
	double exp4 = exp(-e4);
	return -((((1.0 * exp1) + (1.2 * exp2)) + (3.0 * exp3)) + (3.2 * exp4));
}

FPBENCH_INLINE double hartman6(double x1, double x2, double x3, double x4, double x5, double x6) {
	double e1 = (((((10.0 * ((x1 - 0.1312) * (x1 - 0.1312))) + (3.0 * ((x2 - 0.1696) * (x2 - 0.1696)))) + (17.0 * ((x3 - 0.5569) * (x3 - 0.5569)))) + (3.5 * ((x4 - 0.0124) * (x4 - 0.0124)))) + (1.7 * ((x5 - 0.8283) * (x5 - 0.8283)))) + (8.0 * ((x6 - 0.5886) * (x6 - 0.5886)));
	double e2 = (((((0.05 * ((x1 - 0.2329) * (x1 - 0.2329))) + (10.0 * ((x2 - 0.4135) * (x2 - 0.4135)))) + (17.0 * ((x3 - 0.8307) * (x3 - 0.8307)))) + (0.1 * ((x4 - 0.3736) * (x4 - 0.3736)))) + (8.0 * ((x5 - 0.1004) * (x5 - 0.1004)))) + (14.0 * ((x6 - 0.9991) * (x6 - 0.9991)));
	double e3 = (((((3.0 * ((x1 - 0.2348) * (x1 - 0.2348))) + (3.5 * ((x2 - 0.1451) * (x2 - 0.1451)))) + (1.7 * ((x3 - 0.3522) * (x3 - 0.3522)))) + (10.0 * ((x4 - 0.2883) * (x4 - 0.2883)))) + (17.0 * ((x5 - 0.3047) * (x5 - 0.3047)))) + (8.0 * ((x6 - 0.665) * (x6 - 0.665)));
	double e4 = (((((17.0 * ((x1 - 0.4047) * (x1 - 0.4047))) + (8.0 * ((x2 - 0.8828) * (x2 - 0.8828)))) + (0.05 * ((x3 - 0.8732) * (x3 - 0.8732)))) + (10.0 * ((x4 - 0.5743) * (x4 - 0.5743)))) + (0.1 * ((x5 - 0.1091) * (x5 - 0.1091)))) + (14.0 * ((x6 - 0.0381) * (x6 - 0.0381)));
	double exp1 = exp(-e1);
	double exp2 = exp(-e2);
	double exp3 = exp(-e3);
	double exp4 = exp(-e4);
	return -((((1.0 * exp1) + (1.2 * exp2)) + (3.0 * exp3)) + (3.2 * exp4));
}

FPBENCH_INLINE double kepler0(double x1, double x2, double x3, double x4, double x5, double x6) {
	return ((((x2 * x5) + (x3 * x6)) - (x2 * x3)) - (x5 * x6)) + (x1 * (((((-x1 + x2) + x3) - x4) + x5) + x6));
}

FPBENCH_INLINE double kepler1(double x1, double x2, double x3, double x4) {
	return (((((((x1 * x4) * (((-x1 + x2) + x3) - x4)) + (x2 * (((x1 - x2) + x3) + x4))) + (x3 * (((x1 + x2) - x3) + x4))) - ((x2 * x3) * x4)) - (x1 * x3)) - (x1 * x2)) - x4;
}

FPBENCH_INLINE double kepler2(double x1, double x2, double x3, double x4, double x5, double x6) {
	return (((((((x1 * x4) * (((((-x1 + x2) + x3) - x4) + x5) + x6)) + ((x2 * x5) * (((((x1 - x2) + x3) + x4) - x5) + x6))) + ((x3 * x6) * (((((x1 + x2) - x3) + x4) + x5) - x6))) - ((x2 * x3) * x4)) - ((x1 * x3) * x5)) - ((x1 * x2) * x6)) - ((x4 * x5) * x6);
}

FPBENCH_INLINE double intro_example(double t) {
	return t / (t + 1.0);
}

FPBENCH_INLINE double sec4_example(double x, double y) {
	double t = x * y;
	return (t - 1.0) / ((t * t) - 1.0);
}

FPBENCH_INLINE float test01_sum3(float x0, float x1, float x2) {
	float p0 = (x0 + x1) - x2;
	float p1 = (x1 + x2) - x0;
	float p2 = (x2 + x0) - x1;
	return (p0 + p1) + p2;
}

FPBENCH_INLINE double test02_sum8(double x0, double x1, double x2, double x3, double x4, double x5, double x6, double x7) {
	return ((((((x0 + x1) + x2) + x3) + x4) + x5) + x6) + x7;
}

FPBENCH_INLINE double test03_nonlin2(double x, double y) {
	return (x + y) / (x - y);
}

FPBENCH_INLINE double test04_dqmom9(double m0, double m1, double m2, double w0, double w1, double w2, double a0, double a1, double a2) {
	double v2 = (w2 * (0.0 - m2)) * (-3.0 * ((1.0 * (a2 / w2)) * (a2 / w2)));
	double v1 = (w1 * (0.0 - m1)) * (-3.0 * ((1.0 * (a1 / w1)) * (a1 / w1)));
	double v0 = (w0 * (0.0 - m0)) * (-3.0 * ((1.0 * (a0 / w0)) * (a0 / w0)));
	return 0.0 + ((v0 * 1.0) + ((v1 * 1.0) + ((v2 * 1.0) + 0.0)));
}

FPBENCH_INLINE double test05_nonlin1_r4(double x) {
	double r1 = x - 1.0;
	double r2 = x * x;
	return r1 / (r2 - 1.0);
}

FPBENCH_INLINE double test05_nonlin1_test2(double x) {
	return 1.0 / (x + 1.0);
}

FPBENCH_INLINE float test06_sums4_sum1(float x0, float x1, float x2, float x3) {
	return ((x0 + x1) + x2) + x3;
}

FPBENCH_INLINE float test06_sums4_sum2(float x0, float x1, float x2, float x3) {
	return (x0 + x1) + (x2 + x3);
}

FPBENCH_INLINE double gustafson_example(double x) {
	double q = fabs((x - sqrt(((x * x) + 1.0)))) - (1.0 / (x + sqrt(((x * x) + 1.0))));
	double y = q * q;
	double tmp;
	if (y == 0.0) {
		tmp = 1.0;
	} else {
		tmp = (exp(y) - 1.0) / x;
	}
	return tmp;
}

FPBENCH_INLINE double nmse_31(double x) {
	return sqrt((x + 1.0)) - sqrt(x);
}

FPBENCH_INLINE double nmse_33(double x, double eps) {
	return sin((x + eps)) - sin(x);
}

FPBENCH_INLINE double nmse_34(double x) {
	return (1.0 - cos(x)) / sin(x);
}

FPBENCH_INLINE double nmse_35(double N) {
	return atan((N + 1.0)) - atan(N);
}

FPBENCH_INLINE double nmse_36(double x) {
	return (1.0 / sqrt(x)) - (1.0 / sqrt((x + 1.0)));
}

FPBENCH_INLINE double nmse_331(double x) {
	return (1.0 / (x + 1.0)) - (1.0 / x);
}

FPBENCH_INLINE double nmse_332(double x, double eps) {
	return tan((x + eps)) - tan(x);
}

FPBENCH_INLINE double nmse_333(double x) {
	return ((1.0 / (x + 1.0)) - (2.0 / x)) + (1.0 / (x - 1.0));
}

FPBENCH_INLINE double nmse_334(double x) {
	return pow((x + 1.0), (1.0 / 3.0)) - pow(x, (1.0 / 3.0));
}

FPBENCH_INLINE double nmse_335(double x, double eps) {
	return cos((x + eps)) - cos(x);
}

FPBENCH_INLINE double nmse_336(double N) {
	return log((N + 1.0)) - log(N);
}

FPBENCH_INLINE double nmse_337(double x) {
	return (exp(x) - 2.0) + exp(-x);
}

FPBENCH_INLINE double nmse_p42_positive(double a, double b, double c) {
	return (-b + sqrt(((b * b) - (4.0 * (a * c))))) / (2.0 * a);
}

FPBENCH_INLINE double nmse_p42_negative(double a, double b, double c) {
	return (-b - sqrt(((b * b) - (4.0 * (a * c))))) / (2.0 * a);
}

FPBENCH_INLINE double nmse_321_positive(double a, double b2, double c) {
	return (-b2 + sqrt(((b2 * b2) - (a * c)))) / a;
}

FPBENCH_INLINE double nmse_321_negative(double a, double b2, double c) {
	return (-b2 - sqrt(((b2 * b2) - (a * c)))) / a;
}

FPBENCH_INLINE double nmse_37(double x) {
	return exp(x) - 1.0;
}

FPBENCH_INLINE double nmse_38(double N) {
	return (((N + 1.0) * log((N + 1.0))) - (N * log(N))) - 1.0;
}

FPBENCH_INLINE double nmse_39(double x) {
	return (1.0 / x) - (1.0 / tan(x));
}

FPBENCH_INLINE double nmse_310(double x) {
	return log((1.0 - x)) / log((1.0 + x));
}

FPBENCH_INLINE double nmse_341(double x) {
	return (1.0 - cos(x)) / (x * x);
}

FPBENCH_INLINE double nmse_342(double a, double b, double eps) {
	return (eps * (exp(((a + b) * eps)) - 1.0)) / ((exp((a * eps)) - 1.0) * (exp((b * eps)) - 1.0));
}

FPBENCH_INLINE double nmse_343(double eps) {
	return log(((1.0 - eps) / (1.0 + eps)));
}

FPBENCH_INLINE double nmse_344(double x) {
	return sqrt(((exp((2.0 * x)) - 1.0) / (exp(x) - 1.0)));
}

FPBENCH_INLINE double nmse_345(double x) {
	return (x - sin(x)) / (x - tan(x));
}

FPBENCH_INLINE double nmse_346(double x, double n) {
	return pow((x + 1.0), (1.0 / n)) - pow(x, (1.0 / n));
}

FPBENCH_INLINE double nmse_35(double a, double x) {
	return exp((a * x)) - 1.0;
}

FPBENCH_INLINE double nmse_311(double x) {
	return exp(x) / (exp(x) - 1.0);
}

FPBENCH_INLINE double complex_sqrt(double re, double im) {
	return 0.5 * sqrt((2.0 * (sqrt(((re * re) + (im * im))) + re)));
}

FPBENCH_INLINE double complex_sin_cos(double re, double im) {
	return (0.5 * sin(re)) * (exp(-im) - exp(im));
}

FPBENCH_INLINE double prob_in_cluster(double cp, double cn, double t, double s) {
	return (pow((1.0 / (1.0 + exp(-s))), cp) * pow((1.0 - (1.0 / (1.0 + exp(-s)))), cn)) / (pow((1.0 / (1.0 + exp(-t))), cp) * pow((1.0 - (1.0 / (1.0 + exp(-t)))), cn));
}

FPBENCH_INLINE double shoelace(double x1, double y1, double x2, double y2, double x3, double y3) {
	double s1 = (x1 * y2) - (y1 * x2);
	double s2 = (x2 * y3) - (y2 * x3);
	double s3 = (x3 * y1) - (y3 * x1);
	return 0.5 * ((s1 + s2) + s3);
}

FPBENCH_INLINE double arclength(int64_t n) {
	double dppi = (double) M_PI;
	double h = dppi / ((double) n);
	double t1 = 0.0;
	double t2 = 0.0;
	double s1 = 0.0;
	double t1_1 = t1;
	int64_t tmp = 1;
	int64_t i = tmp;
	int tmp_2 = i <= n;
	while (tmp_2) {
		double x = ((double) i) * h;
		float tmp_3 = 1.0f;
		float d1 = tmp_3;
		double t1_4 = x;
		int64_t tmp_5 = 1;
		int64_t k = tmp_5;
		int tmp_6 = k <= 5.0;
		while (tmp_6) {
			float tmp_7 = d1 * 2.0f;
			d1 = tmp_7;
			t1_4 = t1_4 + (sin((((double) d1) * x)) / ((double) d1));
			int64_t tmp_8 = k + 1;
			k = tmp_8;
			tmp_6 = k <= 5.0;
		}
		t2 = t1_4;
		double s0 = sqrt(((h * h) + ((t2 - t1_1) * (t2 - t1_1))));
		long double tmp_9 = ((long double) s1) + ((long double) s0);
		s1 = tmp_9;
		t1_1 = t2;
		int64_t tmp_10 = i + 1;
		i = tmp_10;
		tmp_2 = i <= n;
	}
	return s1;
}

FPBENCH_INLINE double arclength_old(double n) {
	double dppi = acos(-1.0);
	double h = dppi / n;
	double s1 = 0.0;
	double t1 = 0.0;
	double i = 1.0;
	int tmp = i <= n;
	while (tmp) {
		double x = i * h;
		float tmp_1 = 2.0f;
		float d0 = tmp_1;
		double t0 = x;
		double k = 1.0;
		int tmp_2 = k <= 5.0;
		while (tmp_2) {
			float tmp_3 = 2.0f * d0;
			float d0_4 = tmp_3;
			double t0_5 = t0 + (sin((((double) d0) * x)) / ((double) d0));
			double k_6 = k + 1.0;
			d0 = d0_4;
			t0 = t0_5;
			k = k_6;
			tmp_2 = k <= 5.0;
		}
		double t2 = t0;
		double s0 = sqrt(((h * h) + ((t2 - t1) * (t2 - t1))));
		long double tmp_7 = ((long double) s1) + ((long double) s0);
		long double s1_8 = tmp_7;
		double x_9 = i * h;
		float tmp_10 = 2.0f;
		float d0_11 = tmp_10;
		double t0_12 = x_9;
		double k_13 = 1.0;
		int tmp_14 = k_13 <= 5.0;
		while (tmp_14) {
			float tmp_15 = 2.0f * d0_11;
			float d0_16 = tmp_15;
			double t0_17 = t0_12 + (sin((((double) d0_11) * x_9)) / ((double) d0_11));
			double k_18 = k_13 + 1.0;
			d0_11 = d0_16;
			t0_12 = t0_17;
			k_13 = k_18;
			tmp_14 = k_13 <= 5.0;
		}
		double t2_19 = t0_12;
		double t1_20 = t2_19;
		double i_21 = i + 1.0;
		s1 = s1_8;
		t1 = t1_20;
		i = i_21;
		tmp = i <= n;
	}
	return s1;
}

FPBENCH_INLINE double doppler1(double u, double v, double T) {
	double t1 = 331.4 + (0.6 * T);
	return (-t1 * v) / ((t1 + u) * (t1 + u));
}

FPBENCH_INLINE double doppler2(double u, double v, double T) {
	double t1 = 331.4 + (0.6 * T);
	return (-t1 * v) / ((t1 + u) * (t1 + u));
}

FPBENCH_INLINE double doppler3(double u, double v, double T) {
	double t1 = 331.4 + (0.6 * T);
	return (-t1 * v) / ((t1 + u) * (t1 + u));
}

FPBENCH_INLINE double rigidBody1(double x1, double x2, double x3) {
	return ((-(x1 * x2) - ((2.0 * x2) * x3)) - x1) - x3;
}

FPBENCH_INLINE double rigidBody2(double x1, double x2, double x3) {
	return ((((((2.0 * x1) * x2) * x3) + ((3.0 * x3) * x3)) - (((x2 * x1) * x2) * x3)) + ((3.0 * x3) * x3)) - x2;
}

FPBENCH_INLINE double jetEngine(double x1, double x2) {
	double t = (((3.0 * x1) * x1) + (2.0 * x2)) - x1;
	double t_42_ = (((3.0 * x1) * x1) - (2.0 * x2)) - x1;
	double d = (x1 * x1) + 1.0;
	double s = t / d;
	double s_42_ = t_42_ / d;
	return x1 + (((((((((2.0 * x1) * s) * (s - 3.0)) + ((x1 * x1) * ((4.0 * s) - 6.0))) * d) + (((3.0 * x1) * x1) * s)) + ((x1 * x1) * x1)) + x1) + (3.0 * s_42_));
}

FPBENCH_INLINE double turbine1(double v, double w, double r) {
	return ((3.0 + (2.0 / (r * r))) - (((0.125 * (3.0 - (2.0 * v))) * (((w * w) * r) * r)) / (1.0 - v))) - 4.5;
}

FPBENCH_INLINE double turbine2(double v, double w, double r) {
	return ((6.0 * v) - (((0.5 * v) * (((w * w) * r) * r)) / (1.0 - v))) - 2.5;
}

FPBENCH_INLINE double turbine3(double v, double w, double r) {
	return ((3.0 - (2.0 / (r * r))) - (((0.125 * (1.0 + (2.0 * v))) * (((w * w) * r) * r)) / (1.0 - v))) - 0.5;
}

FPBENCH_INLINE double verhulst(double x) {
	double r = 4.0;
	double K = 1.11;
	return (r * x) / (1.0 + (x / K));
}

FPBENCH_INLINE double predatorPrey(double x) {
	double r = 4.0;
	double K = 1.11;
	return ((r * x) * x) / (1.0 + ((x / K) * (x / K)));
}

FPBENCH_INLINE double carbonGas(double v) {
	double p = 35000000.0;
	double a = 0.401;
	double b = 4.27e-5;
	double t = 300.0;
	double n = 1000.0;
	double k = 1.3806503e-23;
	return ((p + ((a * (n / v)) * (n / v))) * (v - (n * b))) - ((k * n) * t);
}

FPBENCH_INLINE double sine(double x) {
	return ((x - (((x * x) * x) / 6.0)) + (((((x * x) * x) * x) * x) / 120.0)) - (((((((x * x) * x) * x) * x) * x) * x) / 5040.0);
}

FPBENCH_INLINE double sqroot(double x) {
	return (((1.0 + (0.5 * x)) - ((0.125 * x) * x)) + (((0.0625 * x) * x) * x)) - ((((0.0390625 * x) * x) * x) * x);
}

FPBENCH_INLINE double sineOrder3(double x) {
	return (0.954929658551372 * x) - (0.12900613773279798 * ((x * x) * x));
}

FPBENCH_INLINE double smartRoot(double c) {
	double a = 3.0;
	double b = 3.5;
	double discr = (b * b) - ((a * c) * 4.0);
	double tmp_1;
	if (((b * b) - (a * c)) > 10.0) {
		double tmp_2;
		if (b > 0.0) {
			tmp_2 = (c * 2.0) / (-b - sqrt(discr));
		} else if (b < 0.0) {
			tmp_2 = (-b + sqrt(discr)) / (a * 2.0);
		} else {
			tmp_2 = (-b + sqrt(discr)) / (a * 2.0);
		}
		tmp_1 = tmp_2;
	} else {
		tmp_1 = (-b + sqrt(discr)) / (a * 2.0);
	}
	return tmp_1;
}

FPBENCH_INLINE double cav10(double x) {
	double tmp;
	if (((x * x) - x) >= 0.0) {
		tmp = x / 10.0;
	} else {
		tmp = (x * x) + 2.0;
	}
	return tmp;
}

FPBENCH_INLINE double squareRoot3(double x) {
	double tmp;
	if (x < 1e-5) {
		tmp = 1.0 + (0.5 * x);
	} else {
		tmp = sqrt((1.0 + x));
	}
	return tmp;
}

FPBENCH_INLINE double squareRoot3Invalid(double x) {
	double tmp;
	if (x < 0.0001) {
		tmp = 1.0 + (0.5 * x);
	} else {
		tmp = sqrt((1.0 + x));
	}
	return tmp;
}

FPBENCH_INLINE double triangle(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle1(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle2(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle3(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle4(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle5(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle6(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle7(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle8(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle9(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle10(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle11(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double triangle12(double a, double b, double c) {
	double s = ((a + b) + c) / 2.0;
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

FPBENCH_INLINE double bspline3(double u) {
	return -((u * u) * u) / 6.0;
}

FPBENCH_INLINE double triangleSorted(double a, double b, double c) {
	double tmp;
	if (a < b) {
		tmp = sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0;
	} else {
		tmp = sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0;
	}
	return tmp;
}

FPBENCH_INLINE double nbody(double x0, double y0, double z0, double vx0, double vy0, double vz0) {
	double dt = 0.1;
	double solarMass = 39.47841760435743;
	double x = x0;
	double y = y0;
	double z = z0;
	double vx = vx0;
	double vy = vy0;
	double vz = vz0;
	double i = 0.0;
	int tmp = i < 100.0;
	while (tmp) {
		double distance = sqrt((((x * x) + (y * y)) + (z * z)));
		double mag = dt / ((distance * distance) * distance);
		double vxNew = vx - ((x * solarMass) * mag);
		double x_1 = x + (dt * vxNew);
		double distance_2 = sqrt((((x * x) + (y * y)) + (z * z)));
		double mag_3 = dt / ((distance_2 * distance_2) * distance_2);
		double vyNew = vy - ((y * solarMass) * mag_3);
		double y_4 = y + (dt * vyNew);
		double distance_5 = sqrt((((x * x) + (y * y)) + (z * z)));
		double mag_6 = dt / ((distance_5 * distance_5) * distance_5);
		double vzNew = vz - ((z * solarMass) * mag_6);
		double z_7 = z + (dt * vzNew);
		double distance_8 = sqrt((((x * x) + (y * y)) + (z * z)));
		double mag_9 = dt / ((distance_8 * distance_8) * distance_8);
		double vx_10 = vx - ((x * solarMass) * mag_9);
		double distance_11 = sqrt((((x * x) + (y * y)) + (z * z)));
		double mag_12 = dt / ((distance_11 * distance_11) * distance_11);
		double vy_13 = vy - ((y * solarMass) * mag_12);
		double distance_14 = sqrt((((x * x) + (y * y)) + (z * z)));
		double mag_15 = dt / ((distance_14 * distance_14) * distance_14);
		double vz_16 = vz - ((z * solarMass) * mag_15);
		double i_17 = i + 1.0;
		x = x_1;
		y = y_4;
		z = z_7;
		vx = vx_10;
		vy = vy_13;
		vz = vz_16;
		i = i_17;
		tmp = i < 100.0;
	}
	return x;
}

FPBENCH_INLINE double pendulum(double t0, double w0, double N) {
	double h = 0.01;
	double L = 2.0;
	double m = 1.5;
	double g = 9.80665;
	double t = t0;
	double w = w0;
	double n = 0.0;
	int tmp = n < N;
	while (tmp) {
		double k1w = (-g / L) * sin(t);
		double k2t = w + ((h / 2.0) * k1w);
		double t_1 = t + (h * k2t);
		double k2w = (-g / L) * sin((t + ((h / 2.0) * w)));
		double w_2 = w + (h * k2w);
		double n_3 = n + 1.0;
		t = t_1;
		w = w_2;
		n = n_3;
		tmp = n < N;
	}
	return t;
}

FPBENCH_INLINE double sine_newton(double x0) {
	double x = x0;
	double i = 0.0;
	int tmp = i < 10.0;
	while (tmp) {
		double x_1 = x - ((((x - (pow(x, 3.0) / 6.0)) + (pow(x, 5.0) / 120.0)) + (pow(x, 7.0) / 5040.0)) / (((1.0 - ((x * x) / 2.0)) + (pow(x, 4.0) / 24.0)) + (pow(x, 6.0) / 720.0)));
		double i_2 = i + 1.0;
		x = x_1;
		i = i_2;
		tmp = i < 10.0;
	}
	return x;
}

FPBENCH_INLINE double rumps_pow(double a, double b) {
	return (((333.75 * pow(b, 6.0)) + (pow(a, 2.0) * (((((11.0 * pow(a, 2.0)) * pow(b, 2.0)) - pow(b, 6.0)) - (121.0 * pow(b, 4.0))) - 2.0))) + (5.5 * pow(b, 8.0))) + (a / (2.0 * b));
}

FPBENCH_INLINE double rumps_from_c(double a, double b) {
	double b2 = b * b;
	double b4 = b2 * b2;
	double b6 = b4 * b2;
	double b8 = b4 * b4;
	double a2 = a * a;
	double firstexpr = ((((11.0 * a2) * b2) - b6) - (121.0 * b4)) - 2.0;
	return (((333.75 * b6) + (a2 * firstexpr)) + (5.5 * b8)) + (a / (2.0 * b));
}

FPBENCH_INLINE double rumps_revisited(double a, double b) {
	double b2 = b * b;
	double b4 = b2 * b2;
	double b6 = b4 * b2;
	double b8 = b4 * b4;
	double a2 = a * a;
	double firstexpr = (((11.0 * a2) * b2) - (121.0 * b4)) - 2.0;
	return ((((333.75 - a2) * b6) + (a2 * firstexpr)) + (5.5 * b8)) + (a / (2.0 * b));
}

FPBENCH_INLINE float odometry(float sr_42_, float sl_42_) {
	float inv_l = 0.1f;
	float c = 12.34f;
	float delta_dl = 0.0f;
	float delta_dr = 0.0f;
	float delta_d = 0.0f;
	float delta_theta = 0.0f;
	float arg = 0.0f;
	float cosi = 0.0f;
	float x = 0.0f;
	float sini = 0.0f;
	float y = 0.0f;
	float theta = -0.985f;
	float t = 0.0f;
	float tmp = sl_42_;
	float sl = sl_42_;
	float sr = sr_42_;
	float j = 0.0f;
	int tmp_1 = t < 1000.0f;
	while (tmp_1) {
		delta_dl = c * sl;
		delta_dr = c * sr;
		delta_d = (delta_dl + delta_dr) * 0.5f;
		delta_theta = (delta_dr - delta_dl) * inv_l;
		arg = theta + (delta_theta * 0.5f);
		cosi = (1.0f - ((arg * arg) * 0.5f)) + ((((arg * arg) * arg) * arg) * 0.0416666666f);
		x = x + (delta_d * cosi);
		sini = (arg - (((arg * arg) * arg) * 0.1666666666f)) + (((((arg * arg) * arg) * arg) * arg) * 0.008333333f);
		y = y + (delta_d * sini);
		theta = theta + delta_theta;
		t = t + 1.0f;
		tmp = sl;
		float tmp_2;
		if (j == 50.0f) {
			tmp_2 = sr;
		} else {
			tmp_2 = sl;
		}
		sl = tmp_2;
		float tmp_3;
		if (j == 50.0f) {
			tmp_3 = tmp;
		} else {
			tmp_3 = sr;
		}
		sr = tmp_3;
		float tmp_4;
		if (j == 50.0f) {
			tmp_4 = 0.0f;
		} else {
			tmp_4 = j + 1.0f;
		}
		j = tmp_4;
		tmp_1 = t < 1000.0f;
	}
	return x;
}

FPBENCH_INLINE double pid(double m, double kp, double ki, double kd, double c) {
	double dt = 0.5;
	double invdt = 1.0 / dt;
	double e = 0.0;
	double p = 0.0;
	double i = 0.0;
	double d = 0.0;
	double r = 0.0;
	double m_1 = m;
	double eold = 0.0;
	double t = 0.0;
	int tmp = t < 100.0;
	while (tmp) {
		e = c - m_1;
		p = kp * e;
		i = i + ((ki * dt) * e);
		d = (kd * invdt) * (e - eold);
		r = (p + i) + d;
		m_1 = m_1 + (0.01 * r);
		eold = e;
		t = t + dt;
		tmp = t < 100.0;
	}
	return m_1;
}

FPBENCH_INLINE float runge_kutta_4(float h, float y_n_42_, float c) {
	float sixieme = 1.0f / 6.0f;
	float eps = 0.005f;
	float k = 1.2f;
	float y_n = y_n_42_;
	float i = 0.0f;
	float e = 1.0f;
	int tmp = e > eps;
	while (tmp) {
		float v = c - y_n;
		float k1 = (k * v) * v;
		float v_1 = c - (y_n + ((0.5f * h) * k1));
		float k2 = (k * v_1) * v_1;
		float v_2 = c - (y_n + ((0.5f * h) * k2));
		float k3 = (k * v_2) * v_2;
		float v_3 = c - (y_n + (h * k3));
		float k4 = (k * v_3) * v_3;
		float y_n_4 = y_n + ((sixieme * h) * (((k1 + (2.0f * k2)) + (2.0f * k3)) + k4));
		float i_5 = i + 1.0f;
		float e_6 = e - eps;
		y_n = y_n_4;
		i = i_5;
		e = e_6;
		tmp = e > eps;
	}
	return fabsf(e);
}

FPBENCH_INLINE float lead_lag(float y, float yd) {
	float eps = 0.01f;
	float Dc = -1280.0f;
	float Ac00 = 0.499f;
	float Ac01 = -0.05f;
	float Ac10 = 0.01f;
	float Ac11 = 1.0f;
	float Bc0 = 1.0f;
	float Bc1 = 0.0f;
	float Cc0 = 564.48f;
	float Cc1 = 0.0f;
	float yc = 0.0f;
	float u = 0.0f;
	float xc0 = 0.0f;
	float xc1 = 0.0f;
	float i = 0.0f;
	float e = 1.0f;
	int tmp = e > eps;
	while (tmp) {
		float v = y - yd;
		float tmp_1;
		if (v < -1.0f) {
			tmp_1 = -1.0f;
		} else if (1.0f < v) {
			tmp_1 = 1.0f;
		} else {
			tmp_1 = v;
		}
		yc = tmp_1;
		u = (Cc0 * xc0) + ((Cc1 * xc1) + (Dc * yc));
		xc0 = (Ac00 * xc0) + ((Ac01 * xc1) + (Bc0 * yc));
		xc1 = (Ac10 * xc0) + ((Ac11 * xc1) + (Bc1 * yc));
		i = i + 1.0f;
		e = fabsf((yc - xc1));
		tmp = e > eps;
	}
	return xc1;
}

FPBENCH_INLINE double trapeze(double u) {
	double a = 0.25;
	double b = 5000.0;
	double n = 25.0;
	double h = (b - a) / n;
	double xb = 0.0;
	double r = 0.0;
	double xa = 0.25;
	int tmp = xa < 5000.0;
	while (tmp) {
		double v = xa + h;
		double tmp_1;
		if (v > 5000.0) {
			tmp_1 = 5000.0;
		} else {
			tmp_1 = v;
		}
		xb = tmp_1;
		double gxa = u / ((((((0.7 * xa) * xa) * xa) - ((0.6 * xa) * xa)) + (0.9 * xa)) - 0.2);
		double gxb = u / ((((((0.7 * xb) * xb) * xb) - ((0.6 * xb) * xb)) + (0.9 * xb)) - 0.2);
		r = r + (((gxa + gxb) * 0.5) * h);
		xa = xa + h;
		tmp = xa < 5000.0;
	}
	return r;
}

FPBENCH_INLINE float rocket_trajectory(float Mf, float A) {
	float R = 6400000.0f;
	float G = 6.67428e-11f;
	float Mt = 5.9736e+24f;
	float dt = 0.1f;
	float T = 24.0f * 3600.0f;
	float nombrepas = T / dt;
	float r0 = (400.0f * 10000.0f) + R;
	float vr0 = 0.0f;
	float teta0 = 0.0f;
	float viss = sqrtf(((G * Mt) / r0));
	float vteta0 = viss / r0;
	float rf = R;
	float vrf = 0.0f;
	float tetaf = 0.0f;
	float vl = sqrtf(((G * Mt) / R));
	float vlrad = vl / r0;
	float vtetaf = 1.1f * vlrad;
	float t_i = 0.0f;
	float mf_i = 0.0f;
	float u1_i = 0.0f;
	float u3_i = 0.0f;
	float w1_i = 0.0f;
	float w3_i = 0.0f;
	float u2_i = 0.0f;
	float u4_i = 0.0f;
	float w2_i = 0.0f;
	float w4_i = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	float i = 1.0f;
	float u1_im1 = r0;
	float u2_im1 = vr0;
	float u3_im1 = teta0;
	float u4_im1 = vteta0;
	float w1_im1 = rf;
	float w2_im1 = vrf;
	float w3_im1 = tetaf;
	float w4_im1 = vtetaf;
	float t_im1 = 0.0f;
	float mf_im1 = Mf;
	int tmp = i < 2000000.0f;
	while (tmp) {
		t_i = t_im1 + dt;
		mf_i = mf_im1 - (A * t_im1);
		u1_i = (u2_im1 * dt) + u1_im1;
		u3_i = (u4_im1 * dt) + u3_im1;
		w1_i = (w2_im1 * dt) + w1_im1;
		w3_i = (w4_im1 * dt) + w3_im1;
		u2_i = ((-G * (Mt / (u1_im1 * u1_im1))) * dt) + ((u1_im1 * u4_im1) * (u4_im1 * dt));
		u4_i = ((-2.0f * (u2_im1 * (u4_im1 / u1_im1))) * dt) + u4_im1;
		float tmp_1;
		if (mf_im1 > 0.0f) {
			tmp_1 = ((A * w2_im1) / (Mf - (A * t_im1))) * dt;
		} else {
			tmp_1 = 0.0f;
		}
		w2_i = (((-G * (Mt / (w1_im1 * w1_im1))) * dt) + ((w1_im1 * w4_im1) * (w4_im1 * dt))) + (tmp_1 + w2_im1);
		float tmp_2;
		if (mf_im1 > 0.0f) {
			tmp_2 = A * ((w4_im1 / (Mf - (A * t_im1))) * dt);
		} else {
			tmp_2 = 0.0f;
		}
		w4_i = ((-2.0f * (w2_im1 * (w4_im1 / w1_im1))) * dt) + (tmp_2 + w4_im1);
		x = u1_i * cosf(u3_i);
		y = u1_i * sinf(u3_i);
		i = i + 1.0f;
		u1_im1 = u1_i;
		u2_im1 = u2_i;
		u3_im1 = u3_i;
		u4_im1 = u4_i;
		w1_im1 = w1_i;
		w2_im1 = w2_i;
		w3_im1 = w3_i;
		w4_im1 = w4_i;
		t_im1 = t_i;
		mf_im1 = mf_i;
		tmp = i < 2000000.0f;
	}
	return x;
}

FPBENCH_INLINE float jacobi_method(float a11, float a22, float a33, float a44, float b1, float b2, float b3, float b4) {
	float eps = 1e-17f;
	float x_n1 = 0.0f;
	float x_n2 = 0.0f;
	float x_n3 = 0.0f;
	float x_n4 = 0.0f;
	float i = 0.0f;
	float e = 1.0f;
	float x1 = 0.0f;
	float x2 = 0.0f;
	float x3 = 0.0f;
	float x4 = 0.0f;
	int tmp = e > eps;
	while (tmp) {
		x_n1 = (((b1 / a11) - ((0.1f / a11) * x2)) - ((0.2f / a11) * x3)) + ((0.3f / a11) * x4);
		x_n2 = (((b2 / a22) - ((0.3f / a22) * x1)) + ((0.1f / a22) * x3)) - ((0.2f / a22) * x4);
		x_n3 = (((b3 / a33) - ((0.2f / a33) * x1)) + ((0.3f / a33) * x2)) - ((0.1f / a33) * x4);
		x_n4 = (((b4 / a44) + ((0.1f / a44) * x1)) - ((0.2f / a44) * x2)) - ((0.3f / a44) * x3);
		i = i + 1.0f;
		e = fabsf((x_n4 - x4));
		x1 = x_n1;
		x2 = x_n2;
		x3 = x_n3;
		x4 = x_n4;
		tmp = e > eps;
	}
	return x2;
}

FPBENCH_INLINE float newton_raphson_method(float x0) {
	float eps = 0.0005f;
	float x_n = 0.0f;
	float e = 1.0f;
	float x = 0.0f;
	float i = 0.0f;
	int tmp = (e > eps) && (i < 100000.0f);
	while (tmp) {
		float f = ((((((x * x) * ((x * x) * x)) - ((10.0f * x) * ((x * x) * x))) + ((40.0f * x) * (x * x))) - ((80.0f * x) * x)) + (80.0f * x)) - 32.0f;
		float ff = (((((5.0f * x) * ((x * x) * x)) - ((40.0f * x) * (x * x))) + ((120.0f * x) * x)) - (160.0f * x)) + 80.0f;
		x_n = x - (f / ff);
		e = fabsf((x - x_n));
		x = x_n;
		i = i + 1.0f;
		tmp = (e > eps) && (i < 100000.0f);
	}
	return x;
}

FPBENCH_INLINE float eigenvalue_comp(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44, float v1, float v2, float v3, float v4) {
	float eps = 0.0005f;
	float vx = 0.0f;
	float vy = 0.0f;
	float vz = 0.0f;
	float vw = 0.0f;
	float i = 0.0f;
	float v1_1 = v1;
	float v2_2 = v2;
	float v3_3 = v3;
	float v4_4 = v4;
	float e = 1.0f;
	int tmp = e > eps;
	while (tmp) {
		vx = ((a11 * v1_1) + (a12 * v2_2)) + ((a13 * v3_3) + (a14 * v4_4));
		vy = ((a21 * v1_1) + (a22 * v2_2)) + ((a23 * v3_3) + (a24 * v4_4));
		vz = ((a31 * v1_1) + (a32 * v2_2)) + ((a33 * v3_3) + (a34 * v4_4));
		vw = ((a41 * v1_1) + (a42 * v2_2)) + ((a43 * v3_3) + (a44 * v4_4));
		i = i + 1.0f;
		v1_1 = vx / vw;
		v2_2 = vy / vw;
		v3_3 = vz / vw;
		v4_4 = 1.0f;
		e = fabsf((1.0f - v1_1));
		tmp = e > eps;
	}
	return v1_1;
}

FPBENCH_INLINE float iterative_gram_schmidt(float Q11, float Q12, float Q13, float Q21, float Q22, float Q23, float Q31, float Q32, float Q33) {
	float eps = 5e-6f;
	float h1 = 0.0f;
	float h2 = 0.0f;
	float h3 = 0.0f;
	float qj1 = Q31;
	float qj2 = Q32;
	float qj3 = Q33;
	float r1 = 0.0f;
	float r2 = 0.0f;
	float r3 = 0.0f;
	float r = ((qj1 * qj1) + (qj2 * qj2)) + (qj3 * qj3);
	float rjj = 0.0f;
	float e = 10.0f;
	float i = 1.0f;
	float rold = sqrtf(r);
	int tmp = e > eps;
	while (tmp) {
		h1 = ((Q11 * qj1) + (Q21 * qj2)) + (Q31 * qj3);
		h2 = ((Q12 * qj1) + (Q22 * qj2)) + (Q32 * qj3);
		h3 = ((Q13 * qj1) + (Q23 * qj2)) + (Q33 * qj3);
		qj1 = qj1 - (((Q11 * h1) + (Q12 * h2)) + (Q13 * h3));
		qj2 = qj2 - (((Q21 * h1) + (Q22 * h2)) + (Q23 * h3));
		qj3 = qj3 - (((Q31 * h1) + (Q32 * h2)) + (Q33 * h3));
		r1 = r1 + h1;
		r2 = r2 + h2;
		r3 = r3 + h3;
		r = ((qj1 * qj1) + (qj2 * qj2)) + (qj3 * qj3);
		rjj = sqrtf(r);
		e = fabsf((1.0f - (rjj / rold)));
		i = i + 1.0f;
		rold = rjj;
		tmp = e > eps;
	}
	return qj1;
}

