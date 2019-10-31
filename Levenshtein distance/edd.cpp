#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <vector> 
#include <algorithm>
#include <iomanip>

using namespace std;

class Transformation 
{
	/* You must not edit above this line */
private:
	enum stepTypes { DELETE, INSERT, EXCHANGE, EQUAL};
	vector<string> stepTypes {"delete", "insert", "exchange", "equal"};
	size_t distance;
	string From, To;
	vector<vector<size_t>> D;  // matrix for dynamic programming

	// Get needed transformation steps from the D matrix from first to last steps
	string Backtrace(size_t i, size_t j) 
	{
		if (i > 0 && D[i - 1][j] + 1 == D[i][j])
			return Backtrace(i - 1, j) + stepTypes[DELETE] + ' ';
		if (j > 0 && D[i][j - 1] + 1 == D[i][j])
			return Backtrace(i, j - 1) + stepTypes[INSERT] + ' ';
		if (i > 0 && j > 0 && D[i - 1][j - 1] + 1 == D[i][j])
			return Backtrace(i - 1, j - 1) + stepTypes[EXCHANGE] + ' ';
		if (i > 0 && j > 0 && D[i - 1][j - 1] == D[i][j])
			return Backtrace(i - 1, j - 1) + stepTypes[EQUAL] + ' ';
		return "";
	}

public: 
	Transformation(const string &From, const string &To) // fill the dynamic programming matrix
	{
		size_t n = From.size();
		size_t m = To.size();
		try {
			this->From = From;
			this->To = To;
			D.resize(n + 1);
			for (size_t i = 0; i <= n; i++)
				D[i].resize(m + 1);
		}
		catch (...) { throw "Out of memory"; }
		for (size_t j = 0; j <= m; j++)
			D[0][j] = j;
		for (size_t i = 0; i <= n; i++)
			D[i][0] = i;
		for (size_t i = 1; i <= n; i++) {
			for (size_t j = 1; j <= m; j++) {
				size_t deleteDist = D[i - 1][j] + 1;
				size_t insertDist = D[i][j - 1] + 1;
				size_t swapDist = From[i - 1] != To[j - 1] ? D[i - 1][j - 1] + 1 : D[i - 1][j - 1];  // swap has distance of 1 if chars aren't equal, otherwise 0.
				D[i][j] = min({ deleteDist, insertDist, swapDist });
			}
		}
		distance = D[n][m];
	}

	size_t Cost() { return distance; }

	friend ostream& operator<< (ostream &TheOstream, Transformation &me) 
	{
		string steps;
		try {
			steps = me.Backtrace(me.From.size(), me.To.size());   // Get needed transformation steps from the D matrix
		}
		catch (...) {
			throw "Out of memory.";
		}

		const size_t width = max(me.From.size(), me.To.size()) + 2;
		string preChange = me.From;
		string afterChange = preChange;
		size_t idxFrom = 0;
		size_t idxTo = 0;		
		size_t stepCntr = 0;
		while(!steps.empty()) {
			string step = steps.substr(0, steps.find(" "));
			try {
				steps.erase(0, steps.find(" ") + 1);
			}
			catch (...) { throw "Problem with erase."; }

			size_t position = idxFrom;
			preChange = afterChange;
			char oldChar = preChange[idxFrom];
			
			if (step == me.stepTypes[EQUAL]) {
				idxTo++;
				idxFrom++;
				continue;
			}
			if (step == me.stepTypes[EXCHANGE])	{
				afterChange[idxFrom] = me.To[idxTo];
				idxTo++;
				idxFrom++;
			}
			else if (step == me.stepTypes[INSERT]) {
				try {
					afterChange.insert(idxFrom, { me.To[idxTo] });
				}
				catch (...) { throw "Problem with insert."; }
				oldChar = afterChange[idxFrom];
				idxTo++;
				idxFrom++;
			}
			else if (step == me.stepTypes[DELETE]) {
				try {
					afterChange.erase(idxFrom, 1);
				}
				catch (...) { throw "Problem with erase."; }
				idxTo--;
			}

			TheOstream << right << setw(5) << " Step " 
				<< setw(3) << stepCntr + 1 
				<< setw(6) << " is to " 
				<< left << setw(8) << step << " "
				<< right << setw(1) << oldChar
				<< setw(12) << " at position"
				<< setw(4) << position << " " 
				<< setw(6) << (step == me.stepTypes[EXCHANGE]? "with " + string(1, me.To[idxTo]) : "") << ":"
				<< setw(width) << preChange << " -> " << left << setw(width) << afterChange << endl;
			stepCntr++;
		}
		return TheOstream;
	}
		 /* You must not edit below this line */
};


void usage() { throw "Usage: edd String1 String2 [ -o ]"; }

int main(int argc, char* argv[]) {

	bool LongOutput = false;  // If true the complete edit steps are printed line by line

	try {
		if (argc<3 || argc>4) usage();

		if (argc == 4) {
			string arg = string(argv[3]);
			if (!arg.compare("-o")) LongOutput = true; else usage();
		}

		Transformation Try = Transformation(string(argv[1]), string(argv[2]));  //Calculate edit steps
		cout << "Transformation from " << argv[1] << " to " << argv[2] << " takes " <<
			Try.Cost() << " Operations" << endl;
		if (LongOutput) cout << endl << Try << endl;  // optional long output

	}
	catch (const char *Reason) {
		cerr << Reason << endl;
		exit(1);
	}
	return 0;
}