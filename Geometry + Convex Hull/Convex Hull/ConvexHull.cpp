#include "../Geometry.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <ctime>

using namespace std;

PointArray SimpleConvexHull(PointArray& Points)
{
	PointArray hull{};
	bool valid = true;

	for (size_t i = 0; i < Points.size(); i++) {
		for (size_t b = 0; b < Points.size(); b++) {
			valid = true;
			if (Points[i] == Points[b])
				continue;

			Line L(Points[i], Points[b]);

			for (size_t z = 0; z < Points.size(); z++) {
				if (Points[z] != L[0] && Points[z] != L[1]) {
					if (!L.ThisfPointLeftOfMe(Points[z])) {
						valid = false;
						break;
					}
				}
			}

			if (valid) {
				hull.push_back(L[0]);
				break;
			}
		}
	}
	return hull;
}

PointArray CalculateHull(PointArray& AllPoints)
{
	if (AllPoints.size() == 1) throw "Hull needs at least 2 Points to build ";
	PointArray::sortByXCoordinate(AllPoints);
	PointArray convexHull = SimpleConvexHull(AllPoints);
	PointArray::sortClockwise(convexHull);
	return convexHull;
}

void usage()
{
	throw "Usage: ConvexHull n | { x1 y1 x2 y2 x3 y3 ... }";
}

int main(int argc, char* argv[]) 
{
	try {
		vector<double> coordinates;
		if (argc != 2 || argc % 2 != 1) usage();
		if (argc == 2) {  // use random coordinates
			int n = 0;
			if (!(istringstream(argv[1]) >> dec >> n)) throw "Malformed number in arguments.";
			if (n <= 0) throw "Parameter n must be positive Integer.";
			try {
				coordinates.resize(2 * n);
			}
			catch (...)	{ throw "Out of memory."; }
			srand(int(time(0)));
			for (double &coordinate : coordinates) {
				double random = pow(-1, rand()) * double(rand()) / (RAND_MAX + 1);
				coordinate = random;
			}
		}
		else {
			try	{
				coordinates.resize(argc - 1);
			}
			catch (...)	{ throw "Out of memory."; }
			for (size_t i = 1; i < argc; i++) { // parse coordinates
				if (!(istringstream(argv[i]) >> dec >> coordinates[i - 1]))
					throw "Malformed number in arguments.";
			}
		}
		cout << "Set of " << coordinates.size() / 2 << " Points is: " << endl;
		PointArray preconvex{};
		for (size_t i = 0; i < coordinates.size(); i += 2) {
			preconvex.push_back(Point{ coordinates[i], coordinates[i + 1] });
			Point::print(cout, preconvex.back());
			cout << endl;
		}
		
		PointArray Convex = CalculateHull(preconvex);
		cout << endl <<  "Convex Hull is build clockwise form the " << Convex.size() << " following Points:" << endl;
		for (size_t i = 0; i < Convex.size(); i ++)	{
			Point::print(cout, Convex[i]);
			cout << endl;
		}
	}
	catch (const char* c) {
		cout << c;
		exit(1);
	}
	return 0;
}