#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

bool dfs(int u, int parent, const vector<vector<pair<int, int>>> &adj, vector<bool> &visited)
{
    visited[u] = true;
    for (auto [v, k] : adj[u])
    {
        if (!visited[v])
        {
            if (dfs(v, u, adj, visited))
                return true;
        }
        else if (v != parent)
            return true;
    }
    return false;
}

bool cycle(const vector<vector<pair<int, int>>> &adj)
{
    vector<bool> visited(adj.size(), false);
    for (int i = 0; i < adj.size(); i++)
        if (!visited[i] && dfs(i, -1, adj, visited))
            return true;
    return false;
}

void prim(const vector<vector<pair<int, int>>> &adj)
{
    int n = adj.size();
    vector<int> key(n, numeric_limits<int>::max());
    vector<int> parent(n, -1);
    vector<bool> in_mst(n, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    key[0] = 0;
    pq.push({key[0], 0});
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        if (in_mst[u])
            continue;
        in_mst[u] = true;
        for (auto [v, k] : adj[u])
            if (!in_mst[v] && k < key[v])
            {
                key[v] = k;
                parent[v] = u;
                pq.push({key[v], v});
            }
    }
    int cost = 0;
    for (int i = 0; i < n; i++)
    {
        cost += key[i];
        cout << parent[i] << " " << i << " " << key[i] << endl;
    }
    cout << "Total Annual Contract Cost: " << cost << endl;
}

int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        int n;
        cin >> n;
        vector<vector<pair<int, int>>> adj(n);
        int m;
        cin >> m;
        for (int i = 0; i < m; i++)
        {
            int u, v, k;
            cin >> u >> v >> k;
            adj[u].push_back({v, k});
            adj[v].push_back({u, k});
        }

        // Objective 1
        cout << (cycle(adj) ? "CIRCULAR DEPENDENCY DETECTED" : "NO CIRCULAR DEPENDENCY") << endl;
        // Objective 2
        prim(adj);
    }
    return 0;
}