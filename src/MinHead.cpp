#include "../include/MinHeap.h"

// constructor

    // swap function
    void MinHeap::swap(Product *x, Product*y)
    {
        Product temp = *x;
        *x = *y;
        *y = temp;
    }

    MinHeap::~MinHeap() {
        delete[] minimum;
    }
    
    // to get parent
    int MinHeap::parent(int i) {return (i - 1) / 2;}

    // to get left child 
    int MinHeap::left(int i) {return (2 * i + 1);}

    // to get right child
    int MinHeap::right(int i) {return (2 * i + 2);}

    // to insert into the minimum heap if this is the products first entry
    void MinHeap::insert(Product p){
        if (min_heap_size == min_capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }

    min_heap_size++;
    int i = min_heap_size - 1;
    minimum[i] = p;

    while (i != 0 && minimum[parent(i)].salesCount > minimum[i].salesCount) {
            swap(&minimum[i], &minimum[parent(i)]);
            i = parent(i);
        }
    }

    // get the root value
    Product MinHeap::getMin(){
        if (min_heap_size <= 0) {
            cout << "Heap is empty\n";
            return Product();
        }
        return minimum[0];
    }

    // increase the sales count of an already existing Product instead of reinserting a new node
    void MinHeap::IncreaseSales(int ProdID, int newSalesCount){
        int j = -1;
        for (int i = 0; i < min_heap_size; i++)
        {
            if (ProdID == minimum[i].id)
            {
                j=i;
                break;
            }
        }

        if (j==-1){ cout<< "\nNo Product found with that ID"; return;}

        minimum[j].salesCount = newSalesCount;

        while (j != 0 && minimum[parent(j)].salesCount > minimum[j].salesCount) {
            swap(&minimum[j], &minimum[parent(j)]);
            j = parent(j);
        }
        
    }

    void MinHeap::printHeap() {
    for (int i = 0; i < min_heap_size; i++)
        cout << minimum[i].name << " (" << minimum[i].quantity << ")  ";
    cout << endl;
}