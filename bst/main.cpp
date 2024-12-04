#include <bits/stdc++.h>
using namespace std;

const int NEG_INF = -1e9 - 1;
const int INF = 1e9 + 1;

struct BSTNode
{
    int value;
    BSTNode *left, *right;

    BSTNode(int _value) : value(_value), left(nullptr), right(nullptr) {}
};

class BST
{
private:
    BSTNode *root;

    BSTNode *getMinimum(BSTNode *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    BSTNode *insert(BSTNode *node, int x)
    {
        if (node == nullptr)
            return new BSTNode(x);

        if (x < node->value)
            node->left = insert(node->left, x);
        else
            node->right = insert(node->right, x);

        return node;
    }

    BSTNode *erase(BSTNode *node, int x)
    {
        if (node == nullptr)
            return nullptr;

        if (x < node->value)
        {
            node->left = erase(node->left, x);
            return node;
        }

        if (x > node->value)
        {
            node->right = erase(node->right, x);
            return node;
        }

        // node has at most one child
        if (node->left == nullptr || node->right == nullptr)
        {
            BSTNode *aux = nullptr;
            if (node->left == nullptr)
                aux = node->right;
            else
                aux = node->left;

            delete node;
            return aux;
        }

        // node has two children
        BSTNode *exchange = getMinimum(node->right); // this node has no left child

        node->value = exchange->value;
        node->right = erase(node->right, exchange->value);
        return node;
    }

    bool search(BSTNode *node, int x)
    {
        if (node == nullptr)
            return false;

        if (x == node->value)
            return true;

        if (x < node->value)
            return search(node->left, x);

        return search(node->right, x);
    }

    int predecessor(BSTNode *node, int x)
    {
        if (node == nullptr)
            return NEG_INF;

        if (x < node->value)
            return predecessor(node->left, x);

        return max(node->value, predecessor(node->right, x));
    }

    int successor(BSTNode *node, int x)
    {
        if (node == nullptr)
            return INF;

        if (x > node->value)
            return successor(node->right, x);

        return min(node->value, successor(node->left, x));
    }

    void inorder_walk(BSTNode *node, int x, int y, vector<int> &ans)
    {
        if (node == nullptr)
            return;

        if (x < node->value)
            inorder_walk(node->left, x, y, ans);

        if (x <= node->value && node->value <= y)
            ans.push_back(node->value);

        if (y > node->value)
            inorder_walk(node->right, x, y, ans);
    }

    void deleteTree(BSTNode *node)
    {
        if (node == nullptr)
            return;

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}

    ~BST()
    {
        deleteTree(this->root);
    }

    void insert(int x)
    {
        if (!search(this->root, x))
            root = insert(this->root, x);
    }
    void erase(int x)
    {
        if (search(this->root, x))
            root = erase(this->root, x);
    }
    bool search(int x)
    {
        return search(this->root, x);
    }
    int predecessor(int x)
    {
        return predecessor(this->root, x);
    }
    int successor(int x)
    {
        return successor(this->root, x);
    }
    void inorder_walk(int x, int y, vector<int> &ans)
    {
        inorder_walk(this->root, x, y, ans);
        if (!ans.size())
        {
            ans.push_back(NEG_INF);
            ans.push_back(INF);
        }
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    BST bst;
    int Q;
    cin >> Q;

    for (int q = 0; q < Q; q++)
    {
        int op, x, y;
        cin >> op;

        if (op == 1)
        {
            cin >> x;
            bst.insert(x);
        }
        else if (op == 2)
        {
            cin >> x;
            bst.erase(x);
        }
        else if (op == 3)
        {
            cin >> x;
            cout << bst.search(x) << '\n';
        }
        else if (op == 4)
        {
            cin >> x;
            cout << bst.predecessor(x) << '\n';
        }
        else if (op == 5)
        {
            cin >> x;
            cout << bst.successor(x) << '\n';
        }
        else
        {
            cin >> x >> y;
            vector<int> inorder_walk;
            bst.inorder_walk(x, y, inorder_walk);
            for (int value : inorder_walk)
                cout << value << ' ';
            cout << '\n';
        }
    }
    return 0;
}