#include <fstream>
#include <iostream>
#include "algo.h"
#include <chrono>
#include <unistd.h>

int main() {
    int nodes_nr, queries_nr;
    std::vector<std::vector<int>> graph;
    std::vector<std::pair<int, int>> queries;

    std::ifstream in;
    in.open("./test.in");

    std::ofstream out;
    out.open("./test.out");

    in >> nodes_nr >> queries_nr;

    for (int i = 0; i < nodes_nr; i++) {
        std::vector<int> level;
        graph.push_back(level);
    }

    // infoarena testing
    /*for (int i = 1; i < nodes_nr; i++) {
        int parent;
        in >> parent;
        graph[parent - 1].push_back(i + 1);
    }*/

    // my testing
    for (int i = 1; i < nodes_nr; i++) {
        int parent, child;
        in >> child >> parent;
        graph[parent - 1].push_back(child);
    }

    for (int i = 0; i < queries_nr; i++) {
        std::pair<int, int> p;
        in >> p.first >> p.second;
        queries.push_back(p);
    }
    auto start = std::chrono::steady_clock::now();
    std::vector<int> answer = lca(graph, queries);
    auto end = std::chrono::steady_clock::now();
    for (long unsigned int i = 0; i < answer.size(); i++) {
        out << answer[i] << std::endl;
    }

    std::cout << "Elapsed time in microseconds: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<<std::endl;

    in.close();
    out.close();

    return 0;
}
