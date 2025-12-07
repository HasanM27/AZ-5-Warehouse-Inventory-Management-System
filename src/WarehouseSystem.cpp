#include "../include/WarehouseSystem.h"
#include "../include/Colors.h"

using namespace Colors;

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
    
    cout << Theme::SUCCESS << "Product '" << Theme::DATA << p.name 
         << Theme::SUCCESS << "' (ID: " << Theme::DATA << p.id 
         << Theme::SUCCESS << ") added to warehouse." << RESET << endl;
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
        
        cout << Theme::WARNING << "Product '" << Theme::DATA << p->name 
             << Theme::WARNING << "' (ID: " << Theme::DATA << productId 
             << Theme::WARNING << ") removed from warehouse (out of stock)." << RESET << endl;
    } else {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
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
        
        cout << Theme::SUCCESS << "Stock updated for Product ID " << Theme::DATA << productId 
             << Theme::SUCCESS << ": New quantity = " << Theme::DATA << qty << RESET << endl;
    } else {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
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
void WarehouseSystem::placeOrder(int productId, int qty) {
    Product* p = productsMap.get(productId);
    if (p == nullptr) {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
        return;
    }
    if (p->quantity < qty) {
        cout << "Insufficient stock! Available: " << p->quantity 
             << ", Requested: " << qty << endl;
        return;
    }
    
    // Create order and add to back of queue (FIFO)
    Order newOrder(nextOrderId++, productId, qty, false);
    orderQueue.push(newOrder);
    
    cout << "Order #" << newOrder.orderId << " placed for Product ID " 
         << productId << " (Qty: " << qty << ")" << endl;
}

// Process the next order: reduces quantity, updates salesCount, updates heaps
// If quantity reaches 0, removes product from AVLTree and HashMap
void WarehouseSystem::processNextOrder() {
    if (orderQueue.empty()) {
        cout << Theme::INFO << "No orders to process." << RESET << endl;
        return;
    }
    
    Order o = orderQueue.front();
    orderQueue.pop();
    
    Product* p = productsMap.get(o.productId);
    if (p == nullptr) {
        cout << Theme::ERR << "Order #" << Theme::DATA << o.orderId 
             << Theme::ERR << " failed: Product not found!" << RESET << endl;
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
    
    cout << Theme::SUCCESS << "Processed Order #" << Theme::DATA << o.orderId 
         << Theme::SUCCESS << ": " << Theme::DATA << p->name 
         << Theme::SUCCESS << " (Qty: " << Theme::DATA << o.quantity << Theme::SUCCESS << ")" << RESET << endl;
    cout << Theme::INFO << "  Remaining stock: " << Theme::DATA << p->quantity 
         << Theme::INFO << ", Total sales: " << Theme::DATA << p->salesCount << RESET << endl;
    
    // If quantity reaches 0, remove product from AVLTree and HashMap
    // (Product stays in heaps as they track sales history)
    if (p->quantity == 0) {
        removeProduct(o.productId);
    }
}

// Print all orders in queue
void WarehouseSystem::printOrders() {
    if (orderQueue.empty()) {
        cout << Theme::INFO << "No pending orders." << RESET << endl;
        return;
    }
    queue<Order> tmp = orderQueue;
    cout << Theme::INFO << "Pending orders:" << RESET << endl;
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
    cout << Theme::INFO << "Lowest selling products (by sales count): " << RESET;
    lowSellingHeap.printHeap();
}

void WarehouseSystem::printBestSellingHeap() {
    cout << Theme::SUCCESS << "Best selling products (by sales count): " << RESET;
    bestSellingHeap.printHeap();
}

// Destructor
WarehouseSystem::~WarehouseSystem() {}
