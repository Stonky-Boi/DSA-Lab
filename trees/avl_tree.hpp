#ifndef avl_tree
#define avl_tree

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

namespace avl
{
    template <typename T>
    class Node
    {
    public:
        T key;
        Node *left;
        Node *right;
        Node *parent;
        int height;
        int size;
        long long subtree_sum;

        Node(const T &k)
        {
            key = k;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            height = 1;
            size = 1;
            subtree_sum = k;
        }
    };

    template <typename T>
    class AVLTree
    {
    public:
        Node<T> *root;

        AVLTree()
        {
            root = nullptr;
        }

        ~AVLTree()
        {
            destroy(root);
        }

        void insert(const T &k)
        {
            root = insert_helper(root, k, nullptr);
        }

        void remove(const T &k)
        {
            root = remove_helper(root, k);
            if (root)
                root->parent = nullptr;
        }

        bool find(const T &k) const
        {
            Node<T> *curr = root;
            while (curr)
            {
                if (k == curr->key)
                    return true;
                if (k < curr->key)
                    curr = curr->left;
                else
                    curr = curr->right;
            }
            return false;
        }

        std::string prev_key(const T &k) const
        {
            Node<T> *curr = root;
            Node<T> *ans = nullptr;
            while (curr)
            {
                if (curr->key < k)
                {
                    ans = curr;
                    curr = curr->right;
                }
                else
                    curr = curr->left;
            }
            if (ans)
                return std::to_string(ans->key);
            return "NONE";
        }

        std::string next_key(const T &k) const
        {
            Node<T> *curr = root;
            Node<T> *ans = nullptr;
            while (curr)
            {
                if (curr->key > k)
                {
                    ans = curr;
                    curr = curr->left;
                }
                else
                    curr = curr->right;
            }
            if (ans)
                return std::to_string(ans->key);
            return "NONE";
        }

        void print_inorder() const
        {
            if (!root)
            {
                std::cout << "EMPTY" << std::endl;
                return;
            }
            bool first = true;
            inorder_print(root, first);
            std::cout << std::endl;
        }

        void print_preorder() const
        {
            if (!root)
            {
                std::cout << "EMPTY" << std::endl;
                return;
            }
            bool first = true;
            preorder_print(root, first);
            std::cout << std::endl;
        }

        void print_postorder() const
        {
            if (!root)
            {
                std::cout << "EMPTY" << std::endl;
                return;
            }
            bool first = true;
            postorder_print(root, first);
            std::cout << std::endl;
        }

        void print_levelorder() const
        {
            if (!root)
            {
                std::cout << "EMPTY" << std::endl;
                return;
            }
            std::queue<Node<T> *> q;
            q.push(root);
            bool first = true;
            while (!q.empty())
            {
                Node<T> *node = q.front();
                q.pop();
                if (!first)
                    std::cout << " ";
                std::cout << node->key;
                first = false;
                if (node->left)
                    q.push(node->left);
                if (node->right)
                    q.push(node->right);
            }
            std::cout << std::endl;
        }

        long long range_count(const T &L, const T &R) const
        {
            if (L > R)
                return 0;
            long long a = count_leq(R);
            long long b = count_leq(L - 1);
            return a - b;
        }

        long long rank_of(const T &x) const
        {
            return rank_helper(root, x);
        }

        std::string select_kth(int k) const
        {
            Node<T> *res = select_helper(root, k);
            if (!res)
                return "INVALID";
            return std::to_string(res->key);
        }

        long long range_sum(const T &L, const T &R) const
        {
            if (L > R)
                return 0;
            long long a = prefix_sum(R);
            long long b = prefix_sum(L - 1);
            return a - b;
        }

    private:
        void destroy(Node<T> *node)
        {
            if (!node)
                return;
            destroy(node->left);
            destroy(node->right);
            delete node;
        }

        int node_height(Node<T> *node) const
        {
            return node ? node->height : 0;
        }

        int node_size(Node<T> *node) const
        {
            return node ? node->size : 0;
        }

        long long node_sum(Node<T> *node) const
        {
            return node ? node->subtree_sum : 0LL;
        }

        void update_node(Node<T> *node)
        {
            if (!node)
                return;
            node->height = 1 + std::max(node_height(node->left), node_height(node->right));
            node->size = 1 + node_size(node->left) + node_size(node->right);
            node->subtree_sum = node->key + node_sum(node->left) + node_sum(node->right);
            if (node->left)
                node->left->parent = node;
            if (node->right)
                node->right->parent = node;
        }

        int balance_factor(Node<T> *node) const
        {
            if (!node)
                return 0;
            return node_height(node->left) - node_height(node->right);
        }

        Node<T> *rotate_right(Node<T> *y)
        {
            Node<T> *x = y->left;
            Node<T> *t = x->right;
            x->right = y;
            y->left = t;
            x->parent = y->parent;
            y->parent = x;
            if (t)
                t->parent = y;
            update_node(y);
            update_node(x);
            return x;
        }

        Node<T> *rotate_left(Node<T> *x)
        {
            Node<T> *y = x->right;
            Node<T> *t = y->left;
            y->left = x;
            x->right = t;
            y->parent = x->parent;
            x->parent = y;
            if (t)
                t->parent = x;
            update_node(x);
            update_node(y);
            return y;
        }

        Node<T> *rebalance(Node<T> *node)
        {
            if (!node)
                return node;
            update_node(node);
            int bf = balance_factor(node);
            if (bf > 1 && balance_factor(node->left) >= 0)
                return rotate_right(node);
            if (bf > 1 && balance_factor(node->left) < 0)
            {
                node->left = rotate_left(node->left);
                return rotate_right(node);
            }
            if (bf < -1 && balance_factor(node->right) <= 0)
                return rotate_left(node);
            if (bf < -1 && balance_factor(node->right) > 0)
            {
                node->right = rotate_right(node->right);
                return rotate_left(node);
            }
            return node;
        }

        Node<T> *insert_helper(Node<T> *node, const T &k, Node<T> *parent)
        {
            if (!node)
            {
                Node<T> *new_node = new Node<T>(k);
                new_node->parent = parent;
                return new_node;
            }
            if (k < node->key)
                node->left = insert_helper(node->left, k, node);
            else if (k > node->key)
                node->right = insert_helper(node->right, k, node);
            else
                return node;
            return rebalance(node);
        }

        Node<T> *min_node(Node<T> *node) const
        {
            Node<T> *curr = node;
            while (curr && curr->left)
                curr = curr->left;
            return curr;
        }

        Node<T> *remove_helper(Node<T> *node, const T &k)
        {
            if (!node)
                return node;
            if (k < node->key)
                node->left = remove_helper(node->left, k);
            else if (k > node->key)
                node->right = remove_helper(node->right, k);
            else
            {
                if (!node->left || !node->right)
                {
                    Node<T> *temp = node->left ? node->left : node->right;
                    delete node;
                    return temp;
                }
                else
                {
                    Node<T> *succ = min_node(node->right);
                    node->key = succ->key;
                    node->right = remove_helper(node->right, succ->key);
                }
            }
            return rebalance(node);
        }

        void inorder_print(Node<T> *node, bool &first) const
        {
            if (!node)
                return;
            inorder_print(node->left, first);
            if (!first)
                std::cout << " ";
            std::cout << node->key;
            first = false;
            inorder_print(node->right, first);
        }

        void preorder_print(Node<T> *node, bool &first) const
        {
            if (!node)
                return;
            if (!first)
                std::cout << " ";
            std::cout << node->key;
            first = false;
            preorder_print(node->left, first);
            preorder_print(node->right, first);
        }

        void postorder_print(Node<T> *node, bool &first) const
        {
            if (!node)
                return;
            postorder_print(node->left, first);
            postorder_print(node->right, first);
            if (!first)
                std::cout << " ";
            std::cout << node->key;
            first = false;
        }

        long long count_leq(const T &x) const
        {
            Node<T> *curr = root;
            long long count = 0;
            while (curr)
            {
                if (curr->key <= x)
                {
                    count += 1 + node_size(curr->left);
                    curr = curr->right;
                }
                else
                    curr = curr->left;
            }
            return count;
        }

        long long rank_helper(Node<T> *node, const T &x) const
        {
            if (!node)
                return 0;
            if (x <= node->key)
                return rank_helper(node->left, x);
            else
                return 1 + node_size(node->left) + rank_helper(node->right, x);
        }

        Node<T> *select_helper(Node<T> *node, int k) const
        {
            if (!node)
                return nullptr;
            int left_size = node_size(node->left);
            if (k == left_size + 1)
                return node;
            if (k <= left_size)
                return select_helper(node->left, k);
            else
                return select_helper(node->right, k - left_size - 1);
        }

        long long prefix_sum(const T &x) const
        {
            Node<T> *curr = root;
            long long total = 0;
            while (curr)
            {
                if (curr->key <= x)
                {
                    total += curr->key + node_sum(curr->left);
                    curr = curr->right;
                }
                else
                    curr = curr->left;
            }
            return total;
        }
    };
}

#endif