#ifndef _SRC_S21_LIST_H_
#define _SRC_S21_LIST_H_

#include <iostream>
#include <initializer_list>

namespace s21 {

template <class T>
class list;

template <class T>
struct Node {
    T value;
    Node *prev = nullptr;
    Node *next = nullptr;
};

template <class T>
class ListIterator {
    private:
        friend class list<T>;
        Node<T> *ptr;
    public:
        T& operator*() {
            return ptr->value;
        }
        ListIterator& operator++(int) {
            ptr = ptr->next;
            return *this;
        }
        ListIterator& operator--(int) {
            ptr = ptr->prev;
            return *this;
        }
        bool operator!=(ListIterator<T> other) {
            if (ptr == other.ptr) return false;
            return true;
        }
        bool operator==(ListIterator<T> other) {
            if (ptr == other.ptr) return true;
            return false;
        }
        ListIterator(Node<T> *node) {
            ptr = node;
        }
};

template <class T>
class ListConstIterator;

template <class T>
class list {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using iterator = ListIterator<T>;
        using const_iterator = ListConstIterator<T>;
        using size_type = std::size_t;
        using node = Node<T>;

    private:
        node *head = nullptr; // front
        node *root = nullptr; // back
        node *fake = nullptr; // = reinterpret_cast<node*>(new int8_t[sizeof(node)]);
        size_type size_ = 0;
        void closedFake() {
            fake->next = fake;
            fake->prev = fake;
            head = fake;
            root = fake;
        }
        void fakeAllocate() {
            fake = new node;
            closedFake();
        }
        void swapNode(node **a, node **b) {
            node *buff = *a;
            *a = *b;
            *b = buff;
        }

    public:

        list() {
            fakeAllocate();
        }
        list(size_type n) {
            fakeAllocate();
            node *prev_ = fake;
            for (int k = 0; k < n; k++) {
                node *add = new node;
                if (prev_ != fake) prev_->next = add;
                add->prev = prev_;
                prev_ = add;
                if (k == 0) head = add;
                if (k == n - 1) {
                    root = add;
                    root->next = fake;
                }
            }
            size_ = n;
        }
        list(std::initializer_list<value_type> const &items) {
            fakeAllocate();
        }
        list(const list<value_type> &l) {
            size_ = l->size_;
            fakeAllocate();
            node *copy = l->head;
            node *prev_ = fake;
            for (size_type k = 0; k < size_; k++) {
                node *add = new node;
                if (prev_ != fake) prev_->next = add;
                add->prev = prev_;
                if (k == 0) head == add;
                if (k == size_ - 1) {
                    root - add;
                    root->next - fake;
                }
                add->value = copy->value;
                copy = copy->next;
            }
        }
        list(list<value_type> &&l) {
            this = *l;
        }
        ~list() {
            while (head != fake) {
                node *del = head;
                head = head->next;
                delete del;
            }
            delete fake;
        }
        list &operator=(list<value_type> &&l) { 
            this = *l;
        }

        const_reference front() {
            return head->value;
        }
        const_reference back() {
            return root->value;
        }

        iterator begin() {
            iterator begin_iter(head);
            return begin_iter;
        }
        iterator end() {
            iterator fake_iter(fake);
            return fake_iter;
        }

        bool empty() {
            if (head == fake && root == fake) return true;
            return false;
        }
        size_type size() {
            return size_;
        }
        size_type max_size();  //  --------------------------------------------------------------- ???
        void clear() {
            node *del = head;
            for (size_type k = 0; k < size_; k++) {
                head = head->next;
                delete del;
                del = head;
            }
            closedFake();
            size_ = 0;
        }
        iterator insert(iterator pos, const_reference value) {
            node *add = new node;
            add->value = value;
            add->next = pos.ptr->next;
            add->next->prev = add;
            pos.ptr->next = add;
            add->prev = pos.ptr;
            if (pos.ptr == fake) head = add;
            if (add->next == fake) root = add;
            size_++;
            iterator ret(add);
            return ret;
        }
        void erase(iterator pos) {
            pos.ptr->next->prev = pos.ptr->prev;
            pos.ptr->prev->next = pos.ptr->next;
            delete pos.ptr;
            size_--;
        }
        void push_back(const_reference value) {
            node *add = new node;
            add->value = value;
            add->next = fake;
            add->prev = root;
            root->next = add;
            root = add;
            if (fake == head) head = root;
            size_++;
        }
        void pop_back() {
            node *del = root;
            root = root->prev;
            root->next = fake;
            delete del;
            size_--;
        }
        void push_front(const_reference value) {
            node *add = new node;
            add->value = value;
            add->next = head;
            add->prev = fake;
            head->prev = add;
            head = add;
            if (fake == root) root = head;
            size_++;
        }
        void pop_front() {
            node *del = head;
            head = head->next;
            head->prev = fake;
            delete del;
            size_--;
        }
        void swap(list<T>& other) {
            swapNode(&head, &other.head);
            swapNode(&root, &other.root);
            swapNode(&fake, &other.fake);
            size_type S = size_;
            size_ = other.size_;
            other.size_ = S;
        }
        void merge(list& other) {
            root->next = other.head;
            other.root->next = fake;
            other.closedFake();
        }
        void splice(const_iterator pos, list& other);
        void reverse();
        void unique();
        void sort();

    protected:

};

}  // namespace s21

#endif // _SRC_S21_LIST_H_