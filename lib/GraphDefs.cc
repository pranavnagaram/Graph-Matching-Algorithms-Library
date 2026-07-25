#include "GraphDefs.h"
#include <iostream>
#include <fstream>

void Graph::printNetwork(std::ofstream & file_out){
    for(int i = 0; i < network.size(); i++){
        for(int j = 0; j < network[i].size(); j++)
            file_out << network[i][j] << " ";
        file_out << std::endl;
    }
}
void Graph::printEdges(std::ofstream & file_out){
    for(int i = 1; i <= k; i++){
        file_out << "Rank " << i << ":\n";
        for(int j = 0; j < rank_edges[i].size(); j++)
            file_out << rank_edges[i][j].id1 << " " << rank_edges[i][j].id2 << std::endl;
    }
}