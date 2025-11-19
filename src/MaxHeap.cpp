#include "../include/MaxHeap.h"

    MaxHeap::MaxHeap(int cap)
    {
        max_heap_size = 0;
        max_capacity = cap;
        maximum = new Product[cap];
    }

    MaxHeap::~MaxHeap() {
        delete[] maximum;
    }

    // swap function 
    void MaxHeap::swap(Product *x, Product*y)
    {
        Product temp = *x;
        *x = *y;
        *y = temp;
    }
    
    // to tell the parent of any element
    int MaxHeap::parent(int i) {return (i - 1) / 2;}

    // to tell the left child of any parent element
    int MaxHeap::left(int i) {return (2 * i + 1);}

    // to tell the right child of any parent element
    int MaxHeap::right(int i) {return (2 * i + 2);}

    // insert function to insert a product if its new
    void MaxHeap::insert(Product p) {
        if (max_heap_size == max_capacity) {
            cout << "Overflow: cannot insert more products\n";
            return;
        }

        int i = max_heap_size++;
        maximum[i] = p;

        while (i != 0 && maximum[parent(i)].salesCount < maximum[i].salesCount) {
            swap(&maximum[i], &maximum[parent(i)]);
            i = parent(i);
        }
    }

    // get the root value
    Product MaxHeap::getMax() {
        if (max_heap_size <= 0) {
            cout << "Heap is empty\n";
            return Product();
        }
        return maximum[0];
    }

    // increase the vlaue if the product already exists inside the heap
    void MaxHeap::increaseSales(int productId, int newSalesCount) {
        int i = -1;
        for (int j = 0; j < max_heap_size; j++) {
            if (maximum[j].id == productId) {
                i = j;
                break;
            }
        }
        if (i==-1){ cout<< "\nNo Product found with that ID"; return;}

        maximum[i].salesCount = newSalesCount;

        while (i != 0 && maximum[parent(i)].salesCount < maximum[i].salesCount) {
            swap(&maximum[i], &maximum[parent(i)]);
            i = parent(i);
        }
    }

    void MaxHeap::printHeap() {
        for (int i = 0; i < max_heap_size; ++i) {
            cout << maximum[i].name << " (" << maximum[i].quantity << ")  ";
        }
        cout << endl;
    }