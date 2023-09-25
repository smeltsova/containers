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

  bool nodeHasTwoChildren(Node* node) const;
  Node* findSuccessor(Node* node) const;
  void copySuccessorData(Node* source, Node* destination);
  Node* findReplacement(Node* node) const;
  void replaceNodeInParent(Node* node, Node* replacement);
  void handleCaseNoReplacement(Node* node);
  void handleCaseNoReplacementSiblingIsRed(Node* node, Node* sibling);
  void handleCaseWithReplacement(Node* node, Node* replacement);
  void handleCaseWithReplacementBlackNodeAndBlackReplacement(Node* node,
                                                             Node* replacement);
  void handleCaseWithReplacementRedNodeOrRedReplacement(Node* node,
                                                        Node* replacement);
  bool isRed(Node* node) const;
  bool isBlack(Node* node) const;
  bool areChildrenBlack(Node* node) const;

  void deleteNode(Node* node);

  void transplant(Node* u, Node* v);
  void deleteFixup(Node* x);

 public:
  // Constructor, Insertion, and other methods...

  // Function to delete a node with a given key
  void deleteNode(int key);
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
    std::cout << "delete " << node->data << "\n";
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }
}

template <typename T, typename Compare>
void RBTree<T, Compare>::helper(Node* node) {
  if (node) {
    std::cout << "helper " << node->data << "\n";
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
  if (pos == end()) {
    // The iterator is at the end; nothing to erase.
    return;
  }

  Node* node_to_delete = pos.current_;
  Node* replacement = nullptr;

  if (nodeHasTwoChildren(node_to_delete)) {
    std::cout << "has two children\n";
    replacement = findSuccessor(node_to_delete);
    std::cout << replacement->data << "\n";
    copySuccessorData(node_to_delete, replacement);
  } else {
    std::cout << "find replacemnt\n";
    replacement = findReplacement(node_to_delete);
  }
  std::cout << "replacing\n";

  if (!replacement) {
    std::cout << "handle no replacement\n";
    handleCaseNoReplacement(node_to_delete);
  } else {
    std::cout << "handle with replacement\n";
    handleCaseWithReplacement(node_to_delete, replacement);
  }
  helper(root_);
  replaceNodeInParent(node_to_delete, replacement);
  std::cout << "\n";
  helper(root_);
  deleteNode(node_to_delete);
  size_--;
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::nodeHasTwoChildren(Node* node) const {
  return node->left && node->right;
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::Node* RBTree<T, Compare>::findSuccessor(
    Node* node) const {
  Node* successor = node->right;
  while (successor->left) {
    successor = successor->left;
  }
  return successor;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::copySuccessorData(Node* source, Node* target) {
  source->data = target->data;
}

template <typename T, typename Compare>
typename RBTree<T, Compare>::Node* RBTree<T, Compare>::findReplacement(
    Node* node) const {
  return (node->left) ? node->left : node->right;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::replaceNodeInParent(Node* node, Node* replacement) {
  if (replacement) {
    replacement->parent = node->parent;
  }
  if (!node->parent) {
    // Node was the root; update the root.
    root_ = replacement;
  } else if (node == node->parent->left) {
    node->parent->left = replacement;
  } else {
    node->parent->right = replacement;
  }
}

template <typename T, typename Compare>
void RBTree<T, Compare>::handleCaseNoReplacement(Node* node) {
  std::cout << "inside no replacement\n";
  if (node->color == Node::RED) {
    std::cout << "case 1\n";
    // Case 1: Node is red; no violation of RBT properties.
    // deleteNode(node);
    return;
  }
  std::cout << "not case 1\n";
  while (node != root_ && node->color == Node::BLACK) {
    Node* sibling =
        (node == node->parent->left) ? node->parent->right : node->parent->left;
    std::cout << "found sibling\n" << node->parent->left->data << " data\n";
    if (isRed(sibling)) {
      std::cout << "case 2.1\n";
      handleCaseNoReplacementSiblingIsRed(node, sibling);
    } else if (std::cout << "1\n" && isBlack(sibling) && std::cout << "2\n" &&
               areChildrenBlack(sibling) && std::cout << "3\n") {
      // Case 2.2: Sibling is black with black children; recolor and move up.
      std::cout << "case 2.2\n";
      sibling->color = Node::RED;
      node = node->parent;
    } else {
      std::cout << "case 2.2 not\n";
      if (node == node->parent->left && isBlack(sibling->right)) {
        std::cout << "case 2.3\n";
        // Case 2.3: Sibling is black, left child is red, right child is black;
        // rotate and recolor.
        sibling->left->color = Node::BLACK;
        sibling->color = Node::RED;
        rightRotate(sibling);
        sibling = node->parent->right;
      } else if (node == node->parent->right && isBlack(sibling->left)) {
        std::cout << "case 2.2(sym)\n";
        // Case 2.3 (symmetric): Sibling is black, right child is red, left
        // child is black; rotate and recolor.
        sibling->right->color = Node::BLACK;
        sibling->color = Node::RED;
        leftRotate(sibling);
        sibling = node->parent->left;
      }

      if (isBlack(sibling->right)) {
        std::cout << "case 2.4\n";
        // Case 2.4: Sibling is black, right child is black; rotate and recolor.
        sibling->color = node->parent->color;
        node->parent->color = Node::BLACK;
        sibling->left->color = Node::BLACK;
        rightRotate(node->parent);
      } else {
        std::cout << "case 2.4(sym)\n";
        // Case 2.4 (symmetric): Sibling is black, right child is rught; rotate
        // and recolor.
        sibling->color = node->parent->color;
        node->parent->color = Node::BLACK;
        sibling->right->color = Node::BLACK;
        leftRotate(node->parent);
      }

      node = root_;  // Terminate the loop.
    }
  }

  node->color = Node::BLACK;  // Ensure the root is always black.
  // deleteNode(node);
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::isRed(Node* node) const {
  return (node && node->color == Node::RED);
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::isBlack(Node* node) const {
  return !isRed(node);
}

template <typename T, typename Compare>
bool RBTree<T, Compare>::areChildrenBlack(Node* node) const {
  std::cout << "are children black\n" << !node;
  //<< !node->left << !node->right;
  bool ans = false;  //((!node->left || isBlack(node->left)) &&
                     //(!node->right || isBlack(node->right)));
  (node == nullptr);
  std::cout << "are children black\n";
  return ans;
}

template <typename T, typename Compare>
void RBTree<T, Compare>::handleCaseNoReplacementSiblingIsRed(Node* node,
                                                             Node* sibling) {
  // Case 2.1: Sibling is red; rotate and recolor to create a black sibling.
  sibling->color = Node::BLACK;
  node->parent->color = Node::RED;
  if (node == node->parent->left) {
    leftRotate(node->parent);
    sibling = node->parent->right;
  } else {
    rightRotate(node->parent);
    sibling = node->parent->left;
  }
}

template <typename T, typename Compare>
void RBTree<T, Compare>::handleCaseWithReplacement(Node* node,
                                                   Node* replacement) {
  if (node->color == Node::RED ||
      (replacement && replacement->color == Node::RED)) {
    // Case 1: Node or its replacement is red; replace and recolor.
    std::cout << "red\n";
    // handleCaseWithReplacementRedNodeOrRedReplacement(node, replacement);
  } else {
    // Case 2: Both node and replacement are black; fix RBT properties.
    std::cout << "black\n";
    handleCaseWithReplacementBlackNodeAndBlackReplacement(node, replacement);
  }
}

template <typename T, typename Compare>
void RBTree<T, Compare>::handleCaseWithReplacementRedNodeOrRedReplacement(
    Node* node, Node* replacement) {
  if (!node->parent) return;

  if (node == node->parent->left) {
    node->parent->left = replacement;
  } else {
    node->parent->right = replacement;
  }
  if (replacement) {
    replacement->parent = node->parent;
  }
  // deleteNode(node);
}

template <typename T, typename Compare>
void RBTree<T, Compare>::handleCaseWithReplacementBlackNodeAndBlackReplacement(
    Node* node, Node* replacement) {
  while (node != root_ &&
         (replacement == nullptr || replacement->color == Node::BLACK)) {
    Node* sibling =
        (node == node->parent->left) ? node->parent->right : node->parent->left;

    if (sibling->color == Node::RED) {
      // Case 2.1: Sibling is red; rotate and recolor to create a black sibling.
      sibling->color = Node::BLACK;
      node->parent->color = Node::RED;
      if (node == node->parent->left) {
        leftRotate(node->parent);
        sibling = node->parent->right;
      } else {
        rightRotate(node->parent);
        sibling = node->parent->left;
      }
    }

    if ((!sibling->left || sibling->left->color == Node::BLACK) &&
        (!sibling->right || sibling->right->color == Node::BLACK)) {
      // Case 2.2: Sibling is black with black children; recolor and move up.
      sibling->color = Node::RED;
      node = node->parent;
    } else {
      if (node == node->parent->left &&
          (!sibling->right || sibling->right->color == Node::BLACK)) {
        // Case 2.3: Sibling is black, left child is red, right child is black;
        // rotate and recolor.
        sibling->left->color = Node::BLACK;
        sibling->color = Node::RED;
        rightRotate(sibling);
        sibling = node->parent->right;
      } else if (node == node->parent->right &&
                 (!sibling->left || sibling->left->color == Node::BLACK)) {
        // Case 2.3 (symmetric): Sibling is black, right child is red, left
        // child is black; rotate and recolor.
        sibling->right->color = Node::BLACK;
        sibling->color = Node::RED;
        leftRotate(sibling);
        sibling = node->parent->left;
      }

      if (node == node->parent->left && isBlack(sibling->right)) {
        // Case 2.4: Sibling is black, right child is black; rotate and recolor.
        sibling->color = node->parent->color;
        node->parent->color = Node::BLACK;
        sibling->right->color = Node::BLACK;
        rightRotate(node->parent);
      } else if (node == node->parent->right && isBlack(sibling->left)) {
        // Case 2.4 (symmetric): Sibling is black, left child is black; rotate
        // and recolor.
        sibling->color = node->parent->color;
        node->parent->color = Node::BLACK;
        sibling->left->color = Node::BLACK;
        leftRotate(node->parent);
      }

      node = root_;  // Terminate the loop.
    }
  }

  if (replacement) {
    replacement->color = Node::BLACK;  // Ensure the replacement is black.
  }
  // deleteNode(node);
}

template <typename T, typename Compare>
void RBTree<T, Compare>::deleteNode(Node* node) {
  if (node) delete node;
}

// Implement the deleteNode function
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
    std::cout << 1 << "\n";
    x = z->right;
    transplant(z, z->right);
  } else if (z->right == nullptr) {
    std::cout << 2 << "\n";

    x = z->left;
    transplant(z, z->left);
  } else {
    std::cout << 3 << "\n";

    y = z->right;
    while (y->left != nullptr) {
      y = y->left;
    }
    y_original_color = y->color;
    x = y->right;

    if (y->parent == z) {
      x->parent = y;
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
    std::cout << "fix"
              << "\n";

    deleteFixup(x);
  }

  delete z;
}

// Implement the transplant function (used in deleteNode)
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

// Implement the deleteFixup function (used in deleteNode)
template <typename T, typename Compare>
void RBTree<T, Compare>::deleteFixup(Node* x) {
  // Implement the various cases (1 through 5) to restore Red-Black Tree
  // properties This function is responsible for fixing the tree after a node
  // deletion
}

}  // namespace s21

#endif