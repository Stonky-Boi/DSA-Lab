#ifndef tree_234
#define tree_234

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

namespace tree234
{
    template <typename T>
    class Node
    {
    public:
        std::vector<T> keys;
        std::vector<Node *> children;
        bool leaf;

        Node(bool leaf_flag = true)
        {
            leaf = leaf_flag;
        }

        ~Node() {}

        bool is_full() const
        {
            return keys.size() == 3;
        }
    };

    template <typename T>
    class Tree234
    {
    public:
        Node<T> *root;

        Tree234()
        {
            root = nullptr;
        }

        ~Tree234()
        {
            clear(root);
        }

        void insert(const T &key)
        {
            if (!root)
            {
                root = new Node<T>(true);
                root->keys.push_back(key);
                return;
            }
            if (root->is_full())
            {
                Node<T> *new_root = new Node<T>(false);
                new_root->children.push_back(root);
                split_child(new_root, 0);
                root = new_root;
            }
            insert_non_full(root, key);
        }

        void remove(const T &key)
        {
            if (!root)
                return;
            delete_key(root, key);
            if (root && root->keys.empty())
            {
                Node<T> *old = root;
                if (!root->leaf)
                    root = root->children[0];
                else
                    root = nullptr;
                delete old;
            }
        }

        bool find(const T &key) const
        {
            Node<T> *curr = root;
            while (curr)
            {
                int i = 0;
                while (i < (int)curr->keys.size() && key > curr->keys[i])
                    ++i;
                if (i < (int)curr->keys.size() && key == curr->keys[i])
                    return true;
                if (curr->leaf)
                    return false;
                curr = curr->children[i];
            }
            return false;
        }

        void print_inorder() const
        {
            if (!root)
            {
                std::cout << "EMPTY" << std::endl;
                return;
            }
            inorder(root);
            std::cout << std::endl;
        }

        void print_level() const
        {
            if (!root)
            {
                std::cout << "EMPTY" << std::endl;
                return;
            }
            std::queue<Node<T> *> q;
            q.push(root);
            bool first_node = true;
            while (!q.empty())
            {
                Node<T> *node = q.front();
                q.pop();
                if (!first_node)
                    std::cout << " ";
                std::cout << "[";
                for (size_t i = 0; i < node->keys.size(); ++i)
                {
                    std::cout << node->keys[i];
                    if (i + 1 < node->keys.size())
                        std::cout << " ";
                }
                std::cout << "]";
                first_node = false;
                if (!node->leaf)
                    for (auto c : node->children)
                        q.push(c);
            }
            std::cout << std::endl;
        }

        void bulk_build(const std::vector<T> &arr)
        {
            clear(root);
            root = build_balanced(arr, 0, (int)arr.size());
        }

    private:
        void clear(Node<T> *node)
        {
            if (!node)
                return;
            if (!node->leaf)
            {
                for (auto c : node->children)
                    clear(c);
            }
            delete node;
        }

        void split_child(Node<T> *parent, int index)
        {
            Node<T> *full = parent->children[index];
            Node<T> *right = new Node<T>(full->leaf);
            T mid = full->keys[1];
            right->keys.push_back(full->keys[2]);
            if (!full->leaf)
            {
                right->children.push_back(full->children[2]);
                right->children.push_back(full->children[3]);
            }
            full->keys.resize(1);
            if (!full->leaf)
                full->children.resize(2);
            parent->keys.insert(parent->keys.begin() + index, mid);
            parent->children.insert(parent->children.begin() + index + 1, right);
        }

        void insert_non_full(Node<T> *node, const T &key)
        {
            int i = (int)node->keys.size() - 1;
            if (node->leaf)
            {
                while (i >= 0 && key < node->keys[i])
                    --i;
                if (i >= 0 && node->keys[i] == key)
                    return;
                node->keys.insert(node->keys.begin() + i + 1, key);
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
                insert_non_full(node->children[i], key);
            }
        }

        void delete_key(Node<T> *node, const T &key)
        {
            int index = 0;
            while (index < (int)node->keys.size() && key > node->keys[index])
                ++index;
            if (index < (int)node->keys.size() && node->keys[index] == key)
            {
                if (node->leaf)
                {
                    node->keys.erase(node->keys.begin() + index);
                    return;
                }
                Node<T> *pred = node->children[index];
                Node<T> *succ = node->children[index + 1];
                if (pred->keys.size() >= 2)
                {
                    T p = get_predecessor(pred);
                    node->keys[index] = p;
                    delete_key(pred, p);
                }
                else if (succ->keys.size() >= 2)
                {
                    T s = get_successor(succ);
                    node->keys[index] = s;
                    delete_key(succ, s);
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

        void ensure_child_has_two_keys(Node<T> *node, int index)
        {
            Node<T> *child = node->children[index];
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

        void borrow_from_prev(Node<T> *node, int index)
        {
            Node<T> *child = node->children[index];
            Node<T> *sibling = node->children[index - 1];
            child->keys.insert(child->keys.begin(), node->keys[index - 1]);
            if (!child->leaf)
                child->children.insert(child->children.begin(), sibling->children.back());
            node->keys[index - 1] = sibling->keys.back();
            sibling->keys.pop_back();
            if (!sibling->leaf)
                sibling->children.pop_back();
        }

        void borrow_from_next(Node<T> *node, int index)
        {
            Node<T> *child = node->children[index];
            Node<T> *sibling = node->children[index + 1];
            child->keys.push_back(node->keys[index]);
            if (!child->leaf)
                child->children.push_back(sibling->children.front());
            node->keys[index] = sibling->keys.front();
            sibling->keys.erase(sibling->keys.begin());
            if (!sibling->leaf)
                sibling->children.erase(sibling->children.begin());
        }

        void merge_nodes(Node<T> *node, int index)
        {
            Node<T> *left = node->children[index];
            Node<T> *right = node->children[index + 1];
            left->keys.push_back(node->keys[index]);
            left->keys.insert(left->keys.end(), right->keys.begin(), right->keys.end());
            if (!left->leaf)
                left->children.insert(left->children.end(), right->children.begin(), right->children.end());
            node->keys.erase(node->keys.begin() + index);
            node->children.erase(node->children.begin() + index + 1);
            delete right;
        }

        T get_predecessor(Node<T> *node)
        {
            while (!node->leaf)
                node = node->children.back();
            return node->keys.back();
        }

        T get_successor(Node<T> *node)
        {
            while (!node->leaf)
                node = node->children.front();
            return node->keys.front();
        }

        void inorder(Node<T> *node) const
        {
            if (!node)
                return;
            int n = (int)node->keys.size();
            for (int i = 0; i < n; ++i)
            {
                if (!node->leaf)
                    inorder(node->children[i]);
                std::cout << node->keys[i] << " ";
            }
            if (!node->leaf)
                inorder(node->children[n]);
        }

        Node<T> *build_balanced(const std::vector<T> &arr, int l, int r)
        {
            int n = r - l;
            if (n <= 0)
                return nullptr;
            if (n <= 3)
            {
                Node<T> *leaf = new Node<T>(true);
                for (int i = l; i < r; ++i)
                    leaf->keys.push_back(arr[i]);
                return leaf;
            }
            int t = n / 3;
            int a = l + t;
            int b = l + 2 * t + (n % 3 ? 1 : 0);
            Node<T> *node = new Node<T>(false);
            node->keys.push_back(arr[a]);
            node->keys.push_back(arr[b]);
            node->children.push_back(build_balanced(arr, l, a));
            node->children.push_back(build_balanced(arr, a + 1, b));
            node->children.push_back(build_balanced(arr, b + 1, r));
            return node;
        }
    };
}

#endif