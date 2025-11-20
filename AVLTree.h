/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <string>
#include <optional>
#include <vector>
#include <iostream>

using namespace std;

class AVLTree {
public:
    using KeyType = string;
    using ValueType = size_t;

    // Constructor
    AVLTree();

    // Big 3: Copy Constructor, Assignment Operator, Destructor
    AVLTree(const AVLTree& other);
    AVLTree& operator=(const AVLTree& other);
    ~AVLTree();

    // Core dictionary operations
    bool insert(const KeyType& key, ValueType value);
    bool remove(const KeyType& key);
    bool contains(const KeyType& key) const;
    optional<ValueType> get(const KeyType& key) const;
    ValueType& operator[](const KeyType& key);

    // Range and traversal operations
    vector<ValueType> findRange(const KeyType& lowKey, const KeyType& highKey) const;
    vector<KeyType> keys() const;

    // Tree properties
    size_t size() const;
    size_t getHeight() const;

    // Output stream overload
    friend ostream& operator<<(ostream& os, const AVLTree& tree);

protected:
    // Internal Node structure
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;

        size_t numChildren() const;
        bool isLeaf() const;
        size_t getHeight() const;

        AVLNode(KeyType k, ValueType v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}

    };

    private:
    AVLNode* root;

    size_t numElements;

    // recursive helpers for modification
    bool remove(AVLNode*& current, KeyType key);
    bool removeNode(AVLNode*& current);
    void balanceNode(AVLNode*& node);

    // recursive helpers for search/insert
    bool insert(AVLNode*& node, const KeyType& key, ValueType value);
    bool contains(AVLNode* node, const KeyType& key) const;
    AVLNode* getNode(AVLNode* node, const KeyType& key) const;

    // helpers for traversal
    void findRange(AVLNode* node, const KeyType& low, const KeyType& high, vector<ValueType>& result) const;
    void getAllKeys(AVLNode* node, vector<KeyType>& result) const;

    void printTree(ostream& os, AVLNode* node, int depth) const;

    // memory management helpers
    AVLNode* copyTree(AVLNode* node);
    void destroyTree(AVLNode* node);

    // rotation and height helpers
    void rotateLeft(AVLNode*& node);
    void rotateRight(AVLNode*& node);
    void updateHeight(AVLNode* node);
    int getBalanceFactor(AVLNode* node) const;


};

#endif //AVLTREE_H