#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

class JobSpecification
{
public:
	JobSpecification() = default;
	JobSpecification(int firstTime, int secondTime) : FirstTime(firstTime), SecondTime(secondTime) {};
	int FirstTime, SecondTime;
	bool operator<(JobSpecification j) { return SecondTime < j.SecondTime; }
};

class JobVector : public vector < JobSpecification > 
{
public:
	JobVector() = default;
	JobVector(char *Filename)
	{
		try {
			bool empty = true;
			ifstream file(Filename);
			if (file.fail()) { throw "File not found."; }
			string line;
			while (getline(file, line, '\n')) {
				empty = false;
				istringstream buffer(line);
				buffer.exceptions(istringstream::failbit | istringstream::badbit);
				int First, Second;
				buffer >> dec >> First;
				buffer.ignore(256, ',');
				buffer >> dec >> Second;
				if (Second < First) throw "End of Job prior to Start of Job.";
				try {
					push_back(JobSpecification(First, Second));
				}
				catch (...) { throw "Out of memory.\n"; }
			}
			file.close();
			if (empty){ throw "Nothing usable in file."; }
		}
		catch (...){
			throw "Garbage in file.";
		}
	}

};

JobVector IntervalScheduling(JobVector &Jobs)
{
	if (Jobs.size() <= 0)
		return JobVector();
	JobVector Schedule;
	try {
		Schedule.reserve(Jobs.size());
		Schedule.push_back(Jobs[0]);
	}
	catch (...){ throw "Out of memory."; }
	sort(Jobs.begin(), Jobs.end());
	size_t j = 0;
	for (size_t i = 1; i < Jobs.size(); i++){
		if (Jobs[i].FirstTime > Jobs[j].SecondTime){
			try{
				Schedule.push_back(Jobs[i]);
			}
			catch (...){ throw "Out of memory."; }
			j = i;
		}
	}
	return Schedule;
}

JobVector LatenessScheduling(JobVector &Jobs)
{
	if (Jobs.size() <= 0)
		return JobVector();
	JobVector Schedule;
	try	{
		Schedule.resize(Jobs.size());
	}
	catch (...){ throw "Out of memory."; }
	sort(Jobs.begin(), Jobs.end());
	size_t z = 0;
	for (size_t i = 0; i < Jobs.size(); i++){
		Schedule[i].FirstTime = z;
		z += Jobs[i].SecondTime - Jobs[i].FirstTime;
		Schedule[i].SecondTime = z;
	}
	return Schedule;
}

void usage() 
{
	throw
	"Usage: Scheduler Filename [ -i | -l ] \n"
	"  Filename must be a name of a file \n"
	"  holding a 2 column csv list with start and end times. \n"
	"Options: \n"
	"  -l : Lateness Scheduling \n"
	"  -i : Interval Scheduling \n";
}

int main(int argc, char *argv[])
{
	try {
		if (argc > 3) throw "Too many parameters!\n";
		if (argc == 1) usage();
		bool interval = true;
		char *filename = argv[1];
		if (argc == 3) {
			if (!string(argv[2]).compare("-l"))
				interval = false;
			if (!string(argv[2]).compare("-i"))
				interval = true;
			if (string(argv[2]).compare("-i") && (string(argv[2]).compare("-l"))){ throw "Wrong Switch, use only one of [ -i | -l ]"; }
		}
		JobVector Jobs(filename);
		JobVector Schedule;
		try {
			Schedule.resize(Jobs.size());
		}
		catch (...) { throw "Out of memory."; }
		if (interval) {
			Schedule =  IntervalScheduling(Jobs);
			for (size_t i = 0; i < Schedule.size(); i++)
				cout << "Job: " << i << " is from " << Schedule[i].FirstTime << " to " << Schedule[i].SecondTime << endl;
			cout << Jobs.size() - Schedule.size() << " Jobs were discarded." << endl;
		}
		if (!interval) {
			Schedule = LatenessScheduling(Jobs);
			int Delay = 0;
			for (size_t i = 0; i < Schedule.size(); i++) {
				cout << "Job: " << i << " takes " << Jobs[i].SecondTime - Jobs[i].FirstTime << " has Deadline of " << Jobs[i].SecondTime << " is from " << Schedule[i].FirstTime << " to " << Schedule[i].SecondTime;
				if (Jobs[i].SecondTime < Schedule[i].SecondTime) {
					cout << " is delayed by " << Schedule[i].SecondTime - Jobs[i].SecondTime;
					if (Schedule[i].SecondTime - Jobs[i].SecondTime > Delay) Delay = Schedule[i].SecondTime - Jobs[i].SecondTime;
				}
				cout << endl;
			}
			cout << "Max. Delay is : " << Delay << endl;
		}
	}
	catch (const char *what) {
		cerr << what;
		exit(1);
	}
	return 0;
}