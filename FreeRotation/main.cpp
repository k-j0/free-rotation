
// Includes
#include "Vector.h"
#include "Matrix.h"
#include "Transformations.h"
#include "NumericalMethods.h"
#include "CSVWriter.h"
#include <ctime>



/// Vector & matrix type definitions
typedef Vec<double, 3> Vec3; // 3-component Vector
typedef Vec<double, 3> DiagMat3; // Diagonal 3x3 Matrix; synonym for a Vec3.
typedef Mat<double, 3, 3> Mat3; // 3x3 square matrix



/// Gravity of Earth
const double g = 9.8; // newtons per kg

/// Definition of the shape needed (cone)
const double Mass = 10.0; // kilograms
const double Radius = 1.0; // meters
const double Height = 4.0; // meters
// Inertia tensor is a 3x3 diagonal matrix, represented here as a vector
const DiagMat3 InertiaTensor = DiagMat3(
	(Radius * Radius + Height * Height / 4.0),
	(Radius * Radius + Height * Height / 4.0),
	2.0 * Radius * Radius
) * (3.0 * Mass / 20.0);

/// Definition of the initial position, linear & angular velocity
const Vec3 Position ( 0.0, 0.0, 0.0 ); // meters
const Vec3 Velocity ( 0.0, 0.0, 200.0 ); // meters per second
const Vec3 AngularVelocity ( 3.0, 1.0, 2.0 ); // radians per second

/// Definition of the step size and time span
const double t_0 = 0.0;
const double t_max = 20.0;
const double h = 0.05; // step size



/// Inserts the data into the csv file
void insertData(CSVWriter& csv, const double& time, const Vec3& velocity, const Vec3& positionCM, const Vec3& angularVelocity, const Vec3& positionP) {
	csv.nextColumn();
	csv.insert(time); // time
	csv.insert(velocity.Z()); // velocity of centre of mass (only writing vertical component)
	csv.insert(positionCM.Z()); // position of centre of mass (only writing vertical component)
	csv.insert(angularVelocity.X()); // angular velocity
	csv.insert(angularVelocity.Y());
	csv.insert(angularVelocity.Z());
	csv.insert(positionP.X());
	csv.insert(positionP.Y());
	csv.insert(positionP.Z());
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
	csv.insert("P x");
	csv.insert("P y");
	csv.insert("P z");

	// initial conditions
	Vec3 pos = Position;
	Vec3 v = Velocity;
	Vec3 ω = AngularVelocity;
	Vec3 a(0.0, 0.0, -g); // constant acceleration
	Vec3 r_local(0.0, 3.0/4.0 * Radius, 0.0);// position of point P within the cone

	// write initial conditions for t = 0 to csv
	insertData(csv, 0, v, pos, ω, pos + r_local);

	// inertia tensor for the shape
	const DiagMat3 I = InertiaTensor;

	// parameters γ for Euler's equations
	const Vec3 γ ( (I[2] - I[1]) / I[0],   // γ1 = (I_3 - I_2) / I_1
				   (I[0] - I[2]) / I[1],   // γ2 = (I_1 - I_3) / I_2
				   (I[1] - I[0]) / I[2] ); // γ3 = (I_2 - I_1) / I_3

	// iterative loop (over an int rather than double for maximum precision)
	for (int n = 1; n <= (t_max - t_0) / h; ++n) {
		double t = double(n) * h + t_0; // current time

		// Semi-Implicit Euler method for position and velocity
		std::tuple<Vec3, Vec3> eulerResult = SemiImplicitEuler(t, h, a, v, pos);
		v = std::get<0>(eulerResult);
		pos = std::get<1>(eulerResult);

		// 4th-order Runge-Kutta method on Euler's equations
		ω = RungeKutta4Euler(h, γ, ω);

		// Find next position of point P in world-space
		double Δθ = sqrt(ω.lengthSqr()) * h;
		Vec3 axis = ω.normalized();
		Mat3 Λ = RotationMatrix3(axis, Δθ); // rotation matrix from previous rotation to current rotation
		r_local = Mult<double, Mat3, 3, 3, Vec3, 3>(Λ, r_local); // get position of P relative to the cone's CM
		Vec3 r_global = pos + r_local; // add to the center of mass to get position of P in world space.

		// Write results to the csv file
		insertData(csv, t, v, pos, ω, r_global);

	}// iterative loop

	// Save results
	csv.writeToFile("results-" + std::to_string(time(0)) + ".csv");

}// int main()
