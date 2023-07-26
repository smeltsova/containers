#include <iostream>

#include "../tree/tree.cpp"

template <typename Key>
class set : public Tree<Key, Key> {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = Tree::iterator;
  using const_iterator = Tree::const_iterator;
  using size_type = size_t;
  set();
  set(std::initializer_list<value_type> const &items);
  set(const set &s);
  set(set &&s);
  ~set();
  operator=(set &&s);
  iterator begin();  //+
  iterator end();    //+
  bool empty();
  size_type size();
  size_type max_size();
  void clear();
  std::pair<iterator, bool> insert(const value_type &value);
  void erase(iterator pos);
  void swap(set &other);
  void merge(set &other);
  iterator find(const Key &key);
  bool contains(const Key &key);
};

int main() {
  set<int> A;
  A.insert(1);
  std::cout << (A.find(1) == nullptr);
  std::cout << (A.find(2) == nullptr);

  Tree<int, int> b;
  b.insert(1);
};