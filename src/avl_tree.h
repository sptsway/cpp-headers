#include<iostream>
using namespace std;

/*
    Node is the interface for AVLTree's node's data
*/
class Node {
public:
    virtual bool operator==(const Node &n) const =0;
    virtual bool operator>(const Node &n) const =0;
    virtual bool operator<(const Node &n) const =0;
};

/*
    NodeImpl is an example implementation for Node interface
*/
class NodeImpl: public Node{
public:
    int val;
    NodeImpl(int v=0): val(v) {}
    ~NodeImpl() {}

    bool operator==(const Node &n) const override {
        const NodeImpl* ni = dynamic_cast<const NodeImpl*>(&n);
        return val == ni->val;
    }
    bool operator>(const Node &n) const override {
        const NodeImpl* ni = dynamic_cast<const NodeImpl*>(&n);
        return val > ni->val;
    }
    bool operator<(const Node &n) const override {
        const NodeImpl* ni = dynamic_cast<const NodeImpl*>(&n);
        return val < ni->val;
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
    AVLTree *left, *right, *parent;

    // finds the largest element in the sub-tree
    AVLTree* largest() {
        if(this->right==NULL) return this;
        return this->right->largest();
    }

    // finds the smallest element in the sub-tree
    AVLTree* smallest() {
        if(this->left==NULL) return this;
        return this->left->smallest();
    }


    // rebalance() rebalances the sub-tree by unit 1
    // rebalance can be thought as a self-balancing pulley with
    // left tension: height of left child subtree
    // right tension: height of right child subtree
    // if left child has more height, then pulley rotates right
    // if right child has more height, then pulley rotates left
    AVLTree* rebalance(AVLTree *root) {
        int l=0,r=0;
        if(root==NULL) return NULL;

        if(root->left!=NULL) l=root->left->height;
        if(root->right!=NULL) r=root->right->height;

        if(abs(l-r)<=1) return root;

        if(l<r) return rotateLeft();
        return rotateRight();
    }

    // rotateLeft() rotates the sub-tree 1unit left
    // it takes care of updating height
    AVLTree* rotateLeft() {
        if (this->right==NULL) return this;
        AVLTree *newRoot= this->right->smallest();

        // pick out the newRoot
        // properties of newRoot: it has no left child.
        newRoot->parent->left = newRoot->right;
        if (newRoot->right) {
            newRoot->right->parent = newRoot->parent;
        }
        newRoot->parent->reviseHeights();

        // put newRoot as par of *this
        newRoot->left = this;
        newRoot->right = this->right;
        newRoot->parent = this->parent;
        if (this->parent) {
            if (this->parent->left == this) this->parent->left = newRoot;
            else this->parent->right = newRoot;
        }
        this->parent = newRoot;
        this->reviseHeights();

        return newRoot;
    }

    // rotateRight() rotates the sub-tree 1unit right
    // it takes care of updating height
    AVLTree* rotateRight() {
        if (this->left==NULL) return this;
        AVLTree *newRoot= this->left->largest();

        // pick out the newRoot
        // properties of newRoot: it has no right child.
        newRoot->parent->right = newRoot->left;
        if (newRoot->left) {
            newRoot->left->parent = newRoot->parent;
        }
        newRoot->parent->reviseHeights();

        // put newRoot as par of *this
        newRoot->right = this;
        newRoot->left = this->left;
        newRoot->parent = this->parent;
        if (this->parent) {
            if (this->parent->left == this) this->parent->left = newRoot;
            else this->parent->right = newRoot;
        }
        this->parent = newRoot;
        this->reviseHeights();

        return newRoot;
    }

    /*
        recomputes heights if it has changed till the root
        leaf is considered as height: 0
    */
    void reviseHeights() {
        int l=-1,r=-1;
        if(this->left!=NULL) l=this->left->height+1;
        if(this->right!=NULL) r=this->right->height+1;

        // if already same
        if(height == max(l,r)+1) return;
        height = max(l,r)+1;
        this->parent->reviseHeights();
    }

    void swapNodeData(AVLTree *treeNode) {
        if (this==NULL || treeNode==NULL) return;
        Node *swapNodeData = treeNode->node;
        treeNode->node = this->node;
        this->node = swapNodeData;
    }

public:
    AVLTree(Node *node= NULL, AVLTree *par=NULL): node(node), parent(par) {
        this->height = 0;
        left = right;
    }

    ~AVLTree() {
        delete node;
        delete left, right;
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
        if(n==NULL) return this;
        if(this->node < n) {
            if(this->left) this->left->insert(n);
            else this->left = new AVLTree(n, this);
        }else {
            if(this->right) this->right->insert(n);
            else this->right = new AVLTree(n, this);
        }
        return rebalance(this);
    }

    /*
        remove node from tree
        TODO: decide how to handle duplicates
    */
    AVLTree* remove(Node *n) {
        if(n==NULL) return this;

        if(this->node==n) {
            // leaf node
            if(!this->left && !this->right) {
                if (this->parent && this->parent->left==this) this->parent->left = NULL;
                else if (this->parent && this->parent->right==this) this->parent->right = NULL;
                delete this;
                return NULL;
            }

            AVLTree *toSwapNode = NULL;
            if (this->left) toSwapNode = this->left->largest();

            // left subtree is empty
            if (toSwapNode == NULL) {
                if (this->parent && this->parent->left==this) this->parent->left = this->right;
                else if (this->parent && this->parent->right==this) this->parent->right = this->right;
                this->right->parent = this->parent;

                AVLTree *newRoot = this->right;
                this->right = NULL;
                delete this;
                return newRoot; // no need to rebalance newRoot, as its already rebalanced
            }

            // swap current and largest of left-subtree, then continue the hunt in left subtree
            this->swapNodeData(toSwapNode);
            this->left->remove(n);
            return rebalance(this);
        }

        if(this->node < n) {
            this->left->remove(n);
        }else {
            this->right->remove(n);
        }
        return rebalance(this);
    }
};
