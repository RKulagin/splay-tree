// Copyright 2021 Your Name <your_email>

#include <binary_search_tree.hpp>
#include <bitset>
#include <stdexcept>

void print_empty_sequence(std::ostream& out, uint64_t size,
                    std::vector<std::string>& cache) {
  uint64_t value = 1;
  for (size_t i = 0; value <= size; ++i) {
    if (cache.size() <= i) {
      cache.push_back(cache.back() + " " + cache.back());
    }
    if (value & size) {
      out << cache[i];
      value <<= 1u;
      if (value <= size) {
        out << " ";
      }
    } else {
      value <<= 1u;
    }
  }
}