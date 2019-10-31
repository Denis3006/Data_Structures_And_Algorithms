#include <iostream>
#include <ctime>
#include <assert.h>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>

using namespace std;

template <class T> 
void QuickSort(vector<T>& a, size_t l, size_t r) 
{
	assert(a.size() > 0);
	size_t i = 0;
	size_t j = 0;
	T Pivot = 0;
	if (l < r) {
		i = l;
		j = r;
		Pivot = a[size_t((l + r) / 2)];
		while (i <= j) {
			while (a[i] < Pivot)
				i++;
			while (a[j] > Pivot)
				j--;
			if (i <= j) {
				swap(a[i], a[j]);
				i++;
				if (j > 0) j--;
			}
		}
		QuickSort(a, l, j);
		QuickSort(a, i, r);
	}
}

template <class T> 
void QuickSort(vector<T>& a) 
{
	assert(a.size() > 0);
	QuickSort(a, 0, a.size() - 1);
	for (size_t i = 0; i < a.size() - 1; i++) assert(a[i] <= a[i + 1]);  // really sorted?
}

template <class T> 
void BubbleSort(vector<T>& a) 
{
	assert(a.size() > 0);
	for (size_t i = a.size() - 1; i > 0; i--)
		for (size_t j = 0; j < i; j++)
			if (a[j] > a[j + 1])
				swap(a[j], a[j + 1]);
	for (size_t i = 0; i < a.size() - 1; i++) assert(a[i] <= a[i + 1]);  // really sorted?
}

vector <double> GenerateNumbers(string Selection, size_t n) 
{
	assert(n > 0);
	vector<double> vec;
	try {
		vec.resize(n, 17);
	}
	catch (...) { throw "Out of memory."; }	
	if (Selection == "constant")
		return vec;
	if (Selection == "up") {
		iota(vec.begin(), vec.end(), 0);
		return vec;
	}
	if (Selection == "down") {
		double j = 0;
		for_each(vec.begin(), vec.end(), [&j](double &x){x = j--;});
		return vec;
	}
	if (Selection == "random") {
		srand(int(time(0)));
		for_each(vec.begin(), vec.end(), [](double &x){x = double(rand()) / (RAND_MAX + 1);});
		return vec;
	}
}

template <class T> void InsertionSort(vector<T>& a) 
{
	assert(a.size() > 0);
	T key;
	int j = 0;
	for (size_t i = 1; i < a.size(); i++) {
		key = a[i];
		j = i - 1;
		while (j >= 0 && a[j] > key) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
	}
	for (size_t i = 0; i < a.size() - 1; i++) assert(a[i] <= a[i + 1]);  // really sorted?
}

template <class T> 
bool IsSortedAndNothingIsLost(const vector <T>& before, const vector <T>& after) 
{
	assert(before.size() > 0 && after.size() > 0);
	vector<bool> in_both_vecs;  // keep track of elements, that are found in both vectors
	try {
		in_both_vecs.resize(after.size(), false);
	}
	catch (...) { throw "Out of memory."; }
	T SumBefore = 0;
	T SumAfter = 0;
	if (before.size() != after.size()) 
		return false;
	for (size_t i = 0; i < after.size(); i++) {
		if(i + 1 < after.size() && after[i] > after[i + 1])   // is sorted
			return false;
		SumBefore += before[i];
		SumAfter += after[i];
		for (size_t j = 0; j < after.size(); j++) {
			if (after[i] == before[j])
				in_both_vecs[j] = true;
		}
	}
	return (SumBefore == SumAfter) && all_of(in_both_vecs.begin(), in_both_vecs.end(), [](bool v){ return v; });
}

// iterative merge with sorting
template <class T>void merge(vector<T>& a, vector<T>& b, size_t left, size_t mid, size_t right) 
{
	if (mid > a.size()) mid = a.size();
	if (right > a.size()) right = a.size();
	size_t i = left, j = mid;
	for (size_t k = left; k < right; k++) {
		if (i == mid) b[k] = a[j++];
		else if (j == right) b[k] = a[i++];
		else if (a[j] < a[i]) b[k] = a[j++];
		else b[k] = a[i++];
	}
}

template <class T>void MergeSort(vector<T>& a)
{
	assert(a.size() > 0);
	vector<T> help_from;
	try {
		help_from.reserve(a.size());
	}
	catch (...) { throw "Out of memory."; }
	help_from = a;
	vector<T> help_to;
	try{
		help_to.reserve(a.size());
	}
	catch (...) { throw "Out of memory."; }
	help_to = a;
	vector<T> temp;
	try{
		temp.reserve(a.size());
	}
	catch (...) { throw "Out of memory."; }
	for (size_t block = 1; block < a.size(); block *= 2) {  // Split vector in blocks with size of 1, 2, 4, 8, 16, ...
			for (size_t i = 0; i < a.size(); i += 2 * block)
				merge(help_from, help_to, i, i + block, i + 2 * block);  // iterative merge with sorting
		// Swap the vectors
		help_from.swap(help_to);
	}
	if (a != help_from)
		a = help_from;
	for (size_t i = 0; i < a.size() - 1; i++) assert(a[i] <= a[i + 1]);  // really sorted?
}

void usage()
{
	throw
	"Usage: \n"
     "Sorting n Direction [ -o ] [ -c ] [ -t ] [ -m | -b | -q | -i ] \n"
     "           n: (int) amount of numbers to sort\n"
	 "   Direction: \n"	
	 "               random:  use random numbers\n"
     "                   up:  use ascending numbers\n"
     "                 down:  use descending numbers\n"
     "             konstant:  use constant number 17\n\n\n"
     "	-o print result\n"
     "	-c check result\n"
     "	-t print duration\n"
     "	-b use BubbleSort (default)\n"
     "	-m use MergeSort\n"
     "	-i use InsertionSort\n"
     "	-q use QuickSort\n";
}

int main(int argc, char* argv[]) {
	enum Sort{BUBBLE_SORT, QUICK_SORT, MERGE_SORT, INSERTION_SORT};
	Sort sortType = BUBBLE_SORT;
	bool MeasTime = false;
	bool Output = false;
	bool IsSorted = false;
	size_t n = 0;
	string Selection;
	vector<double> preSort;
	vector<double> afterSort;
	double MeasuredTime;
	try {
		if (argc <= 2) usage();
		if ((istringstream(argv[1]) >> dec >> n)) {
			if (n <= 0) {
				throw "First Parameter n must be positive integer";
			}
		}
		else throw "Malformed number for SomeInteger.";
		try {
			preSort.reserve(n);
			afterSort.reserve(n);
		}
		catch (...) {
			throw "Out of memory.";
		}
		Selection = argv[2];
		if (Selection != "down" && Selection != "up" && Selection != "random" && Selection
			!= "constant") {
			throw "Second Parameter must be chosen from random|up|down|constant";
		}
		for (int i = 3; i < argc; i++) { // parse options
			if (!string(argv[i]).compare("-o"))
				Output = true;
			else if (!string(argv[i]).compare("-c"))
				IsSorted = true;
			else if (!string(argv[i]).compare("-t"))
				MeasTime = true;
			else if (!string(argv[i]).compare("-m"))
				sortType = MERGE_SORT;
			else if (!string(argv[i]).compare("-i"))
				sortType = INSERTION_SORT;
			else if (!string(argv[i]).compare("-q"))
				sortType = QUICK_SORT;
			else if (!string(argv[i]).compare("-b"))
				sortType = BUBBLE_SORT;
			
			else throw "Wrong switch, use only one of - o | -c | -t | -m | -i | -q";
		}
		preSort = GenerateNumbers(Selection, n);
		afterSort = preSort;
		MeasuredTime = double(clock());
		switch(sortType){
			case BUBBLE_SORT:
				BubbleSort(afterSort);
				break;
			case MERGE_SORT:
				MergeSort(afterSort);
				break;
			case INSERTION_SORT:
				InsertionSort(afterSort);
				break;
			case QUICK_SORT:
				QuickSort(afterSort);
				break;
		}
		MeasuredTime = (double(clock()) - MeasuredTime) / CLOCKS_PER_SEC;

		if (IsSorted) {
			if (IsSortedAndNothingIsLost(preSort, afterSort)) {
				cout << "Success." << endl;
			}
			else {
				throw "Check failed";
			}
		}
		if (Output) {
			for (size_t i = 0; i < n; i++)
				cout << setiosflags(ios::fixed)  << setprecision(10) << setw(20) << preSort[i]
					<< setw(20) << afterSort[i] << endl;
		}
		if (MeasTime)
			cout << '\n' << setw(15) << setprecision(12) << setiosflags(ios::scientific) << "MeasuredTime: " << MeasuredTime << '\n';
	}
	catch (const char* c) {
		cout << c;
		exit(1);
	}
	return 0;
}