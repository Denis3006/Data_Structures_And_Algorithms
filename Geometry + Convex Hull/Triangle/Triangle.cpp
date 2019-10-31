#include "../Geometry.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <assert.h>
#include <ctime>

using namespace std;

void usage()
{
	throw "Usage: Triangle [ x1 y1 x2 y2 x3 y3 ]";
}

int main(int argc, char* argv[]) {
	try	{
		vector<double> coordinates;
		try	{
			coordinates.resize(6);
		}
		catch (...)	{ throw "Out of memory"; }

		if (argc != 1 && argc != 7) usage();
		for (int i = 1; i < argc; i++)  // parse options
			if (!(istringstream(argv[i]) >> dec >> coordinates[i - 1]))
				throw "Malformed number in arguments.";
		if (argc == 1) {
			srand(int(time(0)));
			for (double &coordinate : coordinates) {
				double random = double(rand()) / (RAND_MAX + 1);
				coordinate = random;
				assert(coordinate >= 0 && coordinate < 1);
			}
		}
		Triangle Triangle({{ Point{ coordinates[0], coordinates[1] },
							 Point{ coordinates[2], coordinates[3] },
							 Point{ coordinates[4], coordinates[5] } }});
		cout << "Triangle ( " << Triangle << " " << ") has Girth: " << Triangle.Girth() << '\n';
	}
	catch (const char* c) {
		cout << c;
		exit(1);
	}
	return 0;
}