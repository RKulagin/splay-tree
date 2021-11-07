// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_BINARY_SEARCH_TREE_HPP_
#define INCLUDE_BINARY_SEARCH_TREE_HPP_

#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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

    Node(const key_type& k, const value_type& v, std::weak_ptr<Node> p = {},
         std::shared_ptr<Node> l = {}, std::shared_ptr<Node> r = {})
        : key(k),
          value(v),
          parent(std::move(p)),
          left(std::move(l)),
          right(std::move(r)) {}
  };
  using node_ptr = std::shared_ptr<Node>;

  virtual ~BinarySearchTree() = default;

  /// Adds a tree item with the given key and value
  /// \throws an exception if an object with the given key
  /// already exists in the tree
  virtual std::pair<node_ptr, bool> insert(const key_type& key,
                                           const value_type& value) {
    auto place = _find(key);
    if (place == nullptr) {
      root = std::make_shared<Node>(std::move(key), std::move(value));
      return {root, true};
    }
    if (key == place->key) {
      return {place, false};
    }
    if (cmp(key, place->key)) {
      place->left =
          std::make_shared<Node>(std::move(key), std::move(value), place);
      return {place->left, true};
    } else {
      place->right =
          std::make_shared<Node>(std::move(key), std::move(value), place);
      return {place->right, true};
    }
  }

  /// Access element
  /// \throws out_of_range if n is out of bounds
  /// \return lvalue reference to the value stored under given key
  virtual value_type& at(const key_type& key) {
    auto search_result = find(key);
    if (key == search_result->key) {
      return search_result->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

  /// Access element without any changes to container
  /// \throws out_of_range if n is out of bounds
  /// \return const lvalue reference to the value stored under given key
  virtual const value_type& at(const key_type& key) const {
    auto search_result = find(key);
    if (key == search_result->key) {
      return search_result->value;
    }
    throw std::out_of_range("No node found for the given key.");
  }

  /// Searches for a Node with the given key or one that would become the parent
  /// of the Node if the node with the given key were added now
  virtual node_ptr find(const key_type& key) const { return _find(key); }

  virtual node_ptr find(const key_type& key) { return _find(key); }

  virtual value_type erase(const key_type& key) {
    node_ptr node = find(key);
    auto erased_value = std::move(node->value);
    while (node != nullptr) {
      if (is_leaf(node)) {
        if (is_root(node)) {
          root = nullptr;
          return erased_value;
        } else {
          if (is_left_child(node)) {
            node->parent.lock()->left = nullptr;
            return erased_value;
          } else {
            node->parent.lock()->right = nullptr;
            return erased_value;
          }
        }
      } else if (node->left != nullptr && node->right == nullptr) {
        if (is_root(node)) {
          root = node->left;
          return erased_value;
        } else {
          if (is_left_child(node)) {
            node->parent.lock()->left = node->left;
            return erased_value;
          } else {
            node->parent.lock()->right = node->left;
            return erased_value;
          }
        }
      } else if (node->left == nullptr && node->right != nullptr) {
        if (is_root(node)) {
          root = node->right;
          return erased_value;
        } else {
          if (is_left_child(node)) {
            node->parent.lock()->left = node->right;
            return erased_value;
          } else {
            node->parent.lock()->right = node->right;
            return erased_value;
          }
        }
      } else {
        node_ptr inorder_predecessor = find_inorder_predecessor(node);
        node->key = std::move(inorder_predecessor->key);
        node->value = std::move(inorder_predecessor->value);
        node = inorder_predecessor;
      }
    }
    return erased_value;
  }

  virtual node_ptr min() {
    node_ptr node = root;
    while (node != nullptr && node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

  virtual node_ptr max() {
    node_ptr node = root;
    while (node != nullptr && node->right != nullptr) {
      node = node->right;
    }
    return node;
  }

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
  node_ptr find_inorder_predecessor(node_ptr node) {
    node_ptr inorder_predecessor = node->left;
    while (inorder_predecessor != nullptr &&
           inorder_predecessor->right != nullptr) {
      inorder_predecessor = inorder_predecessor->right;
    }
    return inorder_predecessor;
  }

  node_ptr root;

 private:
  /// Checks if node is a leaf
  static inline bool is_leaf(node_ptr node) {
    return node->left != nullptr || node->right != nullptr;
  }

  void generate_empty(std::ostream& out, uint64_t size) {
    uint64_t value = 1;
    for (size_t i = 0; value <= size; ++i) {
      if (archive.size() <= i) {
        archive.push_back(archive.back() + " " + archive.back());
      }
      if (value & size) {
        out << archive[i];
        value <<= 1u;
        if (value <= size) {
          out << " ";
        }
      } else {
        value <<= 1u;
      }
    }
  }

  node_ptr _find(const key_type& key) const {
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
  Cmp cmp = Cmp();

  std::vector<std::string> archive = {"_", "_ _", "_ _ _ _"};

  template <class Key_, class Value_, class Cmp_>
  friend std::ostream& operator<<(std::ostream& out,
                                  BinarySearchTree<Key_, Value_, Cmp_>& tree);
};

/// Outputs the tree layer by layer to the output stream
/// \param out output stream
/// \param tree tree to print
template <class Key, class Value, class Cmp = std::less<Key>>
std::ostream& operator<<(
    std::ostream& out,
    BinarySearchTree<Key, Value, Cmp>& tree) {  // TODO(rkulagin): make const
  // Print tree root

  if (tree.root == nullptr) {
    out << "_";
    return out;
  }
  out << "[" << tree.root->key << " " << tree.root->value << "]";

  // Print non-root layer
  using layer_type = std::vector<
      std::shared_ptr<typename BinarySearchTree<Key, Value, Cmp>::Node>>;

  layer_type level;
  layer_type next_level;
  level.push_back(tree.root->left);
  level.push_back(tree.root->right);

  bool has_non_leaf_node =      // If all nodes of a layer are leaves, printing
      tree.is_leaf(tree.root);  // should be stopped after this layer

  while (has_non_leaf_node) {
    out << "\n";
    has_non_leaf_node = false;
    size_t buffer_size = 0;
    for (auto it = level.begin(); it != level.end(); ++it) {
      if (*it != nullptr) {
        if (it - buffer_size != level.begin()) {
          out << " ";
        }
        if (buffer_size) {
          tree.generate_empty(out, buffer_size);
          out << " ";
        }
        buffer_size = 0;
        has_non_leaf_node |= tree.is_leaf(*it);
        out << "[" << (*it)->key << " " << (*it)->value << " "
            << (*it)->parent.lock()->key << "]";
        next_level.push_back((*it)->left);
        next_level.push_back((*it)->right);
      } else {
        ++buffer_size;
        next_level.push_back(nullptr);
        next_level.push_back(nullptr);
      }
    }
    if (buffer_size) {
      out << " ";
      tree.generate_empty(out, buffer_size);
    }
    level = std::move(next_level);
    next_level.clear();
  }
  return out;
}

#endif  // INCLUDE_BINARY_SEARCH_TREE_HPP_
