# STL基础(Chap1-3)

## 1 概论

### STL六大组件

- 容器container：存放数据的各种数据结构，比如vector，list，deque，set，map
- 算法algorithm：各种常用算法如sort，search

- 迭代器iterator：泛型指针

- 仿函数functor：可以作为算法的某种策略

- 配接器adapter：修饰containers或functors或iterators接口的东西，比如queue和stack

- 配置器allocator：空间配置与管理

### 用到的一些C++技巧

#### partial specialization和argument deduction

```c++
// 一般化设计
template <class I, class O>
struct testClass {
	testClass()	{ cout << "I, O" << endl; }
};
// 特殊化设计
template <class T>
struct testClass<T*, T*> {
    testClass() { cout << "T*, T*" << endl; }
};
// 特殊化设计
template <class T>
struct testClass<const T*, T*> {
    testClass() { cout << "const T*, T*" << endl; }
};

int main() {
    testClass<int, char> obj1;
    testClass<int*, int*> obj2;
    testClass<const int*, int*> obj3;
}
```

#### bound friend template

class template的某个instantiation与其friend function template的某个instantiation有一对一的关系

```c++
template <class T, class Alloc = alloc, size_t Bufsiz = 0>
class deque {
public:
    deque() { cout << "deque" << endl; }
};

// 根据前一个参数值T，设定下一个参数Sequence的默认值
template <class T, class Sequence = deque<T>>
class stack {
public:
    stack() { cout << "stack" << endl;}
private:
    Sequence c;
};
```

#### 临时对象

在class template后面直接加一对小括号，指定初值，但不指定对象名称

#### 前闭后开表示

[first, last)

#### function call操作符：operator()

如果针对某个class进行operator()重载，它就会成为一个仿函数

## 2 空间配置器allocator

### 具备sub-allocation的SGI空间配置器

#### 一个效率不佳的标准空间配置器：std::allocator

把C++中的`::operator new`和`::operator delete`做了一层简单的包装

#### SGI特殊的空间配置器：std::alloc

#### intuition

C++中的内存配置和释放操作

- 用`new`配置内存然后构造对象：调用`::operator new`配置内存，然后调用`Foo::Foo()`构造对象内容
- 用`delete`析构对象然后释放内存：调用`Foo::~Foo()`将对象析构，然后调用`::operator delete`释放内存

```c++
class Foo {...};
Foo* pf = new Foo;
delete pf;
```

STL allocator将这两阶段区分开来，内存配置操作由`alloc::allocate()`负责，内存释放操作由`alloc::deallocate()`负责，对象构造操作由`::construct()`负责，对象析构操作由`::destroy()`负责

```c++
#include <stl_alloc.h>
#include <stl_construct.h>
```

#### 构造和析构基本工具：construct()和destroy()

- 构造：接受一个指针和初值，并使用`placement new`运算子完成将初值设定到指针所指空间的任务
- 析构：接受first和last两个迭代器，先判断每个对象的析构函数是否trivial，如果是的话什么都不做就结束，否则才循环遍历整个范围

#### 空间的配置与释放

- 向system heap要求空间
- 考虑多线程状态
- 考虑内存不足时的应变措施
- 考虑过多小型区块可能造成的内存碎片问题

**方案：一级配置器与二级配置器**

一级配置器：超过128bytes的大区块

- allocate()直接使用malloc()，deallocate()直接使用free()
- 模拟`set_new_handler()`以处理内存不足的状况：要求系统在内存配置需求无法被满足时，调用一个你所指定的函数

二级配置器：小于128bytes的小区块

- 每次配置一大块内存，并维护对应的free-list，下次若再有相同大小的内存需求，就直接从free-list中拨出
- 如果没有可用区块，就将区块大小上调至8倍数边界
- 思想有点类似于OS里面的伙伴系统，实现起来不太一样

### 内存基本处理工具

#### uninitialized_copy

```c++
template <class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result);
```

- 该方法可以为输入来源范围内的每一个迭代器产生一份复制品，放置到输出范围的相对位置
- 用处：容器的全区间构造函数

#### uninitialized_fill

```c++
template <class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x);
```

为输入范围的迭代器产生x份复制品

#### uninitialized_fill_n

```c++
template <class ForwardIterator, class Size, class T>
ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x);
```

对first到first+n的迭代器产生x份复制品

#### 总结

- commit or rollback：要么构造出所有必要元素，要么不构造任何东西
- 实现时会先判断迭代器的value type是否为POD(Plain Old Data)类型，如果是的话可以采用比较有效率的初值填写手法

## 3 迭代器概念与traits编程技法

### 迭代器是一种smart pointer

- iterator模式：能够按顺序遍历某个容器所含的各个元素，又无需暴露该容器的内部表述方式
- 指针的核心行为是dereference和member access，所以迭代器最重要的编程工作是对`operator*`和`operator->`进行重载

迭代器运用的例子：find函数

```c++
template <class InputIterator, class T>
InputIterator find (InputIterator first, InputIterator last, const T& value) {
	while (first != last && *first != value)
        ++first;
   return first;
}
```

为链表list设计一个迭代器：当dereference这一迭代器时，传回ListItem对象，递增迭代器时指向下一个ListItem对象

```c++
template <typename T>
class List
{
	void insert_front(T value);
	void insert_end(T value);
	void display(std::ostream &os = std::cout)	const;
	// ...
private:
	ListItem<T>* _end;
	ListItem<T>* _front;
	long _size;
};

template <typename T>
class ListItem
{
public:
	T value() const {	return _value;	}
	ListItem* next() const {	return _next;	}
	...
private:
	T _value;
	ListItem* _next;
};
```

迭代器的设计方案

```c++
template <class Item>
struct ListIter
{
	Item* ptr;
	ListIter(Item* p = 0) : ptr(p) {}
	ListIter& operator*() const {	return *ptr;	}
	ListIter* operator->() const {	return ptr;	}
	ListIter& operator++() {
		ptr = ptr->next();
		return *this;
	}
    ListIter operator++(int) {
        ListItem tmp = *this;
        ++*this;
        return tmp;
    }
    bool operator==(const ListIter& i) const {
        return ptr == i.ptr;
    }
    bool operator!=(const ListIter& i) const {
        return ptr != i.ptr;
    }
};
// ListIter可以把List和find()粘合起来
void main() {
    List<int> myList;
    for (int i = 0; i < 5; i++) {
        myList.insert_front(i);
        myList.insert_end(i+2);
    }
    ListIter<ListItem<int>> begin(myList.front());
    ListIter<ListItem<int>> end;
    ListIter<ListItem<int>> iter;
    iter = find(begin, end, 3);
    ...
}
```

但是这样子会暴露很多实现细节，所以把迭代器的开发工作交给容器的设计者好了，把实现细节封装起来

### traits编程技法：强化版的function template参数推导

```c++
// 专门用来萃取迭代器特性的class template
template <class I>
struct iterator_traits {
	typedef typename I::iterator_category iterator_category;	// 读写之类的操作权限
	typedef typename I::value_type value_type;	// 所指对象型别
	typedef typename I::difference_type difference_type;	// 两个迭代器之间的距离
	typedef typename I::pointer pointer;		// 指向迭代器所指之物
	typedef typename I::reference reference;	// 迭代器内容能否被改变，左值可以赋值，右值不行
};
```

