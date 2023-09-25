#include <iostream>
#include <limits>

#ifndef tree_h
#define tree_h

namespace s21 {

template <typename T, typename Compare = std::less<T>>
class RBTree {
 public:
  class Node;

  template <bool IsConst>
  class RBTreeIteratorBase;

  using value_type = T;
  using iterator = RBTreeIteratorBase<false>;  // true for set?
  using const_iterator = RBTreeIteratorBase<true>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

 protected:
  Node* root_;
  size_type size_;
  Compare comparator_ = Compare();

 public:
  RBTree();
  RBTree(std::initializer_list<value_type> const& items);
  RBTree(const RBTree& other);
  RBTree(RBTree&& other);

  RBTree& operator=(const RBTree& other);
  RBTree& operator=(RBTree&& other);

  ~RBTree();

  iterator begin() const;
  iterator end() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void clear();
  std::pair<iterator, bool> insert(const value_type& value);
  void erase(iterator pos);
  void swap(RBTree& other);
  void merge(RBTree& other);

  iterator find(const T& key) const;
  bool contains(const T& key) const;

 private:
  void copyTree(const Node* source_node, Node* parent);
  void deleteTree(Node* node);
  void helper(Node* node);

  Node* createNode(const value_type& value);
  void attachNodeToTree(Node* new_node, Node* parent);
  void fixInsertion(Node* node);
  void leftRotate(Node* node);
  void rightRotate(Node* node);

  void transplant(Node* u, Node* v);
  void deleteFixup(Node* x, Node* x_parent);
  bool isRed(Node* node);
  bool isBlack(Node* node);
  void turnRed(Node* node);
  void turnBlack(Node* node);

 public:
  // Constructor, Insertion, and other methods...

  // Function to delete a node with a given key
  void deleteNode(int key);
  void deleteNode(iterator pos);
};

template <typename T, typename Compare>
class RBTree<T, Compare>::Node {
 public:
  enum Color { RED, BLACK };

  T data;
  Node* parent;
  Node* left;
  Node* right;
  Color color;

  Node(T val)
      : data(val),
        parent(nullptr),
        left(nullptr),
        right(nullptr),
        color(Color::RED) {}
};

template <typename T, typename Compare>
template <bool IsConst>
class RBTree<T, Compare>::RBTreeIteratorBase {
  friend RBTree;

 public:
  using Nodeptr = typename std::conditional<IsConst, const Node*, Node*>::type;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = std::conditional_t<IsConst, const T, T>;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  RBTreeIteratorBase(Nodeptr node, Nodeptr root = nullptr)
      : current_(node), root_(root) {}

  RBTreeIteratorBase& operator++() {
    if (current_->right) {
      current_ = current_->right;
      while (current_->left) current_ = current_->left;
    } else {
      Nodeptr temp = current_->parent;
      while (temp && current_ == temp->right) {
        current_ = temp;
        temp = temp->parent;
      }
      current_ = temp;
    }
    return *this;
  }

  RBTreeIteratorBase operator++(int) {
    RBTreeIteratorBase tmp = *this;
    ++(*this);
    return tmp;
  }

  Nodeptr maximumKey(Nodeptr root) const {
    Nodeptr node = root;
    while (node && node->right) {
      node = node->right;
    }
    return node;
  }

  RBTreeIteratorBase& operator--() {
    if (!current_) {
      current_ = maximumKey(root_);
    } else if (current_->left) {
      current_ = current_->left;
      while (current_->right) current_ = current_->right;
    } else {
      Nodeptr temp = current_->parent;
      while (temp && current_ == temp->left) {
        current_ = temp;
        temp = temp->parent;
      }
      current_ = temp;
    }
    return *this;
  }

  RBTreeIteratorBase operator--(int) {
    RBTreeIteratorBase tmp = *this;
    --(*this);
    return tmp;
  }

  reference operator*() { return current_->data; }

  pointer operator->() { return &current_->data; }

  bool operator==(const RBTreeIteratorBase& other) const {
    return current_ == other.current_;
  }

  bool operator!=(const RBTreeIteratorBase& other) const {
    return current_ != other.current_;
  }

 private:
  Nodeptr current_;
  Nodeptr root_;  // for end
};

template <typename T, typename Compare>
RBTree<T, Compare>::RBTree() : root_(nullptr), size_(0) {}

template <typename T, typename Compare>
RBTree<T, Compare>::RBTree(std::initializer_list<value_type> const& items)
    : RBTree() {
  for (const auto& i : items) {
    insert(i);
  }
}

template <typename T, typename Compare>
RBTree<T, Compare>::RBTree(const RBTree& other)
    : root_(nullptr), size_(other.size_) {
  if (other.root_) {
    copyTree(other.root_, nullptr);
  }
}

template <typename T, typename Compare>
void RBTree<T, Compare>::copyTree(const Node* source_node, Node* parent) {
  if (source_node) {
    Node* new_node = new Node(source_node->data);
    new_node->color = source_node->color;
    new_node->parent = parent;

    if (!root_) {
      root_ = new_node;
    } else {
      if (comparator_(new_node->data, parent->data)) {
        parent->left = new_node;
      } else {
        parent->right = new_node;
      }
    }

    copyTree(source_node->left, new_node);
    copyTree(source_node->right, new_node);
  }
}

template <typename T, typename Compare>
RBTree<T, Compare>::RBTree(RBTree&& other)
    : root_(other.root_), size_(other.size_) {
  other.root_ = nullptr;
  other.size_ = 0;
}

template <typename T, typename Compare>
RBTree<T, Compare>& RBTree<T, Compare>::operator=(const RBTree& other) {
  if (this == &other) {
    return *this;
  }

  clear();
  if (other.root_) {
    copyTree(other.root_, nullptr);
    size_ = other.size_;
  }
  return *this;
}

template <typename T, typename Compare>
RBTree<T, Compare>& RBTree<T, Compare>::operator=(RBTree&& other) {
  if (this == &other) {
    return *this;
  }

  clear();
  root_ = other.root_;
  size_ = other.size_;
  other.root_ = nullptr;
  other.size_ = 0;
  return *this;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::clear() {
  deleteTree(root_);
  root_ = nullptr;
  size_ = 0;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::deleteTree(Node* node) {
  if (node) {
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }
}

template <typename T, typename Compare>
void RBTree<T, Compare>::helper(Node* node) {
  if (node) {
    helper(node->left);
    helper(node->right);
  }
}

template <typename T, typename Compare>
RBTree<T, Compare>::~RBTree() {
  clear();
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::empty() const {
  return size_ == 0;
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::size_type RBTree<T, Compare>::size() const {
  return size_;
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::size_type RBTree<T, Compare>::max_size() const {
  return std::numeric_limits<difference_type>::max() / sizeof(Node);
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::iterator RBTree<T, Compare>::begin() const {
  Node* current = root_;
  if (!current) {
    return iterator(nullptr, root_);
  }

  while (current->left) {
    current = current->left;
  }

  return iterator(current);
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::iterator RBTree<T, Compare>::end() const {
  return iterator(nullptr, root_);
}

template <typename T, typename Compare>
void RBTree<T, Compare>::swap(RBTree& other) {
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comparator_, other.comparator_);
}

template <typename T, typename Compare>
void RBTree<T, Compare>::merge(RBTree& other) {
  RBTree<T, Compare> extra;
  for (auto it = other.begin(); it != other.end(); ++it) {
    if (insert(*it).second == false) extra.insert(*it);
  }
  other = std::move(extra);
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::iterator RBTree<T, Compare>::find(
    const T& key) const {
  Node* current = root_;

  while (current) {
    if (comparator_(key, current->data)) {
      current = current->left;
    } else if (comparator_(current->data, key)) {
      current = current->right;
    } else {
      return iterator(current);
    }
  }
  return iterator(nullptr);
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::contains(const T& key) const {
  return find(key) != end();
}

//-------------
template <typename T, typename Compare>
std::pair<typename RBTree<T, Compare>::iterator, bool>
RBTree<T, Compare>::insert(const value_type& value) {
  Node* new_node = createNode(value);

  if (!new_node) {
    throw std::bad_alloc();  // Handle allocation failure.
  }

  Node* parent = nullptr;
  Node* current = root_;
  while (current) {
    parent = current;
    if (comparator_(new_node->data, current->data)) {
      current = current->left;
    } else if (comparator_(current->data, new_node->data)) {
      current = current->right;
    } else {
      return std::make_pair(iterator(current), false);
    }
  }

  attachNodeToTree(new_node, parent);
  fixInsertion(new_node);

  ++size_;

  return std::make_pair(iterator(new_node), true);
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::Node* RBTree<T, Compare>::createNode(
    const value_type& value) {
  Node* new_node = new Node(value);
  if (!new_node) {
    throw std::bad_alloc();  // Handle allocation failure.
  }
  new_node->color = Node::RED;  // New nodes are initially red.
  return new_node;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::attachNodeToTree(Node* new_node, Node* parent) {
  new_node->parent = parent;
  if (!parent) {
    // The tree was empty; new_node becomes the root.
    root_ = new_node;
  } else if (comparator_(new_node->data, parent->data)) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }
}

template <typename T, typename Compare>
void RBTree<T, Compare>::fixInsertion(Node* node) {
  while (node != root_ && node->parent->color == Node::RED) {
    if (node->parent == node->parent->parent->left) {
      Node* uncle = node->parent->parent->right;
      if (uncle && uncle->color == Node::RED) {
        // Case 1: Uncle is red; recolor and move up.
        node->parent->color = Node::BLACK;
        uncle->color = Node::BLACK;
        node->parent->parent->color = Node::RED;
        node = node->parent->parent;
      } else {
        if (node == node->parent->right) {
          // Case 2: Node is the right child; left-rotate and transform to
          // case 3.
          node = node->parent;
          leftRotate(node);
        }
        // Case 3: Node is the left child; recolor and right-rotate the
        // grandparent.
        node->parent->color = Node::BLACK;
        node->parent->parent->color = Node::RED;
        rightRotate(node->parent->parent);
      }
    } else {
      // Same cases as above with "left" and "right" swapped.
      Node* uncle = node->parent->parent->left;
      if (uncle && uncle->color == Node::RED) {
        // Case 1
        node->parent->color = Node::BLACK;
        uncle->color = Node::BLACK;
        node->parent->parent->color = Node::RED;
        node = node->parent->parent;
      } else {
        if (node == node->parent->left) {
          // Case 2
          node = node->parent;
          rightRotate(node);
        }
        // Case 3
        node->parent->color = Node::BLACK;
        node->parent->parent->color = Node::RED;
        leftRotate(node->parent->parent);
      }
    }
  }

  root_->color = Node::BLACK;  // Ensure the root is always black.
}

template <typename T, typename Compare>
void RBTree<T, Compare>::leftRotate(Node* node) {
  Node* right_child = node->right;
  node->right = right_child->left;

  if (right_child->left) {
    right_child->left->parent = node;
  }

  right_child->parent = node->parent;

  if (!node->parent) {
    // Node was the root; update the root.
    root_ = right_child;
  } else if (node == node->parent->left) {
    node->parent->left = right_child;
  } else {
    node->parent->right = right_child;
  }

  right_child->left = node;
  node->parent = right_child;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::rightRotate(Node* node) {
  Node* left_child = node->left;
  node->left = left_child->right;

  if (left_child->right) {
    left_child->right->parent = node;
  }

  left_child->parent = node->parent;

  if (!node->parent) {
    // Node was the root; update the root.
    root_ = left_child;
  } else if (node == node->parent->right) {
    node->parent->right = left_child;
  } else {
    node->parent->left = left_child;
  }

  left_child->right = node;
  node->parent = left_child;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::erase(iterator pos) {
  deleteNode(pos);
}

template <typename T, typename Compare>
void RBTree<T, Compare>::deleteNode(int key) {
  Node* z = root_;
  while (z != nullptr) {
    if (key == z->data) {
      break;
    } else if (key < z->data) {
      z = z->left;
    } else {
      z = z->right;
    }
  }

  if (z == nullptr) {
    // Node with the given key not found
    return;
  }

  Node* y = z;
  typename Node::Color y_original_color = y->color;
  Node* x;

  if (z->left == nullptr) {
    x = z->right;
    transplant(z, z->right);
  } else if (z->right == nullptr) {
    x = z->left;
    transplant(z, z->left);
  } else {
    y = z->right;
    while (y->left != nullptr) {
      y = y->left;
    }
    y_original_color = y->color;
    x = y->right;

    if (y->parent == z) {
      if (x) x->parent = y;
    } else {
      transplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == Node::BLACK) {
    if (x != nullptr) {
      deleteFixup(x, x->parent);
    } else {
      deleteFixup(x, z->parent);
    }
  }

  delete z;
  --size_;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::deleteNode(iterator pos) {
  if (pos == end()) {
    return;
  }
  Node* z = pos.current_;

  Node* y = z;
  typename Node::Color y_original_color = y->color;
  Node* x;

  if (z->left == nullptr) {
    x = z->right;
    transplant(z, z->right);
  } else if (z->right == nullptr) {
    x = z->left;
    transplant(z, z->left);
  } else {
    y = z->right;
    while (y->left != nullptr) {
      y = y->left;
    }
    y_original_color = y->color;
    x = y->right;

    if (y->parent == z) {
      if (x) x->parent = y;
    } else {
      transplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == Node::BLACK) {
    if (x != nullptr) {
      std::cout << "parent ";  // << x->parent->data.first;
      deleteFixup(x, x->parent);

    } else {
      std::cout << "no parent " << z->parent->data;  //.first;
      deleteFixup(x, z->parent);
    }
  }

  delete z;
  --size_;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::transplant(Node* u, Node* v) {
  if (u->parent == nullptr) {
    root_ = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  if (v != nullptr) {
    v->parent = u->parent;
  }
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::isBlack(Node* node) {
  return (!node || node->color == Node::BLACK);
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::isRed(Node* node) {
  return !isBlack(node);
}

template <typename T, typename Compare>
void RBTree<T, Compare>::turnRed(Node* node) {
  node->color = Node::RED;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::turnBlack(Node* node) {
  if (node) node->color = Node::BLACK;  ////// SEGMENTATION FAULT????????
}

template <typename T, typename Compare>
void RBTree<T, Compare>::deleteFixup(Node* x, Node* x_parent) {
  while (x != root_ && (x == nullptr || isBlack(x))) {
    if (x == x_parent->left) {
      Node* w = x_parent->right;  // x's sibling

      if (isRed(w)) {
        std::cout << "case 1\n";
        // Case 1: x's sibling is red
        turnBlack(w);
        turnRed(x_parent);
        leftRotate(x_parent);
        w = x_parent->right;
        std::cout << "case 1\n";
      }

      if (isBlack(w->left) && isBlack(w->right)) {
        // Case 2: x's sibling is black, and both of w's children
        // are black
        std::cout << "case 2\n";
        w->color = Node::RED;
        x = x_parent;
      } else {
        if (isBlack(w->right)) {
          //  Case 3: x's sibling is black, w's left child is red, and w's
          // right  child is black
          std::cout << "case 3\n";
          turnBlack(w->left);
          w->color = Node::RED;
          rightRotate(w);
          w = x_parent->right;
        }

        // Case 4: x's sibling is black, and w's right child is red
        // std::cout << "case 4\n";
        w->color = x_parent->color;
        turnBlack(x_parent);
        turnBlack(w->right);
        leftRotate(x_parent);
        x = root_;  // Terminate the loop
      }

    } else {
      // // Symmetric cases for x being a right child
      // Node* w = x_parent->left;  // x's sibling

      // if (isRed(w)) {
      //   std::cout << "case 1\n";
      //   // Case 1: x's sibling is red
      //   w->color = Node::BLACK;
      //   x_parent->color = Node::RED;
      //   rightRotate(x_parent);
      //   w = x_parent->left;
      //   std::cout << "case 1\n";
      // }

      // if (isBlack(w->right) && isBlack(w->left)) {
      //   // Case 2: x's sibling is black, and both of w's children are black
      //   w->color = Node::RED;
      //   x = x_parent;
      // } else {
      //   if (isBlack(w->left)) {
      //     // Case 3: x's sibling is black, w's right child is red, and w's
      //     left
      //     // child is black
      //     w->right->color = Node::BLACK;
      //     w->color = Node::RED;
      //     leftRotate(w);
      //     w = x_parent->left;
      //   }

      //   // Case 4: x's sibling is black, and w's left child is red
      //   w->color = x_parent->color;
      //   x_parent->color = Node::BLACK;
      //   w->left->color = Node::BLACK;
      //   rightRotate(x_parent);
      x = root_;  // Terminate the loop
      // }
    }
  }

  if (x != nullptr) x->color = Node::BLACK;
}

}  // namespace s21

#endif