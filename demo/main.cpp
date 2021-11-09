#include <splay_tree.hpp>

int main() {
  SplayTree<int64_t, std::string> tree;
  std::string command;
  int64_t key;
  std::string value;
  while (std::cin >> command) {
    try {
      if (command == "add") {
        std::cin >> key >> value;
        auto [node, status] = tree.insert(key, value);
        if (!status) {
          std::cout << "error" << std::endl;
        }
      } else if (command == "set") {
        std::cin >> key >> value;
        tree.at(key) = value;
      } else if (command == "delete") {
        std::cin >> key;
        tree.erase(key);
      } else if (command == "search") {
        std::cin >> key;
        auto node = tree.find(key);
        if (node != nullptr && node->key == key) {
          std::cout << "1 " << node->value << std::endl;
        } else {
          std::cout << "0" << std::endl;
        }
      } else if (command == "min") {
        auto node = tree.min();
        if (node != nullptr) {
          std::cout << node->key << " " << node->value << std::endl;
        } else {
          std::cout << "error" << std::endl;
        }
      } else if (command == "max") {
        auto node = tree.max();
        if (node != nullptr) {
          std::cout << node->key << " " << node->value << std::endl;
        } else {
          std::cout << "error" << std::endl;
        }
      } else if (command == "print") {
        std::cout << tree << std::endl;
      }
    } catch (std::exception& e) {
      std::cout << "error" << std::endl;
    }
  }
  return 0;
}