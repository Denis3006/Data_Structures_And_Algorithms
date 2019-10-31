#include <iostream>
#include <iomanip>

using namespace std;

template<class T> class Heap{  // Implements a Max Heap
private:
	size_t HeapSize,
		MaxHeapSize;
	T *array;

	size_t parent(size_t i){ return i / 2; }
	size_t left_child(size_t i){ return 2 * i; }
	size_t right_child(size_t i){ return 2 * i + 1; }
public:
	Heap(size_t size) 
	{
		HeapSize = 0; MaxHeapSize = size;
		try { array = (new T[MaxHeapSize]) - 1; }
		catch (...) { throw "No Memory."; }  
	}
	Heap() = delete;
	~Heap() { delete[](array + 1); }  // Offest +1: First element has index 1.
	bool IsEmpty() { return HeapSize == 0; }
	size_t Size() { return HeapSize; }

	/// theese functions had to be implemented by student ///
	const T& operator[](size_t Index) const  // Read-Access to Elements of heap with Index from 0 to Size()-1
	{   
		if (Index >= HeapSize) throw "Index out of bounds.";
		return array[Index + 1];
	}

	void Insert(T what) 
	{
		if (HeapSize == MaxHeapSize) throw "Heap ist full.";
		HeapSize += 1;
		array[HeapSize] = what;
		size_t i = HeapSize;
		while (i != 1 && (parent(i) > 0 && array[parent(i)] < array[i])) {
			if (parent(i) > 0 && array[parent(i)] < array[i]) {
				swap(array[i], array[parent(i)]);
				i = parent(i);
			}
		}
	}

	T ExtractMax()  // Pops the largest Value from the heap.
	{ 
		T result = array[1];
		// after extracting of maximum the heap-properties are lost
		array[1] = array[HeapSize];  // move the smallest element to the top
		size_t i = 1;
		// heapify
		while (i < HeapSize 
			   && (left_child(i) < HeapSize && array[left_child(i)] > array[i])
			   || (right_child(i) < HeapSize && array[right_child(i)] > array[i])) {
			// if left child exist, left child is greater than i-th element and than right child
			if (left_child(i) < HeapSize && array[left_child(i)] > array[i] && array[left_child(i)] >= array[right_child(i)]) {
				// move left child higher
				swap(array[i], array[left_child(i)]);
				i = left_child(i);
			}
			// if right child exist, right child is greater than i-th element and than left child
			else if (right_child(i) < HeapSize && array[right_child(i)] > array[i] && array[right_child(i)] >= array[left_child(i)]) {
				// move right child higher
				swap(array[i], array[right_child(i)]);
				i = right_child(i);
			}
		}
		array[HeapSize] = 0;
		HeapSize -= 1;
		return result;
	}

	T *AscendingContent()  // Returns a pointer to an sorted array
	{
		T* result = nullptr;
		try {
			result = new T[HeapSize];
		}
		catch (...) { throw "Out of Memory!"; }
		size_t origHeapSize = HeapSize;
		// exctract the elements from heap to the result vector
		for (size_t i = origHeapSize; i > 0; i--)
			result[i - 1] = ExtractMax();
		// exctractMax removes elements, therefore restore the heap 
		for (size_t i = 0; i < origHeapSize; i++) 
			Insert(result[i]);
		return result;
	}
	/// end of the functions that had to be implemented by student ///
};

// Theese 4 functions are part of implementation, security by obscurity...
size_t TUG23(size_t rg) { return ((rg << 1) | ((!!(rg&(1<22))) ^ (!!(rg&(1 << 17)))))&((1 << 23) - 1); }
size_t TUG22(size_t rg) { return ((rg << 1) | ((!!(rg&(1<21))) ^ (!!(rg&(1 << 11)))))&((1 << 22) - 1); }
void IncrementHashValue(size_t & HashValue, size_t & zbase, size_t by) { zbase = TUG23(zbase); HashValue = (HashValue + by*(zbase % 1000)) % (1 << 28); }

size_t CheckHeap(size_t run) 
{
	size_t zin = 0xAAAAAA; size_t zbase = 0xa5a5a5;
	size_t EmptyCounter = 0, FullCounter = 0; int In = 0; size_t HashValue = 5;
	Heap<unsigned char> *TestHeap = new Heap<unsigned char>(40);
	for (size_t i = 0; i<1000000; i++) {
		zin = TUG22(zin);
		if (zin % 400 < 400) {
			try { TestHeap->Insert(zin % 101); In++; }
			catch (const char *what) { what; IncrementHashValue(HashValue, zbase, FullCounter++); }
		}
		else {
			try { IncrementHashValue(HashValue, zbase, size_t(TestHeap->ExtractMax())); In--; }
			catch (const char *what) { what; IncrementHashValue(HashValue, zbase, EmptyCounter++); }
		}
		if (run && (i % 2000) == 0 && TestHeap->Size() >0)  {
			unsigned char *Sorted = TestHeap->AscendingContent();
			for (size_t j = 0; j<TestHeap->Size(); j++) IncrementHashValue(HashValue, zbase, size_t(Sorted[j]));
			delete[] Sorted;
			if (TestHeap->Size()) (*TestHeap)[0];
			for (size_t j = TestHeap->Size() - 1; j>0; j--) if (size_t((*TestHeap)[j]) > size_t((*TestHeap)[(j - 1) / 2]))
				throw("This is no Heap.");
		}
		if (In - TestHeap->Size()) IncrementHashValue(HashValue, zbase, (In) >= 0 ? In : -In);
	}
	return HashValue;
}

int main(int argc, char *argv[]) 
{
	cout << "Explain the algorithm of the heap to your tutor using the following output:" << endl << endl;
	Heap<size_t> Stapel(31); size_t mr = 123456;
	for (size_t i = 0; i<300; i++) {
		if (((mr = TUG22(mr)) % 10)<6) {
			try {
				Stapel.Insert((mr = TUG22(mr)) % 100);
				cout << " Insertet:" << setiosflags(ios::fixed) << setw(3) << mr % 100 << " Heap: | ";
				for (size_t j = 0, k = 2; j<Stapel.Size(); j++) {
					cout << setiosflags(ios::fixed) << setw(3) << Stapel[j];
					if (j == k - 2) { cout << " |"; k = 2 * k; }
				}
				cout << endl;
			}
			catch (const char *) { ; }
		}
		else {
			if (Stapel.Size()) {
				size_t pop = Stapel.ExtractMax();
				cout << "Extracted:" << setiosflags(ios::fixed) << setw(3) << pop << " Heap: | ";
				if (Stapel.Size()) {
					for (size_t j = 0, k = 2; j<Stapel.Size(); j++) {
						cout << setiosflags(ios::fixed) << setw(3) << Stapel[j];
						if (j == k - 2) { cout << " |"; k = 2 * k; }
					}
				}
				cout << endl;
			}
		}
	}
	
	cout << endl << "Checking Minimum Implementation, Check Value is "
		<< ((CheckHeap(0) == 14003496) ? "OK." : "false.") << endl;
	cout << endl << "Checking Implementation inclusive AscendingContent, Check Value is "
		<< ((CheckHeap(1) == 145479215) ? "OK." : "false.") << endl;
}