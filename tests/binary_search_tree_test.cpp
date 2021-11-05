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
  const auto& similar_tree = tree;
  tree.at(3) = "Hi";
  EXPECT_EQ(tree.at(1), similar_tree.at(1));
  EXPECT_EQ(tree.at(3), similar_tree.at(3));
  EXPECT_EQ(tree.at(3), "Hi");
  EXPECT_THROW(similar_tree.at(100), std::out_of_range);

  std::stringstream stream;
  stream << tree;
  EXPECT_EQ(stream.str(), "[1 1]\n_ [3 Hi 1]\n");
}

TEST(BST, Insert) {
  BinarySearchTree<int, int> tree;
  std::stringstream stream;

  stream << tree;
  EXPECT_EQ(stream.str(), "");

  tree.insert(1, 10);
  tree.insert(2, 12);
  tree.insert(5, 14);
  tree.insert(-10, 16);
  tree.insert(-5, 18);
  tree.insert(0, 20);
  EXPECT_EQ(tree.at(1), 10);
  EXPECT_EQ(tree.at(2), 12);
  EXPECT_EQ(tree.at(5), 14);
  EXPECT_EQ(tree.at(-5), 18);
  EXPECT_EQ(tree.at(0), 20);
  EXPECT_EQ(tree.at(-10), 16);

  stream << tree;
  EXPECT_EQ(stream.str(),
            "[1 10]\n[-10 16 1] [2 12 1]\n_ [-5 18 -10] _ [5 14 2]\n_ _ _ [0 "
            "20 -5] _ _ _ _\n");
}
