// Copyright 2021 RKulagin

#ifndef INCLUDE_SPLAY_TREE_HPP_
#define INCLUDE_SPLAY_TREE_HPP_

#include <binary_search_tree.hpp>

template <typename Key, typename Value, typename Cmp = std::less<>>
class SplayTree : public BinarySearchTree<Key, Value, Cmp> {
 public:
  using key_type = Key;
  using value_type = Value;
  using bst = BinarySearchTree<Key, Value, Cmp>;

  typename bst::node_ptr insert(Key&& key, Value&& value) override {
    node_ptr node = bst::insert(std::move(key), std::move(value));
    splay(node);
    return bst::top();
  }

  value_type& at(key_type&& key) override {
    auto node = bst::find(key);
    splay(node);
//    std::cout << *this << std::endl;
    if (key == bst::top()->key) {
      return bst::top()->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

 private:
  using node_ptr = typename BinarySearchTree<Key, Value, Cmp>::node_ptr;
  void splay(node_ptr node) {
    while (node != nullptr && !bst::is_root(node)) {
      if (bst::is_root(node->parent.lock())) {
        zig(node);
      } else if ((bst::is_left_child(node) &&
                  bst::is_left_child(node->parent.lock())) ||
                 (bst::is_right_child(node) &&
                  bst::is_right_child(node->parent.lock()))) {
        zigzig(node);
      } else {
        zigzag(node);
      }
      if (node->parent.lock() == nullptr) {
        return;
      }
//      node = node->parent.lock()->parent.lock();
    }
  }

  /// Zig step. The tree is rotated on the edge between x and p. p must be root.
  void zig(node_ptr node) {
    if (bst::is_left_child(node)) {
      bst::rotate_right(node);
    } else {
      bst::rotate_left(node);
    }
  }
  /// Zig-Zig step. Parent of the node must not be the root and both node and
  /// node->parent should be both right or both left children.
  void zigzig(node_ptr node) {
    if (bst::is_left_child(node) && bst::is_left_child(node->parent.lock())) {
      bst::rotate_right(node->parent.lock());
//      std::cout << *this << std::endl;
      bst::rotate_right(node);
//      std::cout << *this << std::endl;

    } else if (bst::is_right_child(node) &&
               bst::is_right_child(node->parent.lock())) {
      bst::rotate_left(node->parent.lock());
      bst::rotate_left(node);
    } else {
      throw std::logic_error("Wrong call of zig-zig function");
    }
  }

  /// Zig-Zag step. Parent of the node must not be the root and node is a right
  /// child and node->parent is a left child or vice versa.
  void zigzag(node_ptr node) {
    if (bst::is_right_child(node) && bst::is_left_child(node->parent.lock())) {
      bst::rotate_left(node);
      bst::rotate_right(node);
    } else if (bst::is_left_child(node) &&
               bst::is_right_child(node->parent.lock())) {
      bst::rotate_right(node);
      bst::rotate_left(node);
    }
  }
};
#endif  // INCLUDE_SPLAY_TREE_HPP_
