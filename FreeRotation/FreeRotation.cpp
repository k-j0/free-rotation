
// Includes
#include "Vector.h"
#include "NumericalMethods.h"
#include "CSVWriter.h"
#include <ctime>



/// Gravity of Earth
const double g = 9.8; // newtons per kg

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

/// Definition of the initial position, linear & angular velocity
const Vec3 Position ( 0.0, 0.0, 0.0 ); // meters
const Vec3 Velocity ( 0.0, 0.0, 200.0 ); // meters per second
const Vec3 AngularVelocity ( 3.0, 1.0, 2.0 ); // radians per second

/// Definition of the step size and time span
const double t_0 = 0.0;
const double t_max = 20.0;
const double h = 0.05; // step size



/// Inserts the data into the csv file
void insertData(CSVWriter& csv, const double& time, const Vec3& velocity, const Vec3& position, const Vec3& angularVelocity) {
	csv.nextColumn();
	csv.insert(time); // time
	csv.insert(velocity.Z()); // velocity (only writing vertical component)
	csv.insert(position.Z()); // position (only writing vertical component)
	csv.insert(angularVelocity.X()); // angular velocity
	csv.insert(angularVelocity.Y());
	csv.insert(angularVelocity.Z());
}// void insertData


/// Entrypoint
int main() {

	// output results to a CSV file
	CSVWriter csv;
	csv.insert("t");
	csv.insert("v z");
	csv.insert("pos z");
	csv.insert("omega x");
	csv.insert("omega y");
	csv.insert("omega z");

	// initial position of centre of mass, velocity and angular velocity
	Vec3 pos = Position;
	Vec3 v = Velocity;
	Vec3 ω = AngularVelocity;

	// write initial conditions for t = 0 to csv
	insertData(csv, 0, v, pos, ω);

	// velocity and acceleration equations
	std::function<Vec3(double, Vec3)> velocity = [](double t, Vec3 v) { return v; }; // velocity as a function of position and time (constant here)
	std::function<Vec3(double, Vec3)> acceleration = [](double t, Vec3 x_n) { return Vec3(0.0, 0.0, -g); }; // acceleration as a function of velocity and time

	// inertia tensor for the shape
	const DiagMat3 I = InertiaTensor;

	// parameters γ for Euler's equations
	const Vec3 γ ( (I[2] - I[1]) / I[0],  // γ1 = (I_3 - I_2) / I_1
				   (I[0] - I[2]) / I[1],  // γ2 = (I_1 - I_3) / I_2
				   (I[1] - I[0]) / I[2] );// γ3 = (I_2 - I_1) / I_3

	// Euler's equations
	std::function<double(double, double, double)> Euler = [](double ω_1, double ω_2, double γ) { return -γ * ω_1 * ω_2; };
	std::function<double(double, double)> EulerX = [&γ, &Euler](double ω_y, double ω_z) { return Euler(γ[0], ω_y, ω_z); };
	std::function<double(double, double)> EulerY = [&γ, &Euler](double ω_x, double ω_z) { return Euler(γ[1], ω_x, ω_z); };
	std::function<double(double, double)> EulerZ = [&γ, &Euler](double ω_x, double ω_y) { return Euler(γ[2], ω_x, ω_y); };

	// iterative loop (over an int rather than double for maximum precision)
	for (int n = 1; n <= (t_max - t_0) / h; ++n) {
		double t = double(n) * h + t_0; // current time

		// Semi-Implicit Euler method for position and velocity
		std::tuple<Vec3, Vec3> r = SemiImplicitEuler(t, h, v, pos, acceleration, velocity);
		v = std::get<0>(r);
		pos = std::get<1>(r);

		// 4th-order Runge-Kutta method on Euler's equations
		Vec3 nextΩ (
			RungeKutta4<double>(h, ω.Y(), ω.Z(), ω.X(), EulerX),
			RungeKutta4<double>(h, ω.X(), ω.Z(), ω.Y(), EulerY),
			RungeKutta4<double>(h, ω.X(), ω.Y(), ω.Z(), EulerZ)
		);
		ω = nextΩ;

		// Write results to the csv file
		insertData(csv, t, v, pos, ω);

	}// iterative loop

	// Save results
	csv.writeToFile("results-" + std::to_string(time(0)) + ".csv");

}// int main()
