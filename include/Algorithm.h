#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "GraphDefs.h"
#include "Vertex.h"
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <limits.h>


class Algorithm{
    private:

    // Class Graph Shift
    std::pair<int, int> getVertexIdx(Graph &G, std::string vname);

    void addEdge(Graph &G, Edge edge);

    void addEdges(Graph &G, int rank);

    void maxFlow(Graph &G, int rank, std::ofstream& file_out); /* Can ignore rank here and instead pass s,t indices*/

    std::vector<bool> inSk(Graph &G); /* Returns the boolean vector on whether the ith vertex is in S or not*/

    std::vector<bool> inTk(Graph &G); /* Returns the boolean vector on whether the ith vertex is in T or not*/

    void deleteReverseMinCutEdges(Graph &G, std::vector<bool> inS);

    void printNetwork(Graph &G, std::ofstream& file_out);

    void deleteUnwatedLREdges(Graph &G, std::vector<bool> inS, std::vector<bool> inT, int i);

    void printMatching(Graph &G, int rank, std::ofstream& file_out, bool final_print);

    void printClassifications(VertexPtr v, std::ofstream& file_out);

    void printClassificationIdx(VertexPtr &v, std::ofstream& file_out);

    void printClassificationIdxs(Graph &G, std::ofstream& file_out);

    public:

    void runAlgorithm(Graph &G, std::ofstream& file_out, bool log, std::ofstream& log_out);
};

#endif