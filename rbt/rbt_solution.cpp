#include <iostream>
#include <vector>
#include <cstdint>
#include <limits>

using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;

const i32 INF = 1'000'000'000;

enum class Color {
  RED, 
  BLACK
};

struct Node {
  explicit Node(const i32 value, const Color color = Color::RED) :  
        parent(nullptr), left(nullptr), right(nullptr), value(value), color(color) {}

  Node* parent;
  Node* left;
  Node *right;
  i32 value;
  Color color;
};

class Set {
public:
  Set() : root(nullptr) {}
  void insert(const i32);
  void erase(const i32);
  const bool contains(const i32) const;
  const i64 range_sum(const i32, const i32) const;
  const i32 lower_bound(const i32) const;
  const i32 upper_bound(const i32) const;
private:
  void _rotate_left(Node*&);
  void _rotate_right(Node*&);
  void _fix_up(Node*&);
  const i32 _lower_bound(const Node*, const i32) const;
  const i32 _upper_bound(const Node*, const i32) const;
  const i64 _range_sum(const Node*, const i32, const i32) const;
  void _erase(Node*);
  Node* _find(const i32) const;
  Node* _find_node_to_replace(const Node*) const;
  void _fix_double_black(Node*);

  Node* root;
};

void Set::insert(const i32 value) {
  if (root == nullptr) {
    root = new Node(value, Color::BLACK);
    return;
  }

  auto root = this->root;
  while (true) {
    if (root -> value == value) {
      return;
    }
    if (root->value > value) {
      if (root->left == nullptr) {
        break;
      }
      root = root->left; 
    } else {
      if (root->right == nullptr) {
        break;
      }
      root = root->right;
    }
  }
  if (root->value > value) {
    root->left = new Node(value);
    root->left->parent = root;
    this->_fix_up(root->left);
  } else {
    root->right = new Node(value);
    root->right->parent = root;
    this->_fix_up(root->right);
  }
}

const bool Set::contains(const i32 target) const {
  return this->_find(target) != nullptr;
}

void Set::erase(const i32 value) {
  auto node = this->_find(value);
  if (node == nullptr) {
    return;
  }
  this->_erase(node);
}

const i64 Set::range_sum(const i32 low, const i32 high) const {
  return this->_range_sum(this->root, low, high);
}

const i32 Set::lower_bound(const i32 target) const {
  return _lower_bound(root, target);
}

const i32 Set::upper_bound(const i32 target) const {
  return _upper_bound(root, target);
}

void Set::_erase(Node* node) {
  auto to_replace = this->_find_node_to_replace(node);
  if (node == this->root && to_replace == nullptr) {
    delete node;
    this->root = nullptr;
    return;
  }
  const bool double_black = node->color == Color::BLACK && 
                            (to_replace == nullptr || to_replace->color == Color::BLACK);

  if (node->left == nullptr && node->right == nullptr) {
    auto sibling = (node->parent->left == node ? node->parent->right : node->parent->left);
    if (double_black) {
      this->_fix_double_black(node);
    } else {
      if (sibling != nullptr) {
        sibling->color = Color::RED;
      }
    }
    if (node == node->parent->left) {
      node->parent->left = nullptr;
    } else {
      node->parent->right = nullptr;
    }
    delete node;
    return;
  }

  if (node->left == nullptr || node->right == nullptr) {
    if (node == this->root) {
      // we only have two nodes in the tree 
      this->root->value = to_replace->value;
      this->root->color = Color::BLACK;
      this->root->left = this->root->right = nullptr;
      delete to_replace;
      return;
    }
    Node* parent = node->parent;
    if (node == node->parent->left) {
      parent->left = to_replace;
    } else {
      parent->right = to_replace;
    }
    to_replace->parent = parent;
    delete node;
    if (double_black) {
      this->_fix_double_black(to_replace);
    } else {
      to_replace->color = Color::BLACK;
    }
    return;
  }

  // to_replace is node's succesor. Should replace node's info are delete succesor
  std::swap(node->value, to_replace->value);
  this->_erase(to_replace); 
}

void Set::_fix_double_black(Node* node) {
  if (node == root) {
    return;
  }

  Node* sibling = (node->parent->left == node ? node->parent->right : node->parent->left);
  if (sibling == nullptr) {
    this->_fix_double_black(node->parent);
    return;
  }

  if (sibling->color == Color::RED) {
    sibling->color = Color::BLACK;
    node->parent->color = Color::RED;
    if (node->parent->left == node) {
      this->_rotate_left(node->parent);
    } else {
      this->_rotate_right(node->parent);
    }
    this->_fix_double_black(node);
    return;
  }

  auto parent = node->parent;
  if ((sibling->left != nullptr && sibling->left->color == Color::RED) || 
      (sibling->right != nullptr && sibling->right->color == Color::RED)) {
    if (sibling->left != nullptr && sibling->left->color == Color::RED) {
      if (sibling == parent->left) {
        sibling->left->color = sibling->color;
        sibling->color = parent->color;
        this->_rotate_right(parent);
      } else {
        sibling->left->color = parent->color;
        this->_rotate_right(sibling);
        this->_rotate_left(parent);
      }
    } else {
      if (sibling == parent->left) {
        sibling->right->color = parent->color;
        this->_rotate_left(sibling);
        this->_rotate_right(parent);
      } else {
        sibling->right->color = sibling->color;
        sibling->color = parent->color;
        this->_rotate_left(parent);
      }
    }
    parent->color = Color::BLACK;
  } else {
    sibling->color = Color::RED;
    if (parent->color == Color::BLACK) {
      this->_fix_double_black(parent);
    } else {
      parent->color = Color::BLACK;
    } 
  }
}

Node* Set::_find(const i32 target) const {
  auto root = this->root;
  while (root != nullptr) {
    if (root->value == target) {
      return root;
    }
    if (root->value < target) {
      root = root->right;
    } else {
      root = root->left;
    }
  }
  return nullptr; 
}

void Set::_rotate_right(Node*& node) {
  auto new_root = node->left;
  node->left = new_root->right;
  if (new_root->right != nullptr) {
    new_root->right->parent = node;
  }
  new_root->parent = node->parent;
  if (node->parent == nullptr) {
    this->root = new_root;
  } else if (node == node->parent->right) {
    node->parent->right = new_root;
  } else {
    node->parent->left = new_root;
  }
  new_root->right = node;
  node->parent = new_root;
}

void Set::_rotate_left(Node*& node) {
  auto new_root = node->right;
  node->right = new_root->left;
  if (new_root->left != nullptr) {
    new_root->left->parent = node;
  }
  new_root->parent = node->parent;
  if (node->parent == nullptr) {
    this->root = new_root;
  } else if (node == node->parent->left) {
    node->parent->left = new_root;
  } else {
    node->parent->right = new_root;
  }
  new_root->left = node;
  node->parent = new_root;
}

void Set::_fix_up(Node*& node) {
  this->root->color = Color::BLACK;
  if (node == this->root || node->color != Color::RED || node->parent->color != Color::RED) {
    return;
  }
  Node* grandparent = node->parent->parent; 
  Node* parent = node->parent;
  Node* uncle = (grandparent->left == parent) ? grandparent->right : grandparent->left;
  if (uncle != nullptr && uncle->color == Color::RED) {
    grandparent->left->color = Color::BLACK;
    grandparent->right->color = Color::BLACK;
    grandparent->color = Color::RED;
    this->_fix_up(grandparent);
    return;
  }

  if (parent == grandparent->left) {
    if (node == parent->right) {
      this->_rotate_left(parent);
      parent = parent->parent;
    }
    this->_rotate_right(grandparent);
  } else {
    if (node == parent->left) {
      this->_rotate_right(parent);
      parent = parent->parent;
    }
    this->_rotate_left(grandparent);
  }
  std::swap(parent->color, grandparent->color);
  this->_fix_up(parent);
}

const i32 Set::_lower_bound(const Node* node, const i32 target) const {
  if (node == nullptr) {
    return -INF - 1;
  }

  if (node->value == target) {
    return node->value;
  }
  if (node->value < target) {
    return std::max(node->value, this->_lower_bound(node->right, target));
  } else {
    return this->_lower_bound(node->left, target);
  }
}

const i32 Set::_upper_bound(const Node* node, const i32 target) const {
  if (node == nullptr) {
    return INF + 1;
  }

  if (node->value == target) {
    return node->value;
  }
  if (node->value < target) {
    return this->_upper_bound(node->right, target);
  } else {
    return std::min(node->value, this->_upper_bound(node->left, target));
  }
}

const i64 Set::_range_sum(const Node* root, const i32 low, const i32 high) const {
  if (root == nullptr) {
    return 0;
  }
  if (root->value < low) {
    return this->_range_sum(root->right, low, high);
  }
  if (root->value > high) {
    return this->_range_sum(root->left, low, high);
  }
  return this->_range_sum(root->left, low, high) + root->value + this->_range_sum(root->right, low, high);
}

Node* Set::_find_node_to_replace(const Node* node) const {
  if (node->left != nullptr && node->right != nullptr) {
    auto succesor = node->right;
    while (succesor->left != nullptr) {
      succesor = succesor->left;
    }
    return succesor;
  }

  if (node->left != nullptr) {
    return node->left;
  }
  if (node->right != nullptr) {
    return node->right;
  }
  return nullptr;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  i32 Q;
  std::cin >> Q;
  Set S;
  for (i32 i = 0; i < Q; i++) {
    i32 op;
    i32 x;
    std::cin >> op >> x;

    switch (op) {
    case 1:
      S.insert(x);
      break;
    case 2:
      S.erase(x);
      break;
    case 3:
      std::cout << S.contains(x) << "\n";
      break;
    case 4:
      std::cout << S.lower_bound(x) << "\n";
      break;
    case 5:
      std::cout << S.upper_bound(x) << "\n";
      break;
    case 6:
      i32 y;
      std::cin >> y;
      std::cout << S.range_sum(x, y) << "\n";
      break;
    default:
      break;
    }
  }
  return 0;
}
