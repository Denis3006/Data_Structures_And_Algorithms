#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <initializer_list>
#include <cmath>

using namespace std;

class Point : public vector<double> 
{
public:
	friend ostream& operator<<(ostream& TheOstream, Point& me)
	{
		return print(TheOstream, me);
	}

	static ostream& print(ostream& TheOstream, Point me) 
	{
		TheOstream << "(";
		for (size_t i = 0; i < me.size(); i++) {
			TheOstream << me[i];
			if (i + 1 < me.size()) {
				TheOstream << ", ";
			}
		}
		TheOstream << ")";
		return TheOstream;
	}

	Point(initializer_list<double> args) 
	{
		for_each(args.begin(), args.end(), [this](auto& arg) {this->push_back(arg);});
	}

	double EuclidDistanceTo(const Point& Other) 
	{
		if (this->size() != Other.size()) throw "Points differ in dimensions.";
		double distance = 0;
		for (size_t i = 0; i < this->size(); i++) {
			distance += pow(Other.at(i) - this->at(i), 2);
		}
		return sqrt(distance);
	}

	double operator-(const Point& Punkt) 
	{
		return EuclidDistanceTo(Punkt);
	}

	double operator[](size_t n) const
	{
		return this->at(n);
	}
};

class PointArray : private vector < Point > 
{
public:
	using vector<Point>::size;
	using vector<Point>::operator[];

	PointArray(initializer_list<Point> args) 
	{
		for_each(args.begin(), args.end(), [this](auto& arg) {this->push_back(arg);});
	}

	void push_back(Point Punkt) 
	{
		if (this->size() != 0 && Punkt.size() != this->at(0).size()) throw "Points differ in dimensions.";
		try
		{
			vector<Point>::push_back(Punkt);
		}
		catch (...)	{ throw "Out of memory."; }
	}

	Point operator[](size_t n)
	{
		return this->at(n);
	}

	Point back()
	{
		return this->at(size() - 1);
	}

	friend ostream& operator<<(ostream& TheOstream, PointArray& me)
	{
		for (size_t i = 0; i < me.size(); i++)
			Point::print(TheOstream, me[i]);
		return TheOstream;
	}

	static void sortByXCoordinate(PointArray& points)
	{
		sort(points.begin(), points.end(), [](Point a, Point b) {return a[0] < b[0]; });
	}

	static void sortClockwise(PointArray& points)
	{
		sort(points.begin(), points.end(), [](Point a, Point b) {return atan2(a[0], a[1]) < atan2(b[0], b[1]); });
	}
};

class Simplex : private PointArray 
{
public:
	using PointArray::size;
	using PointArray::operator[];

	Simplex(const PointArray& ThePointArray) : PointArray(ThePointArray)
	{
		if (ThePointArray.size() != ThePointArray[0].size() + 1) throw "Points differ in dimensions.";
	}
	void push_back(Point Punkt) = delete;
};

class Triangle : public Simplex 
{
public:
	using Simplex::operator[];

	double Girth() 
	{
		double girth = 0;
		for (int i = 0; i < 3; i++)
			girth += this->operator[](i) - this->operator[]((i + 1) % 3);
		return girth;
	}

	Triangle(const Simplex& TheSimplex) :Simplex(TheSimplex) 
	{
		if (TheSimplex.size() != 3) throw "More than 3 points in triangle.";
	}

	friend ostream& operator<<(ostream& TheOstream, Triangle& me)
	{
		for (size_t i = 0; i < me.size(); i++)
			Point::print(TheOstream, me[i]);
		return TheOstream;
	}
};

class Line : private PointArray {
public:
	using PointArray::operator[];

	Line(const Point& P1, const Point& P2) : PointArray({ P1, P2 }) {
		if (P1.size() != 2)throw "Points in Line have to be 2 dimensional!";
	}
	double Length() { return ((*this)[0] - (*this)[1]); }
	bool ThisfPointLeftOfMe(const Point& To) {
		// Nutzt Hessesche Normalenform der Geradengleichung aus. Siehe
		// http://de.wikipedia.org/wiki/Hessesche_Normalform#Abstand
		Point S0 = Point{ (*this)[0][0] - To[0], (*this)[0][1] - To[1] };
		Point S1 = Point{ (*this)[1][0] - To[0], (*this)[1][1] - To[1] };
		double ax = S1[0] - S0[0];
		double ay = S1[1] - S0[1];
		double d = ay * S0[0] - ax * S0[1];
		return d <= 0.0;
	}
};