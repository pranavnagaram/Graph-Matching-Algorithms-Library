#ifndef GRAPH_READER_ALT_H
#define GRAPH_READER_ALT_H

#include "GraphDefs.h"
#include "BipartiteGraph.h"
#include "Vertex.h"

#include <utility>
#include <iostream>
#include <fstream>

class GraphReaderAlt{

    /* We never use the second parameter ?? */
    std::pair<int, int> getVertexIdx(Graph &G, std::string vname); // given name get the vertex id

    void readParititions(Graph &G, std::shared_ptr<BipartiteGraph> & BG); // read from Bipartite Graph and store it into the partA, partP of Graph

    void readEdges(Graph &G); // Fill the rankwise edge_list of the graph

    void fillVAdj(Graph &G); // Fill the adjacency list of the vertices in the graph
    
    public:

    Graph read_graph(std::shared_ptr<BipartiteGraph> & BG);
};

#endif

