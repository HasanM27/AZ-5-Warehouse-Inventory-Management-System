#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "Product.h"
#include <iostream>
using namespace std;

class MaxHeap {
private:
    Product *maximum;     
    int max_capacity;  
    int max_heap_size;

    void swap(Product *a, Product *b);
    int parent(int i);
    int left(int i);
    int right(int i);

public:
    MaxHeap(int cap);
    ~MaxHeap();

    void insert(Product p);
    Product getMax();
    void increaseSales(int productId, int newSalesCount);
    void printHeap();
};

#endif 
