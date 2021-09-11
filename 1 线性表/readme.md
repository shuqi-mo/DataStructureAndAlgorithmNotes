# 线性表

## 线性表的顺序表示

顺序表：由一组地址连续的存储单元依次存储线性表中的数据元素，从而使得逻辑上相邻的两个元素在物理位置上也相邻，即**表中元素的逻辑顺序与其物理顺序相同**

静态空间：array

动态空间：vector

- 动态增加大小并不是在原空间的后面直接构造新元素，要先看看有没有剩余空间
- 如果没有空间了，要以原来大小的两倍另外配置一块较大空间（一个一个加效率太低了），将原来的内容拷贝过来，在后面构造新元素，并释放原空间
- push_back：检查是否还有剩余空间，然后把新元素加到后面
- pop_back：直接让size-1

## 线性表的链式表示

### 单链表

每个节点存有一个值和一个指向下一个节点的指针，leetcode的单链表模板：

```c++
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};
```

基本操作：

- 头插法建立单链表：新节点的next先指向头节点的next，然后让头节点的next指向新节点
- 尾插法建立单链表：找到原来的链表的尾节点，然后在尾节点后面连新节点
- 按序号查找结点值、按值查找结点
- 插入：定位到指定位置，然后用类似头插的思路做
- 删除：用节点的next定位，定位好后跨越修改next

### 双链表

- 每个结点有一个前驱指针prior和一个后继指针next

### 循环单/双链表

- 最后一个节点的next不指向空指针，而是指向头节点

### 链表技巧

#### 链表反转

解法一：头插法，哨兵节点

```c++
    ListNode* reverseList(ListNode* head) {
        if (head == nullptr)
            return head;
        ListNode* newHead = new ListNode(-1);
        newHead->next = head;
        ListNode* p = newHead->next;
        ListNode* r;
        newHead->next = nullptr;
        while (p) {
            r = p->next;
            p->next = newHead->next;
            newHead->next = p;
            p = r;
        }
        return newHead->next;
    }
```

解法二：直接改变链表指针的指向（从前到后-从后到前）

```c++
ListNode* reverseList(ListNode* head) {
	ListNode* prev = nullptr;
    ListNode* curr;
    while (head) {
        curr = head->next;
        head->next = prev;
        prev = head;
        head = curr;
    }
    return prev;
}
```

#### 链表合并

当`l1`和`l2`都不是空链表时，判断`l1`和`l2`哪一个链表的头节点的值更小，将较小值的节点添加到结果里，当一个节点被添加到结果里之后，将对应链表中的节点向后移一位。

注意一下哨兵节点的细节。

```c++
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode* head = new ListNode(0);
        ListNode* node = head;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                node->next = l1;
                l1 = l1->next;
                node = node->next;
            }
            else {
                node->next = l2;
                l2 = l2->next;
                node = node->next;
            }
        }
        node->next = l1 ? l1 : l2;
        return head->next;
    }
```

#### 两两交换链表节点

关键是要模拟指针的变化情况

trick：修改指针时，从远的节点开始修改，而且是一步步递进修改，保持好上一步的结果，下一步就可以改了

```c++
    ListNode* swapPairs(ListNode* head) {
        ListNode* p = head;
        ListNode* s;
        if (p && p->next) {
            s = p->next;
            p->next = s->next;
            s->next = p;
            head = s;
            while (p->next && p->next->next) {
                s = p->next->next;
                p->next->next = s->next;
                s->next = p->next;
                p->next = s;
                p = s->next;
            }
        }
        return head;
    }
```

#### 链表交点



#### 回文链表

快慢指针

## 代码说明

MyVector.cpp是用类手写了一个vector，核心是动态空间的思想

MyLinkedList.cpp是用结构体手写了链表的一些基本操作，包括头插法、尾插法、按指定位置插入、按指定值删除
