#pragma once

#include "Matrix.h"
#include "Vector.h"


/// 3D rotation matrix of an angle θ about an axis. θ in radians.
inline Mat<double, 3, 3> RotationMatrix3(Vec<double, 3> axis, double θ) {
	double lengthSqr = axis.lengthSqr();
	if (lengthSqr != 1.0) axis = axis.normalized();
	double α = axis.X();
	double β = axis.Y();
	double γ = axis.Z();
	double cosθ = cos(θ);
	double sinθ = sin(θ);
	double αβ = α*β;
	double αγ = α*γ;
	double βγ = β*γ;
	return Mat<double, 3, 3>(
		α * α * (1 - cosθ) + cosθ,		αβ * (1 - cosθ) - γ * sinθ,		αγ * (1 - cosθ) + β * sinθ,
		αβ * (1 - cosθ) + γ * sinθ,		β * β * (1 - cosθ) + cosθ,		βγ * (1 - cosθ) - α * sinθ,
		αγ * (1 - cosθ) - β * sinθ,		βγ * (1 - cosθ) + α * sinθ,		γ * γ * (1 - cosθ) + cosθ
	);
}// RotationMatrix3
