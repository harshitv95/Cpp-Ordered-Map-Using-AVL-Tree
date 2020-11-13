//
// Created by harshitv on 11/4/2020.
//

#ifndef AVL_TREE_MAP
#define AVL_TREE_MAP

#include "../tree/AVL.hpp"

#include <functional>

template<typename Key_T, typename Mapped_T>
class Map {
    using ValueType = std::pair<Key_T, Mapped_T>;

protected:
    class MapKeyNode;

    class MapDataNode : public ValueType {
    public:
        MapDataNode(const Key_T key, Mapped_T mappedItem) : ValueType({key, mappedItem}) {}

        MapDataNode(std::pair<const Key_T, Mapped_T> p) : MapDataNode(p.first, p.second) {}

//        MapDataNode(std::pair <Key_T, Mapped_T> p) : MapDataNode(p.first, p.second) {
//            cout << "INPUT key=" << p.first << ", value=" << p.second << endl;
//            cout << "Set key=" << this->MapKeyNode::key << ", value=" << this->mappedItem << endl;
//        }

        MapDataNode(std::initializer_list <Mapped_T> list) : MapDataNode(list.begin(), list.begin() + 1) {}

        const Key_T &getKey() {
            return this->first;
        }

        Mapped_T &getMappedItem() {
            return this->second;
        }

        const static std::function<short int(const MapKeyNode &, MapDataNode &)> key_data_comp;
    };

    class MapKeyNode {
    protected:
        const Key_T key;
    public:
        MapKeyNode(const Key_T key) : key(key) {}

        const Key_T &getKey() {
            return key;
        }

        bool operator<(const MapKeyNode &node) const {
            return key < node.key;
        }

        bool operator==(const MapKeyNode &node) const {
            return key == node.key;
        }

        bool operator<(const MapDataNode &node) const {
            return key < node.first;
        }

        bool operator==(const MapDataNode &node) const {
            return key == node.first;
        }
    };

    class Iterator {
    public:
        Iterator(AVL<MapDataNode> &tree) : Iterator(tree.begin()) {}

        Iterator(AVL<MapDataNode> &tree, MapDataNode &node) : Iterator(tree.begin(node)) {}

        Iterator(const Iterator &it) : Iterator(it.it) {}

        ValueType &operator*() {
            return it.get();
        }

        void operator++() {
            it.next();
        }

        void operator--() {
            it.prev();
        }

    protected:
        typename BST<MapDataNode>::Iterator it;

        Iterator(const typename BST<MapDataNode>::Iterator &it) : it(it) {}

    };

    AVL<MapDataNode> tree;

    Mapped_T &get_mapped(const Key_T &) const;


public:
    // -- constructing
    Map() {}

    Map(std::initializer_list <std::pair<const Key_T, Mapped_T>> list) {
        for (std::pair<const Key_T, Mapped_T> p : list)
            this->insert(p);
    }

    // -- size:
    size_t size() const;

    bool empty() const;

    // -- element access:
    Mapped_T &at(const Key_T &);

    const Mapped_T &at(const Key_T &) const;

    Mapped_T &operator[](const Key_T &key);

    // -- iterators
    Iterator begin() {
        return Iterator(tree);
    }

    Iterator begin(MapDataNode &node) {
        return Iterator(tree, node);
    }

    // -- modifiers:
    std::pair<Iterator, bool> insert(const std::pair<const Key_T, Mapped_T> &);

};

#endif // AVL_TREE_MAP


// - protected
// -- element access
template<typename Key_T, typename Mapped_T>
Mapped_T &Map<Key_T, Mapped_T>::get_mapped(const Key_T &key) const {
    MapKeyNode keyNode(key);
    MapDataNode *dataNode = tree.search(keyNode, MapDataNode::key_data_comp);
    if (!dataNode)
        throw std::out_of_range("specified key does not exist");
    return (dataNode)->getMappedItem();
}


// - public:
// -- size:
template<typename Key_T, typename Mapped_T>
size_t Map<Key_T, Mapped_T>::size() const {
    return tree.size();
}

template<typename Key_T, typename Mapped_T>
bool Map<Key_T, Mapped_T>::empty() const {
    return tree.empty();
}

// -- element access:
template<typename Key_T, typename Mapped_T>
Mapped_T &Map<Key_T, Mapped_T>::operator[](const Key_T &key) {
    MapKeyNode keyNode(key);
    MapDataNode *dataNode = tree.search(keyNode, MapDataNode::key_data_comp);
    if (!dataNode) {
        dataNode = new MapDataNode(key, Mapped_T());
        tree.insert(dataNode);
    }
    return dataNode->getMappedItem();
}

template<typename Key_T, typename Mapped_T>
Mapped_T &Map<Key_T, Mapped_T>::at(const Key_T &key) {
    return this->get_mapped(key);
}

template<typename Key_T, typename Mapped_T>
const Mapped_T &Map<Key_T, Mapped_T>::at(const Key_T &key) const {
    return this->get_mapped(key);
}

// -- modifiers:
template<typename Key_T, typename Mapped_T>
std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>
Map<Key_T, Mapped_T>::insert(const std::pair<const Key_T, Mapped_T> &pair) {
    MapDataNode node(pair);
    tree.insert(node);
    Iterator it = begin(node);
    return {it, true};
}

//template<typename Key_T, typename Mapped_T>
//std::pair<int, bool> Map<Key_T, Mapped_T>::insert(const typename Map<Key_T, Mapped_T>::MapDataNode node) {
//    tree.insert(node);
//    return {1, true};
//}

/*********** MapDataNode ************/
// public:
// -- static members:
template<typename K, typename M>
const std::function<short int(
        const typename Map<K, M>::MapKeyNode &, typename Map<K, M>::MapDataNode &
)> Map<K, M>::MapDataNode::key_data_comp =
        [](const MapKeyNode &key,
           MapDataNode &data) {
            return key < data ? -1 : key == data ? 0 : 1;
        };