#include "../include/WarehouseSystem.h"

// Constructor
WarehouseSystem::WarehouseSystem(int hashMapSize, int minHeapSize, int maxHeapSize)
    : productTable(hashMapSize), lowStockHeap(minHeapSize), bestSalesHeap(maxHeapSize)
{
    nextOrderId = 1; // initialize order IDs
}

// ------------------------
// Private Helper Functions
// ------------------------

// Sync heaps after product update (quantity or salesCount changed)
void WarehouseSystem::syncHeaps(Product &p) {
    lowStockHeap.IncreaseSales(p.id, p.salesCount);   // Update low stock heap (use salesCount for example)
    bestSalesHeap.increaseSales(p.id, p.salesCount);  // Update best-selling heap
}

// Sync product in hash map and heaps
void WarehouseSystem::syncProduct(Product &p) {
    productTable.insert(p);      // insert/update in hash map
    syncHeaps(p);                // update heaps
}

// ------------------------
// Product Management
// ------------------------

void WarehouseSystem::addProduct(Product p) {
    if (productTable.contains(p.id)) {
        cout << "Product with ID " << p.id << " already exists!\n";
        return;
    }
    productTable.insert(p);
    lowStockHeap.insert(p);
    bestSalesHeap.insert(p);
    cout << "Product " << p.name << " added successfully.\n";
}

void WarehouseSystem::removeProduct(int productId) {
    Product* p = productTable.get(productId);
    if (!p) {
        cout << "Product not found!\n";
        return;
    }

    productTable.remove(productId);
    // For heaps, you could implement remove or just ignore (simpler: re-build heap when needed)
    cout << "Product " << p->name << " removed successfully.\n";
}

void WarehouseSystem::updateStock(int productId, int qty) {
    Product* p = productTable.get(productId);
    if (!p) {
        cout << "Product not found!\n";
        return;
    }

    p->quantity += qty;
    syncProduct(*p);
    cout << "Stock for " << p->name << " updated to " << p->quantity << ".\n";
}

Product* WarehouseSystem::searchProduct(int productId) {
    Product* p = productTable.get(productId);
    if (!p) {
        cout << "Product not found!\n";
    }
    return p;
}

// ------------------------
// Order Management
// ------------------------

void WarehouseSystem::placeOrder(int productId, int quantity, bool urgent) {
    Product* p = productTable.get(productId);
    if (!p) {
        cout << "Product not found!\n";
        return;
    }

    if (p->quantity < quantity) {
        cout << "Insufficient stock for product " << p->name << ".\n";
        return;
    }

    Order o(nextOrderId++, productId, quantity, urgent);
    orderQueue.enqueue(o);
    cout << "Order placed for " << quantity << " units of " << p->name << ".\n";
}

void WarehouseSystem::processNextOrder() {
    if (orderQueue.isEmpty()) {
        cout << "No orders to process.\n";
        return;
    }

    Order o = orderQueue.dequeue();
    Product* p = productTable.get(o.productId);
    if (!p) {
        cout << "Product in order not found!\n";
        return;
    }

    if (p->quantity < o.quantity) {
        cout << "Not enough stock to process order for " << p->name << ".\n";
        return;
    }

    p->quantity -= o.quantity;
    p->salesCount += o.quantity;  // update sales
    syncProduct(*p);

    cout << "Processed order #" << o.orderId
         << " for " << o.quantity << " units of " << p->name << ".\n";
}

// ------------------------
// Debug / Reporting
// ------------------------

void WarehouseSystem::printLowStockHeap() {
    cout << "Low Stock Heap:\n";
    lowStockHeap.printHeap();
}

void WarehouseSystem::printBestSalesHeap() {
    cout << "Best Selling Heap:\n";
    bestSalesHeap.printHeap();
}

void WarehouseSystem::printOrders() {
    cout << "Orders in queue:\n";
    orderQueue.printQueue();
}

void WarehouseSystem::displayAllProducts() {
    cout << "All products in warehouse:\n";
    productTable.display();
}
