#include "../include/WarehouseSystem.h"
#include "../include/Product.h"
#include <iostream>
using namespace std;

int main() {
    // Initialize WarehouseSystem
    WarehouseSystem ws(20, 20, 20);

    // -----------------------------
    // Add Products
    // -----------------------------
    Product p1(1, "Apple", "Fruit", 100, 1.5);
    Product p2(2, "Orange", "Fruit", 50, 1.0);
    Product p3(3, "Banana", "Fruit", 30, 0.8);
    Product p4(4, "Milk", "Dairy", 20, 2.0);
    Product p5(5, "Cheese", "Dairy", 10, 3.5);

    ws.addProduct(p1);
    ws.addProduct(p2);
    ws.addProduct(p3);
    ws.addProduct(p4);
    ws.addProduct(p5);

    cout << "\n--- All Products ---\n";
    ws.displayAllProducts();

    // -----------------------------
    // Place Orders
    // -----------------------------
    ws.placeOrder(1, 20);       // 20 Apples
    ws.placeOrder(3, 10, true); // 10 Bananas (urgent)
    ws.placeOrder(5, 5);        // 5 Cheese

    cout << "\n--- Orders Queue ---\n";
    ws.printOrders();

    // -----------------------------
    // Process Orders
    // -----------------------------
    ws.processNextOrder(); // Should process urgent order first (Banana)
    ws.processNextOrder(); // Next order (Apple)
    ws.processNextOrder(); // Cheese

    cout << "\n--- Orders Queue After Processing ---\n";
    ws.printOrders();

    // -----------------------------
    // Update Stock
    // -----------------------------
    ws.updateStock(4, 50);   // Add 50 Milk
    ws.updateStock(2, -20);  // Remove 20 Orange

    // -----------------------------
    // Print Heaps
    // -----------------------------
    cout << "\n--- Low Stock Heap ---\n";
    ws.printLowStockHeap();

    cout << "\n--- Best Selling Heap ---\n";
    ws.printBestSalesHeap();

    // -----------------------------
    // Search Products
    // -----------------------------
    cout << "\n--- Search Product ---\n";
    Product* searchP = ws.searchProduct(1);
    if (searchP) {
        cout << "Found: " << searchP->name 
             << " | Quantity: " << searchP->quantity
             << " | Sales: " << searchP->salesCount << "\n";
    }

    // -----------------------------
    // Remove Product
    // -----------------------------
    ws.removeProduct(3); // Remove Banana
    ws.displayAllProducts();

    return 0;
}
