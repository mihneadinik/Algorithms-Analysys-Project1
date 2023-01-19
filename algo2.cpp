#include "algo.h"
#include <cmath>

// functia de populare a tabelei in functie de nivelele din parcurgerea Euler
int** create_table(std::vector<int> &levels) {
    int **table = new int* [levels.size()];
	for (long unsigned int i = 0; i < levels.size(); ++i) {
		table[i] = new int [20] {0};
	}

    // incepem sa formam tabela punand indicele pe prima coloana de pe fiecare linie
	for (long unsigned int i = 0; i < levels.size(); i++) {
        table[i][0] = i;
    }

    // populam tabela punand pozitiile minime din interval
	for (long unsigned int j = 1; (long unsigned int)(1 << j) <= levels.size(); ++j) {
		for (long unsigned int i = 0; (i + (1 << j) - 1) < levels.size(); ++i) {
			if (levels[table[i][j - 1]] < levels[table[i + (1 << (j - 1))][j - 1]])
                table[i][j] = table[i][j - 1];
            else
                table[i][j] = table[i + (1 << (j - 1))][j - 1];
		}
	}

    return table;
}

// functia care raspunde fiecarei interogari in parte
int answer_querry(int &node1, int &node2, int *first_app,
	std::vector<int> &EulerTour, std::vector<int> &levels, int **table) {
    // calculam capetele intervalului in functie de prima aparitie a nodului
	int L = first_app[node1 - 1];
	int R = first_app[node2 - 1];

    // daca sunt capetele inversate, le schimbam pozitia, intervalul ramane acelasi
	if (L > R) {
        int aux = L;
        L = R;
        R = aux;
    }

	int pos = (int)log2(R - L + 1);

    // aflam indicele nivelului minim din interval si pe baza lui returnam
    // nodul corespunzator din parcurgere
    if (levels[table[L][pos]] <= levels[table[R - (1 << pos) + 1][pos]])
        return EulerTour[table[L][pos]];

    return EulerTour[table[R - (1 << pos) + 1][pos]];
}



// functia de parcurgere a grafului pe baza careia se va realiza parcurgerea euleriana
void dfs(int &node, int level, std::vector<int> &EulerTour, std::vector<int> &levels,
        int* first_app, std::vector<std::vector<int>> &graph) {
    // actualizam vectorii de informatii
    EulerTour.push_back(node);
    levels.push_back(level);
    first_app[node - 1] = EulerTour.size() - 1;

    // mergem pe fiecare copil in parte
    for (long unsigned int i = 0; i < graph[node - 1].size(); i++) {
        // daca nu am mai vizitat copilul
        if (first_app[graph[node - 1][i] - 1] == -1) {
            // vizitez nodul
            dfs(graph[node - 1][i], level + 1, EulerTour, levels, first_app, graph);
            // cand ma intorc, marchez din nou nodul in vectorii de informatii
            EulerTour.push_back(node);
            levels.push_back(level);
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

    // creez vectorii de care ma voi folosi in rezolvarea cerintei

    // pentru parcurgerea euleriana
    std::vector<int> EulerTour, levels;
    // vector care va contine raspunsul fiecarei interogari 
    std::vector<int> queries_answer (queries.size());
    // vector care retine ultima aparitie (toate elementele setate pe -1)
    int *first_app = new int[graph.size()];
    for (long unsigned int i = 0; i < graph.size(); i++) {
        first_app[i] = -1;
    }

    // pornim programul din nodul de radacina ca sa completam
    // vectorii necesari RMQ
    int node = 1;
    dfs(node, 0, EulerTour, levels, first_app, graph);

    // cream tabela de lookup si o populam
	int **table = create_table(levels);

    // raspundem querry-urilor
	for (long unsigned int i = 0; i < queries.size(); ++i) {
		queries_answer[i] = answer_querry(queries[i].first, queries[i].second,
			first_app, EulerTour, levels, table);
	}

    // stergem vectorii alocati dinamic
	delete [] first_app;
	for (long unsigned int i = 0; i < levels.size(); ++i) {
		delete [] table[i];
	}
	delete [] table;

	return queries_answer;
}
