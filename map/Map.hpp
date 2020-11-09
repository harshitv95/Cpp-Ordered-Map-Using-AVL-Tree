//
// Created by harshitv on 11/4/2020.
//

#ifndef AVL_TREE_MAP
#define AVL_TREE_MAP

#include "../tree/AVL.hpp"

template<Key_T, Mapped_T>
class Map {

protected:
    class MapNode {
    protected:
        Key_T key;
        Mapped_T mappedItem;
    public:
        MapNode(Key_T key, Mapped_T mappedItem) : key(key), value(mappedItem) {}

        Key_T getKey() {
            return key;
        }

        Mapped_T getMappedItem() {
            return mappedItem;
        }
    };

    AVL<MapNode> tree;

public:
    size_t size() const;
};

#endif // AVL_TREE_MAP

