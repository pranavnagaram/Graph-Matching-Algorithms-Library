#include "NetworkBuilder.h"

std::pair<int, int> NetworkBuilder::getVertexIdx(Graph &G, std::string vname){
    for(int i = 0; i < G.n; i++)
        if(G.partA[i]->id_ == vname)
            return {i, 0};
        
    for(int i = 0; i < G.m; i++)
        if(G.partP[i]->id_ == vname)
            return {i, 1};

    return {-1, -1};
}

void NetworkBuilder::addFundamenalClasses(Graph &G, int idx, bool isA){
    ClassificationListElement c;
    c.id_ = "C" + std::to_string((int)isA) + std::to_string(idx) + "_star"; // C0i_star
    if(isA) {
        c.upper_quota_ = G.partA[idx]->upper_quota_; 
        c.num_vertices = G.partA[idx]->adj_list.size();
        
        for (int i = 0; i < G.partA[idx]->adj_list.size(); i++)
            c.vertices.push_back(G.partA[idx]->adj_list[i]);

        if (G.partA[idx]->adj_list.size() != 1)
            G.partA[idx]->classifications.add_element(c);

        for (int i = 0; i < G.partA[idx]->adj_list.size(); i++){
            ClassificationListElement c;
            int vidx = getVertexIdx(G, G.partA[idx]->adj_list[i]).first;

            c.id_ = "C" + std::to_string((int) (!isA)) + std::to_string(idx) + "_" + std::to_string(vidx); // C0i_j
            c.upper_quota_ = 1; c.num_vertices = 1; c.vertices = {G.partA[idx]->adj_list[i]};

            G.partA[idx]->classifications.add_element(c);
        }
    }
    else {
        c.upper_quota_ = G.partP[idx]->upper_quota_; 
        c.num_vertices = G.partP[idx]->adj_list.size();
        
        for (int i = 0; i < G.partP[idx]->adj_list.size(); i++)
            c.vertices.push_back(G.partP[idx]->adj_list[i]);

        if (G.partP[idx]->adj_list.size() != 1)
            G.partP[idx]->classifications.add_element(c);

        for (int i = 0; i < G.partP[idx]->adj_list.size(); i++){
            ClassificationListElement c;
            int vidx = getVertexIdx(G, G.partP[idx]->adj_list[i]).first;

            c.id_ = "C" + std::to_string((int) (!isA)) + std::to_string(idx) + "_" + std::to_string(vidx); // C0i_j
            c.upper_quota_ = 1; c.num_vertices = 1; c.vertices = {G.partP[idx]->adj_list[i]};

            G.partP[idx]->classifications.add_element(c);
        }
    }    
}

void NetworkBuilder::sortClassifications(std::vector<VertexPtr> &part, int idx){
    std::sort(part[idx]->classifications.begin(), part[idx]->classifications.end(), [](ClassificationListElement a, ClassificationListElement b){
        return a.num_vertices > b.num_vertices;
    });
}

void NetworkBuilder::enumerateClassifications(std::vector<VertexPtr> &part, int idx){
    for (int i = 0; i < part[idx]->classifications.size(); i++){
        part[idx]->classification_tree.classificationIdx[part[idx]->classifications[i].id_] = i;
        part[idx]->classification_tree.classificationName[i] = part[idx]->classifications[i].id_;
    }
}

std::vector<int> NetworkBuilder::initialiseRoots(ClassificationList &classifications, std::vector<bool> &usedClassification, int &usedClassifications){
    std::vector<int> roots;
    for (int i = 0; i < classifications.size(); i++){
        if (classifications[i].num_vertices == 1){
            roots.push_back(i);
            usedClassification[i] = true;
            usedClassifications++;
        }
    }
    return roots;
}

int NetworkBuilder::findSmallestUnusedIdx(ClassificationList &classifications, std::vector<bool> &usedClassification){
    int smallestUnusedIdx = -1, smallestUnusedSz = INT_MAX;
    for (int i = 0; i < classifications.size(); i++){
        if (!usedClassification[i] && classifications[i].num_vertices < smallestUnusedSz){
            smallestUnusedIdx = i;
            smallestUnusedSz = classifications[i].num_vertices;
        }
    }
    return smallestUnusedIdx;
}

// Use pass by reference ??
bool NetworkBuilder::violatingClassifications(ClassificationListElement c1, ClassificationListElement c2){ // c1.sz >= c2.sz
    int foundCount = 0;
    for (auto v : c2.vertices)
        if (find(c1.vertices.begin(), c1.vertices.end(), v) != c1.vertices.end())
            foundCount++;
    return (foundCount != c2.num_vertices) && (foundCount != 0);
}

// Use pass by reference ??
bool NetworkBuilder::isChild(ClassificationListElement c1, ClassificationListElement c2){
    if (find(c1.vertices.begin(), c1.vertices.end(), c2.vertices[0]) != c1.vertices.end())
        return true;
    return false;
}

// Use pass by reference ??
void NetworkBuilder::isNonLaminar(ClassificationListElement c, std::vector<int> roots, ClassificationList classifications, std::ofstream &file_out){
    for (auto i : roots)
        if (violatingClassifications(c, classifications[i])){
            file_out << "Input is non-laminar\n";
            file_out << "Violating classifications: " << c.id_ << " " << classifications[i].id_ << "\n";
        }
}

std::vector<int> NetworkBuilder::findChildren(ClassificationList classifications, ClassificationListElement c, std::vector<int> roots){
    std::vector<int> children;
    for (auto idx : roots){
        if (isChild(c, classifications[idx])){
            children.push_back(idx);
        }
    }
    return children;
}

void NetworkBuilder::addChildren(int parent, int child, std::vector<std::vector<TreeNode>> &tree, ClassificationList classifications) {
    // Check if parent index is valid
    if (parent < 0 || parent >= tree.size()) {
        std::cerr << "Error: Invalid parent index: " << parent << std::endl;
        return;
    }
    // Add child node to the tree vector
    tree[parent].push_back(TreeNode(child, classifications[child].upper_quota_));

}


std::vector<int> NetworkBuilder::updateRoots(std::vector<int> roots, std::vector<int> children, int smallestUnusedIdx){
    std::vector<int> newRoots;
    for (auto root : roots)
        if (find(children.begin(), children.end(), root) == children.end())
            newRoots.push_back(root);
    newRoots.push_back(smallestUnusedIdx);
    return newRoots;
}

void NetworkBuilder::buildClassificationTree(std::vector<VertexPtr> &part, int idx, std::ofstream &file_out, bool &isLaminar){

    sortClassifications(part, idx);
    enumerateClassifications(part, idx);

    bool laminar = true;
    std::vector<std::pair<ClassificationListElement, ClassificationListElement>> violatingPairs;
    for (int i = 0; i < part[idx]->classifications.size(); i++){
        for (int j = i + 1; j < part[idx]->classifications.size(); j++){
            if (violatingClassifications(part[idx]->classifications[i], part[idx]->classifications[j])){
                laminar = false;
                violatingPairs.push_back({part[idx]->classifications[i], part[idx]->classifications[j]});
            }
        }
    }

    if (!laminar){
        file_out << "Input is non-laminar\n";
        file_out << "Violating pairs:\n";
        for (auto p : violatingPairs) {
            file_out << "C1: " << p.first.id_ << ", quota: " << p.first.upper_quota_ << ", vertices: ";
            for (auto v : p.first.vertices)
                file_out << v << " ";
            file_out << "\n";
            file_out << "C2: " << p.second.id_ << ", quota: " << p.second.upper_quota_ << ", vertices: ";
            for (auto v : p.second.vertices)
                file_out << v << " ";
            file_out << "\n";
        }
        isLaminar = false;
    }
    if(!isLaminar) return;

    part[idx]->classification_tree.classificationTreeChildren.resize(part[idx]->classifications.size());
    int usedClassifications = 0;
    std::vector<bool> usedClassification(part[idx]->classifications.size(), false);

    // add all classifications of size 1 to the roots
    std::vector<int> roots = initialiseRoots(part[idx]->classifications, usedClassification, usedClassifications);

    while (usedClassifications < part[idx]->classifications.size()){
        int smallestUnusedIdx = findSmallestUnusedIdx(part[idx]->classifications, usedClassification);
        std::cout << usedClassification.size() << std::endl;
        usedClassification[smallestUnusedIdx] = true;
        usedClassifications++;
        std::vector<int> children = findChildren(part[idx]->classifications, part[idx]->classifications[smallestUnusedIdx], roots);

        for (auto child : children) {
            // Add child to the classification tree
            addChildren(smallestUnusedIdx, child, part[idx]->classification_tree.classificationTreeChildren, part[idx]->classifications);

        }
        roots = updateRoots(roots, children, smallestUnusedIdx);
    }
    part[idx]->classification_tree.classificationTreeRoot = roots[0];
}

void NetworkBuilder::reenumerateTree(VertexPtr &v, int &offset){
    v->index_offset = offset;
    int numVertices = v->classifications.size();
    std::vector<std::vector<TreeNode>> newClassificationTreeChildren(numVertices + offset);
    for (int i = 0; i < numVertices; i++){
        v->classification_tree.classificationIdx[v->classifications[i].id_] = i + offset;
        v->classification_tree.classificationName[i + offset] = v->classifications[i].id_;
        for (auto child : v->classification_tree.classificationTreeChildren[i])
            newClassificationTreeChildren[i + offset].push_back({child.vertex + offset, child.upper_quota_});
    }
    v->classification_tree.classificationTreeChildren = newClassificationTreeChildren;
    v->classification_tree.classificationTreeRoot += offset;
    offset += numVertices;
}

int NetworkBuilder::reenumerateVertices(Graph &G){
    int offset = 2; // s -> 0, t -> 1
    for (int i = 0; i < G.n; i++)
        reenumerateTree(G.partA[i], offset);
    for (int i = 0; i < G.m; i++)
        reenumerateTree(G.partP[i], offset);
    return offset;    
}

void NetworkBuilder::initialiseNetwork(Graph &G, int numNodes){
    G.network.resize(numNodes);
    for (int i = 0; i < numNodes; i++)
        G.network[i].resize(numNodes, 0);
    
    // add edges from s to roots of A
    for (int i = 0; i < G.n; i++)
        G.network[0][G.partA[i]->classification_tree.classificationTreeRoot] = G.partA[i]->upper_quota_;

    // add edges from roots of P to t
    for (int i = 0; i < G.m; i++)
        G.network[G.partP[i]->classification_tree.classificationTreeRoot][1] = G.partP[i]->upper_quota_;

    // add edges corresponding to trees of A
    for (int i = 0; i < G.n; i++)
        for (int j = 0; j < G.partA[i]->classification_tree.classificationTreeChildren.size(); j++)
            for (auto child : G.partA[i]->classification_tree.classificationTreeChildren[j])
                G.network[j][child.vertex] = child.upper_quota_;

    // add reverse edges corresponding to trees of P
    for (int i = 0; i < G.m; i++)
        for (int j = 0; j < G.partP[i]->classification_tree.classificationTreeChildren.size(); j++)
            for (auto child : G.partP[i]->classification_tree.classificationTreeChildren[j])
                G.network[child.vertex][j] = child.upper_quota_;
}

void NetworkBuilder::initBuild(Graph &G, std::ofstream &file_out){
    for (int i = 0; i < G.n; i++) {
        addFundamenalClasses(G, i, true);
        std::cout << G.partA[i]->classifications.size() << std::endl;
        for(auto &ele : G.partA[i]->classifications){
            std::cout << ele.vertices.size() << " ";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < G.m; i++) {
        addFundamenalClasses(G, i, false);
        std::cout << G.partP[i]->classifications.size() << std::endl;
    }
    bool isLaminar = true;
    for (int i = 0; i < G.n; i++) {
        buildClassificationTree(G.partA, i, file_out, isLaminar);
    }
    for (int i = 0; i < G.m; i++) {
        buildClassificationTree(G.partP, i, file_out, isLaminar);
    }

    if(!isLaminar){
        std::cout << "INPUT IS NON LAMINAR -- EXITING .." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void NetworkBuilder::buildNetwork(Graph &G){
    int numNodes = reenumerateVertices(G);
    initialiseNetwork(G, numNodes);
}