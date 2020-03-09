#pragma once

#include <string>
#include <vector>
#include <fstream>

/// Used to output data to a CSV file.
class CSVWriter {

	/// Represents a single column's worth of data.
	struct Column {
		std::vector<std::string> cells;// individual cells in the column
	};// struct Column

	// the actual data; column by column and row by row
	std::vector<Column*> columns;

	// how tall the longest column is
	int maxColumnHeight = 0;

public:

	/// Moves on to the next column of data
	inline void nextColumn() { columns.push_back(new Column()); }

	/// Inserts a string value in the current column
	inline void insert(std::string val) {
		if (columns.size() <= 0) nextColumn();// first column
		// insert the value
		columns.back()->cells.push_back(val);
		// keep track of the largest column:
		maxColumnHeight = maxColumnHeight < columns.back()->cells.size() ? columns.back()->cells.size() : maxColumnHeight;
	}// void insert()

	/// Inserts a double value in the current column
	inline void insert(double val) { insert(std::to_string(val)); }

	/// Writes the resulting CSV file to a file with the given filename
	inline void writeToFile(std::string filename) {

		std::ofstream stream(filename);
		for (int row = 0; row < maxColumnHeight; ++row) {
			for (int col = 0; col < columns.size(); ++col) {
				if (row < columns[col]->cells.size()) {
					stream << columns[col]->cells[row];
				}
				stream << ",";
			}
			stream << "\n";
		}
		stream.close();

	}// void writeToFile

	/// Default destructor
	inline virtual ~CSVWriter() {
		for (int i = 0; i < columns.size(); ++i) delete columns[i];
		columns.clear();
	}// destructor

};// class CSVWriter
