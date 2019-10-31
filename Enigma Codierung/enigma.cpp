#include <iostream> 
#include <fstream>
#include <sstream>
#include <cstring>
#include <ctime>
#include <cmath>

using namespace std;

char* Rotate(char const* Table, int r)
{
	char* t2 = new (nothrow) char[strlen(Table) + 1];
	if (t2 == nullptr) 
		throw "No Memory";
	t2[strlen(Table)] = '\0';
	int index = 0;
	for (int i = 0; i < strlen(Table); i++) {
		index = abs(i + r) % strlen(Table);
		if (i + r < 0)  // negative rotation
			index = strlen(Table) - index;
		t2[i] = Table[index];
	}
	return t2;
}

char* Crypt(char* Input, bool Encrypt, int InitialRotation,	int Rotation)
{
	int size = strlen(Input);
	char* Result = new (nothrow) char[size + 1];
	if(Result == nullptr) 
		throw "No Memory";
	Result[size] = '\0';  // string termination
	int Offset = InitialRotation;
	char const* CurrentTable = nullptr;
	// Encrypt:			 ABCDEFGHIJKLMNOPQRSTUVWXYZ
	char const* Table = "JPGVOUMFYQBENHZRDKASXLICTW";
	// Decrypt:		    "SKXQLHCNWARVGMEBJPTYFDZUIO"
	for (int i = 0; i < size; i++) {  // iterate input
		if (Input[i]<'A' || Input[i]>'Z')
			throw("Invalid Chars in Input. Use only A-Z.");
		// rotating table by Offset:
		CurrentTable = Rotate(Table, Offset);
		if(Encrypt)
			Result[i] = CurrentTable[(Input[i] - 'A')];
		else {
			int index = 0;
			while (CurrentTable[index] != Input[i] && index < strlen(CurrentTable))
				index++;
			Result[i] = 'A' + index;
		}
		Offset += Rotation;
		delete[] CurrentTable;
	}
	return Result;
}

void usage()
{
	throw
	"Usage: enigma [ -i <int Rotation> ] [ -i <int InitialRotation> ] [ -t ] [-d ] <string Input> \n\n"
	"Simple Encryption.\n\n"
	"Options:\n"
	"-i InitialRotation: initial rotation the codebook\n"
	"-r Rotation : rotation of codebook after every Letter\n"
	"-d : decrypt (instead of encrypt)\n"
	"-t : Calculate processing time\n\n"
	"Options may occur in any order.\n";
}

int main(int argc, char* argv[]) {  
	char* Input = nullptr;
	char* Result = nullptr;
	bool Encrypt = true;
	int InitialRotation = 0;
	int Rotation = 0;
	bool meas_time = false;
	// Read and check parameters;
	try {
		if (argc < 2) usage();
		Input = argv[1]; 
		for (int i = 2; i < argc; i++) { // parse options
			if (!string(argv[i]).compare("-t"))
				meas_time = true;
			else if (!string(argv[i]).compare("-d"))
				Encrypt = false;
			else if (!string(argv[i]).compare("-i")) {
				if (++i == argc)
					throw "-i Must be followed by some integer.";
				if (!(istringstream(argv[i]) >> dec >> InitialRotation))
					throw "Malformed number for SomeInteger.";
			}
			else if (!string(argv[i]).compare("-r")) {
				if (++i == argc)
					throw "-r Must be followed by some integer.";
				if (!(istringstream(argv[i]) >> dec >> Rotation))
					throw "Malformed number for SomeInteger.";
			}
			else {
				cerr << "Unknown parameters." << endl;
				exit(1);
			}
		} 
		if (meas_time) {
			double MeasuredTime = double(clock());
			Result = Crypt(Input, Encrypt, InitialRotation, Rotation);
			MeasuredTime = (double(clock()) - MeasuredTime) / CLOCKS_PER_SEC;
			cout << "Elapsed Time: " << MeasuredTime << endl;
		}
		else
			Result = Crypt(Input, Encrypt, InitialRotation, Rotation);
		cout << Result << endl;
		delete[] Result;
	}
	catch (const char* Reason) {
		// We assume Crypt gives exceptions only of type const char *
		// otherwise more complex exception handlers are needed.
		cerr << Reason << endl; 
		exit(1);
	}
	return 0;
} 