#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
class Shape
{
public:
    int a=0;
    Shape() { std::cout << "Shape" << std::endl; }
    ~Shape() { std::cout << "~Shape" << std::endl; }

    // Shape(Shape &&other) { std::cout << "Shape&& other" << std::endl; }
    Shape(const Shape &other) { std::cout << "const Shape& other" << std::endl; }
    // Shape(Shape &other) { std::cout << "Shape& other" << std::endl; }
    Shape& operator=(const Shape& other)
    {
        std::cout << "operator=" << std::endl;
        (*this).a = other.a;
        return *this;
    }

};

////////////验证 返回值发生拷贝
__attribute__((optimize("O0")))
Shape callShape1()
{
    Shape shape;
    shape.a=1;
    printf("shape=%p\n",&shape);
    return shape;
}
__attribute__((optimize("O0")))
Shape callShape2()
{
    Shape shape;
      return shape;
    // return std::move(shape);
}
__attribute__((optimize("O0")))
Shape callShape3(int i)
{
    Shape shape1;
    Shape shape2;
    if (i == 1)
    {
        return shape1;
    }
    else
    {
        return shape2;
    }
}


int main()
{
    Shape a;
    cout << "*** 1 ***" << endl;
    a = callShape1();
    printf("shape=%p\n",&a);
    cout << "*** 2 ***" << endl;
    // Shape b = callShape2();
    cout << "*** 3 ***" << endl;
    Shape c = callShape3(1);
    cout << "*** 4 ***" << endl;
    return 0;
}