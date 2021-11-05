// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_BINARY_SEARCH_TREE_HPP_
#define INCLUDE_BINARY_SEARCH_TREE_HPP_

template<class Key, class Value, class Cmp = std::less<Key>>
class BinarySearchTree {
 public:
  using key_type = Key;
  using value_type = Value;

  // TODO: rule of five


 private:
  struct Node {
    key_type key = {};
    value_type value = {};
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
  };

  Node *root = nullptr;

};



#endif  // INCLUDE_BINARY_SEARCH_TREE_HPP_
