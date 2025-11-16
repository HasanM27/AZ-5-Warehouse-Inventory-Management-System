#ifndef ORDER_H
#define ORDER_H

struct Order {
    int orderId;
    int productId;
    int quantity;
    bool urgent;

    Order() : orderId(0), productId(0), quantity(0), urgent(false) {}
    Order(int oid, int pid, int qty, bool urg=false) 
        : orderId(oid), productId(pid), quantity(qty), urgent(urg) {}
};

#endif
