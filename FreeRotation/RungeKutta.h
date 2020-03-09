#pragma once

#include <functional>

/// 4th order Runge-Kutta method
/// h: step size
/// x_n: previous value of x
/// y_n: previous value of y
/// z_n: previous value of z
/// f: differential equation dy/dx = f(x, y)
/// returns y_(n+1) the next value of y at x_(n+1) = x_n + h = x_0 + h * n
template<typename T>
T RungeKutta4(const T& h, const T& x_n, const T& y_n, const T& z_n, const std::function<T(T, T)>& f) {

	T k1 = f(x_n, y_n);
	T k2 = f(x_n + h * 0.5, y_n + h * k1 / 2);
	T k3 = f(x_n + h * 0.5, y_n + h * k2 / 2);
	T k4 = f(x_n + h, y_n + h * k3);

	return z_n + h / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);

}// double RungeKutta4
