#ifndef GRAPH_DEFS_H
#define GRAPH_DEFS_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "TDefs.h"


// class VertexAlt;
struct Edge;
class Graph;
class GraphReaderAlt;
class NetworkBuilder;
class Algorithm;
struct Tree;
struct TreeNode;

/* To add a new Class Named This*/
struct TreeNode{
    int vertex; // The node_index of the node
    int upper_quota_; // upper quota of the classification
    int lower_quota_; // lower quota of the classification


    TreeNode(int a, int b): vertex(a), upper_quota_(b) {}
    TreeNode(int a, int b, int c): vertex(a), lower_quota_(b), upper_quota_(c) {}
};


/* Use this in the class Vertex*/
struct Tree{
    int classificationTreeRoot; // RootId
    std::vector<std::vector<TreeNode>> classificationTreeChildren; // stores all the (child, quota) information
    std::map<std::string, int> classificationIdx; // Node Id to its index mapping
    std::map<int, std::string> classificationName; // Index to its Node Id mapping
};

struct Edge{
    IdType id1; // edge endpoint 1
    IdType id2; // edge endpoint 2

    Edge(IdType id1_, IdType id2_): id1(id1_), id2(id2_){}
};

/* Add a new module called Graphs*/
class Graph{
    int n, m, k; 
    std::vector<VertexPtr> partA, partP; // Part A and Part P of the orignal matching
    std::map<RankType, std::vector<Edge>> rank_edges; // The mapping of rank-wise edges of the matching
    std::vector<std::vector<int>> network; // adjacency matrix of residual network (including s,t vertices)

    public:
        /*
        Declaring the below classes as friend
        Another approach is to make set get functions and build an interface for editing and accessing these variables
        Both approaches work !! 
        */
        friend class GraphReaderAlt;
        friend class NetworkBuilder;
        friend class Algorithm;
        friend class GraphReader; 
        void printNetwork(std::ofstream & file_out);
        void printEdges(std::ofstream & file_out);
};

#endif
