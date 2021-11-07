// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_BINARY_SEARCH_TREE_HPP_
#define INCLUDE_BINARY_SEARCH_TREE_HPP_

#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <utility>

template <class Key, class Value, class Cmp = std::less<Key>>
class BinarySearchTree {
 public:
  using key_type = Key;
  using value_type = Value;

  struct Node {
    key_type key = {};
    value_type value = {};
    std::weak_ptr<Node> parent;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(key_type&& k, value_type&& v, std::weak_ptr<Node> p = {},
         std::shared_ptr<Node> l = {}, std::shared_ptr<Node> r = {})
        : key(std::move(k)),
          value(std::move(v)),
          parent(std::move(p)),
          left(std::move(l)),
          right(std::move(r)) {}
  };
  using node_ptr = std::shared_ptr<Node>;

  virtual ~BinarySearchTree() = default;

  /// Adds a tree item with the given key and value
  /// \throws an exception if an object with the given key
  /// already exists in the tree
  virtual node_ptr insert(key_type&& key, value_type&& value) {
    auto place = find(key);
    if (place == nullptr) {
      root = std::make_shared<Node>(std::move(key), std::move(value));
      return root;
    }
    if (key == place->key) {
      throw std::runtime_error(
          "Binary Search Tree can't handle two nodes with similar key");
    }
    if (cmp(key, place->key)) {
      place->left =
          std::make_shared<Node>(std::move(key), std::move(value), place);
      return place->left;
    } else {
      place->right =
          std::make_shared<Node>(std::move(key), std::move(value), place);
      return place->right;
    }
  }

  /// Access element
  /// \throws out_of_range if n is out of bounds
  /// \return lvalue reference to the value stored under given key
  virtual value_type& at(key_type&& key) {
    auto search_result = find(key);
    if (key == search_result->key) {
      return search_result->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

  /// Access element without any changes to container
  /// \throws out_of_range if n is out of bounds
  /// \return const lvalue reference to the value stored under given key
  virtual const value_type& at(key_type&& key) const {
    auto search_result = find(key);
    if (key == search_result->key) {
      return search_result->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

  /// Searches for a Node with the given key or one that would become the parent
  /// of the Node if the node with the given key were added now
  node_ptr find(key_type key) const {
    auto current = root;
    while (current != nullptr) {
      if (key == current->key) {
        break;
      }
      if (cmp(key, current->key)) {
        if (current->left != nullptr) {
          current = current->left;
        } else {
          break;
        }
      } else {
        if (current->right != nullptr) {
          current = current->right;
        } else {
          break;
        }
      }
    }
    return current;
  }

  node_ptr top() { return root; }

 protected:
  inline bool is_left_child(node_ptr node) const {
    return node->parent.lock()->left == node;
  }
  inline bool is_right_child(node_ptr node) const {
    return node->parent.lock()->right == node;
  }

  void rotate_right(node_ptr node) {
    node_ptr middle = node->right;
    node_ptr parent = node->parent.lock();
    if (parent->parent.lock() != nullptr) {
      if (is_left_child(parent)) {
        parent->parent.lock()->left = node;
      } else {
        parent->parent.lock()->right = node;
      }
    } else {
      root = node;
    }
    node->parent = parent->parent;

    node->right = parent;
    parent->parent = node;
    parent->left = middle;
    if (middle != nullptr) {
      middle->parent = parent;
    }
  }
  void rotate_left(node_ptr node) {
    node_ptr middle = node->left;
    node_ptr parent = node->parent.lock();

    if (parent->parent.lock() != nullptr) {
      if (is_left_child(parent)) {
        parent->parent.lock()->left = node;
      } else {
        parent->parent.lock()->right = node;
      }
    } else {
      root = node;
    }
    node->parent = parent->parent;

    node->left = parent;
    parent->parent = node;
    parent->right = middle;
    if (middle != nullptr) {
      middle->parent = parent;
    }
  }

  inline bool is_root(node_ptr node) const {
    return node->parent.lock() == nullptr;
  }

 private:
  /// Checks if vertex is a leaf
  static inline bool is_leaf(std::shared_ptr<Node> vertex) {
    return vertex->left != nullptr || vertex->right != nullptr;
  }

  node_ptr root;
  Cmp cmp = Cmp();

  template <class Key_, class Value_, class Cmp_>
  friend std::ostream& operator<<(
      std::ostream& out, const BinarySearchTree<Key_, Value_, Cmp_>& tree);
};

/// Outputs the tree layer by layer to the output stream
/// \param out output stream
/// \param tree tree to print
template <class Key, class Value, class Cmp = std::less<Key>>
std::ostream& operator<<(std::ostream& out,
                         const BinarySearchTree<Key, Value, Cmp>& tree) {
  using layer_type = std::list<
      std::shared_ptr<typename BinarySearchTree<Key, Value, Cmp>::Node>>;

  layer_type level;
  layer_type next_level;

  // Print tree root

  if (tree.root == nullptr) {
    return out;
  }
  level.push_back(tree.root->left);
  level.push_back(tree.root->right);
  out << "[" << tree.root->key << " " << tree.root->value << "]\n";

  // Print non-root layer

  bool has_non_leaf_node =      // If all nodes of a layer are leaves, printing
      tree.is_leaf(tree.root);  // should be stopped after this layer

  while (has_non_leaf_node) {
    has_non_leaf_node = false;
    for (auto it = level.begin(); it != level.end(); ++it) {
      if (it != level.begin()) {
        out << " ";
      }
      if (*it != nullptr) {
        has_non_leaf_node |= tree.is_leaf(*it);
        out << "[" << (*it)->key << " " << (*it)->value << " "
            << (*it)->parent.lock()->key << "]";
        next_level.push_back((*it)->left);
        next_level.push_back((*it)->right);
      } else {
        out << "_";
        next_level.push_back(nullptr);
        next_level.push_back(nullptr);
      }
    }
    out << "\n";
    level = std::move(next_level);
    next_level.clear();
  }
  return out;
}

#endif  // INCLUDE_BINARY_SEARCH_TREE_HPP_
