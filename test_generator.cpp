#include <fstream>
#include "treegenerator.h"
#include "algo2.cpp"

using namespace tree_generator_by_ouuan;

#define output "/home/mihnea_dnk/Desktop/AA/tests/test10.in"
#define reference "/home/mihnea_dnk/Desktop/AA/reference/test10.out"

#define n 100000
#define q 60000

int main() {
    srand(time(0));
    std::ofstream out;
    out.open(output);

    out << n << " " << q << std::endl;
    
    // arborele
    Tree t(n);
    // exemple de diferite apeluri pentru generarea arborilor
    // t.chain(n, 0);
    // t.complete(n, 10, 0);
    // t.completeBinary(n, 0);
    // t.silkworm(n, 0);
    for (int i = 0; i < n - 1; i++) {
        t.printEdge(i, out);
    }

    // querry-urile
    for (int i = 0; i < q; i++) {
        if (i % 2 == 0)
            out << rand() % (n - 1) + 1 << " " << rand() % (n - 1) + 1<< std::endl;
        else
            out << rand() % (n - 3) + 3 << " " << rand() % (n - 3) + 3<< std::endl;
    }
    
    out.close();

    // generare fisier referinta
    int nodes_nr, queries_nr;
    std::vector<std::vector<int>> graph;
    std::vector<std::pair<int, int>> queries;

    std::ifstream in;
    in.open(output);

    std::ofstream ref;
    ref.open(reference);

    in >> nodes_nr >> queries_nr;

    for (int i = 0; i < nodes_nr; i++) {
        std::vector<int> level;
        graph.push_back(level);
    }

    for (int i = 0; i < nodes_nr - 1; i++) {
        int parent, child;
        in >> child >> parent;
        graph[parent - 1].push_back(child);
    }

    for (int i = 0; i < queries_nr; i++) {
        std::pair<int, int> p;
        in >> p.first >> p.second;
        queries.push_back(p);
    }

    std::vector<int> answer = lca(graph, queries);

    for (int i = 0; i < answer.size(); i++) {
        ref << answer[i] << std::endl;
    }

    return 0;
}
