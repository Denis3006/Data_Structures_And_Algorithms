#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <ctime>
#include <algorithm>
#include <complex>

using namespace std;

int ** dynamicLCS(const string A, const string B)
{
	size_t m = A.size();
	size_t n = B.size();
	
	int **solution = nullptr;
	try {
		solution = new int* [m + 1];
	}
	catch (...) {
		delete[] solution;
		throw "No memory.";
	}
	try {
		for (size_t i = 0; i <= m; i++)
			solution[i] = new int[n + 1];
	}
	catch (...){
		for (size_t i = 0; i <= m; i++) {
			if (solution[i] != nullptr)
				delete[] solution[i];
			else break;
		}
		delete[] solution;
		throw "No memory.";
	}
	for (size_t i = 0; i <= m; i++)
		solution[i][0] = 0;
	for (size_t j = 0; j <= n; j++)
		solution[0][j] = 0;
	for (size_t i = 1; i <= m; i++) {
		for (size_t j = 1; j <= n; j++) {
			if (A[i-1] == B[j-1]) 
				solution[i][j] = solution[i - 1][j - 1] + 1;
			else
				solution[i][j] = max(solution[i - 1][j], solution[i][j - 1]);
		}
	}
	return solution;
}

string LCS(const string A, const string B)
{
	string Result;
	int **solution = dynamicLCS(A, B);
	try{
		Result.resize(solution[A.size()][B.size()]);
	}
	catch (...){ throw "No memory."; }

	int aCntr = A.size() - 1;
	int bCntr = B.size() - 1;
	int resCntr = Result.size() - 1;
	while (resCntr>=0) {
		if (A[aCntr] == B[bCntr]) {
			Result[resCntr] = A[aCntr];
			resCntr--;
			aCntr--;
			bCntr--;
		}
		else if (solution[aCntr][bCntr + 1] >= solution[aCntr + 1][bCntr])
			aCntr--;
		else 
			bCntr--;
	}
	for (size_t i = 0; i <= A.size(); i++)
		delete[] solution[i];
	delete[] solution;
	return Result;
}

string RandomString(const char Seed, const int n, const int Scatter) {
	const string Alphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"); // letters to choose from
	size_t OldCharNumber = Alphabet.find_first_of(Seed, 0);	// first char present in alphabeth?
	assert(string::npos != OldCharNumber);					// check that!
	assert(Scatter >= 0); assert(n>0);
	string Result;
	try { Result.reserve(n); }
	catch (...) { throw "No Memory"; }
	Result = Result + Seed;										// Result starts with seed
	for (int j = 1; j<n; j++) {
		int Deviation = 0;									// simple algorithm to provide
		for (int i = 0; i<Scatter; i++) Deviation += (rand() % 3) - 1;	// random numbers with more or less variation
		while (Deviation<0) Deviation += Alphabet.length();		// Scatter = 1 only the next, same or previos char 
		size_t NewCharNumber = (OldCharNumber + size_t(Deviation)) % Alphabet.length();	// in the alphabet is taken, Scatter > 1  
		Result = Result + Alphabet[NewCharNumber];								// = faster variation
		OldCharNumber = NewCharNumber;
	}
	return Result;
}

void usage()
{
	throw
	"Usage:\n lcs [ -s Scatter ] [ -r Seed ] [ -l m ] [ -o ] [ -t ]  n  \n\n"
	"Options:\n"
	"       -s Scatter : Integer scatter>= 0, gives speed of variation in random strings\n"
	"       -r Seed    : Integer seed>0 for random generator, default: 1, overrides random\n"
	"       -l m       : Optional length of second string, m>0\n"
	"       -o         : Activates output of random strings\n"
	"       -t         : Measure time for LCS\n"
	"        n         : n>0, Length of random strings\n\n"
	"Options may occur in any order.";
}

int main(int argc, char* argv[]) {
	bool Output = false; bool PrintTime = false;	// Tells if user wants output or time measurement
	int Scatter = 1;								// Change chars in string slowly.
	time_t RandomSeed = time(0);					// Default: Random sequence
	int n = -1; int n2 = -1;						// Length of strings. -1 says: Value not provided by user

	try {
		if (argc<2) usage(); // neeeds at least length
			
		for (int i = 1; i<argc; i++)  { // parse options
			if (!string(argv[i]).compare("-o"))  { Output = true; continue; }
			if (!string(argv[i]).compare("-t"))  { PrintTime = true; continue; }
			if (!string(argv[i]).compare("-s"))  { // an integer must follow options -s -r and -l
				if (++i == argc) throw  "-s Must be followed by Scatter";
				try { std::istringstream(argv[i]) >> Scatter; }
				catch (...) { throw "Malformed Number for Scatter"; }
				if (Scatter<0) throw "Scatter must be zero or >0";
				continue;
			}
			if (!string(argv[i]).compare("-r"))  { // if you provide -r , you always get the same sequences
				if (++i == argc) throw  "-r Must be followed by Seed"; // if you use the same arg
				try { std::istringstream(argv[i]) >> RandomSeed; }
				catch (...) { throw "Malformed Number for Seed"; }
				if (RandomSeed<1) throw "Seed must be >0";
				continue;
			}
			if (!string(argv[i]).compare("-l"))  { // with -l user may change length of string b
				if (++i == argc) throw  "-l Must be followed by Length of Second String";
				try { std::istringstream(argv[i]) >> n2; }
				catch (...) { throw "Malformed Number for Seed"; }
				if (n2<1) throw "Length of second String must be >0";
				continue;
			}
			try { std::istringstream(argv[i]) >> n; if (n<1) throw "n must >0"; }	// if you trickle down here,
			catch (char *what) { throw what; }										// this arg must be 
			catch (...) { throw "Malformed Number for n"; }							// the length infomation

		}
		if (n<0) throw "You must provide a value for n";
		if (n2<0) n2 = n; // if user did not provide a length for second string, we assume both to be equal in size
		srand(int(RandomSeed));

		string First = RandomString('a', n, Scatter);
		string Second = RandomString('b', n2, Scatter);
		if (Output) cout << "First  : " << First << endl << "Second : " << Second << endl;

		double MeasuredTime = double(clock());
		string Sequence = LCS(First, Second);
		MeasuredTime = (double(clock()) - MeasuredTime) / CLOCKS_PER_SEC;

		cout << "LCS    : " << Sequence << endl;
		if (PrintTime) cout << "It took " << MeasuredTime << " Seconds to compute." << endl;

	}
	catch (const char *Reason) { cerr << Reason << endl; exit(1); }

	return 0;
}