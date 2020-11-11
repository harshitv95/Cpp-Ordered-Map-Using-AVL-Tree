#include <iostream>
#include <functional>

using namespace std;

#ifndef AVL_TREE
#define AVL_TREE

#define IS_BALANCED(node) ((node->balance < 2) && (node->balance > -2))
#define BALANCE_TYPE(node) (node->balance > 0 ? LEFT_HEAVY : (node->balance < 0 ? RIGHT_HEAVY : BALANCED))

#include "BST.hpp"

template<typename Data_T>
class AVL : public BST<Data_T> {
public:
    /***** Function Members *****/
    AVL();

    AVL(std::function<Data_T()> default_initializer);

    /***** Others *****/
    typedef enum {
        ROOT_NODE, LEFT_NODE, RIGHT_NODE
    } child_type;
    typedef enum {
        LEFT_ROTATE, RIGHT_ROTATE, LEFT_RIGHT_ROTATE, RIGHT_LEFT_ROTATE
    } rotation_type;
    typedef enum {
        LEFT_HEAVY, RIGHT_HEAVY, BALANCED
    } balance_type;

protected:
    class AVLNode : public BST<Data_T>::BinNode {
    public:
        int height, balance;
        AVLNode *parent;
        child_type childType;

//        AVLNode() : BST::BinNode(), height(0), balance(0) {}

        AVLNode(const Data_T &data) : BST<Data_T>::BinNode(data), height(0), balance(0) {}

        AVLNode(const typename BST<Data_T>::BinNode &node) : BST<Data_T>::BinNode(node), height(0), balance(0) {}

        void print();
    };

    int calcHeight(AVLNode *node);

    int calcBalance(AVLNode *node);

    AVLNode *findUnbalanced(AVLNode *rootNode);

    void postInsert(const typename BST<Data_T>::BinNode *, const typename BST<Data_T>::BinNode *);

    void postDelete(const Data_T &data, const typename BST<Data_T>::BinNode *parentNode);

    AVLNode *initNode(const Data_T &data);

    AVLNode *initNode(const typename BST<Data_T>::BinNode &data);

private:
    /***** Private Function Members *****/

    void balance(AVLNode *node);

    void rotate(AVLNode *rotateNode, rotation_type rotationType);

}; // end of class declaration

#endif

//--- Definition of constructor
template<typename Data_T>
AVL<Data_T>::AVL() : BST<Data_T>() {}

template<typename Data_T>
AVL<Data_T>::AVL(std::function<Data_T()> default_initializer) : BST<Data_T>(default_initializer) {}

// Private methods
template<typename Data_T>
void AVL<Data_T>::AVLNode::print() {
//    cout << this->data
//         << " (height: " << this->height
//         << ", balance: " << this->balance
//         << ", " << (
//                 this->childType == ROOT_NODE ? "ROOT" : (
//                         this->childType == LEFT_NODE ? "LEFT" : "RIGHT"
//                 ))
//         << " child of: " << (!this->parent ? -999 : this->parent->data)
//         << ")" << endl;
}

template<typename Data_T>
void AVL<Data_T>::postInsert(const typename BST<Data_T>::BinNode *node, const typename BST<Data_T>::BinNode *parentNode) {
    AVLNode *avlNode = ((AVLNode *) node);
    avlNode->parent = ((AVLNode *) parentNode);

    if (!parentNode)
        avlNode->childType = ROOT_NODE;
    else if (parentNode->left == node)
        avlNode->childType = LEFT_NODE;
    else if (parentNode->right == node)
        avlNode->childType = RIGHT_NODE;

    this->calcHeight((AVLNode *) this->myRoot);
    if (avlNode->parent && avlNode->parent->parent)
        this->balance(avlNode);
}

//void AVL::postDelete(BinNode *parentNode) {
//    this->calcHeight((AVLNode*) this->myRoot);
//    if (!parentNode) return;
//
//    AVLNode *parentAVLNode = (AVLNode*) parentNode;
//    while (parentAVLNode && IS_BALANCED(parentAVLNode)) {
//        switch (BALANCE_TYPE(parentAVLNode)) {
//            case BALANCED:
//                return;
//            case RIGHT_HEAVY:
//                parentAVLNode = (AVLNode*) parentAVLNode->right;
//                break;
//            case LEFT_HEAVY:
//                parentAVLNode = (AVLNode*) parentAVLNode->left;
//                break;
//        }
//    }
//
//    if (!parentAVLNode) return;
//    switch (BALANCE_TYPE(parentAVLNode)) {
//        case BALANCED:
//        case LEFT_HEAVY:
//            parentAVLNode = (AVLNode*) (parentAVLNode->left);
//            break;
//        case RIGHT_HEAVY:
//            parentAVLNode = (AVLNode*) (parentAVLNode->right);
//            break;
//    }
//    if (parentAVLNode->left)
//        parentAVLNode = (AVLNode*) parentAVLNode->left;
//    else if (parentAVLNode->right)
//        parentAVLNode = (AVLNode*) parentAVLNode->right;
//
//    this->balance(parentAVLNode);
//}

template<typename Data_T>
void AVL<Data_T>::postDelete(const Data_T &data, const typename BST<Data_T>::BinNode *parentNode) {
    this->calcHeight((AVLNode *) this->myRoot);
    if (!parentNode)
        return;
    AVLNode *unbalanced;
    while ((unbalanced = this->findUnbalanced((AVLNode *) this->myRoot))) {
        switch (BALANCE_TYPE(unbalanced)) {
            case BALANCED:
            case LEFT_HEAVY:
                unbalanced = (AVLNode *) unbalanced->left;
                break;
            case RIGHT_HEAVY:
                unbalanced = (AVLNode *) unbalanced->right;
                break;
        }
        if (unbalanced->left)
            unbalanced = (AVLNode *) unbalanced->left;
        else if (unbalanced->right)
            unbalanced = (AVLNode *) unbalanced->right;

        this->balance(unbalanced);
    }
}

template<typename Data_T>
typename AVL<Data_T>::AVLNode *AVL<Data_T>::findUnbalanced(AVL<Data_T>::AVLNode *rootNode) {
    if (!rootNode || !IS_BALANCED(rootNode))
        return rootNode;
    AVLNode *unbalanced;
    unbalanced = findUnbalanced((AVLNode *) rootNode->left);
    if (unbalanced)
        return unbalanced;
    unbalanced = findUnbalanced((AVLNode *) rootNode->right);
    return unbalanced;
}

template<typename Data_T>
typename AVL<Data_T>::AVLNode *AVL<Data_T>::initNode(const Data_T &data) {
    AVLNode *node = new AVLNode(data);
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
    return node;
}

template<typename Data_T>
typename AVL<Data_T>::AVLNode *AVL<Data_T>::initNode(const typename BST<Data_T>::BinNode &data) {
    AVLNode *node = new AVLNode(data);
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
    return node;
}

static int max(int a, int b) {
    return a > b ? a : b;
}

template<typename Data_T>
int AVL<Data_T>::calcBalance(AVLNode *node) {
    if (node->left && node->right)
        node->balance =
                ((AVLNode *) node->left)->height -
                ((AVLNode *) node->right)->height;
    else if (!node->left && !node->right)
        node->balance = 0;
    else if (!node->left)
        node->balance = -1 - ((AVLNode *) node->right)->height;
    else
        node->balance = ((AVLNode *) node->left)->height + 1;
    return node->balance;
}

template<typename Data_T>
int AVL<Data_T>::calcHeight(AVLNode *node) {
    if (node == NULL) return -1;
    ((AVLNode *) node)->parent = NULL;
    ((AVLNode *) node)->childType = ROOT_NODE;

    int leftHeight = calcHeight((AVLNode *) node->left);
    int rightHeight = calcHeight((AVLNode *) node->right);

    if (node->left) {
        ((AVLNode *) node->left)->parent = node;
        ((AVLNode *) node->left)->childType = LEFT_NODE;
    }
    if (node->right) {
        ((AVLNode *) node->right)->parent = node;
        ((AVLNode *) node->right)->childType = RIGHT_NODE;
    }

    node->height = 1 + max(leftHeight, rightHeight);
    calcBalance(node);
    return node->height;
}

template<typename Data_T>
void AVL<Data_T>::balance(AVLNode *node) {
    rotation_type rotationType;

    AVLNode *balanceNode = node, *parent, *grandParent, *rotateNode;
    while (
            balanceNode &&
            (parent = balanceNode->parent) &&
            (grandParent = balanceNode->parent->parent)
            ) {
        if (!IS_BALANCED(grandParent)) {
            if (parent->childType == LEFT_NODE) {
                if (balanceNode->childType == LEFT_NODE) {
                    rotationType = RIGHT_ROTATE;
                    rotateNode = parent;
                } else if (balanceNode->childType == RIGHT_NODE) {
                    rotationType = LEFT_RIGHT_ROTATE;
                    rotateNode = balanceNode;
                }
            } else if (parent->childType == RIGHT_NODE) {
                if (balanceNode->childType == LEFT_NODE) {
                    rotationType = RIGHT_LEFT_ROTATE;
                    rotateNode = balanceNode;
                } else if (balanceNode->childType == RIGHT_NODE) {
                    rotationType = LEFT_ROTATE;
                    rotateNode = parent;
                }
            }
//            cout << "Rotating [" << rotateNode->data << "]" << endl;
            this->rotate(rotateNode, rotationType);
            this->calcHeight((AVLNode *) this->myRoot);
            return;
        } else
            balanceNode = balanceNode->parent;
    }
}

template<typename Data_T>
void AVL<Data_T>::rotate(AVLNode *rotateNode, rotation_type rotationType) {
//    AVLNode *grandParent = rotateNode->parent->parent;
    if (!rotateNode || !rotateNode->parent || rotateNode->childType == ROOT_NODE)
        return;
    bool isRoot;
    AVLNode *parentOfRotated = NULL, *rotated;
    rotated = rotateNode->parent;
    isRoot = rotated->childType == ROOT_NODE;
    parentOfRotated = rotated->parent;

    switch (rotationType) {
        case LEFT_ROTATE:
            rotateNode->parent->right = rotateNode->left;
            rotateNode->left = rotateNode->parent;
            break;
        case RIGHT_ROTATE:
            rotateNode->parent->left = rotateNode->right;
            rotateNode->right = rotateNode->parent;
            break;
        case LEFT_RIGHT_ROTATE: {
            this->rotate(rotateNode, LEFT_ROTATE);
            this->rotate(rotateNode, RIGHT_ROTATE);
            return;
        }
        case RIGHT_LEFT_ROTATE: {
            this->rotate(rotateNode, RIGHT_ROTATE);
            this->rotate(rotateNode, LEFT_ROTATE);
            return;
        }
    }

    if (isRoot || parentOfRotated == NULL) // Is parent Root
        this->myRoot = rotateNode;
    else {
        if (rotated->childType == LEFT_NODE) {
            parentOfRotated->left = rotateNode;
            rotateNode->parent = parentOfRotated;
        } else if (rotated->childType == RIGHT_NODE) {
            parentOfRotated->right = rotateNode;
            rotateNode->parent = parentOfRotated;
        }
    }
}