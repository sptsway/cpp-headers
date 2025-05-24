#include<iostream>
#include"node.h"
using namespace std;

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
        int l=-1,r=-1;
        if(root==NULL) return NULL;

        if(root->left!=NULL) l=root->left->height;
        if(root->right!=NULL) r=root->right->height;

        if(abs(l-r)<=1) return root;

        if(l<r) return root->rotateLeft();
        return root->rotateRight();
    }

    // rebalanceTillUp rebalances till tillRoot
    // no return statement as, tillRoot doesnt change
    void rebalanceTillUp(AVLTree *root, const AVLTree *tillRoot) {
        AVLTree *itr= root;
        while (itr != NULL && itr !=tillRoot) {
            AVLTree *parItr = itr->parent;
            rebalance(itr);
            itr = parItr;
        }
    }

    // rotateLeft() rotates the sub-tree 1unit left
    // it takes care of updating height
    AVLTree* rotateLeft() {
        if (this->right==NULL) return this;
        AVLTree *newRoot= this->right->smallest();

        // pick out the newRoot from the tree
        // properties of newRoot: it has no left child.
        if (newRoot->parent == this) this->right = newRoot->right;
        else newRoot->parent->left = newRoot->right;
        if (newRoot->right) {
            newRoot->right->parent = newRoot->parent;
        }
        newRoot->parent->reviseHeights();
        rebalanceTillUp(newRoot->parent, this);

        // put newRoot as par of *this
        newRoot->left = this;
        newRoot->right = this->right; this->right=NULL;
        if (newRoot->right) newRoot->right->parent = newRoot;
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

        // pick out the newRoot from the tree
        // properties of newRoot: it has no right child.
        if (newRoot == this->left) this->left = newRoot->left;
        else newRoot->parent->right = newRoot->left;
        if (newRoot->left) {
            newRoot->left->parent = newRoot->parent;
        }
        newRoot->parent->reviseHeights();
        rebalanceTillUp(newRoot->parent, this);

        // put newRoot as par of *this
        newRoot->right = this;
        newRoot->left = this->left; this->left=NULL;
        if (newRoot->left) newRoot->left->parent = newRoot;

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
        if(this->left!=NULL) l=this->left->height;
        if(this->right!=NULL) r=this->right->height;

        // if already same
        if(height == max(l,r)+1) return;
        height = max(l,r)+1;


        if (this->parent) this->parent->reviseHeights();
    }

    void swapNodeData(AVLTree *treeNode) {
        if (treeNode==NULL) return;
        Node *swapNodeData = treeNode->node;
        treeNode->node = this->node;
        this->node = swapNodeData;
    }

public:
    AVLTree(Node *node= NULL, AVLTree *par=NULL): node(node), parent(par) {
        this->height = 0;
        left = right= NULL;
    }

    ~AVLTree() {
        delete node;
        delete left, right;
    }

    bool contains(Node *n) {
        if(n==NULL) return false;

        if(*this->node == *n) return true;
        if(*this->node>*n) {
            if(this->left) return this->left->contains(n);
        }else {
            if(this->right) return this->right->contains(n);
        }
        return false;
    }

    /*
        inserts into tree
    */
    AVLTree* insert(Node *n) {
        if(n==NULL) return this;

        if(*this->node > *n) {
            if(this->left) this->left->insert(n);
            else this->left = new AVLTree(n, this);
        }else {
            if(this->right) this->right->insert(n);
            else this->right = new AVLTree(n, this);
        }
        this->reviseHeights();
        return rebalance(this);
    }

    /*
        remove node from tree
        TODO: decide how to handle duplicates
        currently deletes only a single instance
    */
    AVLTree* remove(Node *n) {
        if(n==NULL) return this;

        if(*this->node==*n) {
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

        if(*this->node < *n) {
            this->right->remove(n);
        }else {
            this->left->remove(n);
        }
        return rebalance(this);
    }

    void printTreeNode() {
        string leftOut="null", rightOut="null", parOut="null";
        if (this->left) leftOut= this->left->node->getData();
        if (this->right) rightOut= this->right->node->getData();
        if (this->parent) parOut= this->parent->node->getData();
        cout<<"Node: "<<this->node->getData()<<"; h:"<<height<<", l:"<<leftOut<<", r:"<<rightOut<<", par:"<<parOut<<"\n";
    }

    void printTree() {
        printTreeNode();
        if (this->left) this->left->printTree();
        if (this->right) this->right->printTree();
    }
};
