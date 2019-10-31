#include <iostream>
#include <vector>
#include <sstream>
#include <assert.h>
 
using namespace std;

vector<int> change(int value, vector<int> denomination) {
	assert(value > 0);
	if (denomination.size() <= 0) throw "Denomination is empty.";
	vector<int> coins(denomination.size(), 0);
	for (size_t i = 0; i < denomination.size(); i++) {
		while (value >= denomination[i]) {
			value -= denomination[i];
			coins[i] += 1;
		}
	}
	return coins;
}

void usage()
{
	throw 
	"Usage: \n"
	"CoinChange Value Euro | Knopf \n"
	"  Value : integer value \n"
	"  Euro / Knopf decides between available coin values.";
}

int main(int argc, char* argv[]) {
	try {
		if (argc != 3) usage();
		int sum = 0;
		string currency;
		vector<int> denomination, coins;
		if (!(istringstream(argv[1]) >> dec >> sum)) throw "Malformed number for SomeInteger.";
		if (sum <= 0) throw "First Parameter Value must be at least 1";

		if (!string(argv[2]).compare("Euro") || !string(argv[2]).compare("euro")){
			try {
				currency = "Euro";
				denomination = { 200, 100, 50, 20, 10, 5, 2, 1 };
				coins = change(sum, denomination);
			}
			catch (const char* c) { throw c; }
			catch (...) { throw "Out of memory."; }
		} 
		else if (!string(argv[2]).compare("Knopf") || !string(argv[2]).compare("knopf")){
			try {
				currency = "Knopf";
				denomination = { 200, 100, 50, 20, 10, 5, 4, 2, 1 };
				coins = change(sum, denomination);
			}
			catch (const char* c) { throw c; }
			catch (...) { throw "Out of memory."; }
		}
		else throw "Second argument must be either Euro or Knopf.";

		cout << sum << " " << currency << " is: \n";
		for (size_t i = 0; i < coins.size(); i++)
			if (coins[i] > 0)
				cout << coins[i] << " x " << denomination[i] << (i == (coins.size() - 1) ? (" " + currency) : " + ");
	}
	catch (const char* c) {
		cerr << c << '\n';
		exit(1);
	}
	return 0;
}