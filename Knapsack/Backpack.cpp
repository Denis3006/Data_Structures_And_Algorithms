#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include <vector> 
#include <algorithm>

using namespace std;

class Item
{
public:
	int value, weight;
	Item(int value, int weight) : value(value), weight(weight) {}
	bool operator<(Item const Second) const { return (this->value / double(this->weight)) < (Second.value / double(Second.weight)); }  // only needed for greedy
};

class Items : public vector<Item> {};

class Backpack : public Items 
{
public:
	Backpack(Items items, int maxWeight, bool dynamic) 
	{
		if (!dynamic) {  // Greedy Algorithm
			sort(items.begin(), items.end());
			reverse(items.begin(), items.end());

			for (auto item : items) {
				if (maxWeight >= item.weight) {
					maxWeight -= item.weight;
					try { push_back(item); }
					catch (...) { throw("No Memory."); };
					if (maxWeight <= 0) break;
				}
			}
		}
		else { // Algorithm based on dynamic programming
			/* Insert your Code below! You must not edit above this line */
			vector<vector<int>> solution;
			try {
				solution.resize(items.size() + 1);
			}
			catch (...) { throw "Out of Memory!"; }
			for (size_t i = 0; i < solution.size(); i++) {
				try {
					solution[i].resize(maxWeight+1);
				}
				catch (...) { throw "Out of Memory!"; }
			}

			// fill in the solution matrix
			for (size_t i = 1; i <= items.size(); i++) {
				for (int j = 0; j <= maxWeight; j++) {
					int remainingWeight = j - items[i - 1].weight;
					if (remainingWeight >= 0) {  // item fits into backpack with maxWeight = j
						solution[i][j] = max(solution[i - 1][j],  // don't add the item
										    items[i-1].value + solution[i - 1][remainingWeight]);  // add the item
					} 
					else  // item does not fit into backpack
						solution[i][j] = solution[i - 1][j];
				}
			}
			// backtrace the items, that are included in the optimal solution
			size_t remainingWeight = maxWeight;
			for (int i = items.size() - 1; i >= 0; i--) {
				 if (items[i].weight <= remainingWeight && 
				     solution[i + 1][remainingWeight] == items[i].value + solution[i][remainingWeight - items[i].weight]) {
					try { push_back(items[i]); }
					catch (...) { throw("No Memory."); };
					remainingWeight -= items[i].weight;
				}
			}

			/* You must not edit below this line */
		}
	}
	int weighs() { int Result = 0; for (size_t i = 0; i<this->size(); i++) Result += (*this)[i].weight; return Result; }
	int hasValue() { int Result = 0; for (size_t i = 0; i<this->size(); i++) Result += (*this)[i].value; return Result; }
};

void usage() {
	throw
	"Usage: Backpack MaxWeight [-d] [-o] [-i] ( -r n p ) | ( v w v w v w ... )\n\n"
	"Solves integer Backpack problem.\n\n"
	"    Maxweight (int>0): MaximumWeight of Backpack\n"
	"	-d                : Select dynamic programming, otherwise greedy\n"
	"	-o                : Print contents of Backpack\n"
	"	-i                : Print initial list of items too.\n\n"
	"	-r                : Random filling of Backpack, you must specify n and p\n"
	"	n          (int>0): Number of items to Pack\n"
	"	p       (double>1): Blows up values in random number generation. Please see source code.\n\n"
	"	v w ... (int>0) (int>=0): Pairs for items v=value, w=weight\n";
}

int main(int argc, char *argv[]) {
	Items items;
	int maxWeight;
	bool dynamic = false;
	bool Output = false;
	bool OutputInitial = false;
	size_t N = 0;
	double BlowUpFactor = 0;

	try{
		if (argc<2) usage();
		if (!(istringstream(argv[1]) >> dec >> maxWeight) || maxWeight<1) throw "Malformed number for maxWeight, must be integer and > 0.";

		size_t i;
		for (i = 2; i<argc;) {
			string arg = string(argv[i]);
			if (!arg.compare("-o")) { Output = true; i++; }
			else if (!arg.compare("-d")) { dynamic = true; i++; }   // Select dynamic programming
			else if (!arg.compare("-i")) { OutputInitial = true; i++; }
			else break;
		}

		if (i == argc) usage();

		string arg = string(argv[i]);
		if (!arg.compare("-r")) { 
			i++;
			if (i + 2 != argc) usage();

			if (!(istringstream(argv[i++]) >> dec >> N) || 0 >= N) throw "Malformed number for n, Must be positive integer.";
			if (!(istringstream(argv[i++]) >> dec >> BlowUpFactor) || BlowUpFactor <= 1.0) throw "Malformed number for p, must be double>1.";

			try{
				while (items.size()<N) {
					Item newItem(int(0.8 + (1.25 - 0.8)*BlowUpFactor*double(rand()) / RAND_MAX), int(100 + (1000 - 100)*double(rand()) / RAND_MAX));
					if (newItem.value >= 1) try { items.push_back(newItem); }
					catch (...) { throw "No Memory."; }
				}
			}
			catch (...) { throw("No Memory."); }
		}
		else {
			int value, weight;
			size_t ctr;

			for (ctr = 1; i<argc; i++, ctr++) {
				if (!(istringstream(argv[i]) >> dec >> weight)) throw "Malformed Number.";
				if ((ctr % 2) - 1) {
					if (value<1) throw "Malformed number for Value, must be integer>1";
					if (weight<0) throw "Malformed number for Weight, must be integer>0";
					try { items.push_back(Item(value, weight)); }
					catch (...) { throw "No Memory."; }
				}
				value = weight;
			}
			if ((ctr % 2) - 1) throw "Extra Value.";
		}

		if (OutputInitial) {
			cout << "Initial items:" << endl << endl << "Value / Weight" << endl;
			for (auto item : items)
				cout << item.value << " " << item.weight << endl;
			cout << endl;
		}

		Backpack backpack(items, maxWeight, dynamic);

		if (Output) {
			cout << "Backpack filled with:" << endl << endl << "Value / Weight" << endl;
			for (auto item : backpack)
				cout << item.value << " " << item.weight << endl;
			cout << endl << endl;
		}
		cout << "Backpack in total:" << endl << endl << "Value / Weight" << endl << backpack.hasValue() << " " << backpack.weighs() << endl;
	}
	catch (const char *Reason) { cerr << Reason << endl; exit(1); }

	return 0;
}