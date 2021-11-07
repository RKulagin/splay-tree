// Copyright 2021 RKulagin

#ifndef INCLUDE_SPLAY_TREE_HPP_
#define INCLUDE_SPLAY_TREE_HPP_

#include <binary_search_tree.hpp>
#include <functional>
#include <utility>
template <typename Key, typename Value, typename Cmp = std::less<Key>>
class SplayTree : public BinarySearchTree<Key, Value, Cmp> {
 public:
  using key_type = Key;
  using value_type = Value;
  using bst = BinarySearchTree<Key, Value, Cmp>;

  std::pair<typename bst::node_ptr, bool> insert(const Key& key,
                                                 const Value& value) override {
    auto [node, status] = bst::insert(key, value);
    splay(node);
    return {node, status};
  }

  value_type& at(const key_type& key) override {
    auto node = find(key);
    if (node != nullptr && key == bst::root->key) {
      return bst::root->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

  value_type erase(const key_type& key) override {
    node_ptr node = bst::find(key);

    splay(node);
    if (node == nullptr || node->key != key) {
      throw std::out_of_range("No such key found.");
    }
    value_type erased_value = std::move(bst::root->value);

    node_ptr right = bst::root->right;

    bst::root->right = nullptr;

    node_ptr inorder_predecessor = bst::find_inorder_predecessor(bst::root);

    node_ptr left = bst::root->left;
    if (right == nullptr) {
      bst::root = bst::root->left;
      if (left != nullptr) {
        bst::root->parent = node_ptr();
      }

      return erased_value;
    }
    if (left != nullptr) {
      left->parent = node_ptr();
      bst::root = left;
      //      node_ptr inorder_predecessor =
      //      bst::find_inorder_predecessor(bst::root);
      splay(inorder_predecessor);

      bst::root->right = right;

      if (right != nullptr) {
        right->parent = bst::root;
      }
    } else {
      if (right != nullptr) {
        right->parent = node_ptr();
      }
      bst::root = right;
    }
    return erased_value;
  }

  typename bst::node_ptr find(const key_type& key) override {
    auto node = bst::find(key);
    splay(node);
    return node;
  }

  typename bst::node_ptr min() override {
    node_ptr node = bst::min();
    splay(node);
    return node;
  }

  typename bst::node_ptr max() override {
    node_ptr node = bst::max();
    splay(node);
    return node;
  }

 private:
  using node_ptr = typename bst::node_ptr;

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
