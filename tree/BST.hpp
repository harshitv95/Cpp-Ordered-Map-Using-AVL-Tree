#include <iostream>
#include <functional>
#include <queue>
#include <stack>

#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

template<typename Data_T>
class BST {
public:
    /***** Function Members *****/
    BST() : BST(true) {}

    BST(bool updateIfExists);

//    BST(const std::function<Data_T()> &default_initializer);

    BST(const BST<Data_T> &copyFrom);

    BST(const BST<Data_T> *copyFrom);

    bool empty() const;

    Data_T *search(const Data_T &item) const;

    template<typename DataSearch_T>
    Data_T *
    search(const DataSearch_T &item, const std::function<short int(const DataSearch_T &, Data_T &)> &comp) const;

    void insert(const Data_T &item);

    int nodeCount() const;

    void preOrder() const;

    void inOrder() const;

    void postOrder() const;

    void deleteNode(const Data_T &value);

    void clear();

    template<typename DataSearch_T>
    void deleteNode(const DataSearch_T &item, const std::function<short int(const DataSearch_T &, Data_T &)> &comp);

    /***** Others *****/
    typedef enum {
        PRE_ORDER, IN_ORDER, POST_ORDER
    } traversal_order;
    typedef enum {
        LEAF_NODE, ONE_CHILD, TWO_CHILDREN
    } delete_mode;

    BST<Data_T> &operator=(const BST<Data_T> &tree) {
        return BST<Data_T>(tree);
    }

protected:
    const bool updateIfExists;
    std::function<Data_T()> default_init;

    /***** Node class *****/
    class BinNode {
    protected:
        Data_T data;

        virtual void cloneFrom(const BinNode *tree);

    public:
        BinNode *left;
        BinNode *right;

        // Explicit Value -- data part contains item; both links are null.
        BinNode(const Data_T &item)
                : data(item), left(nullptr), right(nullptr) {}

        BinNode(const BinNode &node) : data(node.data), left(nullptr), right(nullptr) {}

        virtual ~BinNode() {
//            delete this->data;
        }

        virtual void print();

        Data_T &getData() {
            return this->data;
        }

        static void deleteSubTree(BinNode *&node);

    };// end of class BinNode declaration

    /***** Data Members *****/
    BinNode *myRoot = nullptr, *smallestNode = nullptr, *largestNode = nullptr;

    /***** Protected Function Members *****/
    BinNode *searchNode(BinNode *startNode, const Data_T &data, BinNode *&parentNode);

    template<typename DataSearch_T>
    BinNode *searchNode(BinNode *startNode, const DataSearch_T &data,
                        const std::function<short int(const DataSearch_T &, Data_T &)> &comp,
                        BinNode *&parentNode) const;

    virtual void postInsert(const BinNode *node, const BinNode *parentNode);

    virtual void postDelete(const Data_T &deletedData, const BinNode *parentNode);

    virtual BinNode *initNode(const Data_T &data);

    virtual BinNode *initNode(const BinNode &data);

    static BinNode *smallest(BinNode *rootNode, BinNode *&parentNode, int &status);

    static BinNode *largest(BinNode *rootNode, BinNode *&parentNode, int &status);

    static BinNode *smallest(BinNode *rootNode);

    static BinNode *largest(BinNode *rootNode);

    virtual void cloneFrom(const BST<Data_T> *tree);

    virtual BinNode *cloneFrom(const BinNode *node);

private:
    /***** Private Function Members *****/
    int nodeCount(BinNode *node) const;

    void traverse(BinNode *node, traversal_order order) const;

    void deleteNode(BinNode *node, BinNode *parentNode, delete_mode mode);

public:
    /****** Iterators ******/
    class Iterator {
    protected:
        std::stack<BST<Data_T>::BinNode *> st, rev_st;
        const BST<Data_T> *tree;

        void init(BST<Data_T>::BinNode *node) {
            while (node) {
                st.push(node);
                node = node->left;
            }
        }

    public:
        Iterator(BST<Data_T>::BinNode *node, const BST<Data_T> *tree) : tree(tree) {
            st.push(nullptr);
            if (node) init(tree->myRoot);
//            std::cout << "Iterator Got root: " << tree->myRoot->getData().first << std::endl;
            while (node != this->st.top())
                next();
        }

        // Copy constr
        Iterator(const Iterator &it) : st(std::stack<BinNode *>(it.st)),
                                       rev_st(std::stack<BinNode *>(it.rev_st)),
                                       tree(it.tree) {}

        bool hasNext() const {
            return !st.empty() && st.top();
        }

        virtual Data_T &get() {
            return this->st.top()->getData();
        }

        virtual Data_T &next() {
            if (!hasNext()) throw std::out_of_range("Tree Iterator reached end, cannot get next");
            BST<Data_T>::BinNode *popped = st.top();
            st.pop();
            if (popped->right) {
                rev_st.push(popped);
                init(popped->right);
            }
            return popped->getData();
        }

        virtual Data_T &prev() {
            if (st.top()->left)
                st.push(st.top()->left);
            else if (!rev_st.empty()) {
                st.push(rev_st.top());
                rev_st.pop();
            } else throw std::out_of_range("Tree Iterator reached first, cannot get previous");
            return st.top()->getData();
        }

        bool operator==(const Iterator &other) {
            return this->st.top() == other.st.top();
        }

        bool operator!=(const Iterator &other) {
            return this->st.top() != other.st.top();
        }
    };

    friend BST<Data_T>::Iterator;

    virtual BST<Data_T>::Iterator begin() {
        return BST<Data_T>::Iterator(smallestNode, this);
    }

    virtual BST<Data_T>::Iterator begin(Data_T &data) {
        BinNode *parent = nullptr, *node = this->searchNode(myRoot, data, parent);
        if (!node) throw std::out_of_range("could not instantiate Iterator, data not found in tree");
        return BST<Data_T>::Iterator(node, this);
    }

    virtual BST<Data_T>::Iterator end() {
        return BST<Data_T>::Iterator(nullptr, this);
    }

}; // end of class declaration

#endif


//--- Definition of constructors
template<typename Data_T>
BST<Data_T>::BST(bool updateIfExists)
        : updateIfExists(updateIfExists), myRoot(0) {}

//template<typename Data_T>
//BST<Data_T>::BST(const std::function<Data_T()> &default_init)
//        : BST<Data_T>(true), default_init(default_init) {}

template<typename Data_T>
BST<Data_T>::BST(const BST<Data_T> &copyFrom): updateIfExists(copyFrom.updateIfExists) {
    cloneFrom(&copyFrom);
}

template<typename Data_T>
BST<Data_T>::BST(const BST<Data_T> *copyFrom): updateIfExists(copyFrom->updateIfExists) {
    cloneFrom(copyFrom);
}

template<typename Data_T>
void BST<Data_T>::cloneFrom(const BST<Data_T> *tree) {
    BinNode::deleteSubTree(this->myRoot);
    this->myRoot = cloneFrom(tree->myRoot);
    this->smallestNode = this->smallest(myRoot);
    this->largestNode = this->largest(myRoot);
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
/*public*/ Data_T *BST<Data_T>::search(const Data_T &item) const {
    BinNode *locptr = myRoot;
    while (locptr) {
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
template<typename DataSearch_T>
/*public*/ Data_T *
BST<Data_T>::search(const DataSearch_T &searchItem,
                    const std::function<short int(const DataSearch_T &, Data_T &)> &comp) const {
    BinNode *locptr = myRoot;
    short int comp_val = 0;
    while (locptr) {
        comp_val = comp(searchItem, locptr->getData());
        if (comp_val < 0)       // descend left
            locptr = locptr->left;
        else if (comp_val > 0)  // descend right
            locptr = locptr->right;
        else // if (comp_val == 0) // item found
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
        if (!smallestNode) {
            smallestNode = locptr;
            largestNode = locptr;
        } else {
            if (locptr->getData() < smallestNode->getData())
                smallestNode = locptr;
            if (largestNode->getData() < locptr->getData())
                largestNode = locptr;
        }

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
    } else if (this->updateIfExists)// Item exists in tree, and updateIfExists set to true
        locptr->getData() = item;
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
void BST<Data_T>::deleteNode(const Data_T &value) {
    BinNode *searchNode, *parentNode;
    parentNode = nullptr;
    delete_mode mode = LEAF_NODE;
    searchNode = this->searchNode(this->myRoot, value, parentNode);
    if (searchNode->left && searchNode->right)
        mode = TWO_CHILDREN;
    else if (searchNode->left || searchNode->right)
        mode = ONE_CHILD;
    this->deleteNode(searchNode, parentNode, mode);
}

template<typename Data_T>
void BST<Data_T>::clear() {
    BinNode::deleteSubTree(this->myRoot);
}

template<typename Data_T>
template<typename DataSearch_T>
void BST<Data_T>::deleteNode(
        const DataSearch_T &item,
        const std::function<short int(const DataSearch_T &, Data_T &)> &comp
) {
    BinNode *searchNode, *parentNode;
    parentNode = nullptr;
    delete_mode mode = LEAF_NODE;
    searchNode = this->searchNode(this->myRoot, item, comp, parentNode);
    if (searchNode->left && searchNode->right)
        mode = TWO_CHILDREN;
    else if (searchNode->left || searchNode->right)
        mode = ONE_CHILD;
    this->deleteNode(searchNode, parentNode, mode);
}


// Private methods
template<typename Data_T>
int BST<Data_T>::nodeCount(BST<Data_T>::BinNode *node) const {
    if (!node)
        return 0;
    return 1 + this->nodeCount(node->left) + this->nodeCount(node->right);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *
BST<Data_T>::searchNode(BST<Data_T>::BinNode *startNode, const Data_T &data, BST<Data_T>::BinNode *&parentNode) {
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
template<typename DataSearch_T>
typename BST<Data_T>::BinNode *BST<Data_T>::searchNode(
        BinNode *startNode, const DataSearch_T &searchData,
        const std::function<short int(const DataSearch_T &, Data_T &)> &comp,
        BinNode *&parentNode
) const {
    parentNode = nullptr;
    short int comp_val = 0;
    while (startNode) {
        comp_val = comp(searchData, startNode->getData());
        if (comp_val == 0)
            return startNode;
        else if (comp_val < 0) {
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
    if (!node)
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
    Data_T &data = node->getData();
//    if (isRoot)
//        cout << "Deleting Root element : " << node->getData() << endl;
//    else
//        cout << "Deleting element [" << node->getData() << "] with parent [" << parentNode->getData() << "]" << endl;
    switch (mode) {
        case LEAF_NODE: {
//            cout << "Deleting Leaf Node " << node->getData() << endl;
            if (isRoot)
                this->myRoot = nullptr;
            else {
                if (parentNode->left == node) {
//                    cout << "Deleting left child of parent: " << parentNode->getData() << endl;
                    parentNode->left = nullptr;
                } else if (parentNode->right == node) {
//                    cout << "Deleting right child of parent: " << parentNode->getData() << endl;
                    parentNode->right = nullptr;
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
            if (parentOfSmallestNode && (status == 1))
                parentOfSmallestNode->left = smallestNode->right;
            else
                node->right = smallestNode->right;
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
    if (node == this->smallestNode)
        this->smallestNode = this->smallest(myRoot);
    if (node == this->largestNode)
        this->largestNode = this->largest(myRoot);
    delete node;
    this->postDelete(data, parentNode);
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::smallest(BinNode *rootNode, BinNode *&parentNode, int &status) {
    while (rootNode && rootNode->left) {
        parentNode = rootNode;
        rootNode = rootNode->left;
        status = 1;
    }
    return rootNode;
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::largest(BinNode *rootNode, BinNode *&parentNode, int &status) {
    while (rootNode && rootNode->right) {
        parentNode = rootNode;
        rootNode = rootNode->right;
        status = 1;
    }
    return rootNode;
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::smallest(BinNode *rootNode) {
    while (rootNode && rootNode->left)
        rootNode = rootNode->left;
    return rootNode;
}

template<typename Data_T>
typename BST<Data_T>::BinNode *BST<Data_T>::largest(BinNode *rootNode) {
    while (rootNode && rootNode->right)
        rootNode = rootNode->right;
    return rootNode;
}

template<typename Data_T>
void BST<Data_T>::postInsert(const BST<Data_T>::BinNode *node, const BST<Data_T>::BinNode *parentNode) {}

template<typename Data_T>
void BST<Data_T>::postDelete(const Data_T &data, const BST<Data_T>::BinNode *parentNode) {}

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
//    cout << this->getData() << endl;
}

template<typename Data_T>
void BST<Data_T>::BinNode::cloneFrom(const BinNode *node) {
    this->data = node->data;
    this->left->cloneFrom(node->left);
    this->right->cloneFrom(node->right);
}

template<typename Data_T>
void BST<Data_T>::BinNode::deleteSubTree(BinNode *&node) {
    if (!node) return;
    std::queue < BinNode * * > q;
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