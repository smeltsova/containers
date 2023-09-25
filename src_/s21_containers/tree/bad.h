
// without balancing
template <typename T, typename Compare>
void RBTree<T, Compare>::erase(iterator pos) {
  Node* node_to_delete = pos.current_;

  if (!node_to_delete) return;

  Node* parent = node_to_delete->parent;

  if (!node_to_delete->left && !node_to_delete->right) {
    if (!parent) {
      delete root_;
      root_ = nullptr;
    } else {
      if (parent->left == node_to_delete)
        parent->left = nullptr;
      else
        parent->right = nullptr;

      delete node_to_delete;
    }
  } else if (!node_to_delete->left || !node_to_delete->right) {
    Node* child =
        (node_to_delete->left) ? node_to_delete->left : node_to_delete->right;

    if (!parent) {
      delete root_;
      root_ = child;
      root_->parent = nullptr;
    } else {
      if (parent->left == node_to_delete)
        parent->left = child;
      else
        parent->right = child;

      child->parent = parent;
      delete node_to_delete;
    }
  }

  else {
    Node* successor = node_to_delete->right;
    while (successor->left) successor = successor->left;

    node_to_delete->data = successor->data;
    erase(iterator(successor));
    /*
    if (!parent) {
        root_ = successor;
    } else {
        if (parent->left == node_to_delete)
            parent->left = successor;
        else
            parent->right = successor;
    }

    node_to_delete->left->parent = successor;
    node_to_delete->right->parent = successor;

    if (successor->right) successor->right->parent = node_to_delete;
            std::swap(node_to_delete->parent, successor->parent);
    std::swap(node_to_delete->left, successor->left);
    std::swap(node_to_delete->right, successor->right);


    erase(iterator(node_to_delete));
    */
  }

  --size_;
}

// without balancing
template <typename T, typename Compare>
std::pair<typename RBTree<T, Compare>::iterator, bool>
RBTree<T, Compare>::insert(const value_type& value) {
  Node* current = root_;
  Node* parent = nullptr;

  while (current) {
    parent = current;

    if (comparator_(value, current->data)) {
      current = current->left;
    } else if (comparator_(current->data, value)) {
      current = current->right;
    } else {
      // not multi
      return std::make_pair(iterator(current), false);
    }
  }

  Node* new_node = new Node(value);
  new_node->parent = parent;

  if (!parent) {
    root_ = new_node;
  } else if (comparator_(value, parent->data)) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }
  // balancing...
  ++size_;
  return std::make_pair(iterator(new_node), true);
}