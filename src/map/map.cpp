#include <iostream>

#include "../tree/tree.cpp"

template <typename Key, typename T>
class map : public Tree<Key, T> {
 public:
  using key_type = Key;
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = Tree::iterator;
  using const_iterator = Tree::const_iterator;
  using size_type = size_t;
  map();
  map(std::initializer_list<value_type> const &items);
  map(const map &m);
  map(map &&m);
  ~map();
  operator=(map &&m);
  T &at(const Key &key);
  T &operator[](const Key &key);
  iterator begin();
  iterator end();
  bool empty();
  size_type size();
  size_type max_size();
  void clear();
  std::pair<iterator, bool> insert(const value_type &value);
  std::pair<iterator, bool> insert(const Key &key, const T &obj);
  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj);
  void erase(iterator pos);
  void swap(map &other);
  void merge(map &other);
  bool contains(const Key &key);
};