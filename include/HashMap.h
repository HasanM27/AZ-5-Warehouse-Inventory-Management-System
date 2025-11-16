#ifndef HASHMAP_H
#define HASHMAP_H

#include "Product.h"
#include <iostream>
using namespace std;

// Node for chaining in hash table
struct HashNode {
    int key;           // Product ID
    Product value;     // Product data
    HashNode* next;    // Pointer to next node in chain

    HashNode(int k, Product v) : key(k), value(v), next(nullptr) {}
};

class HashMap {
private:
    HashNode** buckets;    // Array of pointers to HashNode (buckets)
    int capacity;          // Size of the bucket array
    int size;              // Number of elements in the hash map

    // Hash function: maps product ID to bucket index
    int hashFunction(int key);

    // Helper function to resize the hash table when load factor is too high
    void resize();

public:
    HashMap(int initialCapacity = 16);
    ~HashMap();

    // Insert or update a product in the hash map
    void insert(Product product);

    // Get a product by ID, returns nullptr if not found
    Product* get(int productId);

    // Remove a product by ID
    void remove(int productId);

    // Check if a product exists
    bool contains(int productId);

    // Get the number of products in the hash map
    int getSize();

    // Check if the hash map is empty
    bool isEmpty();

    // Display all products in the hash map
    void display();
};

#endif
