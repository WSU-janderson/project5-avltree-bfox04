/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
// #include <string>
// need also:
// vector
// optional
// iostream

using namespace std;

class AVLTree {
public:
    using KeyType = string;
    using ValueType = size_t;

    // default
    AVLTree();

    AVLTree(const AVLTree& other); // copy
    AVLTree& operator=(const AVLTree& other); // assignment
    ~AVLTree(); // destructor

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;

        // 0, 1 or 2
        size_t numChildren() const;
        // true or false
        bool isLeaf() const;
        // number of hops to deepest leaf node
        size_t getHeight() const;


    };

public:




    private:
    AVLNode* root;

    /* Helper methods for remove */
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);
    // removeNode contains the logic for actually removing a node based on the numebr of children
    bool removeNode(AVLNode*& current);
    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);

};

#endif //AVLTREE_H
