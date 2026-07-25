#include "GraphReaderAlt.h"

    /* We never use the second parameter ?? */
std::pair<int, int> GraphReaderAlt::getVertexIdx(Graph &G, std::string vname){
    for(int i = 0; i < G.n; i++)
        if(G.partA[i]->id_ == vname)
            return {i, 0};
        
    for(int i = 0; i < G.m; i++)
        if(G.partP[i]->id_ == vname)
            return {i, 1};

    return {-1, -1};
}

void GraphReaderAlt::readParititions(Graph &G, std::shared_ptr<BipartiteGraph> & BG){
    BipartiteGraph::ContainerType A = BG->get_A_partition();
    BipartiteGraph::ContainerType B = BG->get_B_partition();
    G.n = A.size();
    G.m = B.size();

    for (auto &ele: A){
        G.partA.push_back(ele.second);
    }
    
    for(auto &ele: B){
        G.partP.push_back(ele.second);
    }
}

void GraphReaderAlt::readEdges(Graph &G){
    for (auto ele : G.partA) {
        G.k = std::max(G.k, ele->pref_list_.get_max_rank());
        for(int i = 0; i < ele->pref_list_.get_max_rank(); i++){
            if(ele->pref_list_.is_tied(i)){
                auto tied_list = ele->pref_list_.get_ties(i);
                for(auto elee: tied_list){
                    Edge e (ele->id_, elee.vertex->id_);
                    if(G.rank_edges.find(i+1) == G.rank_edges.end()){
                        G.rank_edges[i+1] = std::vector<Edge> ();
                    }
                    G.rank_edges[i+1].push_back(e);
                }
            }
            else{
                auto v = ele->pref_list_.at(i);
                Edge e(ele->id_, v.vertex->id_);
                if(G.rank_edges.find(i+1) == G.rank_edges.end()){
                    G.rank_edges[i+1] = std::vector<Edge> ();
                }
                G.rank_edges[i+1].push_back(e);
            }
        }
    }
}

void GraphReaderAlt::fillVAdj(Graph &G){
    for(int i = 1; i <= G.k; i++){
        for(int j = 0; j < G.rank_edges[i].size(); j++){
            int idx1 = getVertexIdx(G, G.rank_edges[i][j].id1).first, idx2 = getVertexIdx(G, G.rank_edges[i][j].id2).first;
            if(idx1 == -1 || idx2 == -1) continue;
            G.partA[idx1]->adj_list.push_back(G.rank_edges[i][j].id2);
            G.partP[idx2]->adj_list.push_back(G.rank_edges[i][j].id1);
        }
    }
}



Graph GraphReaderAlt::read_graph(std::shared_ptr<BipartiteGraph> & BG){
    Graph G;
    readParititions(G, BG);
    readEdges(G);
    fillVAdj(G);
    return G; // pass by value
}


