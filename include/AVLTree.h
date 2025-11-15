#ifndef AVLTREE_H
#define AVLTREE_H

#include "Product.h"
#include <iostream>
using namespace std;

class AVLNode{
    public:
    Product data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Product p): data(p), left(nullptr), right(nullptr), height(1){}
};

class AVLTree{
    private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insertN(AVLNode* node, Product p);
    AVLNode* deleteN(AVLNode* node, int id);
    AVLNode* minNode(AVLNode* node);
    void inorder(AVLNode* node);

    public:
    AVLTree();
    void insert(Product p);
    void remove(int id);
    Product* search(int id);
    void inorderTraverse();
};

#endif