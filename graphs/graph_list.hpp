#ifndef GRAPH_LIST
#define GRAPH_LIST

#include <iostream>
#include <vector>
#include <list>
#include <utility>

class GraphList
{
    int vertices;
    bool directed;
    std::vector<std::list<std::pair<int, int>>> adj;

public:
    GraphList(int v, bool dir = false)
    {
        vertices = v;
        directed = dir;
        adj.resize(v);
    }

    void add_edge(int u, int v, int w = 1)
    {
        adj[u].push_back({v, w});
        if (!directed)
            adj[v].push_back({u, w});
    }

    const std::vector<std::list<std::pair<int, int>>> &get_adj() const
    {
        return adj;
    }

    int V() const
    {
        return vertices;
    }

    bool is_directed() const
    {
        return directed;
    }

    void display() const
    {
        std::cout << "Adjacency List Representation:" << std::endl;
        for (int i = 0; i < vertices; ++i)
        {
            std::cout << i << " -> ";
            for (auto &[first, second] : adj[i])
                std::cout << "(" << first << "," << second << ") ";
            std::cout << std::endl;
        }
    }
};

#endif