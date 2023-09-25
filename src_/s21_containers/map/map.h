#include <iostream>

#include "../tree/tree.h"

namespace s21 {

struct PairFirstComparator {
  template <typename PairType>
  bool operator()(const PairType &lhs, const PairType &rhs) const {
    return lhs.first < rhs.first;
  }
};

template <typename Key, typename T>
class map : public RBTree<std::pair<const Key, T>, PairFirstComparator> {
 public:
  using MyBase = RBTree<std::pair<const Key, T>, PairFirstComparator>;
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename MyBase::iterator;
  using const_iterator = typename MyBase::const_iterator;
  using size_type = size_t;
  using Node = typename MyBase::Node;

  using MyBase::RBTree;

  T &at(const Key &key);
  T &operator[](const Key &key);

  bool contains(const Key &key);

  using MyBase::insert;
  std::pair<iterator, bool> insert(const Key &key, const T &obj);
  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj);

 private:
  using MyBase::find;  // delete
};

template <typename Key, typename T>
T &map<Key, T>::at(const Key &key) {
  Node *current = MyBase::root_;
  while (current) {
    if (key == current->data.first) {
      return current->data.second;
    } else if (key < current->data.first) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  throw std::out_of_range("key not found");
}

template <typename Key, typename T>
T &map<Key, T>::operator[](const Key &key) {
  return (*((insert(std::make_pair(key, T()))).first)).second;  // old version
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key &key, const T &obj) {
  return insert(std::make_pair(key, obj));
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const Key &key, const T &obj) {
  Node *current = MyBase::root_;
  while (current) {
    if (key == current->data.first) {
      current->data.second = obj;
      return std::make_pair(iterator(current), false);
    } else if (key < current->data.first) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  return insert(std::make_pair(key, obj));
}

template <typename Key, typename T>
bool map<Key, T>::contains(const Key &key) {
  Node *current = MyBase::root_;

  while (current) {
    if (key < current->data.first) {
      current = current->left;
    } else if (current->data.first < key) {
      current = current->right;
    } else {
      return true;
    }
  }
  return false;
}

}  // namespace s21