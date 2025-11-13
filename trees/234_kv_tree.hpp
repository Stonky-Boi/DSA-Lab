#ifndef tree_234_kv
#define tree_234_kv

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>

namespace tree234
{
    template <typename K, typename V>
    class NodeKV
    {
    public:
        std::vector<K> keys;
        std::vector<V> values;
        std::vector<NodeKV *> children;
        bool leaf;

        NodeKV(bool leaf_flag = true)
        {
            leaf = leaf_flag;
        }

        ~NodeKV() {}

        bool is_full() const
        {
            return keys.size() == 3;
        }
    };

    template <typename K, typename V>
    class Tree234KV
    {
    public:
        NodeKV<K, V> *root;

        Tree234KV()
        {
            root = nullptr;
        }

        ~Tree234KV()
        {
            clear(root);
        }

        void put(const K &key, const V &val)
        {
            if (!root)
            {
                root = new NodeKV<K, V>(true);
                root->keys.push_back(key);
                root->values.push_back(val);
                return;
            }
            if (root->is_full())
            {
                NodeKV<K, V> *nr = new NodeKV<K, V>(false);
                nr->children.push_back(root);
                split_child(nr, 0);
                root = nr;
            }
            put_non_full(root, key, val);
        }

        void remove(const K &key)
        {
            if (!root)
                return;
            delete_key(root, key);
            if (root && root->keys.empty())
            {
                NodeKV<K, V> *old = root;
                if (!root->leaf)
                    root = root->children[0];
                else
                    root = nullptr;
                delete old;
            }
        }

        void get(const K &key) const
        {
            NodeKV<K, V> *curr = root;
            while (curr)
            {
                int i = 0;
                while (i < (int)curr->keys.size() && key > curr->keys[i])
                    ++i;
                if (i < (int)curr->keys.size() && key == curr->keys[i])
                {
                    std::cout << curr->values[i] << std::endl;
                    return;
                }
                if (curr->leaf)
                    break;
                curr = curr->children[i];
            }
            std::cout << "NOTFOUND" << std::endl;
        }

        void range(const K &L, const K &R) const
        {
            bool printed = false;
            range_traverse(root, L, R, printed);
            if (!printed)
                std::cout << "EMPTY" << std::endl;
        }

        void batch_delete(const std::vector<K> &keys)
        {
            for (auto &k : keys)
                remove(k);
        }

    private:
        void clear(NodeKV<K, V> *n)
        {
            if (!n)
                return;
            if (!n->leaf)
                for (auto c : n->children)
                    clear(c);
            delete n;
        }

        void split_child(NodeKV<K, V> *parent, int index)
        {
            NodeKV<K, V> *full = parent->children[index];
            NodeKV<K, V> *right = new NodeKV<K, V>(full->leaf);
            K mid_key = full->keys[1];
            V mid_value = full->values[1];
            right->keys.push_back(full->keys[2]);
            right->values.push_back(full->values[2]);
            if (!full->leaf)
            {
                right->children.push_back(full->children[2]);
                right->children.push_back(full->children[3]);
            }
            full->keys.resize(1);
            full->values.resize(1);
            if (!full->leaf)
                full->children.resize(2);
            parent->keys.insert(parent->keys.begin() + index, mid_key);
            parent->values.insert(parent->values.begin() + index, mid_value);
            parent->children.insert(parent->children.begin() + index + 1, right);
        }

        void put_non_full(NodeKV<K, V> *node, const K &key, const V &val)
        {
            int i = (int)node->keys.size() - 1;
            if (node->leaf)
            {
                while (i >= 0 && key < node->keys[i])
                    --i;
                if (i >= 0 && node->keys[i] == key)
                {
                    node->values[i] = val;
                    return;
                }
                node->keys.insert(node->keys.begin() + i + 1, key);
                node->values.insert(node->values.begin() + i + 1, val);
            }
            else
            {
                while (i >= 0 && key < node->keys[i])
                    --i;
                ++i;
                if (node->children[i]->is_full())
                {
                    split_child(node, i);
                    if (key > node->keys[i])
                        ++i;
                }
                put_non_full(node->children[i], key, val);
            }
        }

        void delete_key(NodeKV<K, V> *node, const K &key)
        {
            int index = 0;
            while (index < (int)node->keys.size() && key > node->keys[index])
                ++index;
            if (index < (int)node->keys.size() && node->keys[index] == key)
            {
                if (node->leaf)
                {
                    node->keys.erase(node->keys.begin() + index);
                    node->values.erase(node->values.begin() + index);
                    return;
                }
                NodeKV<K, V> *pred = node->children[index];
                NodeKV<K, V> *succ = node->children[index + 1];
                if (pred->keys.size() >= 2)
                {
                    std::pair<K, V> p = get_predecessor(pred);
                    node->keys[index] = p.first;
                    node->values[index] = p.second;
                    delete_key(pred, p.first);
                }
                else if (succ->keys.size() >= 2)
                {
                    std::pair<K, V> s = get_successor(succ);
                    node->keys[index] = s.first;
                    node->values[index] = s.second;
                    delete_key(succ, s.first);
                }
                else
                {
                    merge_nodes(node, index);
                    delete_key(pred, key);
                }
            }
            else
            {
                if (node->leaf)
                    return;
                ensure_child_has_two_keys(node, index);
                delete_key(node->children[index], key);
            }
        }

        void ensure_child_has_two_keys(NodeKV<K, V> *node, int index)
        {
            NodeKV<K, V> *child = node->children[index];
            if (child->keys.size() >= 2)
                return;
            if (index > 0 && node->children[index - 1]->keys.size() >= 2)
                borrow_from_prev(node, index);
            else if (index + 1 < (int)node->children.size() && node->children[index + 1]->keys.size() >= 2)
                borrow_from_next(node, index);
            else
            {
                if (index + 1 < (int)node->children.size())
                    merge_nodes(node, index);
                else
                    merge_nodes(node, index - 1);
            }
        }

        void borrow_from_prev(NodeKV<K, V> *node, int index)
        {
            NodeKV<K, V> *child = node->children[index];
            NodeKV<K, V> *sibling = node->children[index - 1];
            child->keys.insert(child->keys.begin(), node->keys[index - 1]);
            child->values.insert(child->values.begin(), node->values[index - 1]);
            if (!child->leaf)
                child->children.insert(child->children.begin(), sibling->children.back());
            node->keys[index - 1] = sibling->keys.back();
            node->values[index - 1] = sibling->values.back();
            sibling->keys.pop_back();
            sibling->values.pop_back();
            if (!sibling->leaf)
                sibling->children.pop_back();
        }

        void borrow_from_next(NodeKV<K, V> *node, int index)
        {
            NodeKV<K, V> *child = node->children[index];
            NodeKV<K, V> *sibling = node->children[index + 1];
            child->keys.push_back(node->keys[index]);
            child->values.push_back(node->values[index]);
            if (!child->leaf)
                child->children.push_back(sibling->children.front());
            node->keys[index] = sibling->keys.front();
            node->values[index] = sibling->values.front();
            sibling->keys.erase(sibling->keys.begin());
            sibling->values.erase(sibling->values.begin());
            if (!sibling->leaf)
                sibling->children.erase(sibling->children.begin());
        }

        void merge_nodes(NodeKV<K, V> *node, int index)
        {
            NodeKV<K, V> *left = node->children[index];
            NodeKV<K, V> *right = node->children[index + 1];
            left->keys.push_back(node->keys[index]);
            left->values.push_back(node->values[index]);
            left->keys.insert(left->keys.end(), right->keys.begin(), right->keys.end());
            left->values.insert(left->values.end(), right->values.begin(), right->values.end());
            if (!left->leaf)
                left->children.insert(left->children.end(), right->children.begin(), right->children.end());
            node->keys.erase(node->keys.begin() + index);
            node->values.erase(node->values.begin() + index);
            node->children.erase(node->children.begin() + index + 1);
            delete right;
        }

        std::pair<K, V> get_predecessor(NodeKV<K, V> *node)
        {
            while (!node->leaf)
                node = node->children.back();
            return {node->keys.back(), node->values.back()};
        }

        std::pair<K, V> get_successor(NodeKV<K, V> *node)
        {
            while (!node->leaf)
                node = node->children.front();
            return {node->keys.front(), node->values.front()};
        }

        void range_traverse(NodeKV<K, V> *node, const K &L, const K &R, bool &printed) const
        {
            if (!node)
                return;
            int n = (int)node->keys.size();
            for (int i = 0; i < n; ++i)
            {
                if (!node->leaf)
                    range_traverse(node->children[i], L, R, printed);
                if (node->keys[i] >= L && node->keys[i] <= R)
                {
                    printed = true;
                    std::cout << node->keys[i] << ":" << node->values[i] << std::endl;
                }
            }
            if (!node->leaf)
                range_traverse(node->children[n], L, R, printed);
        }
    };
}

#endif