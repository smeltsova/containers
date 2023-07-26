#include "tree.h"

#include <iostream>
#include <set>

int main() {
  Tree<int, int> a;
  a.insert(1);
  a.insert(4);
  a.insert(-1);
  a.insert(5);
  a.insert(2);
  a.insert(3);
  for (auto it : a) {
    std::cout << it << " ";
  }
  return 0;
}

// iterator

template <typename Key, typename T>
typename Tree<Key, T>::iterator& Tree<Key, T>::iterator::operator++() {
  if (node_ == nullptr) return *this;
  if (node_->right) {
    node_ = node_->right;
    while (node_->left) {
      node_ = node_->left;
    }
  } else {
    Node* parent = node_->parent;
    while (parent != nullptr && node_ == parent->right) {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
  }
  return *this;
};

template <typename Key, typename T>
typename Tree<Key, T>::iterator Tree<Key, T>::iterator::operator++(int) {
  iterator tmp = *this;
  ++(*this);
  return tmp;
};

template <typename Key, typename T>
typename Tree<Key, T>::iterator& Tree<Key, T>::iterator::operator--() {
  if (node_ == nullptr) {
    node_ = MaximumKey(root_);
  } else if (node_->left) {
    node_ = node_->left;
    while (node_->right) {
      node_ = node_->right;
    }
  } else {
    Node* parent = node_->parent;
    while (parent != nullptr && node_ == parent->left) {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
  }
  return *this;
}

template <typename Key, typename T>
typename Tree<Key, T>::iterator Tree<Key, T>::iterator::operator--(int) {
  iterator tmp = *this;
  --(*this);
  return tmp;
}

template <typename Key, typename T>
typename Tree<Key, T>::Node* Tree<Key, T>::iterator::MaximumKey(Node* root) {
  if (root == nullptr) return nullptr;
  if (root->right == nullptr) return root;
  return MaximumKey(root->right);
}

template <typename Key, typename T>
Key& Tree<Key, T>::iterator::operator*() {
  if (node_ == nullptr) {
    static Key default_value;
    return default_value;
  }
  return node_->key;
}

template <typename Key, typename T>
Key& Tree<Key, T>::iterator::operator->() {
  return &(node_->key);
}

template <typename Key, typename T>
bool Tree<Key, T>::iterator::operator==(const iterator& other) {
  return node_ == other.node_;
}

template <typename Key, typename T>
bool Tree<Key, T>::iterator::operator!=(const iterator& other) {
  return node_ != other.node_;
}

// tree

template <typename Key, typename T>
typename Tree<Key, T>::iterator Tree<Key, T>::find(const Key& key) {
  Node* cur = root;
  while (cur) {
    if (cur->key == key) {
      return iterator(cur, root);
    }
    if (cur->key < key) {
      cur = cur->right;
    } else {
      cur = cur->left;
    }
  }
  return iterator(cur, root);
}

template <typename Key, typename T>
typename Tree<Key, T>::Node* Tree<Key, T>::insert(Node* node, Node* parent,
                                                  Key key) {
  if (!node) {
    node = new Node(key, parent);
    return node;
  }
  if (node->key > key) {
    node->left = insert(node->left, node, key);
  } else {
    node->right = insert(node->right, node, key);
  }
  return node;
};

template <typename Key, typename T>
void Tree<Key, T>::insert(Key key) {
  root = insert(root, nullptr, key);
};

template <typename Key, typename T>
typename Tree<Key, T>::iterator Tree<Key, T>::begin() {
  Node* minimum_node = root;
  while (minimum_node->left) {
    minimum_node = minimum_node->left;
  }
  return iterator(minimum_node, root);
};

template <typename Key, typename T>
typename Tree<Key, T>::iterator Tree<Key, T>::end() {
  return iterator(nullptr, root);
};