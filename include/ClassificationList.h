#ifndef CLASSIFICATION_LIST_H
#define CLASSIFICATION_LIST_H

#include "TDefs.h"
#include <vector>
#include <list>
#include <memory>

struct ClassificationListElement{
    IdType id_;
    int upper_quota_, lower_quota_;
    int num_vertices;
    std::vector<IdType> vertices;
};

class ClassificationList {
public:
    typedef typename std::vector<ClassificationListElement> ContainerType;
    typedef ContainerType::size_type SizeType;
    typedef ContainerType::iterator Iterator;
    typedef ContainerType::const_iterator ConstIterator;

private:
    ContainerType classification_list_;

public:
    ClassificationList() = default;
    virtual ~ClassificationList();

    bool empty() const;
    SizeType size() const;

    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    Iterator begin() noexcept;
    Iterator end() noexcept;

    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;

    void add_element(ClassificationListElement c);

    ClassificationListElement operator[](int index);
};

#endif
