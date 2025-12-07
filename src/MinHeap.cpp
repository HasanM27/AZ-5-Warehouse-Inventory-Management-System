#include "../include/MinHeap.h"

// constructor
MinHeap::MinHeap(int cap) : minimum(new Product[cap]), min_capacity(cap), min_heap_size(0) {}

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
    
    // to get parent of the element
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

        int oldSalesCount = minimum[j].salesCount;
        minimum[j].salesCount = newSalesCount;

        // If salesCount increased, we might need to bubble DOWN (larger values go down in min heap)
        // If salesCount decreased, we need to bubble UP (smaller values go up in min heap)
        if (newSalesCount > oldSalesCount) {
            // Bubble DOWN: check if current node is larger than its children
            while (true) {
                int smallest = j;
                int l = left(j);
                int r = right(j);
                
                if (l < min_heap_size && minimum[l].salesCount < minimum[smallest].salesCount) {
                    smallest = l;
                }
                if (r < min_heap_size && minimum[r].salesCount < minimum[smallest].salesCount) {
                    smallest = r;
                }
                
                if (smallest != j) {
                    swap(&minimum[j], &minimum[smallest]);
                    j = smallest;
                } else {
                    break;
                }
            }
        } else {
            // Bubble UP: check if current node is smaller than its parent
            while (j != 0 && minimum[parent(j)].salesCount > minimum[j].salesCount) {
                swap(&minimum[j], &minimum[parent(j)]);
                j = parent(j);
            }
        }
    }

    void MinHeap::printHeap() {
    if (min_heap_size == 0) {
        cout << "Heap is empty." << endl;
        return;
    }
    for (int i = 0; i < min_heap_size; i++)
        cout << minimum[i].name << " (sales: " << minimum[i].salesCount << ")  ";
    cout << endl;
    cout << "Root (lowest selling): " << minimum[0].name 
         << " with salesCount = " << minimum[0].salesCount << endl;
}