#pragma once

#include "Vector.h"

/// Represents an NxM matrix with elements of type T
template<typename T, int N, int M>
class Mat {

	// N columns, M rows
	Vec<Vec<T, M>, N> elements;

public:

	/// Default constructor
	inline Mat() {}

	/// Constructor with full initializers
	/// variadic notation for ease of declaration (note: should pass the exact type of T - ie. if T = double, pass 1.0 double literal instead of 1 or 1.0f)
	inline Mat(T m_00, ...) {
		set(0, 0, m_00);// insert first element (0,0)
		va_list arguments;
		va_start(arguments, m_00);
		for (int row = 0; row < M; ++row) {
			for (int col = 0; col < N; ++col) {
				if (row == 0 && col == 0) continue;// first element m_00 has already been inserted
				T elem = va_arg(arguments, T);
				set(col, row, elem);
			}
		}
		va_end(arguments);
	}// full constructor

	/// Getters
	inline T get(int col, int row) { assert(col < N && row < M); return elements[col][row]; }
	inline Vec<T, M> operator[](int col) { assert(col < N); return elements[col]; }// direct access to each column as a vector

	/// Setters
	inline void set(int col, int row, T elem) {
		Vec<T, M> column = elements[col];
		column.set(row, elem);
		elements.set(col, column);
	}// void set()

	/// String conversion, for debug output
	inline std::string toString() {
		std::string s = "";
		for (int row = 0; row < M; ++row) {
			s += "[ ";
			for (int col = 0; col < N; ++col) {
				s += std::to_string(get(col, row));
				if (col < N - 1) s += ",\t";
			}
			s += "\t]\n";
		}
		return s;
	}// std::string toString()

};// class Mat



/// Matrix multiplication - Matrix x Matrix
template<typename T, typename MatLeft, int MLeft, int NLeft, typename MatRight, int MRight, int NRight>
inline Mat<T, MRight, NLeft> Mult(MatLeft left, MatRight right) {
	assert(MLeft == NRight);
	Mat<T, MRight, NLeft> result;
	for (int col = 0; col < MRight; ++col) {
		for (int row = 0; row < NLeft; ++row) {
			T val = 0;
			// add up the dot products of the first matrix's rows and the second's columns.
			for (int index = 0; index < MLeft; ++index) {
				val += left.get(index, row) * right.get(col, index);
			}
			result.set(col, row, val);
		}
	}
	return result;
}// Matrix multiplication


/// Matrix multiplication - Matrix x Vector
template<typename T, typename MatLeft, int MLeft, int NLeft, typename VecRight, int NRight>
inline Vec<T, NLeft> Mult(MatLeft left, VecRight right) {
	assert(MLeft == NRight);
	Vec<T, NLeft> result;
	for (int row = 0; row < NLeft; ++row) {
		T val = 0;
		// add up the dot products of the matrix's rows and the vector.
		for (int index = 0; index < MLeft; ++index) {
			val += left.get(index, row) * right.get(index);
		}
		result.set(row, val);
	}
	return result;
}// Matrix multiplication (Matrix x Vector)
