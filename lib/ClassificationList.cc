#include "ClassificationList.h"
#include "Vertex.h"
#include <algorithm>
#include <cassert>

ClassificationList::~ClassificationList() {
    classification_list_.clear();
}

bool ClassificationList::empty() const {
    return classification_list_.empty();
}

ClassificationList::SizeType ClassificationList::size() const {
    return classification_list_.size();
}

ClassificationList::ConstIterator ClassificationList::cbegin() const noexcept {
    return classification_list_.cbegin();
}

ClassificationList::ConstIterator ClassificationList::cend() const noexcept {
    return classification_list_.cend();
}

ClassificationList::Iterator ClassificationList::begin() noexcept {
    return classification_list_.begin();
}

ClassificationList::Iterator ClassificationList::end() noexcept {
    return classification_list_.end();
}

ClassificationList::ConstIterator ClassificationList::begin() const noexcept {
    return classification_list_.begin();
}

ClassificationList::ConstIterator ClassificationList::end() const noexcept {
    return classification_list_.end();
}

/// add a vertex to the list of matched partners
void ClassificationList::add_element(ClassificationListElement c) {
    classification_list_.push_back(c);
}


ClassificationListElement ClassificationList::operator[](int index){
    return classification_list_[index];
}