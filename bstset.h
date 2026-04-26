#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename T>
class BSTSet {
 private:
  struct BSTNode {
    // These two members should never change after initialization!
    const T value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(T value, BSTNode* parent)
        : value(value), parent(parent), left(nullptr), right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;

  // TODO_STUDENT: add private helper function definitions here
  bool containsHelper(BSTNode* node, const T& value) const {
    if (node == nullptr) {
      return false;
    }
    if (value < node->value) {
      return containsHelper(node->left, value);
    } else if (value > node->value) {
      return containsHelper(node->right, value);
    } else {
      return true;
    }
  }

  void toStringHelper(BSTNode* node, ostringstream& ss, bool& first) const {
    if (node == nullptr) {
      return;
    }
    toStringHelper(node->left, ss, first);
    if (!first) {
      ss << ", ";
    }
    ss << node->value;
    first = false;
    toStringHelper(node->right, ss, first);
  }

  BSTNode* copyHelper(BSTNode* node, BSTNode* parent) {
    if (node == nullptr) {
      return nullptr;
    }

    BSTNode* newNode = new BSTNode(node->value, parent);
    newNode->left = copyHelper(node->left, newNode);
    newNode->right = copyHelper(node->right, newNode);
    return newNode;
  }

  BSTNode* findMinHelper(BSTNode* node) const {
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

  void eraseNodeHelper(BSTNode* node) {
    if (node->left != nullptr && node->right != nullptr) {
      BSTNode* next = findMinHelper(node->right);
      if (next->parent != node) {
        next->parent->left = next->right;
        if (next->right != nullptr) {
          next->right->parent = next->parent;
        }
        next->right = node->right;
        next->right->parent = next;
      }
      next->left = node->left;
      next->left->parent = next;
      next->parent = node->parent;
      if (node->parent == nullptr) {
        root = next;
      } else if (node == node->parent->left) {
        node->parent->left = next;
      } else {
        node->parent->right = next;
      }
      delete node;
    } else {
      BSTNode* child;
      if (node->left != nullptr) {
        child = node->left;
      } else {
        child = node->right;
      }
      if (child != nullptr) {
        child->parent = node->parent;
      }
      if (node->parent == nullptr) {
        root = child;
      } else if (node == node->parent->left) {
        node->parent->left = child;
      } else {
        node->parent->right = child;
      }
      delete node;
    }
  }

 public:
  /**
   * Creates an empty `BSTSet`. Runs in O(1).
   */
  BSTSet() {
    // TODO_STUDENT
    sz = 0;
    root = nullptr;
    curr = nullptr;
  }

  /**
   * Checks if the `BSTSet` is empty. Runs in O(1).
   */
  bool empty() const {
    // TODO_STUDENT
    return sz == 0;
  }

  /**
   * Returns the number of elements in the `BSTSet`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return sz;
  }

  /**
   * Adds the `value` to the `BSTSet`. If the value already
   * exists in the set, does not do anything.
   *
   * Uses the `value` to determine the location in the underlying BST.
   * Creates exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(T value) {
    // TODO_STUDENT
    if (root == nullptr) {
      root = new BSTNode(value, nullptr);
      sz++;
      return;
    }
    BSTNode* curr = root;
    while (true) {
      if (value < curr->value) {
        if (curr->left == nullptr) {
          curr->left = new BSTNode(value, curr);
          sz++;
          return;
        }
        curr = curr->left;
      } else if (value > curr->value) {
        if (curr->right == nullptr) {
          curr->right = new BSTNode(value, curr);
          sz++;
          return;
        }
        curr = curr->right;
      } else {
        return;
      }
    }
  }

  /**
   * Returns `true` if the `value` is present in the set,
   * and `false` otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const T& value) const {
    // TODO_STUDENT
    return containsHelper(root, value);
  }

  /**
   * Empties the `BSTSet`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  void clear() {
    // TODO_STUDENT
    while (root != nullptr) {
      BSTNode* curr = root;
      while (curr->left != nullptr) {
        curr = curr->left;
      }
      if (curr->parent != nullptr) {
        curr->parent->left = curr->right;
      } else {
        root = curr->right;
      }
      if (curr->right != nullptr) {
        curr->right->parent = curr->parent;
      }
      delete curr;
      sz--;
    }
  }

  /**
   * Destructor, cleans up the `BSTSet`.
   *
   * Runs in O(N), where N is the number of elements in the set.
   */
  ~BSTSet() {
    // TODO_STUDENT
    clear();
  }

  /**
   * Converts the `BSTSet` to a string representation, with the values
   * in-order by value.
   *
   * Example:
   *
   * c++
   * BSTSet<string> names;
   * names.insert("Gwen");
   * names.insert("Jen");
   * names.insert("Ben");
   * names.insert("Sven");
   *
   * Calling `names.to_string()` would return the following
   * string: `{Ben, Gwen, Jen, Sven}`
   *
   * Runs in O(N), where N is the number of elements.
   */
  string to_string() const {
    // TODO_STUDENT
    ostringstream ss;
    ss << "{";
    bool first = true;
    toStringHelper(root, ss, first);
    ss << "}";
    return ss.str();
  }

  /**
   * Copy constructor.
   *
   * Copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of elements in `other`.
   */
  BSTSet(const BSTSet& other) {
    // TODO_STUDENT
    sz = other.sz;
    root = copyHelper(other.root, nullptr);
    curr = nullptr;
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the elements from the provided `BSTSet`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of elements in `this`, and O is
   * the number of elements in `other`.
   */
  BSTSet& operator=(const BSTSet& other) {
    // TODO_STUDENT
    if (this == &other) {
      return *this;
    }
    clear();
    root = copyHelper(other.root, nullptr);
    sz = other.sz;
    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest element in the BSTSet, and removes it
   * from the `BSTSet`. If the `BSTSet` is empty, throws a
   * `runtime_error`.
   *
   * Creates no new nodes, and does not update any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  T remove_min() {
    // TODO_STUDENT
    if (root == nullptr) {
      throw runtime_error("BSTSet is empty");
    }

    BSTNode* minNode = findMinHelper(root);
    T minVal = minNode->value;

    if (minNode->parent == nullptr) {
      root = minNode->right;
    } else {
      minNode->parent->left = minNode->right;
    }

    if (minNode->right != nullptr) {
      minNode->right->parent = minNode->parent;
    }
    delete minNode;
    sz--;
    return minVal;
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin() {
    // TODO_STUDENT
    if (root == nullptr) {
      curr = nullptr;
    } else {
      curr = findMinHelper(root);
    }
  }

  /**
   * Uses the internal state to return the next in-order element
   * by reference, and advances the internal state. Returns `true`
   * if the reference was set, and `false` otherwise.
   *
   * Example usage:
   *
   * c++
   * BSTSet<string> bst;
   * bst.begin();
   * string value;
   * while (bst.next(val)) {
   *   cout << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(T& value) {
    // TODO_STUDENT
    if (curr == nullptr) {
      return false;
    }
    value = curr->value;
    if (curr->right != nullptr) {
      curr = findMinHelper(curr->right);
    } else {
      while (curr->parent != nullptr && curr == curr->parent->right) {
        curr = curr->parent;
      }
      curr = curr->parent;
    }
    return true;
  }

  // =========================================================================

  /**
   * Removes the given element from the `BSTSet`, and returns the
   * value.
   *
   * Throws `out_of_range` if the value is not present in the set.
   * Creates no new nodes, and does not update the value of any
   * existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  T erase(const T& value) {
    // TODO_STUDENT
    BSTNode* node = root;
    while (node != nullptr) {
      if (value < node->value) {
        node = node->left;
      } else if (value > node->value) {
        node = node->right;
      } else {
        break;
      }
    }
    if (node == nullptr) {
      throw out_of_range("Value not found");
    }
    T result = node->value;
    eraseNodeHelper(node);
    sz--;
    return result;
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTSet`. For autograder
   * testing purposes only.
   */
  void* getRoot() const {
    return this->root;
  }
};
