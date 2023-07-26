#include <iostream>
#include <set>

template <typename Key, typename T>
class Tree {
 private:
  class Node {
   public:
    Key key;
    Node* left;
    Node* right;
    Node* parent;

    Node(Key key) : key(key), left(nullptr), right(nullptr), parent(nullptr){};
    Node(Key key, Node* parent)
        : key(key), left(nullptr), right(nullptr), parent(parent){};
  };

    template <typename NodePtr, typename U>
  class BinaryTreeIterator {
   private:
    NodePtr node_;
    NodePtr root_;

   public:
    BinaryTreeIterator() : node_(nullptr), root_(nullptr){};
    BinaryTreeIterator(NodePtr node) : node_(node), root_(nullptr){};
    BinaryTreeIterator(NodePtr node, NodePtr root) : node_(node), root_(root){};

    BinaryTreeIterator& operator++();
    BinaryTreeIterator operator++(int);
    BinaryTreeIterator& operator--();
    BinaryTreeIterator operator--(int);
    U& operator*() const;
    U* operator->() const;
    bool operator==(const BinaryTreeIterator& other);
    bool operator!=(const BinaryTreeIterator& other);

   private:
    NodePtr MaximumKey(NodePtr root);
  };

  Node* root;

 public:
  using iterator = BinaryTreeIterator<Node*, T>;
  using const_iterator = BinaryTreeIterator<const Node*, const T>;
  Tree() : root(nullptr){};

  iterator find(const Key& key);
  void insert(Key key);
  iterator begin();
  iterator end();

 private:
  Node* insert(Node* node, Node* parent, Key key);
};