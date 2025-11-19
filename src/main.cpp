#include "../include/WarehouseSystem.h"
#include "../include/AVLTree.h"
#include "../include/OrderQueue.h"
#include "../include/HashMap.h"
#include "../include/MinHeap.h"
#include "../include/MaxHeap.h"
#include "../include/Product.h"
#include "HashMap.cpp"
#include "MaxHeap.cpp"
#include "MinHeap.cpp"
#include "AVLTree.cpp"
#include "OrderQueue.cpp"
#include "Product.cpp"
#include "WarehouseSystem.cpp"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct DemoProductSeed {
    int id;
    const char* name;
    const char* category;
    int quantity;
    double price;
    int salesCount;
};

vector<Product> buildSampleProducts() {
    vector<DemoProductSeed> seeds = {
        {1, "Apple", "Fruit", 120, 1.50, 80},
        {2, "Orange", "Fruit", 90,  1.30, 65},
        {3, "Banana", "Fruit", 70,  0.90, 120},
        {4, "Milk", "Dairy", 45, 2.30, 30},
        {5, "Cheese", "Dairy", 25, 4.80, 15},
        {6, "Butter", "Dairy", 15, 5.30, 5}
    };

    vector<Product> products;
    products.reserve(seeds.size());
    for (const auto& s : seeds) {
        products.emplace_back(s.id, s.name, s.category, s.quantity, s.price, s.salesCount);
    }
    return products;
}

void printSection(const string& title) {
    cout << "\n==================== " << title << " ====================\n";
}

void demoHashMap(const vector<Product>& products) {
    printSection("HashMap");
    HashMap productMap;
    for (const auto& p : products) {
        productMap.insert(p);
    }
    productMap.display();

    Product* found = productMap.get(2);
    if (found) {
        cout << "\nLookup Product 2 -> " << found->name << " (Qty: " << found->quantity << ")\n";
        found->quantity += 25;
        cout << "Updated quantity for " << found->name << " to " << found->quantity << endl;
    }

    cout << "\nRemoving Product 6...\n";
    productMap.remove(6);
    productMap.display();
}

void demoAVLTree(const vector<Product>& products) {
    printSection("AVL Tree");
    AVLTree tree;
    for (const auto& p : products) {
        tree.insert(p);
    }
    cout << "In-order traversal (sorted by Product ID):\n";
    tree.inorderTraverse();

    Product* p = tree.search(4);
    if (p) {
        cout << "\nSearch result -> " << p->name << " (Price: $" << fixed << setprecision(2) << p->price << ")\n";
    }

    cout << "\nRemoving Product ID 3 and traversing again:\n";
    tree.remove(3);
    tree.inorderTraverse();
}

void demoHeaps(const vector<Product>& products) {
    printSection("Heaps (Low Stock / Best Sales)");
    MinHeap lowStockHeap(20);
    MaxHeap bestSalesHeap(20);

    for (const auto& p : products) {
        lowStockHeap.insert(p);
        bestSalesHeap.insert(p);
    }

    Product low = lowStockHeap.getMin();
    Product best = bestSalesHeap.getMax();

    cout << "Lowest sales count product: " << low.name << " | Sales: " << low.salesCount << endl;
    cout << "Best selling product: " << best.name << " | Sales: " << best.salesCount << endl;

    cout << "\nAdjusted sales count for Butter to 60 (should bubble up in heaps)\n";
    lowStockHeap.IncreaseSales(6, 60);
    bestSalesHeap.increaseSales(6, 60);

    cout << "Low stock heap snapshot:\n";
    lowStockHeap.printHeap();
    cout << "Best sales heap snapshot:\n";
    bestSalesHeap.printHeap();
}

void demoOrderQueue() {
    printSection("OrderQueue");
    OrderQueue oq;
    oq.enqueue(Order(1, 1, 12, false));
    oq.enqueue(Order(2, 2, 8, true));   // urgent -> should move to front
    oq.enqueue(Order(3, 5, 4, false));
    oq.enqueue(Order(4, 4, 6, true));   // urgent -> should move to front

    cout << "Queue after enqueues (urgent orders front-loaded):\n";
    oq.printQueue();

    cout << "\nProcessing two orders...\n";
    Order processed1 = oq.dequeue();
    cout << "Processed Order #" << processed1.orderId << " (Product " << processed1.productId << ")\n";
    Order processed2 = oq.dequeue();
    cout << "Processed Order #" << processed2.orderId << " (Product " << processed2.productId << ")\n";

    cout << "\nQueue snapshot:\n";
    oq.printQueue();
}

void demoWarehouseSystem(const vector<Product>& products) {
    printSection("WarehouseSystem Integration");
    WarehouseSystem ws(20, 20, 32);

    cout << "Loading products...\n";
    for (const auto& p : products) {
        ws.addProduct(p);
    }
    ws.displayAllProducts();

    cout << "\nPlacing orders (one urgent)...\n";
    ws.placeOrder(3, 20);            // bananas
    ws.placeOrder(6, 5, true);       // urgent butter
    ws.placeOrder(2, 15);            // oranges
    ws.printOrders();

    cout << "\nProcessing queued orders...\n";
    ws.processNextOrder();
    ws.processNextOrder();
    ws.printOrders();

    cout << "\nUpdating stock...\n";
    ws.updateStock(4, 75);
    ws.updateStock(1, 110);

    cout << "\nLow stock heap (MinHeap-backed):\n";
    ws.printLowStockHeap();
    cout << "Best sales heap (MaxHeap-backed):\n";
    ws.printBestSalesHeap();

    cout << "\nSearching for Product ID 2...\n";
    if (Product* p = ws.searchProduct(2)) {
        cout << "Found " << p->name << " | Qty: " << p->quantity << " | Sales: " << p->salesCount << endl;
    }

    cout << "\nRemoving Product ID 5 (Cheese)...\n";
    ws.removeProduct(5);
    ws.displayAllProducts();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Product> sampleProducts = buildSampleProducts();

    demoHashMap(sampleProducts);
    demoAVLTree(sampleProducts);
    demoHeaps(sampleProducts);
    demoOrderQueue();
    demoWarehouseSystem(sampleProducts);

    printSection("Demo Complete");
    return 0;
}
