#include <iostream>
#include <sstream>
#include <vector> 
#include <algorithm>
#include <iterator>
#include <ctime>
#include <tuple>

using namespace std;

// You must not edit above this line
template<typename It> 
pair<It, It> NaiveMaxSubsum(It TheBegin, It TheEnde)
{
	typedef typename remove_reference<decltype(*TheBegin)>::type SumType;
	SumType max(0);
	SumType sum(0);
	It MyBegin = TheBegin, MyEnde = TheBegin;
	for (auto i = TheBegin; i != TheEnde; i++) {
		for (auto j = i; j != TheEnde; j++) {
			sum += *j;
			if (sum > max) {
				max = sum;
				MyBegin = i;
				MyEnde = j+1;
			}
		}
		sum = 0;
	}
	return make_pair(MyBegin, MyEnde);
}

template<typename It> 
pair<It, It> DynamicMaxSubsum(It TheBegin, It TheEnde)
{
	typedef typename remove_reference<decltype(*TheBegin)>::type SumType;
	SumType maxcurr(*TheBegin);
	SumType maxglob(0);
	It Begcurr = TheBegin, Beg = TheBegin, End = TheBegin;
	for (auto i = TheBegin + 1; i != TheEnde; i++) {
		maxcurr = max(*i, maxcurr + *i);
		if (maxcurr == *i && maxcurr > 0)		        // the sub-array with max-sum begins
			Begcurr = i;  
		if (maxcurr > maxglob) {					    // the sub-array with max-sum continues
			maxglob = maxcurr;
			Beg = Begcurr;
			End = i + 1;
		}
		else if (maxglob > maxcurr && maxglob < 0) {    // the sub-array with max-sum ends
			maxcurr = 0;
			End = i;
		}
	}
	return make_pair(Beg, End);
}

void usage() 
{
	throw
	" MaxSum [-d] [-o] [-i] ( -r n ) | ( v v v v v  ... )\n\n"
	" Solves Problem of Maximum Sub-Sums.\n\n"
	"  -d                    : Select dynamic programming, otherwise naive\n"
	"  -o                    : Print resulting values\n"
	"  -i                    : Print initial list values too.\n"
	"  -t                    : Print time\n\n"
	"  -r                    : use random values, you must specify n\n"
	"   n            (int>0) : Number of values\n\n"
	"   v v v v ... (double) : values to sum up\n";
}

int main(int argc, char *argv[]) 
{
	pair<vector<int>::iterator, vector<int>::iterator>
		(*TestOnly1)(vector<int>::iterator, vector<int>::iterator) = NaiveMaxSubsum;  // Check if function template is implemented

	pair<vector<double>::iterator, vector<double>::iterator>
		(*MaxSubsum)(vector<double>::iterator, vector<double>::iterator) = NaiveMaxSubsum;

	bool Output = false;
	bool Random = false;
	bool OutputInitial = false;
	bool Timing = false;
	vector<double> Values;
	vector<double>::iterator Start, End;

	try{
		if (argc<2) usage();
		int i;
		for (i = 1; i<argc;) {
			string arg = string(argv[i]);
			if (!arg.compare("-o")) { Output = true; i++; }
			else if (!arg.compare("-d")) { MaxSubsum = DynamicMaxSubsum; i++; }
			else if (!arg.compare("-i")) { OutputInitial = true; i++; }
			else if (!arg.compare("-t")) { Timing = true; i++; }
			else break;
		}

		if (i == argc) usage();
		string arg = string(argv[i]);
		if (!arg.compare("-r")) {
			size_t N;
			Random = true; i++;
			if (i + 1 != argc) usage();

			if (!(istringstream(argv[i++]) >> dec >> N) || 0 >= N) throw "Malformed number for n, must be positive integer.";

			try {
				Values.resize(N);
			}
			catch (...) { throw("No Memory."); };
			for (auto &value : Values)
				value = (double(rand()) / RAND_MAX - 0.5);
		}
		else {
			double value;
			int ctr;

			for (ctr = 1; i<argc; i++, ctr++) {
				if (!(istringstream(argv[i]) >> dec >> value)) throw "Malformed Number.";
				try { Values.push_back(value); }
				catch (...) { throw "No Memory."; };
			}
		}

		if (OutputInitial) {
			cout << "Initial items:" << endl;
			for (auto i = begin(Values); i != end(Values); i++)
				cout << *i << endl;
		}

		double MeasuredTime = double(clock());
		tie(Start, End) = MaxSubsum(begin(Values), end(Values));
		MeasuredTime = (double(clock()) - MeasuredTime) / CLOCKS_PER_SEC;

		if (Output) {
			cout << "Best subsum:" << endl;
			for (auto i = Start; i != End; i++)
				cout << *i << "  ";
			cout << endl;
		}

		typedef std::remove_reference<decltype(*Start)>::type MyValuesType;
		MyValuesType Sum(0);

		for (auto i = Start; i != End; i++)
			Sum += *i;
		cout << "Total Sum: " << Sum;
		if (Sum>0)
			cout << " are " << End - Start << " Elements from " << Start - begin(Values) << " to " << End - begin(Values) - 1 << endl;
		else
			cout << " no Elements are used" << endl;

		if (Timing) cout << "took " << MeasuredTime << " seconds" << endl;

	}
	catch (const char *Reason) { cerr << Reason << endl; exit(1); }

	return 0;
}

