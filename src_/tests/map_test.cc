#include "../s21_containers/map/map.h"

#include <gtest/gtest.h>

#include <map>

TEST(MapTest, DefaultConstructor) {
  s21::map<int, std::string> s21_map;
  std::map<int, std::string> std_map;

  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_map.empty(), std_map.empty());
  EXPECT_TRUE(s21_map.empty());
  EXPECT_EQ(s21_map.size(), 0);
}

TEST(MapTest, ConstuctorInitializerList) {
  s21::map<std::string, int> s21_map = {
      {"apple", 2}, {"banana", 5}, {"cherry", 3}};
  std::map<std::string, int> std_map = {
      {"apple", 2}, {"banana", 5}, {"cherry", 3}};

  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_map["apple"], std_map["apple"]);
  EXPECT_EQ(s21_map["banana"], std_map["banana"]);
  EXPECT_EQ(s21_map["cherry"], std_map["cherry"]);
}

TEST(MapTest, CopyConstructor) {
  s21::map<int, double> s21_map1 = {{1, 1.1}, {2, 2.2}, {3, 3.3}};
  s21::map<int, double> s21_map2 = s21_map1;

  EXPECT_EQ(s21_map1.size(), s21_map2.size());
  EXPECT_EQ(s21_map2[1], 1.1);
  EXPECT_EQ(s21_map2[2], 2.2);
  EXPECT_EQ(s21_map2[3], 3.3);

  EXPECT_EQ(s21_map1[1], 1.1);
  EXPECT_EQ(s21_map1[2], 2.2);
  EXPECT_EQ(s21_map1[3], 3.3);
}

TEST(MapTest, MoveConstructor) {
  s21::map<std::string, int> s21_map1 = {
      {"apple", 2}, {"banana", 5}, {"cherry", 3}};
  s21::map<std::string, int> s21_map2 = std::move(s21_map1);

  EXPECT_EQ(s21_map2.size(), 3);
  EXPECT_EQ(s21_map2["apple"], 2);
  EXPECT_EQ(s21_map2["banana"], 5);
  EXPECT_EQ(s21_map2["cherry"], 3);

  EXPECT_EQ(s21_map1.size(), 0);
  EXPECT_FALSE(s21_map1.contains("apple"));
  EXPECT_FALSE(s21_map1.contains("banana"));
  EXPECT_FALSE(s21_map1.contains("cherry"));
}

TEST(MapTest, CopyAssignment) {
  s21::map<int, std::string> s21_map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  s21::map<int, std::string> s21_map2 = {{4, "four"}, {5, "five"}};
  s21_map2 = s21_map1;

  EXPECT_EQ(s21_map1.size(), s21_map2.size());
  EXPECT_EQ(s21_map2[1], "one");
  EXPECT_EQ(s21_map2[2], "two");
  EXPECT_EQ(s21_map2[3], "three");
  EXPECT_FALSE(s21_map2.contains(4));
  EXPECT_FALSE(s21_map2.contains(5));

  EXPECT_EQ(s21_map1[1], "one");
  EXPECT_EQ(s21_map1[2], "two");
  EXPECT_EQ(s21_map1[3], "three");
  EXPECT_FALSE(s21_map1.contains(4));
  EXPECT_FALSE(s21_map1.contains(5));
}

TEST(MapTest, MoveAssignment) {
  s21::map<int, double> s21_map1 = {{1, 1.1}, {2, 2.2}, {3, 3.3}};
  s21::map<int, double> s21_map2 = {{4, 4.4}, {5, 5.5}};
  s21_map2 = std::move(s21_map1);

  EXPECT_EQ(s21_map2.size(), 3);
  EXPECT_EQ(s21_map2[1], 1.1);
  EXPECT_EQ(s21_map2[2], 2.2);
  EXPECT_EQ(s21_map2[3], 3.3);

  EXPECT_EQ(s21_map1.size(), 0);
  EXPECT_FALSE(s21_map1.contains(1));
  EXPECT_FALSE(s21_map1.contains(2));
  EXPECT_FALSE(s21_map1.contains(3));
}

TEST(MapTest, AtMethod) {
  s21::map<std::string, int> s21_map = {
      {"apple", 2}, {"banana", 5}, {"cherry", 3}};

  EXPECT_EQ(s21_map.at("apple"), 2);
  EXPECT_EQ(s21_map.at("banana"), 5);
  EXPECT_EQ(s21_map.at("cherry"), 3);

  EXPECT_THROW(s21_map.at("orange"), std::out_of_range);
}

TEST(MapTest, OperatorBracket) {
  s21::map<std::string, int> s21_map = {
      {"apple", 2}, {"banana", 5}, {"cherry", 3}};

  EXPECT_EQ(s21_map["apple"], 2);
  EXPECT_EQ(s21_map["banana"], 5);
  EXPECT_EQ(s21_map["cherry"], 3);

  EXPECT_EQ(s21_map["orange"], 0);

  EXPECT_EQ(s21_map.size(), 4);
  EXPECT_EQ(s21_map["orange"], 0);

  s21_map["banana"] = 7;
  EXPECT_EQ(s21_map["banana"], 7);

  std::string key = "grape";
  int& value = s21_map[key];
  EXPECT_EQ(value, 0);

  value = 9;
  EXPECT_EQ(s21_map["grape"], 9);
}

TEST(MapTest, BeginEnd) {
  s21::map<int, std::string> s21_map = {
      {100, "100"}, {200, "200"}, {300, "300"}};

  s21::map<int, std::string>::iterator begin_iter = s21_map.begin();
  s21::map<int, std::string>::iterator end_iter = s21_map.end();

  EXPECT_EQ(begin_iter->first, 100);
  EXPECT_EQ(begin_iter->second, "100");

  --end_iter;
  EXPECT_EQ(end_iter->first, 300);
  EXPECT_EQ(end_iter->second, "300");

  int expected_key = 100;
  for (auto iter = s21_map.begin(); iter != s21_map.end(); ++iter) {
    EXPECT_EQ(iter->first, expected_key);
    EXPECT_EQ(iter->second, std::to_string(expected_key));
    expected_key += 100;
  }

  expected_key = 300;
  for (auto iter = --s21_map.end(); iter != s21_map.begin(); --iter) {
    EXPECT_EQ(iter->first, expected_key);
    EXPECT_EQ(iter->second, std::to_string(expected_key));
    expected_key -= 100;
  }

  expected_key = 100;
  for (const auto& pair : s21_map) {
    EXPECT_EQ(pair.first, expected_key);
    EXPECT_EQ(pair.second, std::to_string(expected_key));
    expected_key += 100;
  }
}

TEST(MapTest, Empty) {
  s21::map<int, std::string> empty_map;
  s21::map<int, std::string> non_empty_map = {
      {1, "one"}, {2, "two"}, {3, "three"}};

  EXPECT_TRUE(empty_map.empty());

  EXPECT_FALSE(non_empty_map.empty());
}

TEST(MapTest, Size) {
  s21::map<int, std::string> empty_map;
  s21::map<int, std::string> non_empty_map = {
      {1, "one"}, {2, "two"}, {3, "three"}};

  EXPECT_EQ(empty_map.size(), 0);

  EXPECT_EQ(non_empty_map.size(), 3);

  non_empty_map.insert({4, "four"});
  EXPECT_EQ(non_empty_map.size(), 4);
}

TEST(MapTest, MaxSize) {
  s21::map<int, int> s21_map1 = {{1, 1}, {2, 2}, {3, 3}};
  std::map<int, int> std_map1 = {{1, 1}, {2, 2}, {3, 3}};
  EXPECT_EQ(s21_map1.max_size(), std_map1.max_size());

  s21::map<double, double> s21_map2 = {{1.56, 1.56}};
  std::map<double, double> std_map2 = {{1.56, 1.56}};
  EXPECT_EQ(s21_map2.max_size(), std_map2.max_size());

  s21::map<std::string, int> s21_map3 = {
      {"ccv", 5}, {"wedf", 10}, {"dssv", 7}, {"", 0}};
  std::map<std::string, int> std_map3 = {
      {"ccv", 5}, {"wedf", 10}, {"dssv", 7}, {"", 0}};
  EXPECT_EQ(s21_map3.max_size(), std_map3.max_size());

  s21::map<bool, bool> s21_map4;
  std::map<bool, bool> std_map4;
  EXPECT_EQ(s21_map4.max_size(), std_map4.max_size());
}

TEST(MapTest, SwapMethod) {
  s21::map<int, std::string> s21_map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  s21::map<int, std::string> s21_map2 = {{4, "four"}, {5, "five"}};

  std::map<int, std::string> std_map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  std::map<int, std::string> std_map2 = {{4, "four"}, {5, "five"}};

  s21_map1.swap(s21_map2);
  std_map1.swap(std_map2);

  EXPECT_EQ(s21_map1.size(), std_map1.size());
  EXPECT_EQ(s21_map2.size(), std_map2.size());

  EXPECT_EQ(s21_map1[4], "four");
  EXPECT_EQ(s21_map1[5], "five");

  EXPECT_EQ(s21_map2[1], "one");
  EXPECT_EQ(s21_map2[2], "two");
  EXPECT_EQ(s21_map2[3], "three");

  s21_map2.swap(s21_map1);
  std_map2.swap(std_map1);

  EXPECT_EQ(s21_map1.size(), std_map1.size());
  EXPECT_EQ(s21_map2.size(), std_map2.size());

  EXPECT_EQ(s21_map1[4], std_map1[4]);
  EXPECT_EQ(s21_map2[5], std_map2[5]);
}

TEST(MapTest, MergeMethod) {
  s21::map<int, std::string> s21_map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  s21::map<int, std::string> s21_map2 = {{4, "four"}, {5, "five"}};

  s21_map1.merge(s21_map2);

  EXPECT_EQ(s21_map1.size(), 5);
  EXPECT_EQ(s21_map1[1], "one");
  EXPECT_EQ(s21_map1[2], "two");
  EXPECT_EQ(s21_map1[3], "three");
  EXPECT_EQ(s21_map1[4], "four");
  EXPECT_EQ(s21_map1[5], "five");

  EXPECT_TRUE(s21_map2.empty());
}

TEST(MapTest, MergeMethod2) {
  s21::map<int, std::string> s21_map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  s21::map<int, std::string> s21_map2 = {
      {3, "modified_three"}, {4, "four"}, {5, "five"}};

  std::map<int, std::string> std_map1 = {{1, "one"}, {2, "two"}, {3, "three"}};
  std::map<int, std::string> std_map2 = {
      {3, "modified_three"}, {4, "four"}, {5, "five"}};

  s21_map1.merge(s21_map2);
  std_map1.merge(std_map2);

  EXPECT_EQ(s21_map1.size(), 5);
  EXPECT_EQ(s21_map1.size(), std_map1.size());
  EXPECT_EQ(s21_map1[1], "one");
  EXPECT_EQ(s21_map1[2], "two");
  EXPECT_EQ(s21_map1[3], "three");
  EXPECT_EQ(s21_map1[4], "four");
  EXPECT_EQ(s21_map1[5], "five");

  /*
  const std::string& ref1 = s21_map1[1];
  const std::string& ref3 = s21_map1[3];
  const std::string& ref4 = s21_map1[4];

  s21_map2.merge(s21_map1);

  EXPECT_EQ(ref1, "one");
  EXPECT_EQ(ref3, "three");
  EXPECT_EQ(ref4, "four");*/
}

TEST(MapTest, InsertValue) {
  s21::map<std::string, int> s21_map;
  std::map<std::string, int> std_map;

  auto s21_result = s21_map.insert({"apple", 2});
  auto std_result = std_map.insert({"apple", 2});
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(s21_result.first->second, std_result.first->second);

  s21_result = s21_map.insert({"banana", 5});
  std_result = std_map.insert({"banana", 5});
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(s21_result.first->second, std_result.first->second);

  s21_result = s21_map.insert({"apple", 3});
  std_result = std_map.insert({"apple", 3});
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(s21_result.first->second, std_result.first->second);
}

TEST(MapTest, InsertKeyValue) {
  s21::map<int, std::string> s21_map;
  std::map<int, std::string> std_map;

  auto s21_result = s21_map.insert(1, "one");
  auto std_result = std_map.insert(std::make_pair(1, "one"));
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(s21_result.first->second, std_result.first->second);

  s21_result = s21_map.insert(2, "two");
  std_result = std_map.insert(std::make_pair(2, "two"));
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(s21_result.first->second, std_result.first->second);

  s21_result = s21_map.insert(1, "uno");
  std_result = std_map.insert(std::make_pair(1, "uno"));
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_EQ(s21_result.second, std_result.second);
  EXPECT_EQ(s21_result.first->second, std_result.first->second);
}

TEST(MapTest, Erase) {
  s21::map<int, std::string> s21_map = {
      {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};
  std::map<int, std::string> std_map = {
      {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};

  s21_map.erase(s21_map.begin());
  std_map.erase(std_map.begin());
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_FALSE(s21_map.contains(1));

  s21_map.erase(++s21_map.begin());
  std_map.erase(++std_map.begin());
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_FALSE(s21_map.contains(3));

  s21_map.erase(--s21_map.end());
  std_map.erase(--std_map.end());
  EXPECT_EQ(s21_map.size(), std_map.size());
  EXPECT_FALSE(s21_map.contains(4));
}

TEST(MapTest, InsertOrAssign) {
  s21::map<std::string, int> s21_map;

  auto [inserted1, success1] = s21_map.insert_or_assign("apple", 2);
  EXPECT_TRUE(success1);
  EXPECT_EQ(inserted1->first, "apple");
  EXPECT_EQ(inserted1->second, 2);

  auto [inserted2, success2] = s21_map.insert_or_assign("apple", 3);
  EXPECT_FALSE(success2);
  EXPECT_EQ(inserted2->first, "apple");
  EXPECT_EQ(inserted2->second, 3);

  auto [inserted3, success3] = s21_map.insert_or_assign("banana", 5);
  EXPECT_TRUE(success3);
  EXPECT_EQ(inserted3->first, "banana");
  EXPECT_EQ(inserted3->second, 5);

  EXPECT_EQ(s21_map.size(), 2);
  EXPECT_EQ(s21_map["apple"], 3);
  EXPECT_EQ(s21_map["banana"], 5);
}

TEST(MapTest, ContainsMethod) {
  s21::map<int, std::string> s21_map = {{1, "one"}, {2, "two"}, {3, "three"}};

  EXPECT_TRUE(s21_map.contains(1));
  EXPECT_TRUE(s21_map.contains(2));
  EXPECT_TRUE(s21_map.contains(3));

  EXPECT_FALSE(s21_map.contains(0));
  EXPECT_FALSE(s21_map.contains(4));
  EXPECT_FALSE(s21_map.contains(5));

  s21_map.insert(4, "four");
  EXPECT_TRUE(s21_map.contains(4));

  s21::map<int, std::string> empty_map;
  EXPECT_FALSE(empty_map.contains(1));
}