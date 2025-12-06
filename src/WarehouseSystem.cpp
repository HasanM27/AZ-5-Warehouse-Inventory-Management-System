#include "../include/WarehouseSystem.h"

// Constructor
WarehouseSystem::WarehouseSystem(int minHeapCap, int maxHeapCap, int hashMapCap)
    : lowSellingHeap(minHeapCap),
      bestSellingHeap(maxHeapCap),
      productsMap(hashMapCap),
      nextOrderId(1) {}

// Add a new product to all data structures
void WarehouseSystem::addProduct(Product p) {
    // Add to AVLTree for O(log n) search
    productsTree.insert(p);
    
    // Add to HashMap for O(1) average retrieval
    productsMap.insert(p);
    
    // Add to heaps for O(1) retrieval of best/lowest selling products
    lowSellingHeap.insert(p);
    bestSellingHeap.insert(p);
    
    cout << "Product '" << p.name << "' (ID: " << p.id << ") added to warehouse." << endl;
}

// Remove product from AVLTree and HashMap (only when quantity reaches 0)
// Note: Product remains in heaps as they track sales history
void WarehouseSystem::removeProduct(int productId) {
    Product* p = productsMap.get(productId);
    if (p != nullptr) {
        // Remove from AVLTree
        productsTree.remove(productId);
        
        // Remove from HashMap
        productsMap.remove(productId);
        
        cout << "Product '" << p->name << "' (ID: " << productId 
             << ") removed from warehouse (out of stock)." << endl;
    } else {
        cout << "Product not found!" << endl;
    }
}

// Update stock quantity in both AVLTree and HashMap
void WarehouseSystem::updateStock(int productId, int qty) {
    // Update in HashMap
    Product* p = productsMap.get(productId);
    if (p != nullptr) {
        p->quantity = qty;
        
        // Update in AVLTree
        Product* treeProduct = productsTree.search(productId);
        if (treeProduct != nullptr) {
            treeProduct->quantity = qty;
        }
        
        cout << "Stock updated for Product ID " << productId 
             << ": New quantity = " << qty << endl;
    } else {
        cout << "Product not found!" << endl;
    }
}

// Search for a product (using HashMap for O(1) average retrieval)
Product* WarehouseSystem::searchProduct(int productId) {
    return productsMap.get(productId);
}

// Display all products
void WarehouseSystem::displayAllProducts() {
    productsMap.display();
}

// Helper function to calculate total pending quantity for a product in the queue
int WarehouseSystem::getPendingQuantity(int productId) {
    int totalPending = 0;
    queue<Order> tempQueue = orderQueue; // Copy queue to avoid modifying original
    
    while (!tempQueue.empty()) {
        Order o = tempQueue.front();
        tempQueue.pop();
        if (o.productId == productId) {
            totalPending += o.quantity;
        }
    }
    return totalPending;
}

// Place order (adds to queue, doesn't process yet)
void WarehouseSystem::placeOrder(int productId, int qty, bool urgent) {
    Product* p = productsMap.get(productId);
    if (p == nullptr) {
        cout << "Product not found!" << endl;
        return;
    }
    
    // Calculate total pending quantity for this product in the queue
    int pendingQty = getPendingQuantity(productId);
    
    // Check if we have enough stock considering pending orders
    int availableStock = p->quantity - pendingQty;
    
    if (availableStock < qty) {
        cout << "Insufficient stock! Current: " << p->quantity 
             << ", Pending orders: " << pendingQty
             << ", Available: " << availableStock
             << ", Requested: " << qty << endl;
        return;
    }
    
    Order newOrder(nextOrderId++, productId, qty, urgent);
    if (urgent) {
        // For urgent orders, push to front (simulate priority queue)
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
    
    cout << "Order #" << newOrder.orderId << " placed for Product ID " 
         << productId << " (Qty: " << qty << ")" << endl;
    cout << "  Available stock after this order: " << (availableStock - qty) << endl;
}

// Process the next order: reduces quantity, updates salesCount, updates heaps
// If quantity reaches 0, removes product from AVLTree and HashMap
void WarehouseSystem::processNextOrder() {
    if (orderQueue.empty()) {
        cout << "No orders to process." << endl;
        return;
    }
    
    Order o = orderQueue.front();
    orderQueue.pop();
    
    Product* p = productsMap.get(o.productId);
    if (p == nullptr) {
        cout << "Order #" << o.orderId << " failed: Product not found!" << endl;
        return;
    }
    
    // Safety check: Ensure we have enough stock (in case stock was updated externally)
    if (p->quantity < o.quantity) {
        cout << "Order #" << o.orderId << " failed: Insufficient stock!" << endl;
        cout << "  Available: " << p->quantity << ", Required: " << o.quantity << endl;
        return;
    }
    
    // Reduce quantity
    p->quantity -= o.quantity;
    
    // Update quantity in AVLTree as well
    Product* treeProduct = productsTree.search(o.productId);
    if (treeProduct != nullptr) {
        treeProduct->quantity = p->quantity;
    }
    
    // Update salesCount (previous sales + current order quantity)
    p->salesCount += o.quantity;
    
    // Update salesCount in AVLTree
    if (treeProduct != nullptr) {
        treeProduct->salesCount = p->salesCount;
    }
    
    // Update heaps with new salesCount (for best/lowest selling tracking)
    bestSellingHeap.increaseSales(o.productId, p->salesCount);
    lowSellingHeap.IncreaseSales(o.productId, p->salesCount);
    
    cout << "Processed Order #" << o.orderId << ": " << p->name 
         << " (Qty: " << o.quantity << ")" << endl;
    cout << "  Remaining stock: " << p->quantity 
         << ", Total sales: " << p->salesCount << endl;
    
    // If quantity reaches 0, remove product from AVLTree and HashMap
    // (Product stays in heaps as they track sales history)
    if (p->quantity == 0) {
        removeProduct(o.productId);
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
void WarehouseSystem::printLowSellingHeap() {
    cout << "Lowest selling products (by sales count): ";
    lowSellingHeap.printHeap();
}

void WarehouseSystem::printBestSellingHeap() {
    cout << "Best selling products (by sales count): ";
    bestSellingHeap.printHeap();
}

// Destructor
WarehouseSystem::~WarehouseSystem() {}
