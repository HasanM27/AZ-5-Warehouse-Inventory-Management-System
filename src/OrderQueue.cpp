#include "../include/OrderQueue.h"

// Constructor
OrderQueue::OrderQueue() {
    front = 0;
    rear = -1;
    size = 0;
}

// Check if queue is empty
bool OrderQueue::isEmpty() {
    return size == 0;
}

// Check if queue is full
bool OrderQueue::isFull() {
    return size == MAX_ORDERS;
}

// Add order to the queue
void OrderQueue::enqueue(Order o) {
    if (isFull()) {
        cout << "Order queue is full!\n";
        return;
    }

    if (o.urgent) {
        // Place urgent orders at the front
        for (int i = size; i > 0; i--) {
            orders[i] = orders[i-1];
        }
        orders[0] = o;
        rear++;
    } else {
        rear = (rear + 1) % MAX_ORDERS;
        orders[rear] = o;
    }
    size++;
}

// Remove order from the front
Order OrderQueue::dequeue() {
    if (isEmpty()) {
        cout << "Order queue is empty!\n";
        return Order();
    }
    Order o = orders[front];
    front = (front + 1) % MAX_ORDERS;
    size--;
    return o;
}

// Peek at the front order
Order OrderQueue::peek() {
    if (isEmpty()) {
        cout << "Order queue is empty!\n";
        return Order();
    }
    return orders[front];
}

// Print queue for debugging
void OrderQueue::printQueue() {
    cout << "Order Queue: ";
    int count = size;
    int idx = front;
    while (count--) {
        cout << "(" << orders[idx].productId << "," << orders[idx].quantity << ")";
        idx = (idx + 1) % MAX_ORDERS;
        if (count) cout << " <- ";
    }
    cout << endl;
}
