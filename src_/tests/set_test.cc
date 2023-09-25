#include "../s21_containers/set/set.h"

#include <gtest/gtest.h>

#include <set>

TEST(SetTest, DefaultConstuctor) {
  s21::set<int> s21_set;
  std::set<int> std_set;

  EXPECT_EQ(s21_set.size(), std_set.size());
  EXPECT_EQ(s21_set.empty(), std_set.empty());
  EXPECT_TRUE(s21_set.empty());
  EXPECT_EQ(s21_set.size(), 0);
  EXPECT_EQ(s21_set.begin(), nullptr);
}

TEST(SetTest, ConstuctorInitializerList) {
  s21::set<int> s21_set = {2, 7, 3, 4, -100, 2, 3, 3, 3, 3, 5};
  std::set<int> std_set = {2, 7, 3, 4, -100, 2, 3, 3, 3, 3, 5};

  EXPECT_EQ(s21_set.size(), std_set.size());
  EXPECT_TRUE(s21_set.contains(2));
  EXPECT_TRUE(s21_set.contains(7));
  EXPECT_TRUE(s21_set.contains(3));
  EXPECT_TRUE(s21_set.contains(4));
  EXPECT_TRUE(s21_set.contains(-100));
  EXPECT_TRUE(s21_set.contains(5));
}

TEST(SetTest, CopyConstuctor) {
  s21::set<int> s21_set1 = {1, 2, 2, 2, 3};
  s21::set<int> s21_set2 = s21_set1;

  EXPECT_EQ(s21_set1.size(), s21_set2.size());
  EXPECT_TRUE(s21_set2.contains(1));
  EXPECT_TRUE(s21_set2.contains(2));
  EXPECT_TRUE(s21_set2.contains(3));

  EXPECT_TRUE(s21_set1.contains(1));
  EXPECT_TRUE(s21_set1.contains(2));
  EXPECT_TRUE(s21_set1.contains(3));
}

TEST(SetTest, MoveConstuctor) {
  s21::set<double> s21_set1 = {1, 2, 2, 2, 3};
  s21::set<double> s21_set2 = std::move(s21_set1);

  EXPECT_EQ(s21_set2.size(), 3);
  EXPECT_TRUE(s21_set2.contains(1));
  EXPECT_TRUE(s21_set2.contains(2));
  EXPECT_TRUE(s21_set2.contains(3));

  EXPECT_EQ(s21_set1.size(), 0);
  EXPECT_FALSE(s21_set1.contains(1));
  EXPECT_FALSE(s21_set1.contains(2));
  EXPECT_FALSE(s21_set1.contains(3));
}

TEST(SetTest, CopyAssignment) {
  s21::set<int> s21_set1 = {1, 2, 2, 2, 3};
  s21::set<int> s21_set2 = {4, 5};
  s21_set2 = s21_set1;

  EXPECT_EQ(s21_set1.size(), s21_set2.size());
  EXPECT_TRUE(s21_set2.contains(1));
  EXPECT_TRUE(s21_set2.contains(2));
  EXPECT_TRUE(s21_set2.contains(3));
  EXPECT_FALSE(s21_set2.contains(4));
  EXPECT_FALSE(s21_set2.contains(5));

  EXPECT_TRUE(s21_set1.contains(1));
  EXPECT_TRUE(s21_set1.contains(2));
  EXPECT_TRUE(s21_set1.contains(3));
}

TEST(SetTest, MoveAssignment) {
  s21::set<int> s21_set1 = {1, 2, 2, 2, 3};
  s21::set<int> s21_set2 = {4, 5};
  s21_set2 = std::move(s21_set1);

  EXPECT_EQ(s21_set2.size(), 3);
  EXPECT_TRUE(s21_set2.contains(1));
  EXPECT_TRUE(s21_set2.contains(2));
  EXPECT_TRUE(s21_set2.contains(3));
  EXPECT_FALSE(s21_set2.contains(4));
  EXPECT_FALSE(s21_set2.contains(5));

  EXPECT_EQ(s21_set1.size(), 0);
  EXPECT_FALSE(s21_set1.contains(1));
  EXPECT_FALSE(s21_set1.contains(2));
  EXPECT_FALSE(s21_set1.contains(3));
}

TEST(SetTest, BeginEnd) {
  s21::set<int> s21_set = {-8, 2, -4, 5};
  std::set<int> std_set = {-8, 2, -4, 5};

  EXPECT_EQ(*s21_set.begin(), *std_set.begin());
  EXPECT_EQ(5, *(--s21_set.end()));
}

// add iterators tests

TEST(SetTest, Empty) {
  s21::set<int> empty_set;
  std::set<int> not_empty_set = {1, 3, 2, 2};

  EXPECT_TRUE(empty_set.empty());
  EXPECT_FALSE(not_empty_set.empty());
  not_empty_set.clear();
  EXPECT_TRUE(not_empty_set.empty());
}

TEST(SetTest, Size) {
  s21::set<int> s21_set1 = {1, 1, 1, 1, 1, 1};
  std::set<int> std_set1 = {1, 1, 1, 1, 1, 1};
  EXPECT_EQ(s21_set1.size(), std_set1.size());

  s21::set<double> s21_set2 = {1.56};
  std::set<double> std_set2 = {1.56};
  EXPECT_EQ(s21_set2.size(), std_set2.size());

  s21::set<std::string> s21_set3 = {"ccv", "wedf", "dssv", ""};
  std::set<std::string> std_set3 = {"ccv", "wedf", "dssv", ""};
  EXPECT_EQ(s21_set3.size(), std_set3.size());

  s21::set<bool> s21_set4;
  std::set<bool> std_set4;
  EXPECT_EQ(s21_set4.size(), std_set4.size());
}

TEST(SetTest, MaxSize) {
  s21::set<int> s21_set1 = {1, 1, 1, 1, 1, 1};
  std::set<int> std_set1 = {1, 1, 1, 1, 1, 1};
  EXPECT_EQ(s21_set1.max_size(), std_set1.max_size());

  s21::set<double> s21_set2 = {1.56};
  std::set<double> std_set2 = {1.56};
  EXPECT_EQ(s21_set2.max_size(), std_set2.max_size());

  s21::set<std::string> s21_set3 = {"ccv", "wedf", "dssv", ""};
  std::set<std::string> std_set3 = {"ccv", "wedf", "dssv", ""};
  EXPECT_EQ(s21_set3.max_size(), std_set3.max_size());

  s21::set<bool> s21_set4;
  std::set<bool> std_set4;
  EXPECT_EQ(s21_set4.max_size(), std_set4.max_size());
}

TEST(SetTest, Clear) {
  s21::set<std::string> s21_set = {"ccv", "wedf", "dssv", ""};
  s21_set.clear();
  EXPECT_TRUE(s21_set.empty());

  s21_set.insert("AEWAD");
  s21_set.insert("fvf");
  EXPECT_FALSE(s21_set.empty());
  s21_set.clear();
  EXPECT_TRUE(s21_set.empty());
}

TEST(SetTest, Insert) {
  s21::set<int> s21_set;
  std::set<int> std_set;

  auto s21_result = s21_set.insert(1);
  auto std_result = std_set.insert(1);
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(*s21_result.first, *std_result.first);

  s21_result = s21_set.insert(2);
  std_result = std_set.insert(2);
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(*s21_result.first, *std_result.first);

  s21_result = s21_set.insert(1);
  std_result = std_set.insert(1);
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(*s21_result.first, *std_result.first);

  EXPECT_EQ(s21_set.size(), std_set.size());

  s21::set<std::string> s21_set2 = {"a", "abc", "", "d"};
  std::set<std::string> std_set2 = {"a", "abc", "", "d"};

  auto s21_result2 = s21_set2.insert("a");
  auto std_result2 = std_set2.insert("a");
  EXPECT_EQ(s21_result2.second, std_result2.second);
  EXPECT_EQ(*s21_result2.first, *std_result2.first);

  s21_result2 = s21_set2.insert("arara");
  std_result2 = std_set2.insert("arara");
  EXPECT_EQ(s21_result2.second, std_result2.second);
  EXPECT_EQ(*s21_result2.first, *std_result2.first);

  EXPECT_EQ(s21_set2.size(), std_set2.size());
}

TEST(SetTest, Erase) {
  s21::set<int> s21_set = {1, 2, 3, 4};
  std::set<int> std_set = {1, 2, 3, 4};
  s21_set.deleteNode(s21_set.begin());
  std_set.erase(std_set.begin());
  // EXPECT_FALSE(s21_set.contains(22));
  EXPECT_EQ(s21_set.size(), std_set.size());

  /*
s21::set<int> s21_set = {10, 20, 15, 1, 22, 0, 12, 78, 11};
  std::set<int> std_set = {10, 20, 15, 1, 22, 0, 12, 78, 11};
  s21_set.deleteNode(++ ++ ++ ++ ++ ++ ++s21_set.begin());
  std_set.erase(++ ++ ++ ++ ++ ++ ++std_set.begin());
  EXPECT_FALSE(s21_set.contains(22));
  EXPECT_EQ(s21_set.size(), std_set.size());

    s21_set.erase(++ ++ ++s21_set.begin());
    std_set.erase(++ ++ ++std_set.begin());
    EXPECT_EQ(s21_set.size(), std_set.size());
    EXPECT_FALSE(s21_set.contains(4));

      s21_set.erase(++s21_set.begin());
      std_set.erase(++std_set.begin());
      EXPECT_EQ(s21_set.size(), std_set.size());
      EXPECT_FALSE(s21_set.contains(3));

      s21_set.erase(--s21_set.end());
      std_set.erase(--std_set.end());
      EXPECT_EQ(s21_set.size(), std_set.size());
      EXPECT_FALSE(s21_set.contains(5));

      s21_set.insert(3);
      s21_set.insert(3);
      std_set.insert(3);
      std_set.insert(3);
      EXPECT_TRUE(s21_set.contains(3));
      s21_set.erase(s21_set.find(3));
      std_set.erase(std_set.find(3));
      EXPECT_FALSE(s21_set.contains(3));
      EXPECT_EQ(s21_set.size(), std_set.size());*/
}
/*
TEST(SetTest, Swap) {
  s21::set<int> s21_set1 = {1, 2, 3};
  s21::set<int> s21_set2 = {3, 4, 5, 6};
  s21_set1.swap(s21_set2);
  EXPECT_EQ(s21_set1.size(), 4);
  EXPECT_EQ(s21_set2.size(), 3);

  EXPECT_TRUE(s21_set1.contains(3));
  EXPECT_TRUE(s21_set1.contains(4));
  EXPECT_TRUE(s21_set1.contains(5));
  EXPECT_TRUE(s21_set1.contains(6));
  EXPECT_FALSE(s21_set1.contains(1));
  EXPECT_FALSE(s21_set1.contains(2));

  EXPECT_TRUE(s21_set2.contains(1));
  EXPECT_TRUE(s21_set2.contains(2));
  EXPECT_TRUE(s21_set2.contains(3));
  EXPECT_FALSE(s21_set2.contains(4));
  EXPECT_FALSE(s21_set2.contains(5));
  EXPECT_FALSE(s21_set2.contains(6));

  s21::set<std::string> s21_set3 = {"aboba", "abc"};
  s21::set<std::string> s21_set4;
  s21_set3.swap(s21_set4);
  EXPECT_EQ(s21_set3.size(), 0);
  EXPECT_EQ(s21_set4.size(), 2);
  EXPECT_TRUE(s21_set4.contains("aboba"));
  EXPECT_TRUE(s21_set4.contains("abc"));

  s21_set3.swap(s21_set4);
  EXPECT_EQ(s21_set4.size(), 0);
  EXPECT_EQ(s21_set3.size(), 2);
}

TEST(SetTest, Merge) {
  s21::set<int> s21_set1 = {1, 2, 3};
  s21::set<int> s21_set2 = {3, 4, 5, 6};

  std::set<int> std_set1 = {1, 2, 3};
  std::set<int> std_set2 = {3, 4, 5, 6};

  s21_set1.merge(s21_set2);
  std_set1.merge(std_set2);

  EXPECT_EQ(s21_set1.size(), std_set1.size());
  EXPECT_EQ(s21_set2.size(), std_set2.size());
  EXPECT_EQ(s21_set2.contains(3), std_set2.count(3) > 0);

  EXPECT_TRUE(s21_set1.contains(1));
  EXPECT_TRUE(s21_set1.contains(2));
  EXPECT_TRUE(s21_set1.contains(3));
  EXPECT_TRUE(s21_set1.contains(4));
  EXPECT_TRUE(s21_set1.contains(5));
  EXPECT_TRUE(s21_set1.contains(6));

  s21::set<std::string> s21_set3 = {"aboba", "abc"};
  s21::set<std::string> s21_set4;
  s21_set3.merge(s21_set4);
  EXPECT_EQ(s21_set3.size(), 2);
  EXPECT_EQ(s21_set4.size(), 0);
  s21_set4.merge(s21_set3);
  EXPECT_EQ(s21_set3.size(), 0);
  EXPECT_EQ(s21_set4.size(), 2);
}

TEST(SetTest, Find) {
  s21::set<int> s21_set = {6, 3, 89, -1, 3, 6, 3, 5, 4, 2};
  std::set<int> std_set = {6, 3, 89, -1, 3, 6, 3, 5, 4, 2};

  auto s21_it = s21_set.find(-1);
  auto std_it = std_set.find(-1);
  EXPECT_EQ(*s21_it, *std_it);
  s21_set.erase(s21_it);
  std_set.erase(std_it);
  EXPECT_FALSE(s21_set.contains(-1));

  s21_it = s21_set.find(89);
  std_it = std_set.find(89);
  EXPECT_EQ(*s21_it, *std_it);
  s21_set.erase(s21_it);
  std_set.erase(std_it);
  EXPECT_FALSE(s21_set.contains(89));

  s21_it = s21_set.find(3);
  std_it = std_set.find(3);
  EXPECT_EQ(*s21_it, *std_it);
  s21_set.erase(s21_it);
  std_set.erase(std_it);
  EXPECT_FALSE(s21_set.contains(3));

  s21_it = s21_set.find(-100);
  std_it = std_set.find(-100);
  EXPECT_TRUE(s21_it == s21_set.end());
  EXPECT_TRUE(std_it == std_set.end());
}

TEST(SetTest, Contains) {
  s21::set<int> s21_set = {-3, 4, 17, 2, 2, 2, 2, 3};
  s21_set.insert(0);
  EXPECT_TRUE(s21_set.contains(0));
  EXPECT_TRUE(s21_set.contains(-3));
  EXPECT_TRUE(s21_set.contains(17));
  EXPECT_TRUE(s21_set.contains(2));
  EXPECT_FALSE(s21_set.contains(555));

  s21::set<double> empty;
  EXPECT_FALSE(empty.contains(23.2));
}
*/