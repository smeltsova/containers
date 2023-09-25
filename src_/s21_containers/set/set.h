

#include "../tree/tree.h"
namespace s21 {
template <typename Key>
class set : public RBTree<Key> {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename RBTree<Key>::iterator;  // const?
  using const_iterator = typename RBTree<Key>::const_iterator;
  using size_type = size_t;

  using RBTree<Key>::RBTree;
};
}  // namespace s21
