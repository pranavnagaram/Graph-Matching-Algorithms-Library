#ifndef NETWORK_BUILDER_H
#define NETWORK_BUILDER_H

#include "GraphDefs.h"
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <limits.h>
#include <algorithm>

class NetworkBuilder{
    private:

    std::pair<int, int> getVertexIdx(Graph &G, std::string vname);
    
    void addFundamenalClasses(Graph &G, int idx, bool isA);

    void sortClassifications(std::vector<VertexPtr> &part, int idx);
    
    void enumerateClassifications(std::vector<VertexPtr> &part, int idx);

    std::vector<int> initialiseRoots(ClassificationList &classifications, std::vector<bool> &usedClassification, int &usedClassifications);

    int findSmallestUnusedIdx(ClassificationList &classifications, std::vector<bool> &usedClassification);

    bool violatingClassifications(ClassificationListElement c1, ClassificationListElement c2);

    bool isChild(ClassificationListElement c1, ClassificationListElement c2);

    void isNonLaminar(ClassificationListElement c, std::vector<int> roots, ClassificationList classifications, std::ofstream &file_out);

    std::vector<int> findChildren(ClassificationList classifications, ClassificationListElement c, std::vector<int> roots);

    void addChildren(int parent, int child, std::vector<std::vector<TreeNode>> &tree, ClassificationList classifications);

    std::vector<int> updateRoots(std::vector<int> roots, std::vector<int> children, int smallestUnusedIdx);

    void buildClassificationTree(std::vector<VertexPtr> &part, int idx, std::ofstream &file_out, bool& isLaminar);

    void reenumerateTree(VertexPtr &v, int &offset);

    int reenumerateVertices(Graph &G);

    void initialiseNetwork(Graph &G, int numNodes);

    public:

    void initBuild(Graph &G, std::ofstream &file_out);

    void buildNetwork(Graph &G);
};



#endif