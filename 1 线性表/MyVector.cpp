#include<iostream>
#include<algorithm>
using namespace std;

template <typename T>
class MyVector {
public:
    // 构造函数：开辟一块大小为theCapacity的空间
    // explicit关键字：禁止隐式转换
    explicit MyVector(int initSize = 0) : theSize{initSize=0}, theCapacity{initSize + SPARE_CAPACITY} {
        objects = new T[theCapacity];
    }

    // 构造函数
    MyVector(const MyVector &rhs) : theSize{rhs.theSize}, theCapacity{rhs.theCapacity}, objects{nullptr} {
        objects = new T[theCapacity];
        for(int k = 0; k < theSize; ++k) {
            objects[k] = rhs.objects[k];
        }
    }

    // 深拷贝
    MyVector& operator=(const MyVector &rhs) {
        MyVector copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    ~MyVector() {
        delete [] objects;
    }

    void resize(int newSize) {
        if (newSize > theCapacity)
            reserve(newSize * 2);   // 开一块原大小2倍的空间
        theSize = newSize;
    }

    void reserve(int newCapacity) {
        if (newCapacity < theSize)
            return;
        T* newArray = new T[newCapacity];
        for (int k = 0; k < theSize; ++k)
            newArray[k] = std::move(objects[k]);
        theCapacity = newCapacity;
        std::swap(objects, newArray);
        delete [] newArray;
    }

    T& operator[](int index) {
        return objects[index];
    }

    bool empty() const {
        return size() == 0;
    }

    int size() const {
        return theSize;
    }

    int capacity() const {
        return theCapacity;
    }

    void push_back(const T& x) {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        objects[theSize++] = x;
    }

    void pop_back() {
        --theSize;
    }

    // 在给定位置插入元素
    void insert(int pos, const T& x) {
        if (theSize == theCapacity)
            reserve(2 * theCapacity + 1);
        for (int i = pos; i < theSize; i++) {
            objects[i+1] = objects[i];
        }
        objects[pos] = x;
        theSize++;
    }

    // 删除给定位置的元素
    void erase(int pos) {
        for (int i = pos; i < theSize - 1; i++) {
            objects[i] = objects[i+1];
        }
        theSize--;
    }

    // 查找给定值的下标
    int find(int value) {
        for (int i = 0; i < theSize; i++) {
            if (objects[i] == value)
                return i;
        }
        return -1;
    }

    static const int SPARE_CAPACITY = 2;

private:
    int theSize;
    int theCapacity;
    T* objects;
};

int main() {
    MyVector<int> v;
    cout << "size = " << v.size() << endl;
    cout << "capacity = " << v.capacity() << endl;
    v.push_back(1);
    cout << "size = " << v.size() << endl;
    cout << "capacity = " << v.capacity() << endl;
    v.push_back(3);
    cout << "size = " << v.size() << endl;
    cout << "capacity = " << v.capacity() << endl;
    v.push_back(5);
    cout << "size = " << v.size() << endl;
    cout << "capacity = " << v.capacity() << endl;
    cout << v[1] << endl;
    v.insert(1,7);
    cout << "size = " << v.size() << endl;
    cout << "capacity = " << v.capacity() << endl;
    cout << v[1] << endl;
    v.erase(1);
    cout << "size = " << v.size() << endl;
    cout << "capacity = " << v.capacity() << endl;
    cout << v[1] << endl;
    cout << v.find(3) << endl;
    cout << v.find(9) << endl;
    return 0;
}