#include "Algorithm.h"

std::pair<int, int> Algorithm::getVertexIdx(Graph &G, std::string vname){
    for(int i = 0; i < G.n; i++)
        if(G.partA[i]->id_ == vname)
            return {i, 0};
        
    for(int i = 0; i < G.m; i++)
        if(G.partP[i]->id_ == vname)
            return {i, 1};

    return {-1, -1};
}

void Algorithm::addEdge(Graph &G, Edge edge){
    int u = getVertexIdx(G, edge.id1).first, v = getVertexIdx(G, edge.id2).first;
    if (u == -1 || v == -1) return;
    int LIdx = G.partA[u]->classification_tree.classificationIdx["C0" + std::to_string(u) + "_" + std::to_string(v)];
    int RIdx = G.partP[v]->classification_tree.classificationIdx["C1" + std::to_string(v) + "_" + std::to_string(u)];
    G.network[LIdx][RIdx] = 1;
}

void Algorithm::addEdges(Graph &G, int rank){
    for (auto edge : G.rank_edges[rank])
        addEdge(G, edge);
}

void Algorithm::maxFlow(Graph &G, int rank, std::ofstream& file_out){
    int s = 0, t = 1;
    int maxFlow = 0;
    while (true){
        // BFS to find augmenting path
        std::vector<int> parent(G.network.size(), -1);
        std::queue<int> q;
        q.push(s);
        while (!q.empty()){
            int u = q.front();
            q.pop();
            for (int v = 0; v < G.network.size(); v++){
                if (parent[v] == -1 && G.network[u][v] > 0){
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (parent[t] == -1) break; // no augmenting path found

        // find path flow
        int pathFlow = INT_MAX;
        for (int v = t; v != s; v = parent[v]){
            int u = parent[v];
            pathFlow = std::min(pathFlow, G.network[u][v]);
        }

        // augment path flow
        for (int v = t; v != s; v = parent[v]){
            int u = parent[v];
            G.network[u][v] -= pathFlow;
            G.network[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }
}

std::vector<bool> Algorithm::inSk(Graph &G){
    std::vector<bool> inSk(G.network.size(), false);
    std::queue<int> q;
    q.push(0);
    inSk[0] = true;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        for (int v = 0; v < G.network.size(); v++){
            if (!inSk[v] && G.network[u][v] > 0){
                inSk[v] = true;
                q.push(v);
            }
        }
    }
    return inSk;
}

std::vector<bool> Algorithm::inTk(Graph &G){
    std::vector<bool> inTk(G.network.size(), false);
    std::queue<int> q;
    q.push(1);
    inTk[1] = true;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        for (int v = 0; v < G.network.size(); v++){
            if (!inTk[v] && G.network[v][u] > 0){
                inTk[v] = true;
                q.push(v);
            }
        }
    }
    return inTk;
}

void Algorithm::deleteReverseMinCutEdges(Graph &G, std::vector<bool> inS){
    for (int i = 0; i < G.network.size(); i++)
        for (int j = 0; j < G.network.size(); j++)
            if (inS[i] && !inS[j] && G.network[i][j] > 0)
                G.network[i][j] = 0;
}

void Algorithm::printNetwork(Graph &G, std::ofstream& file_out){
    int numNodes = G.network.size();
    file_out << "x\t";
    for (int i = 0; i < numNodes; i++)
        file_out << i << "\t";
    file_out << "\n";
    for (int i = 0; i < numNodes; i++){
        file_out << i << "\t";
        for (int j = 0; j < numNodes; j++)
            file_out << G.network[i][j] << "\t";
        file_out << "\n";
    }
}

void Algorithm::deleteUnwatedLREdges(Graph &G, std::vector<bool> inS, std::vector<bool> inT, int i){
    for (int j = i + 1; j <= G.k; j++){
        std::vector<Edge> edgesToKeep;
        for (auto edge : G.rank_edges[j]){
            int u = getVertexIdx(G, edge.id1).first, v = getVertexIdx(G, edge.id2).first;
            if (u == -1 || v == -1) continue;
            int LIdx = G.partA[u]->classification_tree.classificationIdx["C0" + std::to_string(u) + "_" + std::to_string(v)];
            int RIdx = G.partP[v]->classification_tree.classificationIdx["C1" + std::to_string(v) + "_" + std::to_string(u)];

            if (inS[LIdx] && inT[RIdx])
                edgesToKeep.push_back(edge);
        }
        G.rank_edges[j] = edgesToKeep;
    }
}

void Algorithm::printMatching(Graph &G, int rank, std::ofstream& file_out, bool final_print = false){
    // file_out << "Final Matching:\n";
    std::vector <std::pair <std::string, std::string>> matching;
    for (int i = 1; i <= rank; i++){
        if(!final_print) file_out << "Rank " << i << ":\n";
        for (auto edge : G.rank_edges[i]){
            int u = getVertexIdx(G, edge.id1).first, v = getVertexIdx(G, edge.id2).first;
            if (u == -1 || v == -1) continue;
            int LIdx = G.partA[u]->classification_tree.classificationIdx["C0" + std::to_string(u) + "_" + std::to_string(v)];
            int RIdx = G.partP[v]->classification_tree.classificationIdx["C1" + std::to_string(v) + "_" + std::to_string(u)];
            if (G.network[LIdx][RIdx] == 0){
               if(!final_print)  file_out << "(" << edge.id1 << ", " << edge.id2 << "), ";
                matching.push_back({edge.id1, edge.id2});
            }
        }
        if(!final_print) file_out << "\n";
    }
    if(final_print){
        if (matching.size() == 0){
            file_out << "Empty matching\n";
            return;
        }
        for (int i = 0; i < matching.size() - 1; i++)
            file_out << "(" << matching[i].first << ", " << matching[i].second << "), ";
        file_out << "(" << matching[matching.size() - 1].first << ", " << matching[matching.size() - 1].second << ")\n";
    }
}

void Algorithm::printClassifications(VertexPtr v, std::ofstream& file_out){
    file_out << v->id_ << ": " << v->upper_quota_ << "\n";
    for (int i = 0; i < v->classifications.size(); i++){
        file_out << v->classifications[i].id_ << " " << v->classifications[i].upper_quota_ << " " << v->classifications[i].num_vertices << "\n";
        for (auto vertex : v->classifications[i].vertices)
            file_out << vertex << " ";
        file_out << "\n\n";
    }
}

void Algorithm::printClassificationIdx(VertexPtr &v, std::ofstream& file_out){
    file_out << v->id_ << ": " << v->upper_quota_ << "\n";
    for (auto it : v->classification_tree.classificationIdx)
        file_out << it.first << " " << it.second << "\n";
}

void Algorithm::printClassificationIdxs(Graph &G, std::ofstream& file_out){
    for (int i = 0; i < G.n; i++)
        printClassificationIdx(G.partA[i], file_out);
    for (int i = 0; i < G.m; i++)
        printClassificationIdx(G.partP[i], file_out);
}

void Algorithm::runAlgorithm(Graph &G, std::ofstream& file_out, bool log, std::ofstream& log_out){
    for (int i = 1; i <= G.k; i++){
        addEdges(G, i);
        maxFlow(G, i, file_out);
        std::vector<bool> inS = inSk(G), inT = inTk(G); // S, T, U decomposition
        deleteReverseMinCutEdges(G, inS);
        deleteUnwatedLREdges(G, inS, inT, i);
        if(log) printMatching(G, G.k, log_out);
    }
    printMatching(G, G.k, file_out, true);
}