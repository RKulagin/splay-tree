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

  /// \brief Add item in a three
  /// \throws an exception if an object with
  /// the given key already exists in the tree
  ///
  /// Adds a tree item with the given key and value as normal binary search tree
  /// and splay inserted node in the end.
  std::pair<typename bst::node_ptr, bool> insert(const Key& key,
                                                 const Value& value) override {
    return _insert(key, value);
  }
  std::pair<typename bst::node_ptr, bool> insert(Key&& key,
                                                 Value&& value) override {
    return _insert(key, value);
  }

  /// \brief Access to tree data.
  /// \param key The key for which data should be retrieved.
  /// \throw std::out_of_range If no such data is present.
  /// \return A reference to the data whose key is equivalent to \a key, if
  /// such a data is present in the %heap
  ///
  /// Provides access to the data contained in tree under key.
  /// The key is checked that heap contains data with such key.
  /// The splay function is called for the found vertex or the vertex closest
  /// to the found one.
  /// The function throws out_of_range if the check fails.
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
    node_ptr left = bst::root->left;
    bst::root->right = nullptr;
    node_ptr inorder_predecessor = bst::find_inorder_predecessor(bst::root);
    if (right == nullptr) {
      bst::root = bst::root->left;
      if (left != nullptr) {
        bst::root->parent = node_ptr();
      }
      return erased_value;
    }
    if (left == nullptr) {
      right->parent = node_ptr();
      bst::root = right;
    } else {
      left->parent = node_ptr();
      bst::root = left;
      splay(inorder_predecessor);
      bst::root->right = right;
      right->parent = bst::root;
    }
    return erased_value;
  }

  typename bst::node_ptr find(const key_type& key) override {
    auto node = bst::find(key);
    splay(node);
    return node;
  }

  /// Retrieve node with minimum key (find, splay and return it)
  typename bst::node_ptr min() override {
    node_ptr node = bst::min();
    splay(node);
    return node;
  }

  /// Retrieve node with maximum key (find, splay and return it)
  typename bst::node_ptr max() override {
    node_ptr node = bst::max();
    splay(node);
    return node;
  }

 private:
  using node_ptr = typename bst::node_ptr;

  /// Operation to move node to the root of the tree.
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
      if (bst::is_root(node)) {
        return;
      }
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
      bst::rotate_right(node);
    } else if (bst::is_right_child(node) &&
               bst::is_right_child(node->parent.lock())) {
      bst::rotate_left(node->parent.lock());
      bst::rotate_left(node);
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

  template <typename K, typename V>
  std::pair<typename bst::node_ptr, bool> _insert(K&& key, V&& value) {
    auto [node, status] =
        bst::insert(std::forward<K>(key), std::forward<V>(value));
    splay(node);
    return {node, status};
  }
};
#endif  // INCLUDE_SPLAY_TREE_HPP_
