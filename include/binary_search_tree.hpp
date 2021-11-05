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

  /// Adds a tree item with the given key and value
  /// \throws an exception if an object with the given key
  /// already exists in the tree
  void insert(key_type&& key, value_type&& value) {
    auto place = find(key);
    if (place == nullptr) {
      root = std::make_shared<Node>(std::move(key), std::move(value));
      return;
    }
    if (key == place->key) {
      throw std::runtime_error(
          "Binary Search Tree can't handle two nodes with similar key");
    }
    if (cmp(key, place->key)) {
      place->left =
          std::make_shared<Node>(std::move(key), std::move(value), place);
    } else {
      place->right =
          std::make_shared<Node>(std::move(key), std::move(value), place);
    }
  }

  /// Access element
  /// \throws out_of_range if n is out of bounds
  /// \return lvalue reference to the value stored under given key
  value_type& at(key_type&& key) {
    auto search_result = find(key);
    if (key == search_result->key) {
      return search_result->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

  /// Access element without any changes to container
  /// \throws out_of_range if n is out of bounds
  /// \return const lvalue reference to the value stored under given key
  const value_type& at(key_type&& key) const {
    auto search_result = find(key);
    if (key == search_result->key) {
      return search_result->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

 private:
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

  /// Searches for a Node with the given key or one that would become the parent
  /// of the Node if the node with the given key were added now
  std::shared_ptr<Node> find(key_type key) const {
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

  /// Checks if vertex is a leaf
  static inline bool is_leaf(std::shared_ptr<Node> vertex) {
    return vertex->left != nullptr || vertex->right != nullptr;
  }

  std::shared_ptr<Node> root;
  Cmp cmp = Cmp();

  template <class Key_, class Value_, class Cmp_>
  friend std::ostream& operator<<(
      std::ostream& out, const BinarySearchTree<Key_, Value_, Cmp_>& tree);
};

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

  bool inserted = tree.is_leaf(tree.root);
  while (inserted) {
    inserted = false;
    for (auto it = level.begin(); it != level.end(); ++it) {
      if (it != level.begin()) {
        out << " ";
      }
      if (*it != nullptr) {
        inserted |= tree.is_leaf(*it);
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
