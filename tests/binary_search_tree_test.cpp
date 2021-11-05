// Copyright 2021 Your Name <your_email>

#include <gtest/gtest.h>

#include <binary_search_tree.hpp>
#include <stdexcept>

TEST(BST, BasicInsert) {
  BinarySearchTree<int, std::string> tree;
  tree.insert(1, "1");
  tree.insert(3, "3");
  EXPECT_EQ(tree.at(1), "1");
  EXPECT_EQ(tree.at(3), "3");
  EXPECT_NE(tree.at(3), "2");
  EXPECT_THROW(tree.at(2), std::out_of_range);
  EXPECT_THROW(tree.insert(3, "2"), std::runtime_error);
}
