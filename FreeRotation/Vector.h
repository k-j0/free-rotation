#pragma once

#include <cstdarg> // variadic function support
#include <assert.h>
#include <string>

/// N-component vector
template<typename T, int N>
class Vec {

	T components[N];// x, y, z, etc components of the vector

public:

	/// Default constructor. Does not initialize any values.
#pragma warning(push)
#pragma warning(disable: 26495) // disable warning about uninitialized components - this is intended here; calling code is expected to set the components up after declaration of the vector.
	inline Vec() {}
#pragma warning(pop)

	/// Constructor with all initial values
	/// N values should be passed.
	/// Variadic params for ease of use - note that the exact type T should be passed ie for T = double, pass double literal (1.0) rather than int literal (1) or float literal (1.0f)
	inline Vec(T x, ...) {
		components[0] = x;// insert first element
		va_list arguments;
		va_start(arguments, x);// assuming N arguments are passed
		for (int comp = 1; comp < N; ++comp) {
			T next = va_arg(arguments, T);
			components[comp] = next; // insert all the arguments into the vector of size N.
		}
		va_end(arguments);
	}// constructor

	/// General getter
	inline T get(const int& i) const { assert(N > i); return components[i]; }
	inline T operator[](const int& i) const { return get(i); }

	/// Getters for standard components x, y, z
	inline T X() const { return get(0); }
	inline T Y() const { return get(1); }
	inline T Z() const { return get(2); }

	/// Setter for inidividual values
	inline void set(const int& position, const T& val) {
		assert(N > position);
		components[position] = val;
	}// void set()

	/// Vector length squared
	inline T lengthSqr() {
		T val = 0;
		for (int i = 0; i < N; ++i) {
			val += get(i) * get(i);
		}
		return val;
	}// T lengthSqr()

	/// Normalized vector
	inline Vec<T, N> normalized() {
		Vec<T, N> v;
		T length = (T)sqrt((double)lengthSqr());
		for (int i = 0; i < N; ++i) {
			v.set(i, get(i) / length);
		}
		return v;
	}// Vec<T, N> normalized

	/// Component-wise addition
	inline Vec<T, N> operator+(const Vec<T, N>& v) const {
		Vec<T, N> ret;
		for (int i = 0; i < N; ++i) {
			ret.set(i, v[i] + get(i));
		}
		return ret;
	}// operator+

	/// Component-wise addition shorthand
	inline Vec<T, N> operator+=(const Vec<T, N>& v) {
		Vec<T, N> n = (*this) + v;
		for (int i = 0; i < N; ++i) {
			set(i, n[i]);
		}
		return *this;
	}// operator+=

	/// Component-wise product
	inline Vec<T, N> operator*(const double& f) const {
		Vec<T, N> ret;
		for (int i = 0; i < N; ++i) {
			ret.set(i, get(i) * f);
		}
		return ret;
	}// operator*

	/// String conversion for debugging purposes
	inline std::string toString() const {
		std::string ret = "( ";
		for (int i = 0; i < N; ++i) {
			ret += std::to_string(components[i]);
			if (i < N - 1) ret += ",\t";
		}
		return ret + " )";
	}// std::string toString()

};// class Vec
