#include <iostream>
#include <string>
#include "avl_tree.hpp"

int main()
{
    avl::AVLTree<int> tree;
    std::string command;
    while (std::cin >> command)
    {
        if (command == "INSERT")
        {
            int x;
            std::cin >> x;
            tree.insert(x);
        }
        else if (command == "DELETE")
        {
            int x;
            std::cin >> x;
            tree.remove(x);
        }
        else if (command == "FIND")
        {
            int x;
            std::cin >> x;
            std::cout << (tree.find(x) ? "FOUND" : "NOTFOUND") << std::endl;
        }
        else if (command == "PREV")
        {
            int x;
            std::cin >> x;
            std::cout << tree.prev_key(x) << std::endl;
        }
        else if (command == "NEXT")
        {
            int x;
            std::cin >> x;
            std::cout << tree.next_key(x) << std::endl;
        }
        else if (command == "RANK")
        {
            int x;
            std::cin >> x;
            std::cout << tree.rank_of(x) << std::endl;
        }
        else if (command == "SELECT")
        {
            int k;
            std::cin >> k;
            std::cout << tree.select_kth(k) << std::endl;
        }
        else if (command == "RANGE_SUM")
        {
            int L, R;
            std::cin >> L >> R;
            std::cout << tree.range_sum(L, R) << std::endl;
        }
        else if (command == "RANGE_COUNT")
        {
            int L, R;
            std::cin >> L >> R;
            std::cout << tree.range_count(L, R) << std::endl;
        }
        else if (command == "PRINT")
        {
            std::string which;
            std::cin >> which;
            if (which == "IN")
                tree.print_inorder();
            else if (which == "PRE")
                tree.print_preorder();
            else if (which == "POST")
                tree.print_postorder();
            else if (which == "LEVEL")
                tree.print_levelorder();
        }
    }
    return 0;
}