#include "../include/WarehouseSystem.h"

// Constructor
WarehouseSystem::WarehouseSystem(int minHeapCap, int maxHeapCap, int hashMapCap)
    : lowStockHeap(minHeapCap),
      bestSalesHeap(maxHeapCap),
      productsMap(hashMapCap),
      nextOrderId(1) {}

// Add a new product
void WarehouseSystem::addProduct(Product p) {
    productsMap.insert(p);
    lowStockHeap.insert(p);
    bestSalesHeap.insert(p);
}

// Remove product
void WarehouseSystem::removeProduct(int productId) {
    Product* p = productsMap.get(productId);
    if (p != nullptr) {
        productsMap.remove(productId);
        cout << "Product " << p->name << " removed from warehouse." << endl;
    } else {
        cout << "Product not found!" << endl;
    }
}

// Update stock quantity
void WarehouseSystem::updateStock(int productId, int qty) {
    Product* p = productsMap.get(productId);
    if (p != nullptr) {
        p->quantity = qty;
        lowStockHeap.IncreaseSales(productId, p->salesCount); // Rebalance heaps
        bestSalesHeap.increaseSales(productId, p->salesCount);
    }
}

// Search for a product
Product* WarehouseSystem::searchProduct(int productId) {
    return productsMap.get(productId);
}

// Display all products
void WarehouseSystem::displayAllProducts() {
    productsMap.display();
}

// Place order
void WarehouseSystem::placeOrder(int productId, int qty, bool urgent) {
    Product* p = productsMap.get(productId);
    if (p == nullptr) {
        cout << "Product not found!" << endl;
        return;
    }
    if (p->quantity < qty) {
        cout << "Insufficient stock!" << endl;
        return;
    }
    Order newOrder(nextOrderId++, productId, qty, urgent);
    if (urgent) {
        // For urgent, just push to front (simulate priority)
        queue<Order> tmp;
        tmp.push(newOrder);
        while (!orderQueue.empty()) {
            tmp.push(orderQueue.front());
            orderQueue.pop();
        }
        orderQueue = tmp;
    } else {
        orderQueue.push(newOrder);
    }
    p->salesCount += qty;
    bestSalesHeap.increaseSales(productId, p->salesCount);
    lowStockHeap.IncreaseSales(productId, p->salesCount);
}

// Process the next order
void WarehouseSystem::processNextOrder() {
    if (orderQueue.empty()) {
        cout << "No orders to process." << endl;
        return;
    }
    Order o = orderQueue.front();
    orderQueue.pop();
    Product* p = productsMap.get(o.productId);
    if (p != nullptr) {
        p->quantity -= o.quantity;
        cout << "Processed order #" << o.orderId << ": " << p->name
             << ", Qty: " << o.quantity << endl;
    }
}

// Print all orders in queue
void WarehouseSystem::printOrders() {
    if (orderQueue.empty()) {
        cout << "No pending orders." << endl;
        return;
    }
    queue<Order> tmp = orderQueue;
    cout << "Pending orders:" << endl;
    while (!tmp.empty()) {
        Order o = tmp.front();
        tmp.pop();
        cout << "Order #" << o.orderId << " | Product ID: " << o.productId
             << " | Qty: " << o.quantity 
             << ", Urgent: " << (o.urgent ? "Yes" : "No") << endl;
    }
}

// Print heaps
void WarehouseSystem::printLowStockHeap() {
    cout << "Low stock products: ";
    lowStockHeap.printHeap();
}

void WarehouseSystem::printBestSalesHeap() {
    cout << "Best selling products: ";
    bestSalesHeap.printHeap();
}

// Destructor
WarehouseSystem::~WarehouseSystem() {}
