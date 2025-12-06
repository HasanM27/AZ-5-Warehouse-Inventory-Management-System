#include "../include/WarehouseSystem.h"
#include "../src/Product.cpp"
#include "../src/MinHeap.cpp"
#include "../src/MaxHeap.cpp"
#include "../src/HashMap.cpp"
#include "../src/AVLTree.cpp"
#include "../src/OrderQueue.cpp"
#include "../src/WarehouseSystem.cpp"

#include <iostream>
#include <iomanip>

using namespace std;

void displayMenu()
{
    cout << "\n========== WAREHOUSE MANAGEMENT SYSTEM ==========" << endl;
    cout << "1.  Add Product" << endl;
    cout << "2.  Remove Product" << endl;
    cout << "3.  Update Stock" << endl;
    cout << "4.  Search Product" << endl;
    cout << "5.  Display All Products" << endl;
    cout << "6.  Place Order" << endl;
    cout << "7.  Process Next Order" << endl;
    cout << "8.  View Pending Orders" << endl;
    cout << "9.  View Lowest Selling Products" << endl;
    cout << "10. View Best Selling Products" << endl;
    cout << "11. Exit" << endl;
    cout << "=================================================" << endl;
    cout << "Enter your choice: ";
}

void addProductMenu(WarehouseSystem &warehouse)
{
    int id, quantity, salesCount = 0;
    string name, category;
    double price;

    cout << "\n--- Add Product ---" << endl;
    cout << "Enter Product ID: ";
    cin >> id;

    // Check if product already exists
    if (warehouse.searchProduct(id) != nullptr)
    {
        cout << "Product with ID " << id << " already exists!" << endl;
        return;
    }

    cin.ignore(); // Clear input buffer
    cout << "Enter Product Name: ";
    getline(cin, name);

    cout << "Enter Category: ";
    getline(cin, category);

    cout << "Enter Quantity: ";
    cin >> quantity;

    cout << "Enter Price: $";
    cin >> price;

    cout << "Enter Initial Sales Count (default 0): ";
    cin >> salesCount;

    Product p(id, name, category, quantity, price, salesCount);
    warehouse.addProduct(p);
}

void removeProductMenu(WarehouseSystem &warehouse)
{
    int id;
    cout << "\n--- Remove Product ---" << endl;
    cout << "Enter Product ID to remove: ";
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << "Product not found!" << endl;
        return;
    }

    cout << "Are you sure you want to remove '" << p->name << "' (ID: " << id << ")? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y')
    {
        warehouse.removeProduct(id);
    }
    else
    {
        cout << "Removal cancelled." << endl;
    }
}

void updateStockMenu(WarehouseSystem &warehouse)
{
    int id, qty;
    cout << "\n--- Update Stock ---" << endl;
    cout << "Enter Product ID: ";
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << "Product not found!" << endl;
        return;
    }

    cout << "Current stock for '" << p->name << "': " << p->quantity << endl;
    cout << "Enter new quantity: ";
    cin >> qty;

    if (qty < 0)
    {
        cout << "Invalid quantity! Quantity cannot be negative." << endl;
        return;
    }

    warehouse.updateStock(id, qty);
}

void searchProductMenu(WarehouseSystem &warehouse)
{
    int id;
    cout << "\n--- Search Product ---" << endl;
    cout << "Enter Product ID: ";
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << "Product not found!" << endl;
        return;
    }

    cout << "\n--- Product Details ---" << endl;
    cout << "ID: " << p->id << endl;
    cout << "Name: " << p->name << endl;
    cout << "Category: " << p->category << endl;
    cout << "Quantity: " << p->quantity << endl;
    cout << fixed << setprecision(2) << "Price: $" << p->price << endl;
    cout << "Total Sales: " << p->salesCount << endl;
}

void placeOrderMenu(WarehouseSystem &warehouse)
{
    int id, qty;

    cout << "\n--- Place Order ---" << endl;
    cout << "Enter Product ID: ";
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << "Product not found!" << endl;
        return;
    }

    cout << "Product: " << p->name << " | Available: " << p->quantity << endl;
    cout << "Enter Quantity: ";
    cin >> qty;

    if (qty <= 0)
    {
        cout << "Invalid quantity!" << endl;
        return;
    }

    warehouse.placeOrder(id, qty);
}

int main()
{
    // Initialize warehouse system with default capacities
    // Note: HashMap will automatically resize when needed (load factor > 0.75)
    // Heaps have fixed capacity, so we set a reasonable default
    const int DEFAULT_MIN_HEAP_CAP = 1000;  // For lowest selling products
    const int DEFAULT_MAX_HEAP_CAP = 1000;  // For best selling products
    const int DEFAULT_HASHMAP_CAP = 16;      // Will auto-resize dynamically

    cout << "========== WAREHOUSE MANAGEMENT SYSTEM ==========" << endl;
    cout << "Initializing system with default capacities..." << endl;
    cout << "  MinHeap: " << DEFAULT_MIN_HEAP_CAP << " (lowest selling products)" << endl;
    cout << "  MaxHeap: " << DEFAULT_MAX_HEAP_CAP << " (best selling products)" << endl;
    cout << "  HashMap: " << DEFAULT_HASHMAP_CAP << " (auto-resizes dynamically)" << endl;

    WarehouseSystem warehouse(DEFAULT_MIN_HEAP_CAP, DEFAULT_MAX_HEAP_CAP, DEFAULT_HASHMAP_CAP);

    cout << "\nWarehouse system initialized successfully!" << endl;

    int choice;
    bool running = true;

    while (running)
    {
        displayMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            addProductMenu(warehouse);
            break;

        case 2:
            removeProductMenu(warehouse);
            break;

        case 3:
            updateStockMenu(warehouse);
            break;

        case 4:
            searchProductMenu(warehouse);
            break;

        case 5:
            cout << "\n--- All Products ---" << endl;
            warehouse.displayAllProducts();
            break;

        case 6:
            placeOrderMenu(warehouse);
            break;

        case 7:
            cout << "\n--- Process Next Order ---" << endl;
            warehouse.processNextOrder();
            break;

        case 8:
            cout << "\n--- Pending Orders ---" << endl;
            warehouse.printOrders();
            break;

        case 9:
            cout << "\n--- Lowest Selling Products ---" << endl;
            warehouse.printLowSellingHeap();
            break;

        case 10:
            cout << "\n--- Best Selling Products ---" << endl;
            warehouse.printBestSellingHeap();
            break;

        case 11:
            cout << "\nThank you for using Warehouse Management System!" << endl;
            running = false;
            break;

        default:
            cout << "\nInvalid choice! Please enter a number between 1-11." << endl;
            break;
        }

        if (running)
        {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    return 0;
}
