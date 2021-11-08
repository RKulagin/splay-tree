// Copyright 2021 RKulagin

#include <gtest/gtest.h>

#include <splay_tree.hpp>

TEST(Splay, Tree) {
  SplayTree<int, int> tree;
  std::stringstream stream;

  tree.insert(8, 10);
  stream << tree;
  EXPECT_EQ(stream.str(), "[8 10]");
  stream.str("");
  tree.insert(4, 14);
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[4 14]\n"
            "_ [8 10 4]");
  stream.str("");
  tree.insert(7, 15);
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[7 15]\n"
            "[4 14 7] [8 10 7]");
  stream.str("");

  tree.at(8) = 11;
  stream << tree;
  EXPECT_EQ(stream.str(), "[8 11]\n[7 15 8] _\n[4 14 7] _ _ _");
  tree.insert(3, 13);
  stream.str("");
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[3 13]\n_ [8 11 3]\n_ _ [4 14 8] _\n_ _ _ _ _ [7 15 4] _ _");
  tree.insert(5, 16);
  EXPECT_THROW(tree.at(88), std::out_of_range);
  EXPECT_EQ(tree.at(7), 15);
}
TEST(Splay, Delete) {
  SplayTree<int, int> tree;

  tree.insert(8, 10);
  tree.insert(4, 14);
  tree.insert(7, 15);
  tree.at(8) = 11;
  std::stringstream stream;
  tree.insert(3, 13);
  tree.insert(5, 16);
  EXPECT_THROW(tree.at(88), std::out_of_range);
  EXPECT_EQ(tree.at(7), 15);
  EXPECT_EQ(tree.erase(5), 16);
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[4 14]\n"
            "[3 13 4] [7 15 4]\n"
            "_ _ _ [8 11 7]");
}

TEST(Splay, HugeInsert) {
  SplayTree<int, int> tree;
  tree.insert(8, 10);
  tree.insert(4, 14);
  tree.insert(7, 15);
  tree.insert(9, 11);
  tree.insert(3, 13);
  tree.insert(5, 16);
  tree.insert(88, 1);
  tree.insert(11, 2);
  tree.insert(6, 18);
  tree.at(11) = 121;
  EXPECT_THROW(tree.at(66), std::out_of_range);

  tree.at(8) = 64;
  tree.at(5) = 25;
  tree.at(88) = 100500;
  std::stringstream stream;
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[88 100500]\n"
            "[5 25 88] _\n"
            "[3 13 5] [8 64 5] _ _\n"
            "_ [4 14 3] [6 18 8] [11 121 8] _ _ _ _\n"
            "_ _ _ _ _ [7 15 6] [9 11 11] _ _ _ _ _ _ _ _ _");
  tree.erase(8);
  tree.erase(4);
  tree.erase(7);
  tree.erase(9);
  tree.erase(3);
  tree.erase(5);
  tree.erase(88);
  tree.erase(11);
  tree.erase(6);
  stream.str("");
  stream << tree;
  EXPECT_EQ(stream.str(), "_");
}

TEST(Splay, SomeTest) {
  SplayTree<int, int> tree;
  std::stringstream stream;

  tree.insert(8, 10);
  stream << tree;
  EXPECT_EQ(stream.str(), "[8 10]");
  stream.str("");

  tree.insert(4, 14);
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[4 14]\n"
            "_ [8 10 4]");
  stream.str("");

  tree.insert(7, 15);
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[7 15]\n"
            "[4 14 7] [8 10 7]");
  stream.str("");

  tree.insert(9, 11);

  stream << tree;
  EXPECT_EQ(stream.str(),
            "[9 11]\n"
            "[8 10 9] _\n"
            "[7 15 8] _ _ _\n"
            "[4 14 7] _ _ _ _ _ _ _");
  stream.str("");

  tree.insert(3, 13);

  stream << tree;
  EXPECT_EQ(stream.str(),
            "[3 13]\n"
            "_ [8 10 3]\n"
            "_ _ [4 14 8] [9 11 8]\n"
            "_ _ _ _ _ [7 15 4] _ _");
  stream.str("");

  tree.insert(5, 16);
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[5 16]\n"
            "[3 13 5] [8 10 5]\n"
            "_ [4 14 3] [7 15 8] [9 11 8]");
  stream.str("");
}

//TEST(Generate, Func){
//  std::stringstream stream;
//  generate_empty(stream, 17);
//  EXPECT_EQ(stream.str(), "_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _");
//}