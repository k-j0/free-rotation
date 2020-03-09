#pragma once

#include <functional>
#include <tuple>

/// 4th order Runge-Kutta method
/// Δx: step size
/// x_n: previous value of x
/// y_n: previous value of y
/// z_n: previous value of z
/// f: differential equation dy/dx = f(x, y)
/// returns y_(n+1) the next value of y at x_(n+1) = x_n + h = x_0 + h * n
template<typename T>
T RungeKutta4(const double& Δx, const T& x_n, const T& y_n, const T& z_n, const std::function<T(T, T)>& f) {

	T k1 = f(x_n, y_n);
	T k2 = f(x_n + Δx * 0.5, y_n + Δx * k1 / 2);
	T k3 = f(x_n + Δx * 0.5, y_n + Δx * k2 / 2);
	T k4 = f(x_n + Δx, y_n + Δx * k3);

	return z_n + Δx / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4);

}// T RungeKutta4

/// Semi-Implicit Euler method
/// t: current time
/// Δt: step size
/// v_n: previous value of v
/// x_n: previous value of x
/// g: differential equation dv/dt = g(t, x)
/// f: differential equation dx/dt = f(t, v)
/// returns (v_next, x_next) the next values of both velocity and position
template<typename T>
std::tuple<T, T> SemiImplicitEuler(const double& t, const double& Δt, const T& v_n, const T& x_n, const std::function<T(double, T)> g, const std::function<T(double, T)> f) {
	
	T v_next = v_n + g(t, x_n) * Δt;
	T x_next = x_n + f(t, v_next) * Δt;

	return { v_next, x_next };

}// T SemiImplicitEuler
