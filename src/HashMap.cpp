#include "../include/HashMap.h"
#include <algorithm>

// Initialize hash map with given capacity
HashMap::HashMap(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    buckets = new HashNode*[capacity];
    
    // Initialize all buckets to nullptr
    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }
}

//  Free all memory
HashMap::~HashMap() {
    for (int i = 0; i < capacity; i++) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] buckets;
}

// Hash function which is a simple modulo hash for product IDs
int HashMap::hashFunction(int key) {
    return abs(key) % capacity;
}

// Insert or update a product
void HashMap::insert(Product product) {
    int index = hashFunction(product.id);
    HashNode* current = buckets[index];

    // Check if product already exists in this bucket
    while (current != nullptr) {
        if (current->key == product.id) {
            // Update existing product
            current->value = product;
            return;
        }
        current = current->next;
    }

    // Product doesn't exist, insert new node at the beginning of the chain
    HashNode* newNode = new HashNode(product.id, product);
    newNode->next = buckets[index];
    buckets[index] = newNode;
    size++;

    // Resize if load factor exceeds 0.75 
    if ((double)size / capacity > 0.75) {
        resize();
    }
}

// Get a product by ID
Product* HashMap::get(int productId) {
    int index = hashFunction(productId);
    HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->key == productId) {
            return &(current->value);
        }
        current = current->next;
    }

    return nullptr; 
}

// Remove a product by ID
void HashMap::remove(int productId) {
    int index = hashFunction(productId);
    HashNode* current = buckets[index];
    HashNode* prev = nullptr;

    // Traverse the chain to find the product
    while (current != nullptr) {
        if (current->key == productId) {
            // Found the product, remove it
            if (prev == nullptr) {
                // Product is at the head of the chain
                buckets[index] = current->next;
            } else {
                // Product is in the middle or end
                prev->next = current->next;
            }
            delete current;
            size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Check if a product exists
bool HashMap::contains(int productId) {
    return get(productId) != nullptr;
}

// Get the number of products
int HashMap::getSize() {
    return size;
}

// Check if the hash map is empty
bool HashMap::isEmpty() {
    return size == 0;
}

// Resize the hash table to maintain O(1) average performance
void HashMap::resize() {
    int oldCapacity = capacity;
    capacity *= 2; // Double the capacity
    
    // Create new bucket array
    HashNode** newBuckets = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        newBuckets[i] = nullptr;
    }

    // Rehash all existing elements
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            
            // Calculate new index for this key
            int newIndex = hashFunction(current->key);
            
            // Insert into new bucket
            current->next = newBuckets[newIndex];
            newBuckets[newIndex] = current;
            
            current = next;
        }
    }

    // Delete old bucket array and update pointer
    delete[] buckets;
    buckets = newBuckets;
}

// Display all products in the hash map
void HashMap::display() {
    if (isEmpty()) {
        cout << "HashMap is empty." << endl;
        return;
    }

    cout << "Products in HashMap (Total: " << size << "):" << endl;
    for (int i = 0; i < capacity; i++) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            Product p = current->value;
            cout << "ID: " << p.id << ", Name: " << p.name 
                 << ", Category: " << p.category 
                 << ", Quantity: " << p.quantity 
                 << ", Price: $" << p.price << endl;
            current = current->next;
        }
    }
}
