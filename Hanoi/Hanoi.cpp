#include <iostream>
#include <sstream>
#include <cstdlib>

void move(int quantity, char start, char help, char target){
	static char sum = start + help + target;
	if (quantity > 0) {
		move(quantity - 1, start, help, sum - start - target);
		std::cout << "Move the top disc from " << start << " to " << target << '\n';
		move(quantity - 1, sum - start - target, help, target);
	}
}

int main(int argc, char *argv[]) 
{
	size_t N;
	if (argc != 2 || !(std::istringstream(argv[1]) >> std::dec >> N) || 0 >= N) {
		std::cerr << "Malformed number for n, Must be positive integer." << '\n';
		exit(1);
	}
	move(N, 'A', 'B', 'C');
	return 0;
}