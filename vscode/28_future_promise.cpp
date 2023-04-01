#include <iostream>
#include <thread>
#include <future>
#include <chrono>
//应用场景一：
//show函数用于计算平方差，因为std::thread没有提供获取返回值的函数，所以show函数的返回值是void还是int都没区别
//为了拿到返回值，只能在参数列表动手脚了；新增了一个参数p用于存放返回值，注意它是引用
void show(int a, int b, std::promise<int> &p)
{
    //这里可以故意延迟几秒钟，模拟复杂计算
    //std::this_thread::sleep_for(std::chrono::seconds(3));
    a = a * a;
    b = b * b;
    p.set_value(a-b);
}

//应用场景二：
void sub(int a, std::future<int> &b, std::promise<int> &p)
{
    a = a * 10;
    int b_copy = b.get() * 2;
    p.set_value(a-b_copy);
}

int main()
{
    //应用场景一：
    //想用p来存放show函数运算的结果，p现在还是空的，未来(即过一会)p会存放运算结果
    std::promise<int> p;
    //虽然p现在还是空的，但是我们可以先给它的未来起一个名字
    std::future<int> result = p.get_future();
    //让一个子线程去干这种事，主线程干别的
    //不使用std::ref会报错，暂时不知道为什么
    std::thread t1(show, 3, 2, std::ref(p));
    t1.detach();
    //......;
    //......;
    //......;
    //我们想要使用计算结果了，但是这个时候子线程到底有没有把结果算出来并且存放到p中，不知道
    //不过没关系，不管p现在存没存结果反正它的未来肯定有，所以直接使用p的未来也就是变量result
    //get()只能使用一次
    std::cout << "平方差的结果是" << result.get() << "\n";

    //应用场景二：
    //现在函数sub的参数a是确定的，待会创建子线程的时候可以直接传进去(这样就跟应用场景一一样)，但是参数b暂时还没定下来
    //这时候怎么办呢，只能先把b的未来作为参数传进去了
    std::promise<int> b;
    std::future<int> bResult = b.get_future();
    //p2和应用场景一的p一样是用来存放结果的
    std::promise<int> p2;
    std::future<int> p2Result = p2.get_future();
    std::thread t2(sub, 4, std::ref(bResult), std::ref(p2));
    t2.detach();
    //......;
    //......;
    //......;
    //经过若干操作，主线程终于把b的结果求出来了
    b.set_value(3);
    //显示sub函数的运算结果
    std::cout << "sub的result:" << p2Result.get() << "\n";
}