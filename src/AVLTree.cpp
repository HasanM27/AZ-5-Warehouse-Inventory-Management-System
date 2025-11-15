#include "../include/AVLTree.h"

//constructor
AVLTree::AVLTree(){
    root=nullptr;
}

int AVLTree::getHeight(AVLNode* n){
    return n? n->height:0;
}

int AVLTree::getBalance(AVLNode* n){
    return n ? getHeight(n->left)-getHeight(n->right) : 0;
}

//right rotate
AVLNode* AVLTree::rotateRight(AVLNode* y){
    AVLNode* x=y->left;
    AVLNode* t2=x->right;

    x->right=y;
    y->left=t2;

    y->height=max(getHeight(y->left), getHeight(y->right))+1;
    x->height=max(getHeight(x->left), getHeight(x->right))+1;

    return x;

}

//left rotate
AVLNode* AVLTree::rotateLeft(AVLNode* x){
    AVLNode* y=x->right;
    AVLNode* t2=y->left;

    y->left=x;
    x->right=t2;

    x->height=max(getHeight(x->left), getHeight(x->right))+1;
    y->height=max(getHeight(y->left), getHeight(y->right))+1;

    return y;

}

AVLNode* AVLTree::insertN(AVLNode* node, Product p) {
    if (node == nullptr)
        return new AVLNode(p);

    // BST insert
    if (p.id < node->data.id)
        node->left = insertN(node->left, p);
    else if (p.id > node->data.id)
        node->right = insertN(node->right, p);
    else
        return node; // No duplicates

    // Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Get balance factor
    int balance = getBalance(node);

    // ---------------- Rotations based on imbalance ----------------

    // Left Left Case
    if (balance > 1 && p.id < node->left->data.id)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && p.id > node->right->data.id)
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && p.id > node->left->data.id) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && p.id < node->right->data.id) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}


void AVLTree::insert(Product p) {
    root = insertN(root, p);
}

//minimum value of a node
AVLNode* AVLTree::minNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

//node deletion
AVLNode* AVLTree::deleteN(AVLNode* node, int id) {
    if (node == nullptr)
        return node;

    if (id < node->data.id)
        node->left = deleteN(node->left, id);
    else if (id > node->data.id)
        node->right = deleteN(node->right, id);
    else {
        if ((node->left == nullptr) || (node->right == nullptr)) {
            AVLNode* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            AVLNode* temp = minNode(node->right);
            node->data = temp->data;
            node->right = deleteN(node->right, temp->data.id);
        }
    }

    if (node == nullptr)
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    //balance
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLTree::remove(int id) {
    root = deleteN(root, id);
}

//search the product from id
Product* AVLTree::search(int id) {
    AVLNode* current = root;
    while (current) {
        if (id == current->data.id)
            return &(current->data);
        else if (id < current->data.id)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

//inorder using id
void AVLTree::inorder(AVLNode* node) {
    if (node != nullptr) {
        inorder(node->left);
        cout << node->data.name << " (ID: " << node->data.id << ")" << endl;
        inorder(node->right);
    }
}

void AVLTree::inorderTraverse() {
    inorder(root);
}
