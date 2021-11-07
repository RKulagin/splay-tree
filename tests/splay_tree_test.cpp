// Copyright 2021 RKulagin

#include <gtest/gtest.h>
#include <splay_tree.h>

TEST(Splay, Tree) {
  SplayTree<int, int> tree;

  tree.insert(8, 10);
  tree.insert(4, 14);
  tree.insert(7, 15);
  tree.at(8) = 11;
  std::stringstream stream;
  stream << tree;
  EXPECT_EQ(stream.str(), "[8 11]\n[7 15 8] _\n[4 14 7] _ _ _\n");
  tree.insert(3, 13);
  stream.str("");
  stream << tree;
  EXPECT_EQ(stream.str(),
            "[3 13]\n_ [8 11 3]\n_ _ [4 14 8] _\n_ _ _ _ _ [7 15 4] _ _\n");
  tree.insert(5, 16);
  EXPECT_THROW(tree.at(88), std::out_of_range);
  EXPECT_EQ(tree.at(7), 15);
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
            "_ _ _ _ _ [7 15 6] [9 11 11] _ _ _ _ _ _ _ _ _\n");
}