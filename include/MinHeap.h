#ifndef MINHEAP_H
#define MINHEAP_H

#include "Product.h"
#include <iostream>
using namespace std;

class MinHeap {
private:
    Product *minimum;     
    int min_capacity;  
    int min_heap_size;

    void swap(Product *a, Product *b);
    int parent(int i);
    int left(int i);
    int right(int i);

public:
    MinHeap(int cap);
    ~MinHeap();

    void insert(Product p);
    Product getMin();
    void IncreaseSales(int productId, int newQty);
    void printHeap(); 
};

#endif 
