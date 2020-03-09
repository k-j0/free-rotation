
// Includes
#include "Vector.h"
#include "RungeKutta.h"
#include "CSVWriter.h"
#include <ctime>



/// Definition of the shape needed (cone)
const double Mass = 10.0; // kilograms
const double Radius = 1.0; // meters
const double Height = 4.0; // meters
#define IT_F 3.0 * Mass / 20.0 // base inertia tensor factor
// Inertia tensor is a 3x3 diagonal matrix, represented here as a vector
const DiagMat3 InertiaTensor(
	IT_F * (Radius * Radius + Height * Height / 4.0),
	IT_F * (Radius * Radius + Height * Height / 4.0),
	IT_F * 2.0 * Radius * Radius
);
#undef IT_F

/// Definition of the angular velocity
const Vec3 AngularVelocity ( 3.0, 1.0, 2.0 ); // radians per second

/// Definition of the step size and time span
const double t_0 = 0.0;
const double t_max = 20.0;
const double h = 0.05; // step size



int main() {

	// output results to a CSV file
	CSVWriter csv;
	csv.insert("t");
	csv.insert("omega x");
	csv.insert("omega y");
	csv.insert("omega z");

	// initial angular velocity
	Vec3 ω = AngularVelocity;

	// inertia tensor for the shape
	const DiagMat3 I = InertiaTensor;

	// parameters γ for Euler's equations
	const Vec3 γ ( (I[2] - I[1]) / I[0],  // γ1 = (I_3 - I_2) / I_1
				   (I[0] - I[2]) / I[1],  // γ2 = (I_1 - I_3) / I_2
				   (I[1] - I[0]) / I[2] );// γ3 = (I_2 - I_1) / I_3

	// iterative loop (over an int rather than double for maximum precision)
	for (int n = 0; n <= (t_max - t_0) / h; ++n) {
		double t = double(n) * h + t_0; // current time

		// Define Euler's equations
		std::function<double(double, double, double)> Euler = [] (double ω_1, double ω_2, double γ) { return -γ * ω_1 * ω_2; };
		std::function<double(double, double)> EulerX = [&γ, &Euler] (double ω_y, double ω_z) { return Euler(γ[0], ω_y, ω_z); };
		std::function<double(double, double)> EulerY = [&γ, &Euler] (double ω_x, double ω_z) { return Euler(γ[1], ω_x, ω_z); };
		std::function<double(double, double)> EulerZ = [&γ, &Euler] (double ω_x, double ω_y) { return Euler(γ[2], ω_x, ω_y); };

		// 4th-order Runge-Kutta method on Euler's equations
		Vec3 nextΩ (
			RungeKutta4<double>(h, ω.Y(), ω.Z(), ω.X(), EulerX),
			RungeKutta4<double>(h, ω.X(), ω.Z(), ω.Y(), EulerY),
			RungeKutta4<double>(h, ω.X(), ω.Y(), ω.Z(), EulerZ)
		);
		ω = nextΩ;

		// Write results to the csv file
		csv.nextColumn();
		csv.insert(t);
		csv.insert(ω.X());
		csv.insert(ω.Y());
		csv.insert(ω.Z());

	}// iterative loop

	// Save results
	csv.writeToFile("results-" + std::to_string(time(0)) + ".csv");

}// int main()
