//
// Created by harshitv on 11/4/2020.
//

#ifndef AVL_TREE_MAP
#define AVL_TREE_MAP

#include "../tree/AVL.hpp"

template<typename Key_T, typename Mapped_T>
class Map {

protected:
    class MapKeyNode {
    protected:
        Key_T key;
    public:
        MapKeyNode(const Key_T key) : key(key) {}

        Key_T getKey() {
            return key;
        }

    };

    class MapDataNode : public MapKeyNode {
    protected:
        Mapped_T mappedItem;
    public:
        MapDataNode(const Key_T key, Mapped_T mappedItem) : MapKeyNode(key), mappedItem(mappedItem) {}

        Mapped_T &getMappedItem() {
            return mappedItem;
        }
    };

    AVL<MapKeyNode> tree;

    Mapped_T &get_mapped(const Key_T &) const;

public:
    // -- size:
    size_t size() const;

    bool empty() const;

    // -- element access:
    Mapped_T &at(const Key_T &);

    const Mapped_T &at(const Key_T &) const;

    Mapped_T &operator[](const Key_T &key);
};

#endif // AVL_TREE_MAP


// - protected
// -- element access
template<typename Key_T, typename Mapped_T>
Mapped_T &Map<Key_T, Mapped_T>::get_mapped(const Key_T &key) const {
    MapKeyNode keyNode(key);
    MapDataNode *dataNode = (MapDataNode *) tree.search(keyNode);
    if (!dataNode)
        throw std::out_of_range("specified key does not exist");
    return dataNode->getMappedItem();
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
    MapDataNode *dataNode = (MapDataNode *) tree.search(keyNode);
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