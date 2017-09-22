#include <vector>
#include <list>
#include <cstdlib>

//	1)  Initialize contracted graph CG as copy of original graph
//	2)  While there are more than 2 vertices.
//    	a) Pick a random edge (u, v) in the contracted graph.
//      	b) Merge (or contract) u and v into a single vertex (update 
//       	  the contracted graph).
//     	 c) Remove self-loops
//	3) Return cut represented by two vertices.

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
	Funções do projeto de coloração, precisam ser adaptadas para as novas
	estruturas.
*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



void buildLists(const std::vector<std::vector<bool>> &adjMatrix,
 				std::vector<std::list<std::pair<int, ReversePointer>>> &adjList) {
	for (unsigned int i = 0; i < adjMatrix.size(); ++i) {
		for (unsigned int j = i+1; j < adjMatrix.size(); ++j) {
			if (adjMatrix[i][j]) {
				adjList[i].push_back(std::pair<int, ReversePointer> (j, ReversePointer()));
				adjList[j].push_back(std::pair<int, ReversePointer> (i, ReversePointer()));

				(--adjList[i].end())->second.it = --adjList[j].end();
				(--adjList[j].end())->second.it = --adjList[i].end();
			}
		}
	}
}

void fillAdjMatrixFromFile(std::string filename, int &numVer, std::vector<std::vector<bool>> &adjMat){
    std::ifstream file(filename);
    std::string file_line;
    char aux[1000];
    
    do {
        file.getline(aux, 1000);
        file_line = aux;
    } while (file_line[0] != 'p');
    
    file_line.erase(0, file_line.find_last_of("e") + 2);
    file_line.erase(file_line.find_last_of(" "), file_line.size());

    numVer = stoi(file_line);

    adjMat.resize(numVer, std::vector<bool>(numVer));

    do{
        file.getline(aux, 1000);
        file_line = aux;
        file_line.erase(0, 2);
        std::string sV1 = file_line, sV2 = file_line;
        sV1.erase(sV1.find_last_of(" ") + 1, sV1.size());
        sV2.erase(0, sV1.find_last_of(" ") + 1);
        
        int v1 = stoi(sV1); //vertices on file start counting on 1
        int v2 = stoi(sV2); //vertices on file start counting on 1
        adjMat[v1-1][v2-1] = 1; //vertices on matrix start counting on 0
        adjMat[v2-1][v1-1] = 1; //vertices on matrix start counting on 0
    } while(!file.eof());
    
    file.close();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
	Fim das dunções do projeto de coloração
*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void merge(std::vector<std::list<Edge>> &listaCopia, const int &originVertex, const int &destinyVertex, int &numArestas) {
	for (auto edge = listaCopia[destinyVertex].begin(); edge != listaCopia[destinyVertex].end(); edge++) {
		// if edge[i] for para origin, delete this
		if (edge->destinyVertex == originVertex) {
			numArestas--;
			
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
		
		// if edge[i] já tiver lá, aumenta a multiplicidade e reduz o numero de arestas
		if (found != nullptr) {
			numArestas--;
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

struct Edge {
	int multiplicidade;
	int std::vector<int> EdgeIds;
	int destinyVertex;
	std::list<Edge>::iterator reversePointer;
};

int main(int argc, char const *argv[]) {
	std::vector<std::list<Edge>> adjList;
	srand(time());

	int numArestas;
	
	preencherAdjList(adjList, numArestas);

	listaCopia = adjList;
	for (int i = 0; i < adjList.size() - 2; i++) {
		int selectedEdge = (rand() % numArestas) + 1;
		int origVer = 0;

		// for que percorre e seleciona a aresta aleatória
		std::list<Edge>::iterator it;
		for (int i = 0; i < selectedEdge; /* empty */) {
			for (it = listaCopia[origVer].begin(); it != listaCopia[origVer].end(); it++) {
				if (it->destinyVertex > origVer) i++;
				if (i == selectedEdge) break;
			}
			origVer++;
		}

		// resultados finais usados em merge
		merge(listaCopia, origVer, it->destinyVertex, numArestas);
	}

	// percorre até encontrar a última aresta
	// corte mínimo será EdgeIds
	// tamanho do corte mínimo será EdgeIds.size()
	std::list<Edge>::iterator resEdge;
	std::vector<int> res;
	for (int i = 0; i < listaCopia.size(); i++)
		if (!listaCopia[i].empty()) {
			res = listaCopia[i].begin()->EdgeIds;
			break;
		}

	std::cout << "Tamanho do corte mínimo: " << res.size() << "\n";
	std::cout << "O corte mínimo é formado pelas arestas: ";
	for (int i = 0; i < res.size(); ++i) std::cout << res[i] << " ";
	std::cout << "\n";
	
	return 0;
}