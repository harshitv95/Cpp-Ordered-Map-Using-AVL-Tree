#include <iostream>
#include <functional>
#include <queue>

using namespace std;

#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

template<typename Data_T>
class BST {
public:
    /***** Function Members *****/
    BST();

    BST(std::function<Data_T()> default_initializer);

    BST(const BST<Data_T> &copyFrom);

    bool empty() const;

    Data_T *search(const Data_T &item) const;

    void insert(const Data_T &item);

    int nodeCount() const;

    void preOrder() const;

    void inOrder() const;

    void postOrder() const;

    void deleteNode(Data_T value);

    /***** Others *****/
    typedef enum {
        PRE_ORDER, IN_ORDER, POST_ORDER
    } traversal_order;
    typedef enum {
        LEAF_NODE, ONE_CHILD, TWO_CHILDREN
    } delete_mode;

protected:
    std::function<Data_T()> default_init;

    Data_T *search(const Data_T &item, Data_T *&parent) const;


    /***** Node class *****/
    class BinNode {
    protected:
        Data_T data;

        virtual void cloneFrom(const BinNode *tree);

    public:
        BinNode *left;
        BinNode *right;

        // BinNode constructors
        // Default -- data part is default int value; both links are null.
//        BinNode()
//                : left(0), right(0) {}

        // Explicit Value -- data part contains item; both links are null.
        BinNode(Data_T item)
                : data(item), left(nullptr), right(nullptr) {}

        BinNode(const BinNode &node) : data(node.data), left(nullptr), right(nullptr) {}

        virtual void print();

        Data_T &getData() {
            return this->data;
        }

        static void deleteSubTree(BinNode *node);

    };// end of class BinNode declaration

//    class DataBinNode : public BinNode {
//    private:
//        Data_T data;
//    public:
//        DataBinNode(Data_T data_in) : BinNode(true), data(data_in) {}
//    };

    /***** Data Members *****/
    BinNode *myRoot;

    /***** Protected Function Members *****/
    virtual void postInsert(BinNode *node, BinNode *parentNode);

    virtual void postDelete(Data_T deletedData, BinNode *parentNode);

//    virtual BinNode *initNode();

    virtual BinNode *initNode(const Data_T &data);

    virtual BinNode *initNode(const BinNode &data);

    BinNode *smallest(BinNode *rootNode, BinNode *&parentNode, int &status);

    BinNode *largest(BinNode *rootNode, BinNode *&parentNode, int &status);

    BinNode *smallest(BinNode *rootNode);

    BinNode *largest(BinNode *rootNode);

    virtual void cloneFrom(const BST<Data_T> *tree);

    virtual BinNode *cloneFrom(const BinNode *node);

private:
    /***** Private Function Members *****/
    int nodeCount(BinNode *node) const;

    void traverse(BinNode *node, traversal_order order) const;

    BinNode *searchNode(BinNode *startNode, const Data_T &data, BinNode *&parentNode);

    void deleteNode(BinNode *node, BinNode *parentNode, delete_mode mode);

}; // end of class declaration

#endif


//--- Definition of constructors
template<typename Data_T>
BST<Data_T>::BST()
        : myRoot(0) {}

template<typename Data_T>
BST<Data_T>::BST(function<Data_T()> default_init)
        : myRoot(0), default_init(default_init) {}

template<typename Data_T>
BST<Data_T>::BST(const BST<Data_T> &copyFrom) {
    cloneFrom(copyFrom);
}

template<typename Data_T>
void BST<Data_T>::cloneFrom(const BST<Data_T> *tree) {
    BinNode::deleteSubTree(this->myRoot);
    this->myRoot = cloneFrom(tree->myRoot);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::cloneFrom(const BinNode *node) {
    if (!node) return nullptr;
    BinNode *retNode = initNode(*node);
    retNode->left = cloneFrom(node->left);
    retNode->right = cloneFrom(node->right);
    return retNode;
}

template<typename Data_T>
bool BST<Data_T>::empty() const { return !myRoot; }

template<typename Data_T>
Data_T *BST<Data_T>::search(const Data_T &item) const {
    BinNode *locptr = myRoot;
    while (locptr != 0) {
        if (item < locptr->getData())       // descend left
            locptr = locptr->left;
        else if (locptr->getData() < item)  // descend right
            locptr = locptr->right;
        else if (locptr->getData() == item) // item found
            return &(locptr->getData());
    }
    return nullptr;
}

template<typename Data_T>
Data_T *BST<Data_T>::search(const Data_T &item, Data_T *&parent) const {
    BinNode *locptr = myRoot;   // search pointer
//    BinNode *parent = 0;        // pointer to parent of current node
    while (locptr != 0) {
        parent = locptr;
        if (item < locptr->getData()) {      // descend left
            parent = locptr;
            locptr = locptr->left;
        } else if (locptr->getData() < item) {  // descend right
            parent = locptr;
            locptr = locptr->right;
        } else                           // item found
            return &(locptr->getData());
    }
    return nullptr;
}

template<typename Data_T>
void BST<Data_T>::insert(const Data_T &item) {
    BinNode *parent;        // pointer to parent of current node
    BinNode *locptr = this->searchNode(myRoot, item, parent);   // search pointer

    if (!locptr) {                       // construct node containing item
        locptr = this->initNode(item);
        if (!parent) {             // empty tree
            myRoot = locptr;
            this->postInsert(myRoot, nullptr);
        } else if (item < parent->getData()) { // insert to left of parent
            parent->left = locptr;
            this->postInsert(parent->left, parent);
        } else {                          // insert to right of parent
            parent->right = locptr;
            this->postInsert(parent->right, parent);
        }
    } else
        locptr->getData() = item;
//    else
//        cout << "Item already in the tree\n";
}

// Public methods to be called from user program
template<typename Data_T>
int BST<Data_T>::nodeCount() const {
    return this->nodeCount(this->myRoot);
}

template<typename Data_T>
void BST<Data_T>::preOrder() const {
//    cout << "PreOrder Traversal:" << endl;
    this->traverse(this->myRoot, PRE_ORDER);
}

template<typename Data_T>
void BST<Data_T>::inOrder() const {
//    cout << "InOrder Traversal:" << endl;
    this->traverse(this->myRoot, IN_ORDER);
}

template<typename Data_T>
void BST<Data_T>::postOrder() const {
//    cout << "PostOrder Traversal:" << endl;
    this->traverse(this->myRoot, POST_ORDER);
}

template<typename Data_T>
void BST<Data_T>::deleteNode(Data_T value) {
    BinNode *searchNode, *parentNode;
    parentNode = NULL;
    delete_mode mode = LEAF_NODE;
    searchNode = this->searchNode(this->myRoot, value, parentNode);
    if (searchNode->left && searchNode->right)
        mode = TWO_CHILDREN;
    else if (searchNode->left || searchNode->right)
        mode = ONE_CHILD;
    this->deleteNode(searchNode, parentNode, mode);
//    cout << "Deleted " << value << " successfully, data in tree:" << endl;
    this->traverse(this->myRoot, IN_ORDER);
}


// Private methods
template<typename Data_T>
int BST<Data_T>::nodeCount(BST<Data_T>::BinNode *node) const {
    if (node == NULL || node == 0)
        return 0;
    return 1 + this->nodeCount(node->left) + this->nodeCount(node->right);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *
BST<Data_T>::searchNode(BST<Data_T>::BinNode *startNode, const Data_T &data, BST<Data_T>::BinNode *&parentNode) {
//    if (startNode == NULL || startNode == 0) return nullptr;
    parentNode = nullptr;
    while (startNode) {
        if (startNode->getData() == data)
            return startNode;
        else if (data < startNode->getData()) {
            parentNode = startNode;
            startNode = startNode->left;
        } else {
            parentNode = startNode;
            startNode = startNode->right;
        }
    }
    return nullptr;
}

template<typename Data_T>
void BST<Data_T>::traverse(BST<Data_T>::BinNode *node, traversal_order order) const {
    if (node == NULL || node == 0)
        return;
    BinNode *left = node->left;
    BinNode *right = node->right;
    switch (order) {
        case PRE_ORDER:
            node->print();
            traverse(left, order);
            traverse(right, order);
            break;
        case IN_ORDER:
            traverse(left, order);
            node->print();
            traverse(right, order);
            break;
        case POST_ORDER:
            traverse(left, order);
            traverse(right, order);
            node->print();
            break;
    }
}

template<typename Data_T>
void
BST<Data_T>::deleteNode(BST<Data_T>::BinNode *node, BST<Data_T>::BinNode *parentNode, BST<Data_T>::delete_mode mode) {
    bool isRoot = (node == this->myRoot);
    Data_T data = node->getData();
//    if (isRoot)
//        cout << "Deleting Root element : " << node->getData() << endl;
//    else
//        cout << "Deleting element [" << node->getData() << "] with parent [" << parentNode->getData() << "]" << endl;
    switch (mode) {
        case LEAF_NODE: {
//            cout << "Deleting Leaf Node " << node->getData() << endl;
            if (isRoot)
                this->myRoot = NULL;
            else {
                if (parentNode->left == node) {
//                    cout << "Deleting left child of parent: " << parentNode->getData() << endl;
                    parentNode->left = NULL;
                } else if (parentNode->right == node) {
//                    cout << "Deleting right child of parent: " << parentNode->getData() << endl;
                    parentNode->right = NULL;
                }
            }
            break;
        }
        case ONE_CHILD: {
            BinNode *child = node->left ? node->left : node->right;
            if (isRoot)
                this->myRoot = child;
            else {
                if (parentNode->left == node)
                    parentNode->left = child;
                else if (parentNode->right == node)
                    parentNode->right = child;
            }
            break;
        }
        case TWO_CHILDREN: {
            int status = 0;
            BinNode *parentOfSmallestNode, *smallestNode;
            smallestNode = smallest(node->right, parentOfSmallestNode, status);
            if (parentOfSmallestNode != NULL && status == 1)
                parentOfSmallestNode->left = smallestNode->right;
            else
                node->right = smallestNode->right;

//            cout << "Found smallest : [" << smallestNode->getData() << "] with parent : [" << parentOfSmallestNode->getData() << "]";

            smallestNode->left = node->left;
            smallestNode->right = node->right;

            if (isRoot) {
                this->myRoot = smallestNode;
            } else {
                if (parentNode->left == node)
                    parentNode->left = smallestNode;
                else if (parentNode->right == node)
                    parentNode->right = smallestNode;
            }
            break;
        }
    }
    delete node;
    this->postDelete(data, parentNode);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::smallest(BinNode *rootNode, BinNode *&parentNode, int &status) {
    if (rootNode->left == NULL) return rootNode;
    parentNode = rootNode;
    status = 1;
    return smallest(rootNode->left, parentNode, status);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::largest(BinNode *rootNode, BinNode *&parentNode, int &status) {
    if (rootNode->right == NULL) return rootNode;
    parentNode = rootNode;
    status = 1;
    return largest(rootNode->right, parentNode, status);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::smallest(BinNode *rootNode) {
    if (rootNode->left == NULL) return rootNode;
    return smallest(rootNode->left);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::largest(BinNode *rootNode) {
    if (rootNode->right == NULL) return rootNode;
    return largest(rootNode->right);
}

template<typename Data_T>
void BST<Data_T>::postInsert(BST<Data_T>::BinNode *node, BST<Data_T>::BinNode *parentNode) {}

template<typename Data_T>
void BST<Data_T>::postDelete(Data_T data, BST<Data_T>::BinNode *parentNode) {}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::initNode(const Data_T &data) {
    return new BinNode(data);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::initNode(const BinNode &data) {
    return new BinNode(data);
}

// BinNode impl:
template<typename Data_T>
void BST<Data_T>::BinNode::print() {
    cout << this->getData() << endl;
}

template<typename Data_T>
void BST<Data_T>::BinNode::cloneFrom(const BinNode *node) {
    this->data = node->data;
    this->left->cloneFrom(node->left);
    this->right->cloneFrom(node->right);
}

template<typename Data_T>
void BST<Data_T>::BinNode::deleteSubTree(BinNode *node) {
    queue < BinNode * * > q;
    q.push(&node);
    while (!q.empty()) {
        if ((*q.front())->left)
            q.push(&(*q.front())->left);
        if ((*q.front())->right)
            q.push(&(*q.front())->right);
        delete *(q.front());
        *(q.front()) = nullptr;
        q.pop();
    }
}