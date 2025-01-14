#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;

constexpr int NEG_INF = -1000000000;
constexpr int POS_INF = 1000000000;

struct WeakAVLNode {
    int value;
    int height;
    shared_ptr<WeakAVLNode> left;
    shared_ptr<WeakAVLNode> right;

    WeakAVLNode(int val) : value(val), height(1), left(nullptr), right(nullptr) {}
};

class WeakAVLTree {
private:
    shared_ptr<WeakAVLNode> root;

    int height(shared_ptr<WeakAVLNode> node) {
        return node ? node->height : 0;
    }

    int balanceFactor(shared_ptr<WeakAVLNode> node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    shared_ptr<WeakAVLNode> rotateRight(shared_ptr<WeakAVLNode> y) {
        auto x = y->left;
        auto T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    shared_ptr<WeakAVLNode> rotateLeft(shared_ptr<WeakAVLNode> x) {
        auto y = x->right;
        auto T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    shared_ptr<WeakAVLNode> balance(shared_ptr<WeakAVLNode> node) {
        if (!node) return node;

        node->height = max(height(node->left), height(node->right)) + 1;
        int balance = balanceFactor(node);

        if (balance > 2) {
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -2) {
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    shared_ptr<WeakAVLNode> insert(shared_ptr<WeakAVLNode> node, int value) {
        if (!node) return make_shared<WeakAVLNode>(value);

        if (value < node->value)
            node->left = insert(node->left, value);
        else if (value > node->value)
            node->right = insert(node->right, value);

        return balance(node);
    }

    shared_ptr<WeakAVLNode> findMin(shared_ptr<WeakAVLNode> node) {
        while (node->left) node = node->left;
        return node;
    }

    shared_ptr<WeakAVLNode> erase(shared_ptr<WeakAVLNode> node, int value) {
        if (!node) return node;

        if (value < node->value) {
            node->left = erase(node->left, value);
        } else if (value > node->value) {
            node->right = erase(node->right, value);
        } else {
            if (!node->left || !node->right) {
                return node->left ? node->left : node->right;
            }

            auto temp = findMin(node->right);
            node->value = temp->value;
            node->right = erase(node->right, temp->value);
        }

        return balance(node);
    }

    bool contains(shared_ptr<WeakAVLNode> node, int value) {
        if (!node) return false;
        if (value == node->value) return true;

        return value < node->value ? contains(node->left, value) : contains(node->right, value);
    }

    int predecesor(shared_ptr<WeakAVLNode> node, int value) {
        if (!node) return NEG_INF;

        if (node->value >= value)
            return predecesor(node->left, value);

        int res = predecesor(node->right, value);
        return max(node->value, res);
    }

    int succesor(shared_ptr<WeakAVLNode> node, int value) {
        if (!node) return POS_INF;

        if (node->value <= value)
            return succesor(node->right, value);

        int res = succesor(node->left, value);
        return min(node->value, res);
    }

    void query(shared_ptr<WeakAVLNode> node, int X, int Y, vector<int>& result) {
        if (!node) return;

        if (X <= node->value) query(node->left, X, Y, result);
        if (X <= node->value && node->value <= Y) result.push_back(node->value);
        if (Y >= node->value) query(node->right, X, Y, result);
    }

public:
    WeakAVLTree() : root(nullptr) {}

    void insert(int value) {
        root = insert(root, value);
    }

    void erase(int value) {
        root = erase(root, value);
    }

    bool contains(int value) {
        return contains(root, value);
    }

    int predecesor(int value) {
        return predecesor(root, value);
    }

    int succesor(int value) {
        return succesor(root, value);
    }

    vector<int> query(int X, int Y) {
        vector<int> result;
        query(root, X, Y, result);
        return result;
    }
};

int main() {
    ifstream f("abce.in");
    ofstream g("abce.out");

    int Q;
    f >> Q;

    WeakAVLTree avl;
    while (Q--) {
        int op, X, Y;
        f >> op >> X;
        switch (op) {
            case 1:
                avl.insert(X);
                break;
            case 2:
                avl.erase(X);
                break;
            case 3:
                g << avl.contains(X) << "\n";
                break;
            case 4:
                g << avl.predecesor(X) << "\n";
                break;
            case 5:
                g << avl.succesor(X) << "\n";
                break;
            case 6:
                f >> Y;
                vector<int> result = avl.query(X, Y);
                for (int num : result) g << num << " ";
                g << "\n";
                break;
        }
    }

    return 0;
}
