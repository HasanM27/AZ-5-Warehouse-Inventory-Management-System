#ifndef WAREHOUSESYSTEM_H
#define WAREHOUSESYSTEM_H

#include "Product.h"
#include "MinHeap.h"
#include "MaxHeap.h"
#include "HashMap.h"
#include "AVLTree.h"
#include "Order.h"
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

class WarehouseSystem {
private:
    AVLTree productsTree;      // For O(log n) search by ID
    HashMap productsMap;       // For O(1) average retrieval by ID
    MinHeap lowSellingHeap;    // For O(1) retrieval of lowest selling product (by salesCount)
    MaxHeap bestSellingHeap;   // For O(1) retrieval of best selling product (by salesCount)

    queue<Order> orderQueue;
    int nextOrderId;

public:
    WarehouseSystem(int minHeapCap, int maxHeapCap, int hashMapCap);

    // Product management
    void addProduct(Product p);
    void removeProduct(int productId);
    void updateStock(int productId, int qty);
    Product* searchProduct(int productId);
    void displayAllProducts();

    // Orders
    void placeOrder(int productId, int qty, bool urgent = false);
    void processNextOrder();
    void printOrders();

    // Heap display
    void printLowSellingHeap();
    void printBestSellingHeap();

    ~WarehouseSystem();
};

#endif
