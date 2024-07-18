#include <iostream>
#include <memory>

class father
{
public:
    int a;

    static void set_a(father &obj, int val);

private:
};

class child : public father
{
public:
    child() : father()
    {}
};

void father::set_a(father &obj, int val)
{
    obj.a = val;
}

int main()
{
    std::shared_ptr<child> obj1 = std::make_shared<child>();
    father::set_a(*obj1, 10);
    std::cout << obj1->a << std::endl; // To verify the value was set correctly
    return 0;
}
