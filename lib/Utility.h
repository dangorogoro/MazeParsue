#pragma once

#include <vector>
#include <algorithm>
#include "Parsue_Conf.h"
class GeneralIndexVector {
  private:
    std::vector<int32_t> index_vector;
  public:
    GeneralIndexVector() {index_vector.reserve(128);}
    void clear() { index_vector.clear();}
    inline std::vector<int32_t>::const_iterator begin() const {return index_vector.begin(); }
    inline std::vector<int32_t>::const_iterator end() const { return index_vector.end(); }
    inline size_t size() const { return index_vector.size(); }
    inline void push_back(const uint32_t &id) { index_vector.push_back(id); }
    void reverse() {std::reverse(index_vector.begin(), index_vector.end());}
    const int32_t &operator[](size_t i) const { return index_vector[i]; }
};
extern GeneralIndexVector indexVector;
