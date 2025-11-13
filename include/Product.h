#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
using namespace std;

struct Product {
    int id;
    string name;
    string category;
    int quantity;
    double price;
    int salesCount; 

    Product();
    Product(int id, string name, string category, int qty, double price, int sales = 0);
};

#endif 
