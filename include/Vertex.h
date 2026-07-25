#ifndef VERTEX_H
#define VERTEX_H

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "TDefs.h"
#include "GraphDefs.h"
#include "PreferenceList.h"
#include "ClassificationList.h"

class Vertex {
private:
    IdType id_;                 // id of this vertex
    IdType cloned_for_id_;      // id of the vertex this was cloned for
    unsigned lower_quota_ = 0;           // least number of partners to this vertex
    unsigned upper_quota_ = 1;           // maximum number of partners to this vertex
    bool dummy_;                // is this a dummy vertex
    bool last_resort_;          // is this a last resort vertex
    int index_offset;           // stores the index offset of the vertex (used in classification analysis)
    std::vector<IdType> adj_list;  // adjacency list of a vertex (can make this into a seperate file as interface like preference list if needed) Works without that as well
    ClassificationList classifications; // stores the classification list of the vertex 
    ClassificationListElement temp_stored; // TEMP storage of the classificationList element

    Tree classification_tree;   // The classification Tree build from the list for the given vertex
    PreferenceList pref_list_;  // preference list according to priority
    PreferenceList pref_list_lq_;  // preference list containing LQ vertices

public:
    Vertex();
    explicit Vertex(const IdType& id, ClassificationListElement &cle);
    explicit Vertex(const IdType& id);
    explicit Vertex(const IdType& id, unsigned upper_quota);
    explicit Vertex(const IdType& id, unsigned lower_quota,
                    unsigned upper_quota);
    explicit Vertex(const IdType& id, unsigned lower_quota,
                    unsigned upper_quota, bool dummy, bool last_resort);
    explicit Vertex(const IdType& id, const IdType& cloned_for_id,
                    unsigned lower_quota, unsigned upper_quota, bool dummy,
                    bool last_resort);
    virtual ~Vertex() = default;

    const IdType& get_id() const;
    const IdType& get_cloned_for_id() const;
    unsigned get_lower_quota() const;
    unsigned get_upper_quota() const;
    unsigned get_adj_list_size(); /**/
    bool is_dummy() const;
    bool is_last_resort() const;

    PreferenceList& get_preference_list();
    const PreferenceList& get_preference_list() const;

    
    PreferenceList& get_preference_list_lq();
    const PreferenceList& get_preference_list_lq() const;

    /*
        Declaring the below classes as friend
        Another approach is to make set get functions and build an interface for editing and accessing these variables
        Both approaches work !! 
    */
    friend class Algorithm; /**/
    friend class NetworkBuilder; /**/
    friend class GraphReaderAlt; /**/
    friend class GraphReader; /**/
};

/// print this vertex in the format v: p1, ..., pk
std::ostream& operator<<(std::ostream& out, VertexPtr v);

#endif
