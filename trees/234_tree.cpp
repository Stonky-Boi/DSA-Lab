#include <iostream>
#include <string>
#include <vector>
#include "234_tree.hpp"
#include "234_kv_tree.hpp"

int main()
{
    tree234::Tree234<int> t;
    tree234::Tree234KV<int, int> kv;
    std::string command;
    while (std::cin >> command)
    {
        if (command == "INSERT")
        {
            int x;
            std::cin >> x;
            t.insert(x);
        }
        else if (command == "DELETE")
        {
            int x;
            std::cin >> x;
            t.remove(x);
        }
        else if (command == "FIND")
        {
            int x;
            std::cin >> x;
            std::cout << (t.find(x) ? "FOUND" : "NOTFOUND") << std::endl;
        }
        else if (command == "PRINT")
        {
            std::string which;
            std::cin >> which;
            if (which == "LEVEL")
                t.print_level();
            else if (which == "IN")
                t.print_inorder();
        }
        else if (command == "BULK")
        {
            int n;
            std::cin >> n;
            std::vector<int> arr(n);
            for (int i = 0; i < n; ++i)
                std::cin >> arr[i];
            t.bulk_build(arr);
        }
        else if (command == "PUT")
        {
            int k, v;
            std::cin >> k >> v;
            kv.put(k, v);
        }
        else if (command == "GET")
        {
            int k;
            std::cin >> k;
            kv.get(k);
        }
        else if (command == "RANGE")
        {
            int L, R;
            std::cin >> L >> R;
            kv.range(L, R);
        }
        else if (command == "BATCH_DELETE")
        {
            int m;
            std::cin >> m;
            std::vector<int> ks(m);
            for (int i = 0; i < m; ++i)
                std::cin >> ks[i];
            kv.batch_delete(ks);
        }
    }
    return 0;
}