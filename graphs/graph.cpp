#include <iostream>
#include "graph_list.hpp"
#include "graph_matrix.hpp"
#include "graph_algorithms.hpp"

int main()
{
    int V, E;
    bool directed;
    std::cout << "Enter number of vertices and edges: ";
    std::cin >> V >> E;
    std::cout << "Directed? (1/0): ";
    std::cin >> directed;
    GraphList g(V, directed);
    std::cout << "Enter edges (u v w):" << std::endl;
    for (int i = 0; i < E; ++i)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    std::cout << std::endl
              << "Q1: Graph Representations" << std::endl;
    g.display();
    std::cout << std::endl
              << "Q2: BFS Traversal" << std::endl;
    bfs(g, 0);
    std::cout << std::endl
              << "Q3: DFS Traversals" << std::endl;
    dfs_recursive(g, 0);
    dfs_iterative(g, 0);
    std::cout << std::endl
              << "Q4: Dijkstra's Shortest Paths" << std::endl;
    dijkstra(g, 0);
    std::cout << std::endl
              << "Q5: Prim's MST" << std::endl;
    prim_mst(g);
    if (directed)
    {
        std::cout << std::endl
                  << "Q7: Topological Sort" << std::endl;
        topological_sort(g);
        std::cout << std::endl
                  << "Q9: Strongly Connected Components (SCCs)" << std::endl;
        strongly_connected_components(g);
        std::cout << std::endl
                  << "Q8: Directed Cycle Detection: "
                  << (has_cycle_directed(g) ? "YES" : "NO") << std::endl;
    }
    else
    {
        std::cout << std::endl
                  << "Q8: Undirected Cycle Detection: "
                  << (has_cycle_undirected(g) ? "YES" : "NO") << std::endl;
        std::cout << std::endl
                  << "Q10: Bipartite Check: "
                  << (is_bipartite(g) ? "YES" : "NO") << std::endl;
    }
    return 0;
}