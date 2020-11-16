//
// Created by harshitv on 11/4/2020.
//

#ifndef AVL_TREE_MAP
#define AVL_TREE_MAP

#include "tree/AVL.hpp"

#include <functional>

namespace cs540 {

    template<typename Key_T, typename Mapped_T>
    class Map {
        using ValueType = std::pair<Key_T, Mapped_T>;

    public:
        class MapDataNode : public ValueType {
        public:
            MapDataNode(const Key_T key, Mapped_T mappedItem) : ValueType({key, mappedItem}) {}

            MapDataNode(const std::pair<const Key_T, Mapped_T> &p) : MapDataNode(p.first, p.second) {}

            MapDataNode(std::initializer_list <Mapped_T> list) : MapDataNode(list.begin(), list.begin() + 1) {}

            const Key_T &getKey() {
                return this->first;
            }

            Mapped_T &getMappedItem() {
                return this->second;
            }

            bool operator<(const MapDataNode &node) const {
                return this->first < node.first || (this->first == node.first && this->second < node.second);
//            return (this->first < node.first && this->second <= node.second) ||
//                   (this->first <= node.first && this->second < node.second);
            }

            bool operator==(const MapDataNode &node) const {
                return this->first == node.first && this->second == node.second;
            }

            MapDataNode operator=(const MapDataNode &other) {
                return MapDataNode(other);
            }
        };

        class MapKeyNode {
        protected:
            const Key_T key;
        public:
            MapKeyNode(const Key_T key) : key(key) {}

            const Key_T &getKey() {
                return key;
            }

            bool operator<(const typename Map<Key_T, Mapped_T>::MapKeyNode &node) const {
                return key < node.key;
            }

            bool operator==(const typename Map<Key_T, Mapped_T>::MapKeyNode &node) const {
                return key == node.key;
            }

            bool operator<(const typename Map<Key_T, Mapped_T>::MapDataNode &node) const {
                return key < node.first;
            }

            bool operator==(const typename Map<Key_T, Mapped_T>::MapDataNode &node) const {
                return key == node.first;
            }

            const static std::function<short int(const MapKeyNode &, MapDataNode &)> key_data_comp;
        };

        class Iterator {
            friend Map<Key_T, Mapped_T>;
        public:
            Iterator(const Iterator &it) : Iterator(it.it) {}

            virtual ~Iterator() {}

            virtual void inc() {
                it.next();
            }

            virtual void dec() {
                it.prev();
            }

            ValueType &operator*() const {
                return this->it.get();
            }

            ValueType *operator->() const {
                return &(this->it.get());
            }

            // Prefix inc/dec
            Iterator &operator++() {
                this->inc();
                return *this;
            }

            Iterator &operator--() {
                this->dec();
                return *this;
            }

            // Postfix inc/dec
            Iterator operator++(int) {
                Iterator old = *this;
                this->inc();
                return old;
            }

            Iterator operator--(int) {
                Iterator old = *this;
                this->dec();
                return old;
            }

            bool operator==(const typename Map<Key_T, Mapped_T>::Iterator &other) {
                return this->it == other.it;
            }

            bool operator!=(const typename Map<Key_T, Mapped_T>::Iterator &other) {
                return this->it != other.it;
            }

        protected:
            typename AVL<MapDataNode>::Iterator it;

            Iterator(const AVL<MapDataNode> &tree) : Iterator(tree.begin()) {}

            Iterator(const AVL<MapDataNode> &tree, const MapDataNode &node) : Iterator(tree.begin(node)) {}

            Iterator(const typename BST<MapDataNode>::Iterator &it) : it(it) {}

        };

        class ConstIterator : public Iterator {
            friend Map<Key_T, Mapped_T>;
        public:
            ConstIterator(const Iterator &it) : Iterator(it) {}

            ConstIterator(const ConstIterator &it) : Iterator(it) {}

            ~ConstIterator() {}

            const ValueType &operator*() const {
                return this->it.get();
            }

            const ValueType *operator->() const {
                return &(this->it.get());
            }

        protected:
            ConstIterator(const AVL<MapDataNode> &tree) : Iterator(tree) {}

            ConstIterator(const AVL<MapDataNode> &tree, const MapDataNode &node) : Iterator(tree, node) {}

            ConstIterator(const typename BST<MapDataNode>::Iterator &it) : Iterator(it) {}
        };

        class ReverseIterator : public Iterator {
            friend Map<Key_T, Mapped_T>;
        public:
            ReverseIterator(const ReverseIterator &it) : Iterator(it) {}

            ~ReverseIterator() {}

        protected:
            ReverseIterator(AVL<MapDataNode> &tree) : Iterator(tree.rbegin()) {}

            ReverseIterator(const typename BST<MapDataNode>::Iterator &it) : Iterator(it) {}
        };

        // -- constructing
        Map() : tree(false) {}

        Map(std::initializer_list <std::pair<const Key_T, Mapped_T>> list) : tree(false) {
            for (std::pair<const Key_T, Mapped_T> p : list)
                this->insert(p);
        }

        Map(const Map &map) : tree(map.tree) {}

        ~Map() {
            this->clear();
        }

        Map<Key_T, Mapped_T> &operator=(const Map<Key_T, Mapped_T> &other) {
            this->clear();
            return Map<Key_T, Mapped_T>(other);
        }

        // -- size:
        size_t size() const;

        bool empty() const;

        // -- element access:
        Mapped_T &at(const Key_T &);

        const Mapped_T &at(const Key_T &) const;

        Iterator find(const Key_T &);

        ConstIterator find(const Key_T &) const;

        Mapped_T &operator[](const Key_T &key);

        // -- iterators
        Iterator begin() {
            return Iterator(tree);
        }

        Iterator begin(const MapDataNode &node) {
            return Iterator(tree, node);
        }

        Iterator end() {
            return Iterator(tree.end());
        }

        ConstIterator begin() const {
            return ConstIterator(tree);
        }

        ConstIterator begin(MapDataNode &node) const {
            return ConstIterator(tree, node);
        }

        ConstIterator end() const {
            return ConstIterator(tree.end());
        }

        ReverseIterator rbegin() {
            return ReverseIterator(tree.rbegin());
        }

        ReverseIterator rend() {
            return ReverseIterator(tree.rend());
        }

        // -- modifiers:
        std::pair<Iterator, bool> insert(const std::pair<const Key_T, Mapped_T> &);

        template<typename IT_T>
        void insert(IT_T range_beg, IT_T range_end);

        void erase(const Key_T &);

//        void erase(const Iterator &&);

        void erase(Iterator);

        void clear();

        // -- equality:
        bool operator==(const Map<Key_T, Mapped_T> &other) {
            return this->size() == other.size() && this->tree == other.tree;
        }

        bool operator!=(const Map<Key_T, Mapped_T> &other) {
            return this->size() != other.size() || this->tree != other.tree;
        }

        bool operator<(const Map<Key_T, Mapped_T> &other) {
            return this->tree < other.tree;
        }

    protected:
        AVL<MapDataNode> tree;

        MapDataNode *get_data_node(const Key_T &) const;

        MapDataNode *get_data_node(const MapDataNode &) const;

    };

#endif // AVL_TREE_MAP


// - protected
// -- element access
    template<typename Key_T, typename Mapped_T>
    typename Map<Key_T, Mapped_T>::MapDataNode *Map<Key_T, Mapped_T>::get_data_node(const Key_T &key) const {
        return tree.search(MapKeyNode(key), MapKeyNode::key_data_comp);
    }

    template<typename Key_T, typename Mapped_T>
    typename Map<Key_T, Mapped_T>::MapDataNode *
    Map<Key_T, Mapped_T>::get_data_node(const typename Map<Key_T, Mapped_T>::MapDataNode &node) const {
        return tree.search(node);
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
        MapDataNode *dataNode = tree.search(keyNode, MapKeyNode::key_data_comp);
        if (!dataNode) {
            dataNode = new MapDataNode(key, Mapped_T());
            tree.insert(dataNode);
        }
        return dataNode->getMappedItem();
    }

    template<typename Key_T, typename Mapped_T>
    Mapped_T &Map<Key_T, Mapped_T>::at(const Key_T &key) {
        MapDataNode *node = this->get_data_node(key);
        if (!node)
            throw std::out_of_range("specified key does not exist");
        return node->getMappedItem();
    }

    template<typename Key_T, typename Mapped_T>
    const Mapped_T &Map<Key_T, Mapped_T>::at(const Key_T &key) const {
        const MapDataNode *node = this->get_data_node(key);
        if (!node)
            throw std::out_of_range("specified key does not exist");
        return node->getMappedItem();
    }

    template<typename Key_T, typename Mapped_T>
    typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T &key) {
        const MapDataNode *node = this->get_data_node(key);
        return node ? begin(*node) : end();
    }

    template<typename Key_T, typename Mapped_T>
    typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find(const Key_T &key) const {
        const MapDataNode *node = this->get_data_node(key);
        return node ? begin(*node) : end();
    }

// -- modifiers:
    template<typename Key_T, typename Mapped_T>
    std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>
    Map<Key_T, Mapped_T>::insert(const std::pair<const Key_T, Mapped_T> &pair) {
        const MapDataNode node(pair);
        bool new_inserted;
        if ((new_inserted = !get_data_node(node))) {
            tree.insert(node);
        }
        return {begin(node), new_inserted};
    }

    template<typename Key_T, typename Mapped_T>
    template<typename IT_T>
    void Map<Key_T, Mapped_T>::insert(IT_T range_beg, IT_T range_end) {
        while (range_beg != range_end) {
            tree.insert(MapDataNode(*range_beg));
            range_beg++;
        }
    }

    template<typename Key_T, typename Mapped_T>
    void Map<Key_T, Mapped_T>::erase(const Key_T &key) {
        tree.deleteNode(MapKeyNode(key), MapKeyNode::key_data_comp);
    }

//    template<typename Key_T, typename Mapped_T>
//    void Map<Key_T, Mapped_T>::erase(const Iterator &&it) {
//        this->erase((*it).first);
//    }

    template<typename Key_T, typename Mapped_T>
    void Map<Key_T, Mapped_T>::erase(Iterator it) {
        this->erase((*it).first);
    }

    template<typename Key_T, typename Mapped_T>
    void Map<Key_T, Mapped_T>::clear() {
        tree.clear();
    }

/*********** MapKeyNode ************/
// public:
// -- static members:
    template<typename K, typename M>
    const std::function<short int(
            const typename Map<K, M>::MapKeyNode &, typename Map<K, M>::MapDataNode &
    )> Map<K, M>::MapKeyNode::key_data_comp =
            [](const MapKeyNode &key,
               MapDataNode &data) {
                return key < data ? -1 : key == data ? 0 : 1;
            };

}