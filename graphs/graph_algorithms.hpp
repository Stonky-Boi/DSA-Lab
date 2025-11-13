#ifndef GRAPH_ALGORITHMS
#define GRAPH_ALGORITHMS

#include "graph_list.hpp"
#include <queue>
#include <stack>
#include <set>
#include <limits>
#include <algorithm>

void bfs(const GraphList &graph, int start)
{
    int n = graph.V();
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    std::cout << "BFS Traversal: ";
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        std::cout << u << " ";
        for (auto [vertex, weight] : graph.get_adj()[u])
            if (!visited[vertex])
            {
                visited[vertex] = true;
                q.push(vertex);
            }
    }
    std::cout << std::endl;
}

void dfs_recursive_util(int u, const GraphList &graph, std::vector<bool> &visited)
{
    visited[u] = true;
    std::cout << u << " ";
    for (auto [vertex, weight] : graph.get_adj()[u])
        if (!visited[vertex])
            dfs_recursive_util(vertex, graph, visited);
}

void dfs_recursive(const GraphList &graph, int start)
{
    std::vector<bool> visited(graph.V(), false);
    std::cout << "DFS (Recursive): ";
    dfs_recursive_util(start, graph, visited);
    std::cout << std::endl;
}

void dfs_iterative(const GraphList &graph, int start)
{
    std::vector<bool> visited(graph.V(), false);
    std::stack<int> st;
    st.push(start);
    std::cout << "DFS (Iterative): ";
    while (!st.empty())
    {
        int u = st.top();
        st.pop();
        if (!visited[u])
        {
            visited[u] = true;
            std::cout << u << " ";
            for (auto it = graph.get_adj()[u].rbegin(); it != graph.get_adj()[u].rend(); ++it)
                if (!visited[it->first])
                    st.push(it->first);
        }
    }
    std::cout << std::endl;
}

void dijkstra(const GraphList &graph, int start)
{
    int n = graph.V();
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    dist[start] = 0;
    using pii = std::pair<int, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<>> pq;
    pq.push({0, start});
    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u])
            continue;
        for (auto [v, w] : graph.get_adj()[u])
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
    }
    std::cout << "Dijkstra from " << start << ":" << std::endl;
    for (int i = 0; i < n; ++i)
        std::cout << "  " << i << ": " << (dist[i] == std::numeric_limits<int>::max() ? -1 : dist[i]) << std::endl;
}

void prim_mst(const GraphList &graph)
{
    int n = graph.V();
    std::vector<int> key(n, std::numeric_limits<int>::max()), parent(n, -1);
    std::vector<bool> inMST(n, false);
    using pii = std::pair<int, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<>> pq;
    key[0] = 0;
    pq.push({0, 0});
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        if (inMST[u])
            continue;
        inMST[u] = true;
        for (auto [v, w] : graph.get_adj()[u])
            if (!inMST[v] && w < key[v])
            {
                key[v] = w;
                parent[v] = u;
                pq.push({key[v], v});
            }
    }
    std::cout << "Prim's MST Edges:" << std::endl;
    for (int i = 1; i < n; ++i)
        std::cout << parent[i] << " - " << i << " (w=" << key[i] << ")" << std::endl;
}

void topological_sort(const GraphList &graph)
{
    int n = graph.V();
    std::vector<int> indeg(n, 0);
    for (int u = 0; u < n; ++u)
        for (auto [vertex, weight] : graph.get_adj()[u])
            indeg[vertex]++;
    std::queue<int> q;
    for (int i = 0; i < n; ++i)
        if (indeg[i] == 0)
            q.push(i);
    std::cout << "Topological Order: ";
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        std::cout << u << " ";
        for (auto [vertex, weight] : graph.get_adj()[u])
            if (--indeg[vertex] == 0)
                q.push(vertex);
    }
    std::cout << std::endl;
}

bool dfs_cycle_undirected(int u, int parent, const GraphList &g, std::vector<bool> &vis)
{
    vis[u] = true;
    for (auto [v, w] : g.get_adj()[u])
    {
        if (!vis[v])
        {
            if (dfs_cycle_undirected(v, u, g, vis))
                return true;
        }
        else if (v != parent)
            return true;
    }
    return false;
}

bool has_cycle_undirected(const GraphList &graph)
{
    std::vector<bool> vis(graph.V(), false);
    for (int i = 0; i < graph.V(); ++i)
        if (!vis[i] && dfs_cycle_undirected(i, -1, graph, vis))
            return true;
    return false;
}

bool dfs_cycle_directed(int u, const GraphList &g, std::vector<int> &state)
{
    state[u] = 1;
    for (auto [v, w] : g.get_adj()[u])
    {
        if (state[v] == 1)
            return true;
        if (state[v] == 0 && dfs_cycle_directed(v, g, state))
            return true;
    }
    state[u] = 2;
    return false;
}

bool has_cycle_directed(const GraphList &graph)
{
    std::vector<int> state(graph.V(), 0);
    for (int i = 0; i < graph.V(); ++i)
        if (state[i] == 0 && dfs_cycle_directed(i, graph, state))
            return true;
    return false;
}

void dfs_fill(int u, const GraphList &g, std::vector<bool> &vis, std::stack<int> &st)
{
    vis[u] = true;
    for (auto [v, w] : g.get_adj()[u])
        if (!vis[v])
            dfs_fill(v, g, vis, st);
    st.push(u);
}

void dfs_scc(int u, const GraphList &g, std::vector<bool> &vis)
{
    vis[u] = true;
    std::cout << u << " ";
    for (auto [v, w] : g.get_adj()[u])
        if (!vis[v])
            dfs_scc(v, g, vis);
}

GraphList get_transpose(const GraphList &g)
{
    GraphList gt(g.V(), true);
    for (int u = 0; u < g.V(); ++u)
        for (auto [v, w] : g.get_adj()[u])
            gt.add_edge(v, u, w);
    return gt;
}

void strongly_connected_components(const GraphList &graph)
{
    int n = graph.V();
    std::vector<bool> vis(n, false);
    std::stack<int> st;
    for (int i = 0; i < n; ++i)
        if (!vis[i])
            dfs_fill(i, graph, vis, st);
    GraphList gt = get_transpose(graph);
    std::fill(vis.begin(), vis.end(), false);
    std::cout << "SCCs:" << std::endl;
    while (!st.empty())
    {
        int u = st.top();
        st.pop();
        if (!vis[u])
        {
            dfs_scc(u, gt, vis);
            std::cout << std::endl;
        }
    }
}

bool is_bipartite(const GraphList &graph)
{
    int n = graph.V();
    std::vector<int> color(n, -1);
    std::queue<int> q;
    for (int i = 0; i < n; ++i)
        if (color[i] == -1)
        {
            color[i] = 0;
            q.push(i);
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                for (auto [v, w] : graph.get_adj()[u])
                {
                    if (color[v] == -1)
                    {
                        color[v] = 1 - color[u];
                        q.push(v);
                    }
                    else if (color[v] == color[u])
                        return false;
                }
            }
        }
    return true;
}

#endif