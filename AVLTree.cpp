/*
 * CS3100 - Project 5
 * November 19, 2025
 * Bryce Fox
 */


#include "AVLTree.h"

#include <string>
#include <algorithm>

using namespace std;

AVLTree::AVLTree() : root(nullptr), numElements(0) {}

AVLTree::~AVLTree() {
    destroyTree(root);
}

void AVLTree::destroyTree(AVLNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

AVLTree::AVLNode* AVLTree::copyTree(AVLNode* node) {
    if (node == nullptr) {
        return nullptr;
    }

    AVLNode* newNode = new AVLNode(node->key, node->value);
    newNode->height = node->height;

    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);

    return newNode;
}

AVLTree::AVLTree(const AVLTree& other) : root(nullptr), numElements(0) {
    root = copyTree(other.root);
    numElements = other.numElements;
}

AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this != &other) {
        destroyTree(root);

        root = copyTree(other.root);
        numElements = other.numElements;
    }
    return *this;
}

size_t AVLTree::size() const {
    return numElements;
}

size_t AVLTree::getHeight() const {
    return root ? root->getHeight() : 0;
}

std::ostream& operator<<(std::ostream& os, const AVLTree& tree) {
    tree.printTree(os, tree.root, 0);
    return os;
}

void AVLTree::printTree(std::ostream& os, AVLNode* node, int depth) const {
    if (node) {
        printTree(os, node->right, depth + 1);

        for (int i = 0; i < depth; ++i) {
            os << "    ";
        }
        os << "{" << node->key << ": " << node->value << "}" << std::endl;

        printTree(os, node->left, depth + 1);
    }
}

bool AVLTree::insert(const KeyType& key, ValueType value) {
    return insert(root, key, value);
}

bool AVLTree::insert(AVLNode*& node, const KeyType& key, ValueType value) {
    if (node == nullptr) {
        node = new AVLNode(key, value);
        numElements++;
        return true;
    }

    if (key == node->key) {
        return false;
    }

    bool success = false;
    if (key < node->key) {
        success = insert(node->left, key, value);
    } else {
        success = insert(node->right, key, value);
    }

    if (success) {
        balanceNode(node);
    }

    return success;
}

AVLTree::AVLNode* AVLTree::getNode(AVLNode* node, const KeyType& key) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (key == node->key) {
        return node;
    }

    if (key < node->key) {
        return getNode(node->left, key);
    } else {
        return getNode(node->right, key);
    }
}

bool AVLTree::contains(const KeyType& key) const {
    return contains(root, key);
}

bool AVLTree::contains(AVLNode* node, const KeyType& key) const {
    if (node == nullptr) return false;
    if (key == node->key) return true;

    if (key < node->key) {
        return contains(node->left, key);
    } else {
        return contains(node->right, key);
    }
}

std::optional<AVLTree::ValueType> AVLTree::get(const KeyType& key) const {
    AVLNode* node = getNode(root, key);
    if (node) {
        return node->value;
    }
    return std::nullopt;
}

AVLTree::ValueType& AVLTree::operator[](const KeyType& key) {
    AVLNode* node = getNode(root, key);
    return node->value;
}

std::vector<AVLTree::ValueType> AVLTree::findRange(const KeyType& lowKey, const KeyType& highKey) const {
    std::vector<ValueType> result;
    findRange(root, lowKey, highKey, result);
    return result;
}

void AVLTree::findRange(AVLNode* node, const KeyType& low, const KeyType& high, std::vector<ValueType>& result) const {
    if (node == nullptr) {
        return;
    }

    if (node->key > low) {
        findRange(node->left, low, high, result);
    }

    if (node->key >= low && node->key <= high) {
        result.push_back(node->value);
    }

    if (node->key < high) {
        findRange(node->right, low, high, result);
    }
}

std::vector<AVLTree::KeyType> AVLTree::keys() const {
    std::vector<KeyType> result;
    result.reserve(numElements);
    getAllKeys(root, result);
    return result;
}

void AVLTree::getAllKeys(AVLNode* node, std::vector<KeyType>& result) const {
    if (node == nullptr) {
        return;
    }
    getAllKeys(node->left, result);
    result.push_back(node->key);
    getAllKeys(node->right, result);
}

bool AVLTree::remove(const KeyType& key) {
    return remove(root, key);
}

bool AVLTree::remove(AVLNode*& current, KeyType key) {
    if (current == nullptr) {
        return false;
    }

    if (key < current->key) {
        bool result = remove(current->left, key);
        if (result) balanceNode(current);
        return result;
    }
    else if (key > current->key) {
        bool result = remove(current->right, key);
        if (result) balanceNode(current);
        return result;
    }
    else {
        bool success = removeNode(current);
        if (success) {
            numElements--;
            if (current) {
                balanceNode(current);
            }
        }
        return success;
    }
}

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
    size_t nChildren = current->numChildren();
    if (current->isLeaf()) {
        current = nullptr;
    } else if (current->numChildren() == 1) {
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        AVLNode* smallestInRight = current->right;
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;

        remove(root, smallestInRight->key);

        current->key = newKey;
        current->value = newValue;

        numElements++;

        current->height = current->getHeight();
        balanceNode(current);

        return true;
    }
    delete toDelete;

    return true;
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
