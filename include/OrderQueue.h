#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "Order.h"
#include <iostream>
using namespace std;

#define MAX_ORDERS 100

class OrderQueue {
private:
    Order orders[MAX_ORDERS];
    int front;
    int rear;
    int size;

public:
    OrderQueue();

    void enqueue(Order o);        // Add order
    Order dequeue();              // Remove order
    Order peek();                 // See next order
    bool isEmpty();
    bool isFull();
    void printQueue();
};

#endif
