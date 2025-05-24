#include <iostream>
using namespace std;
/*
    Node is the interface for AVLTree's node's data
*/
class Node {
public:
    virtual ~Node() = default;

    virtual bool operator==(const Node &n) const =0;
    virtual bool operator>(const Node &n) const =0;
    virtual bool operator<(const Node &n) const =0;
    virtual string getData() const =0;
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
    string getData() const override {
        return to_string(val);
    }
};
