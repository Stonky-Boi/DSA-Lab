#ifndef GRAPH_MATRIX
#define GRAPH_MATRIX

#include <iostream>
#include <vector>
#include <iomanip>

class GraphMatrix
{
    int vertices;
    bool directed;
    std::vector<std::vector<int>> mat;

public:
    GraphMatrix(int v, bool dir = false)
    {
        vertices = v;
        directed = dir;
        mat.resize(v);
        for (int i = 0; i < v; i++)
            mat[i].resize(v, 0);
    }

    void add_edge(int u, int v, int w = 1)
    {
        mat[u][v] = w;
        if (!directed)
            mat[v][u] = w;
    }

    void display() const
    {
        std::cout << "Adjacency Matrix Representation:" << std::endl;
        for (int i = 0; i < vertices; ++i)
        {
            for (int j = 0; j < vertices; ++j)
                std::cout << std::setw(3) << mat[i][j] << " ";
            std::cout << std::endl;
        }
    }
};

#endif