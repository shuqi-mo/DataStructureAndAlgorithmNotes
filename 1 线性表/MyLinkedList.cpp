#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 头插法：新节点的next先指向头节点的next，然后让头节点的next指向新节点
void headInsert (ListNode*& head, int num) {
    ListNode* node = new ListNode(num);
    node->next = head->next;
    head->next = node;
}

// 尾插法：找到原来的链表的尾节点，然后在尾节点后面连新节点
void tailInsert (ListNode*& head, int num) {
    ListNode* node = new ListNode(num);
    ListNode* tail = head;
    while (tail->next) {
        tail = tail -> next;
    }
    tail->next = node;
}

// 指定位置后插入：先安排新节点的next，再安排原有节点的next
void locateInsert (ListNode*& head, int num, int locate) {
    int cnt = 0;
    ListNode* node = new ListNode(num);
    ListNode* loc = head;
    while (cnt < locate) {
        loc = loc -> next;
        cnt++;
    }
    node -> next = loc -> next;
    loc -> next = node;
}

// 删除指定值的节点
void valueDelete (ListNode*& head, int value) {
    ListNode* node = head;
    while (node->next) {
        if (node->next->val == value) {
            node->next = node->next->next;
            break;
        }
        node = node -> next;
    }
}

void printList(ListNode* head) {
    cout << "现在的链表是这样子的哦..." << endl;
    while (head) {
        if (head->val != -1)
            cout << head->val << " ";
        head = head -> next;
    }
    cout << endl;
}

int main() {
    int num, locate;
    ListNode* head = new ListNode(-1);
    cout << "先试一下用头插法建立链表吧，输入0结束" << endl;
    cin >> num;
    while (num) {
        headInsert(head, num);
        cin >> num;
    }
    printList(head);
    cout << "接下来是尾插法，输入0结束" << endl;
    cin >> num;
    while (num) {
        tailInsert(head, num);
        cin >> num;
    }
    printList(head);
    cout << "接下来是指定位置插入法，输入0 0结束" << endl;
    cin >> num >> locate;
    while (num && locate) {
        locateInsert(head, num, locate);
        cin >> num >> locate;
    }
    printList(head);
    cout << "接下来是指定值删除对应节点，输入0结束" << endl;
    cin >> num;
    while (num) {
        valueDelete(head, num);
        cin >> num;
    }
    printList(head);
    return 0;
}