#include "../include/Product.h"

Product::Product() {
    id = 0;
    name = "";
    category = "";
    quantity = 0;
    price = 0.0;
    salesCount = 0;
}

Product::Product(int id, string name, string category, int qty, double price, int sales) {
    this->id = id;
    this->name = name;
    this->category = category;
    this->quantity = qty;
    this->price = price;
    this->salesCount = sales;
}
