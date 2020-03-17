#pragma once

#include <tuple>
#include <functional>


/// 4th order Runge-Kutta method for Euler's Equations
/// Δt: step size
/// γ: moment of inertia parameters
/// ω: previous angular velocity
/// Returns the new value of ω after time Δt.
/// Note: T should be defined as a 3D vector type, although no assumptions are made except a 3-argument constructor & operator[] accessor.
template<typename T>
inline T RungeKutta4Euler(const double& Δt, const T& γ, const T& ω) {

	T k1 = T(-γ[0] * ω[1] * ω[2],
			 -γ[1] * ω[0] * ω[2],
			 -γ[2] * ω[0] * ω[1]);

	T k2 = T(-γ[0] * (ω[1] + 0.5 * Δt * k1[1]) * (ω[2] + 0.5 * Δt * k1[2]),
			 -γ[1] * (ω[0] + 0.5 * Δt * k1[0]) * (ω[2] + 0.5 * Δt * k1[2]),
			 -γ[2] * (ω[0] + 0.5 * Δt * k1[0]) * (ω[1] + 0.5 * Δt * k1[1]));

	T k3 = T(-γ[0] * (ω[1] + 0.5 * Δt * k2[1]) * (ω[2] + 0.5 * Δt * k2[2]),
			 -γ[1] * (ω[0] + 0.5 * Δt * k2[0]) * (ω[2] + 0.5 * Δt * k2[2]),
			 -γ[2] * (ω[0] + 0.5 * Δt * k2[0]) * (ω[1] + 0.5 * Δt * k2[1]));

	T k4 = T(-γ[0] * (ω[1] + Δt * k3[1]) * (ω[2] + Δt * k3[2]),
			 -γ[1] * (ω[0] + Δt * k3[0]) * (ω[2] + Δt * k3[2]),
			 -γ[2] * (ω[0] + Δt * k3[0]) * (ω[1] + Δt * k3[1]));

	return ω + (k1 + k2 * 2 + k3 * 2 + k4) * (Δt / 6.0); // next value of ω.

}// T RungeKutta4Euler


/// Semi-Implicit Euler method
/// t: current time
/// Δt: step size
/// a: acceleration
/// v_n: previous value of v
/// x_n: previous value of x
/// returns (v_next, x_next) the next values of both velocity and position
template<typename T>
inline std::tuple<T, T> SemiImplicitEuler(const double& t, const double& Δt, const T& a, const T& v_n, const T& x_n) {
	
	T v_next = v_n + a * Δt;
	T x_next = x_n + v_next * Δt;

	return { v_next, x_next }; // return both the new velocity and the new position.

}// T SemiImplicitEuler
