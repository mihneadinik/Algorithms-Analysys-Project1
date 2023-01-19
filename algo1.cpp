#include "algo.h"

// functie care gaseste nodul de baza al unei multimi, pornind de la
// oricare alt nod din ea
int find_parent(int &node, int *parent) {
    if (node != parent[node - 1])
        parent[node - 1] = find_parent(parent[node - 1], parent);

    return parent[node - 1];
}

// functie care realizeaza includerea a 2 noduri in aceeasi uniune
void make_union(int &node, int &child, int *parent, int *rank) {
    int par1 = find_parent(node, parent);
    int par2 = find_parent(child, parent);

    // daca nu fac deja parte din aceeasi multime, le unim
    if (par1 != par2) {
        // punem nodul cu rang mai mare drept parintele celui mic
        if (rank[par1 - 1] < rank[par2 - 1]) {
            parent[par1 - 1] = par2;
        } else {
            parent[par2 - 1] = par1;
        }
    // daca au rankurile egale, unul trebuie incrementat
        if (rank[par1 - 1] == rank[par2 - 1])
            rank[par1 - 1]++;
    }
}

// functia de parcuregere a grafului, pe baza careia se vor crea uniunile
// de multimi si se va raspunde, pe rand, la interogari
void dfs(int &node, bool *visited, int *parent, int *ancestor, int *rank,
        std::vector<int> &queries_answer, std::vector<std::vector<int>> &graph,
        std::vector<std::pair<int, int>> &queries) {
    // actualizam vectorii de informatii
    visited[node - 1] = 1;
    rank[node - 1] = 1;
    parent[node - 1] = node;
    ancestor[node - 1] = node;

    // mergem pe fiecare copil nevizitat si refacem uniunea
    for (long unsigned int i = 0; i < graph[node - 1].size(); i++) {
        if (!visited[graph[node - 1][i] - 1]) {
            dfs(graph[node - 1][i], visited, parent, ancestor,
                rank, queries_answer, graph, queries);
            make_union(node, graph[node - 1][i], parent, rank);
            // actualizam stramosul uniunii din subarbore drept nodul curent
            ancestor[find_parent(node, parent) - 1] = node;
        }
    }

    // cu fiecare nod parcurs, verificam daca putem realiza vreo interogare
    for (long unsigned int i = 0; i < queries.size(); i++) {
        // daca nodul curent este primul termen din interogare
        if (queries[i].first == node) {
            // daca si al 2-lea nod a fost vizitat
            if (visited[queries[i].second - 1]) {
                // putem sa raspundem la interogare
                queries_answer[i] = ancestor[find_parent(queries[i].second, parent) - 1];
            }
        } else {
            // verificam pentru inversul nodurilor
            if (queries[i].second == node) {
                if (visited[queries[i].first - 1]) {
                    queries_answer[i] = ancestor[find_parent(queries[i].first, parent) - 1];
                }
            }
        }
    }
}

// functia primeste un vector de vectori reprezentand graful (pentru fiecare
// din cele n noduri avem un vector de copii) si un vectori de perechi
// reprezentand query-urile (dimenisune m) si returneaza un vector de
// raspunsuri la interogarile cerute (tot dimensiune m)
// Atentie! nodul i se afla pe pozitia i - 1 in vectorul graph
std::vector<int> lca (std::vector<std::vector<int>> &graph,
        std::vector<std::pair<int, int>> &queries) {

    // creez vectorii necesari solutionarii algoritmului

    // vector care va contine raspunsul fiecarei interogari 
    std::vector<int> queries_answer (queries.size());
    // vector care tine evidenta nodurilor vizitate, initial 0
    bool *visited = new bool[graph.size()] {0};
    // vector care tine minte parintele fiecarui nod din graf
    int *parent = new int[graph.size()];
    // vector care tine cont de stramosul fiecarui nod dintr-o uniune
    int *ancestor = new int[graph.size()];
    // vector care retine rankul fiecarui nod
    int *rank = new int[graph.size()];

    // pornim programul din primul nod
    int node = 1;
    dfs(node, visited, parent, ancestor, rank, queries_answer, graph, queries);

    // stergem vectorii alocati dinamic
    delete [] visited;
    delete [] parent;
    delete [] ancestor;
    delete [] rank;

    return queries_answer;
}
