#include <assert.h>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

template <class T> 
void BubbleSort(vector<T>& a) noexcept
{
	assert(a.size() > 0);
	for (size_t i = a.size() - 1; i > 0; i--)
		for (size_t j = 0; j < i; j++)
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);
	for (size_t i = 0; i < a.size() - 1; i++) assert(a[i] <= a[i + 1]); // really sorted?
}

vector<double> GenerateNumbers(size_t size) 
{
	assert(size > 0);
	vector<double> Vec;
	try {
		Vec.resize(size);
	}
	catch (...) { throw "Out of memory."; }
	srand(int(time(0)));
	for (double &x : Vec) {
		x = double(rand()) / RAND_MAX;
		assert(x >= 0 && x <= 1);
	}
	return Vec;
}

double timeToSort(size_t n)
{
	vector<double> vec;
	try {
		vec = GenerateNumbers(n);
	}
	catch (...) { throw "Out of memory."; }
	clock_t begin = clock();
	BubbleSort(vec);
	return double((clock() - begin)) / CLOCKS_PER_SEC;
}

void usage()
{
	throw 
	"Usage: \n"
	"duration n \n"
	"  n : needed duration (double)  \n";
}

int main(int argc, char* argv[]) {
	const double PRECISION = 0.01;
	double durationRequested = 0;
	double duration = 0;
	size_t n = 1;
	try {
		if (argc == 1) usage();
		if (!(istringstream(argv[1]) >> dec >> durationRequested)) throw "Malformed number for Duration.";
		if (durationRequested <= 0) throw "Duration should be positive.";
		if (argc > 2) throw "Too many arguments!";

		cout << "Initial search...\n\n"
			<< setw(18) << " Length | Duration  \n"
			<< setw(18) << "--------+---------- \n";
		for (; duration < durationRequested; n *= 2) {
			duration = timeToSort(n);
			cout << right << setiosflags(ios::fixed) << setw(7) << n << " | " 
				<< left << setw(7) << setprecision(4) << duration << '\n';
		}
		n /= 2;

		cout << "\nStarting iteration...\n\n"
			<< "   Left |       n |   Right | MeasuredTime | DeltaTime \n"
			<< "--------+---------+---------+--------------+---------- \n";
		size_t left = n / 2, right = n;
		while (left <= right - 1 && abs(duration - durationRequested) > PRECISION) {
			n = (left + right) / 2;
			duration = timeToSort(n);

			cout << setiosflags(ios::right) << setw(7) << left << " | " 
											   << setw(7) << n << " | " 
										   << setw(7) << right << " | "
									<< setw(9) <<  duration << "    | " 
								<< setw(8) << setiosflags(ios::left) << duration - durationRequested << " \n";
			
			if (duration > durationRequested)
				right = n;
			else if (duration < durationRequested)
				left = n;
		}
		cout << "\nChoosing n=" << n << " @ " << duration << " Seconds. \n";
	}
	catch (const char* Reason) {
		cerr << Reason;
		exit(1);
	}
	return 0;
}