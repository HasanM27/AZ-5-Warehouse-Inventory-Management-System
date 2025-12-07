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

// Place order (adds to queue, doesn't process yet)
void WarehouseSystem::placeOrder(int productId, int qty, bool urgent) {
    Product* p = productsMap.get(productId);
    if (p == nullptr) {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
        return;
    }
    if (p->quantity < qty) {
        cout << Theme::ERR << "Insufficient stock! Available: " << Theme::DATA << p->quantity 
             << Theme::ERR << ", Requested: " << Theme::DATA << qty << RESET << endl;
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
    
    if (urgent) {
        cout << Theme::URGENT << "Order #" << Theme::DATA << newOrder.orderId 
             << Theme::URGENT << " placed for Product ID " << Theme::DATA << productId 
             << Theme::URGENT << " (Qty: " << Theme::DATA << qty 
             << Theme::URGENT << ") [URGENT]" << RESET << endl;
    } else {
        cout << Theme::SUCCESS << "Order #" << Theme::DATA << newOrder.orderId 
             << Theme::SUCCESS << " placed for Product ID " << Theme::DATA << productId 
             << Theme::SUCCESS << " (Qty: " << Theme::DATA << qty << Theme::SUCCESS << ")" << RESET << endl;
    }
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
        if (o.urgent) {
            cout << Theme::URGENT << "Order #" << Theme::DATA << o.orderId 
                 << Theme::URGENT << " | Product ID: " << Theme::DATA << o.productId
                 << Theme::URGENT << " | Qty: " << Theme::DATA << o.quantity 
                 << Theme::URGENT << ", Urgent: " << Theme::DATA << "Yes" << RESET << endl;
        } else {
            cout << Theme::INFO << "Order #" << Theme::DATA << o.orderId 
                 << Theme::INFO << " | Product ID: " << Theme::DATA << o.productId
                 << Theme::INFO << " | Qty: " << Theme::DATA << o.quantity 
                 << Theme::INFO << ", Urgent: " << Theme::DATA << "No" << RESET << endl;
        }
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
