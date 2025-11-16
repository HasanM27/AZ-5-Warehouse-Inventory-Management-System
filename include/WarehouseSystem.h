#ifndef WAREHOUSE_SYSTEM_H
#define WAREHOUSE_SYSTEM_H

#include "Product.h"
#include "HashMap.h"
#include "MinHeap.h"
#include "MaxHeap.h"
#include "OrderQueue.h"
#include <iostream>
using namespace std;

class WarehouseSystem {
private:
    HashMap productTable;     // Fast lookup
    MinHeap lowStockHeap;     // Track low-stock products
    MaxHeap bestSalesHeap;    // Track top-selling products
    OrderQueue orderQueue;    // Order processing

    int nextOrderId;          // Auto-increment order IDs

    // Helper functions to sync heaps and hash map
    void syncHeaps(Product &p);
    void syncProduct(Product &p);

public:
    WarehouseSystem(int hashMapSize, int minHeapSize, int maxHeapSize);

    // Product management
    void addProduct(Product p);
    void removeProduct(int productId);
    void updateStock(int productId, int qty);
    Product* searchProduct(int productId);

    // Order management
    void placeOrder(int productId, int quantity, bool urgent = false);
    void processNextOrder();

    // Debug / reporting
    void printLowStockHeap();
    void printBestSalesHeap();
    void printOrders();
    void displayAllProducts();
};

#endif
