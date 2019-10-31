#include <iostream>
#include <sstream>

template<class T> class Tree {
public:
	Tree() { root = nullptr; }

	void Insert(T v) {
		Node* predcessor = nullptr;
		Node* iterator = root;
		Node* newNode = nullptr;
		while (iterator != nullptr) {
			predcessor = iterator;
			if (v < iterator->data) 
				iterator = iterator->lc;
			else 
				iterator = iterator->rc;
		}
		try {
			newNode = new Node();
		}
		catch (...) {
			throw "Out of Memory.";
		}
		newNode->data = v;
		if (predcessor == nullptr) root = newNode;
		else {
			if (newNode->data < predcessor->data) predcessor->lc = newNode;
			else predcessor->rc = newNode;
		}
	}

	void InOrder() {
		if (root != nullptr) {
			InOrder(root);
		}
	}

	void PreOrder() {
		if (root != nullptr) {
			PreOrder(root);
		}
	}

	void PostOrder() {
		if (root != nullptr) {
			PostOrder(root);
		}
	}
	
	~Tree() {
		clear(root);
		root = nullptr;
	}

private:
	struct Node {
		T data;
		Node* lc, * rc;
	} *root;

	void clear(Node* x) {
		if (x != nullptr) {
			clear(x->lc);
			x->lc = nullptr;
			clear(x->rc);
			x->rc = nullptr;
			delete x; 
		}
	}

	void InOrder(Node* x) {
		if (x != nullptr) {
			InOrder(x->lc);
			std::cout << x->data << " ";
			InOrder(x->rc);
		}
	}

	void PreOrder(Node* x) {
		if (x != nullptr) {
			std::cout << x->data << " ";
			PreOrder(x->lc);
			PreOrder(x->rc);
		}
	}

	void PostOrder(Node* x) {
		if (x != nullptr) {
			PostOrder(x->lc);
			PostOrder(x->rc);
			std::cout << x->data << " ";
		}
	}
};

void usage() { throw "Test for Binary Tree. Simply enter integers for nodes."; }

int main(int argc, char* argv[]) {
	try {
		Tree<int> Bm;

		if (argc < 2) usage();

		for (int i = 1; i < argc; i++) {
			int Input;
			if (!(std::istringstream(argv[i]) >> std::dec >> Input)) throw "Malformed Number.";
			Bm.Insert(Input);
		}
		std::cout << "  InOrder: "; Bm.InOrder();  std::cout << '\n';
		std::cout << " PreOrder: "; Bm.PreOrder();  std::cout << '\n';
		std::cout << "PostOrder: "; Bm.PostOrder();  std::cout << '\n';
	}
	catch (const char* Reason) { std::cerr << Reason << '\n'; exit(1); }
	std::cin.get();
	return 0;
}