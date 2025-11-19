#ifndef WAREHOUSESYSTEM_H
#define WAREHOUSESYSTEM_H

#include "Product.h"
#include "MinHeap.h"
#include "MaxHeap.h"
#include "HashMap.h"
#include "Order.h"
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

class WarehouseSystem {
private:
    MinHeap lowStockHeap;
    MaxHeap bestSalesHeap;
    HashMap productsMap;

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
    void printLowStockHeap();
    void printBestSalesHeap();

    ~WarehouseSystem();
};

#endif
