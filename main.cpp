#include <vector>
#include <list>
#include <utility>
#include <cstdlib>

//	1)  Initialize contracted graph CG as copy of original graph
//	2)  While there are more than 2 vertices.
//    	a) Pick a random edge (u, v) in the contracted graph.
//      	b) Merge (or contract) u and v into a single vertex (update 
//       	  the contracted graph).
//     	 c) Remove self-loops
//	3) Return cut represented by two vertices.

struct Edge {
	int multiplicidade;
	int std::vector<std::pair<int, int>> EdgeIds;
	int destinyVertex;
	std::list<Edge>::iterator reversePointer;
};

void preencherAdjList(const std::string &filename, std::vector<std::list<Edge>> &adjList, int &numArestas) {
	int numVer;
	std::ifstream file (filename);
	file >> numVer >> numArestas;
	adjList.resize(numVer);

	int orig, dest;
	Edge e1, e2;
	e1.multiplicidade = e2.multiplicidade = 1;

	for (int i = 0; i < numArestas; ++i) {
		file >> orig >> dest;
		e1.EdgeIds.push_back(std::pair<int,int>(orig, dest));
		e2.EdgeIds.push_back(std::pair<int,int>(orig, dest));

		e1.destinyVertex = dest;
		e2.destinyVertex = orig;

		adjList[orig].push_back(e1);
		adjList[dest].push_back(e2);

		(--adjList[orig].end())->reversePointer = --adjList[dest].end();
		(--adjList[dest].end())->reversePointer = --adjList[orig].end();
	}

	file.close();
}

void merge(std::vector<std::list<Edge>> &listaCopia, const int &originVertex, const int &destinyVertex, int &numArestas) {
	for (auto edge = listaCopia[destinyVertex].begin(); edge != listaCopia[destinyVertex].end(); edge++) {
		// if edge[i] for para origin, delete this
		if (edge->destinyVertex == originVertex) {
			numArestas -= edge->multiplicidade;
			
			auto reverseIt = edge->reversePointer;
			auto itselfIt = reverseIt->reversePointer;

			listaCopia[originVertex].erase(reverseIt);
			listaCopia[destinyVertex].erase(itselfIt);

			continue;
		}

		// Verifica se a aresta está no vertice original
		std::list<Edge>::iterator found = nullptr;
		for (auto e = listaCopia[originVertex].begin(); e != listaCopia[originVertex].end(); e++)
			if (e->destinyVertex == edge->destinyVertex) {
				found = e;
				break;
			}
		
		// if edge[i] já tiver lá, aumenta a multiplicidade
		if (found != nullptr) {
			found->multiplicidade += edge->multiplicidade;
			found->EdgeIds.insert(found->EdgeIds.end(), edge->EdgeIds.begin(), edge->EdgeIds.end());
		} else {
		// senao copia edge[i] para originVertex
			Edge &destiny = *edge.reversePointer;
			auto newEdgeIt = listaCopia[originVertex].insert(listaCopia[originVertex].end(), *edge);
			destiny.reversePointer = newEdgeIt;
		}
	}

	listaCopia[destinyVertex].clear();
}

int main(int argc, char const *argv[]) {
	std::vector<std::list<Edge>> listaCopia;
	srand(time());

	int numArestas;
	
	preencherAdjList(listaCopia, numArestas);

	// listaCopia = adjList;
	for (int i = 0; i < adjList.size() - 2; i++) {
		int selectedEdge = (rand() % numArestas) + 1;
		int origVer = -1;

		// for que percorre e seleciona a aresta aleatória
		std::list<Edge>::iterator it;
		for (int i = 0; i < selectedEdge; /* empty */) {
			origVer++;
			for (it = listaCopia[origVer].begin(); it != listaCopia[origVer].end(); it++) {
				if (it->destinyVertex > origVer) i += it->multiplicidade;
				if (i >= selectedEdge) break;
			}
		}

		// resultados finais usados em merge
		merge(listaCopia, origVer, it->destinyVertex, numArestas);
	}

	// percorre até encontrar a última aresta
	// corte mínimo será EdgeIds
	// tamanho do corte mínimo será EdgeIds.size()
	std::list<Edge>::iterator resEdge;
	std::vector<std::pair<int, int>> res;
	for (int i = 0; i < listaCopia.size(); i++)
		if (!listaCopia[i].empty()) {
			res = listaCopia[i].begin()->EdgeIds;
			break;
		}

	std::cout << "Tamanho do corte mínimo: " << res.size() << "\n";
	std::cout << "O corte mínimo é formado pelas arestas: ";
	for (int i = 0; i < res.size(); ++i) std::cout << "(" << res[i].first << ", " << res[i].second << ") ";
	std::cout << "\n";
	
	return 0;
}