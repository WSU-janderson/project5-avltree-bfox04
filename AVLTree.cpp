#include "AVLTree.h"

#include <string>
#include <algorithm>

using namespace std;

size_t AVLTree::AVLNode::numChildren() const {
    size_t numOfKids = 0;
    if (left) numOfKids++;
    if (right) numOfKids++;
    return numOfKids;
}

bool AVLTree::AVLNode::isLeaf() const {
    return numChildren() == 0;
}

size_t AVLTree::AVLNode::getHeight() const {
    return height;
}

void AVLTree::updateHeight(AVLNode* node) {
    if (!node) return;

    size_t leftHeight = node->left ? node->left->getHeight() : 0;
    size_t rightHeight = node->right ? node->right->getHeight() : 0;

    node->height = 1 + max(leftHeight, rightHeight);
}

int AVLTree::getBalanceFactor(AVLNode* node) const {
    if (!node) return 0;

    int leftHeight = node->left ? static_cast<int>(node->left->getHeight()) : 0;
    int rightHeight = node->right ? static_cast<int>(node->right->getHeight()) : 0;

    return leftHeight - rightHeight;
}

bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();
    if (current->isLeaf()) {
        // case 1 we can delete the node
        current = nullptr;
    } else if (current->numChildren() == 1) {
        // case 2 - replace current with its only child
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        // case 3 - we have two children,
        // get smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode* smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;

        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    return false;
}

void AVLTree::rotateLeft(AVLNode*& node) {
    AVLNode* tempRoot = node->right;

    node->right = tempRoot->left;

    tempRoot->left = node;

    updateHeight(node);
    updateHeight(tempRoot);

    node = tempRoot;
}

void AVLTree::rotateRight(AVLNode*& node) {
    AVLNode* tempRoot = node->left;

    node->left = tempRoot->right;

    tempRoot->right = node;

    updateHeight(node);
    updateHeight(tempRoot);

    node = tempRoot;
}

void AVLTree::balanceNode(AVLNode*& node) {
    if (!node) return;

    updateHeight(node);

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        if (getBalanceFactor(node->left) < 0) {
            rotateLeft(node->left);
        }
        rotateRight(node);
    }
    else if (balanceFactor < -1) {
        if (getBalanceFactor(node->right) > 0) {
            rotateRight(node->right);
        }
        rotateLeft(node);
    }
}
