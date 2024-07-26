#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

template <typename Key, typename Value, typename Compare = std::less<Key>>
class Bst
{
private:
    Compare compare = Compare();
    struct Node {
        Key k;
        Value v;
        std::shared_ptr<Node> l;
        std::shared_ptr<Node> r;

        explicit Node(Key k) : k(std::move(k)), l(nullptr), r(nullptr)
        {}

        explicit Node(Key k, Value v) : k(std::move(k)), v(std::move(v)), l(nullptr), r(nullptr)
        {
            // std::cout << this->k << ", \"" << this->v << "\"" << std::endl;
        }

        ~Node() = default;
        static std::shared_ptr<Node> from(const Key k, const Value v)
        {
            return std::make_shared<Node>(Node(k, v));
        }
    };

public:
    Bst() = default;
    ~Bst() = default;
    void insert(const Key &k, const Value &v)
    {
        rootNode = _insert(rootNode, k, v);
    }
    void inorder_iter(void)
    {
        _inorder_iter(rootNode);
    }
    // void delete(const Key &k)
    // {}

private:
    std::shared_ptr<Node> rootNode = nullptr;

    std::shared_ptr<Node> _insert(std::shared_ptr<Node> &root, const Key &k, const Value &v)
    {
        if (!root) {
            return std::make_shared<Node>(Node(k, v));
        }

        if (compare(k, root->k)) {
            root->l = _insert(root->l, k, v);
        } else if (compare(root->k, k)) {
            root->r = _insert(root->r, k, v);
        } else {
            return root;
        }

        return root;
    }

    void _inorder_iter(std::shared_ptr<Node> &root)
    {
        if (!root)
            return;
        _inorder_iter(root->l);
        std::cout << root->k << ", \"" << root->v << "\"" << std::endl;
        _inorder_iter(root->r);
    }
};

int main(int argc, char *argv[])
{
    int table[] = {9, 12, 4, 7, 3, 1, 13, 18, 9};
    Bst<int, std::string> bstMap;

    for (const auto &i : table) {
        bstMap.insert(i, std::to_string(i));
    }

    bstMap.inorder_iter();

    return 0;
}
