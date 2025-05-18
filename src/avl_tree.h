#include<iostream>
using namespace std;

/*
    Node is the interface for AVLTree's node's data
*/
class Node {
public:
    virtual bool operator==(const Node &n);
    virtual bool operator<(const Node &n);
};

/*
    NodeImpl is an example implementation for Node interface
*/
class NodeImpl: public Node{
public:
    int val;
    NodeImpl(int v=0): val(v) {}
    ~NodeImpl() {}

    bool operator==(const NodeImpl &n) {
        return val == n.val;
    }
    bool operator>(const NodeImpl &n) {
        return val > n.val;
    }
    bool operator<(const NodeImpl &n) {
        return val < n.val;
    }
};


/*
        node
        /  \
   smaller  bigger(or equal)
*/
class AVLTree{
private:
    int height;
    Node *node;
    AVLTree *left, *right;
public:
    AVLTree(Node *node= NULL): node(node) {

    }

    ~AVLTree() {
        delete node;
    }

    bool contains(Node *n) {
        if(n==NULL) return false;
        if(this==NULL) return false;

        if(this->node ==n) return true;
        if(this->node<n) return this->left->contains(n);
        return this->right->contains(n);
    }

    /*
        inserts into tree
    */
    AVLTree* insert(Node *n) {
        return NULL;
    }

    /*
        remove node from tree
        TODO: decide how to handle duplicates
    */
    AVLTree* remove(Node *n) {
        return NULL;
    }
};
