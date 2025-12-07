#include "../include/WarehouseSystem.h"
#include "../include/Colors.h"
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
using namespace Colors;

void displayMenu()
{
    cout << "\n" << Theme::HEADER << "========== WAREHOUSE MANAGEMENT SYSTEM ==========" << RESET << endl;
    cout << Theme::MENU_ITEM << "1.  Add Product" << RESET << endl;
    cout << Theme::MENU_ITEM << "2.  Remove Product" << RESET << endl;
    cout << Theme::MENU_ITEM << "3.  Update Stock" << RESET << endl;
    cout << Theme::MENU_ITEM << "4.  Search Product" << RESET << endl;
    cout << Theme::MENU_ITEM << "5.  Display All Products" << RESET << endl;
    cout << Theme::MENU_ITEM << "6.  Place Order" << RESET << endl;
    cout << Theme::MENU_ITEM << "7.  Process Next Order" << RESET << endl;
    cout << Theme::MENU_ITEM << "8.  View Pending Orders" << RESET << endl;
    cout << Theme::MENU_ITEM << "9.  View Lowest Selling Products" << RESET << endl;
    cout << Theme::MENU_ITEM << "10. View Best Selling Products" << RESET << endl;
    cout << Theme::MENU_ITEM << "11. Exit" << RESET << endl;
    cout << Theme::SEPARATOR << "=================================================" << RESET << endl;
    cout << Theme::PROMPT << "Enter your choice: " << RESET;
}

void addProductMenu(WarehouseSystem &warehouse)
{
    int id, quantity, salesCount = 0;
    string name, category;
    double price;

    cout << "\n" << Theme::HEADER << "--- Add Product ---" << RESET << endl;
    cout << Theme::PROMPT << "Enter Product ID: " << RESET;
    cin >> id;

    // Check if product already exists
    if (warehouse.searchProduct(id) != nullptr)
    {
        cout << Theme::ERR << "Product with ID " << id << " already exists!" << RESET << endl;
        return;
    }

    cin.ignore(); // Clear input buffer
    cout << Theme::PROMPT << "Enter Product Name: " << RESET;
    getline(cin, name);

    cout << Theme::PROMPT << "Enter Category: " << RESET;
    getline(cin, category);

    cout << Theme::PROMPT << "Enter Quantity: " << RESET;
    cin >> quantity;

    cout << Theme::PROMPT << "Enter Price: $" << RESET;
    cin >> price;

    cout << Theme::PROMPT << "Enter Initial Sales Count (default 0): " << RESET;
    cin >> salesCount;

    Product p(id, name, category, quantity, price, salesCount);
    warehouse.addProduct(p);
}

void removeProductMenu(WarehouseSystem &warehouse)
{
    int id;
    cout << "\n" << Theme::HEADER << "--- Remove Product ---" << RESET << endl;
    cout << Theme::PROMPT << "Enter Product ID to remove: " << RESET;
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
        return;
    }

    cout << Theme::WARNING << "Are you sure you want to remove '" << Theme::DATA << p->name 
         << Theme::WARNING << "' (ID: " << Theme::DATA << id << Theme::WARNING << ")? (y/n): " << RESET;
    char confirm;
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y')
    {
        warehouse.removeProduct(id);
    }
    else
    {
        cout << Theme::INFO << "Removal cancelled." << RESET << endl;
    }
}

void updateStockMenu(WarehouseSystem &warehouse)
{
    int id, qty;
    cout << "\n" << Theme::HEADER << "--- Update Stock ---" << RESET << endl;
    cout << Theme::PROMPT << "Enter Product ID: " << RESET;
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
        return;
    }

    cout << Theme::INFO << "Current stock for '" << Theme::DATA << p->name 
         << Theme::INFO << "': " << Theme::DATA << p->quantity << RESET << endl;
    cout << Theme::PROMPT << "Enter new quantity: " << RESET;
    cin >> qty;

    if (qty < 0)
    {
        cout << Theme::ERR << "Invalid quantity! Quantity cannot be negative." << RESET << endl;
        return;
    }

    warehouse.updateStock(id, qty);
}

void searchProductMenu(WarehouseSystem &warehouse)
{
    int id;
    cout << "\n" << Theme::HEADER << "--- Search Product ---" << RESET << endl;
    cout << Theme::PROMPT << "Enter Product ID: " << RESET;
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
        return;
    }

    cout << "\n" << Theme::HEADER << "--- Product Details ---" << RESET << endl;
    cout << Theme::INFO << "ID: " << Theme::DATA << p->id << RESET << endl;
    cout << Theme::INFO << "Name: " << Theme::DATA << p->name << RESET << endl;
    cout << Theme::INFO << "Category: " << Theme::DATA << p->category << RESET << endl;
    cout << Theme::INFO << "Quantity: " << Theme::DATA << p->quantity << RESET << endl;
    cout << fixed << setprecision(2) << Theme::INFO << "Price: $" << Theme::DATA << p->price << RESET << endl;
    cout << Theme::INFO << "Total Sales: " << Theme::DATA << p->salesCount << RESET << endl;
}

void placeOrderMenu(WarehouseSystem &warehouse)
{
    int id, qty;

    cout << "\n" << Theme::HEADER << "--- Place Order ---" << RESET << endl;
    cout << Theme::PROMPT << "Enter Product ID: " << RESET;
    cin >> id;

    Product *p = warehouse.searchProduct(id);
    if (p == nullptr)
    {
        cout << Theme::ERR << "Product not found!" << RESET << endl;
        return;
    }

    cout << Theme::INFO << "Product: " << Theme::DATA << p->name 
         << Theme::INFO << " | Available: " << Theme::DATA << p->quantity << RESET << endl;
    cout << Theme::PROMPT << "Enter Quantity: " << RESET;
    cin >> qty;

    if (qty <= 0)
    {
        cout << Theme::ERR << "Invalid quantity!" << RESET << endl;
        return;
    }

    warehouse.placeOrder(id, qty);
}

int main()
{
    // Enable ANSI colors on Windows
    enableColors();
    
    // Initialize warehouse system with default capacities
    // Note: HashMap will automatically resize when needed (load factor > 0.75)
    // Heaps have fixed capacity, so we set a reasonable default
    const int DEFAULT_MIN_HEAP_CAP = 1000;  // For lowest selling products
    const int DEFAULT_MAX_HEAP_CAP = 1000;  // For best selling products
    const int DEFAULT_HASHMAP_CAP = 16;      // Will auto-resize dynamically

    cout << Theme::HEADER << "========== WAREHOUSE MANAGEMENT SYSTEM ==========" << RESET << endl;
    cout << Theme::INFO << "Initializing system with default capacities..." << RESET << endl;
    cout << Theme::INFO << "  MinHeap: " << Theme::DATA << DEFAULT_MIN_HEAP_CAP 
         << Theme::INFO << " (lowest selling products)" << RESET << endl;
    cout << Theme::INFO << "  MaxHeap: " << Theme::DATA << DEFAULT_MAX_HEAP_CAP 
         << Theme::INFO << " (best selling products)" << RESET << endl;
    cout << Theme::INFO << "  HashMap: " << Theme::DATA << DEFAULT_HASHMAP_CAP 
         << Theme::INFO << " (auto-resizes dynamically)" << RESET << endl;

    WarehouseSystem warehouse(DEFAULT_MIN_HEAP_CAP, DEFAULT_MAX_HEAP_CAP, DEFAULT_HASHMAP_CAP);

    cout << "\n" << Theme::SUCCESS << "Warehouse system initialized successfully!" << RESET << endl;

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
            cout << "\n" << Theme::HEADER << "--- All Products ---" << RESET << endl;
            warehouse.displayAllProducts();
            break;

        case 6:
            placeOrderMenu(warehouse);
            break;

        case 7:
            cout << "\n" << Theme::HEADER << "--- Process Next Order ---" << RESET << endl;
            warehouse.processNextOrder();
            break;

        case 8:
            cout << "\n" << Theme::HEADER << "--- Pending Orders ---" << RESET << endl;
            warehouse.printOrders();
            break;

        case 9:
            cout << "\n" << Theme::HEADER << "--- Lowest Selling Products ---" << RESET << endl;
            warehouse.printLowSellingHeap();
            break;

        case 10:
            cout << "\n" << Theme::HEADER << "--- Best Selling Products ---" << RESET << endl;
            warehouse.printBestSellingHeap();
            break;

        case 11:
            cout << "\n" << Theme::SUCCESS << "Thank you for using Warehouse Management System!" << RESET << endl;
            running = false;
            break;

        default:
            cout << "\n" << Theme::ERR << "Invalid choice! Please enter a number between 1-11." << RESET << endl;
            break;
        }

        if (running)
        {
            cout << "\n" << Theme::PROMPT << "Press Enter to continue..." << RESET;
            cin.ignore();
            cin.get();
        }
    }

    return 0;
}
