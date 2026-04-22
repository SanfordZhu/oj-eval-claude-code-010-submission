#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        node(const T &value, node *p = nullptr, node *n = nullptr)
            : data(new T(value)), prev(p), next(n) {}
        ~node() {
            delete data;
        }
    };

protected:
    /**
     * add data members for linked list as protected members
     */
    node *head;
    node *tail;
    size_t list_size;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        if (pos == nullptr || cur == nullptr) return cur;

        cur->prev = pos->prev;
        cur->next = pos;
        if (pos->prev) pos->prev->next = cur;
        else head = cur;
        pos->prev = cur;
        list_size++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (pos == nullptr) return nullptr;

        if (pos->prev) pos->prev->next = pos->next;
        else head = pos->next;

        if (pos->next) pos->next->prev = pos->prev;
        else tail = pos->prev;

        pos->prev = pos->next = nullptr;
        list_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        node *ptr;
        const list *container;

    public:
        iterator(node *p = nullptr, const list *c = nullptr) : ptr(p), container(c) {}
        iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        friend class list;
        friend class const_iterator;
        /**
         * iter++
         */
        iterator operator++(int) {
            if (ptr == nullptr) throw invalid_iterator();
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr == nullptr) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == nullptr) throw invalid_iterator();
            iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr == nullptr || ptr->prev == nullptr) throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) throw invalid_iterator();
            return *(ptr->data);
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) throw invalid_iterator();
            return ptr->data;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class const_iterator;
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *ptr;
        const list *container;

    public:
        const_iterator(const node *p = nullptr, const list *c = nullptr) : ptr(p), container(c) {}
        const_iterator(const const_iterator &other) : ptr(other.ptr), container(other.container) {}
        const_iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        const_iterator operator++(int) {
            if (ptr == nullptr) throw invalid_iterator();
            const_iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        const_iterator & operator++() {
            if (ptr == nullptr) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == nullptr) throw invalid_iterator();
            const_iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        const_iterator & operator--() {
            if (ptr == nullptr || ptr->prev == nullptr) throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }
        const T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) throw invalid_iterator();
            return *(ptr->data);
        }
        const T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) throw invalid_iterator();
            return ptr->data;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class iterator;
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : head(nullptr), tail(nullptr), list_size(0) {}
    list(const list &other) : head(nullptr), tail(nullptr), list_size(0) {
        for (node *curr = other.head; curr != nullptr; curr = curr->next) {
            push_back(*(curr->data));
        }
    }
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (node *curr = other.head; curr != nullptr; curr = curr->next) {
            push_back(*(curr->data));
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) throw container_is_empty();
        return *(head->data);
    }
    const T & back() const {
        if (empty()) throw container_is_empty();
        return *(tail->data);
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head, this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(nullptr, this);
    }
    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return list_size == 0;
    }
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return list_size;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (head != nullptr) {
            node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        list_size = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) throw invalid_iterator();

        node *new_node = new node(value);
        if (pos.ptr == nullptr) {
            if (tail == nullptr) {
                head = tail = new_node;
            } else {
                new_node->prev = tail;
                tail->next = new_node;
                tail = new_node;
            }
            list_size++;
            return iterator(new_node, this);
        } else {
            node *inserted = insert(pos.ptr, new_node);
            return iterator(inserted, this);
        }
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) throw container_is_empty();
        if (pos.container != this || pos.ptr == nullptr) throw invalid_iterator();

        node *next_node = pos.ptr->next;
        node *to_erase = pos.ptr;
        erase(to_erase);
        delete to_erase;
        return iterator(next_node, this);
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *new_node = new node(value);
        if (tail == nullptr) {
            head = tail = new_node;
        } else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        list_size++;
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();
        node *to_delete = tail;
        tail = tail->prev;
        if (tail == nullptr) {
            head = nullptr;
        } else {
            tail->next = nullptr;
        }
        delete to_delete;
        list_size--;
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *new_node = new node(value);
        if (head == nullptr) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        list_size++;
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();
        node *to_delete = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        } else {
            head->prev = nullptr;
        }
        delete to_delete;
        list_size--;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (size() < 2) return;

        // Use merge sort approach - split list, sort halves, then merge
        // This avoids needing default constructor
        list sorted = merge_sort(*this);
        *this = sorted;
    }

private:
    static list merge_sort(list &lst) {
        if (lst.size() < 2) return lst;

        // Split the list into two halves
        size_t mid = lst.size() / 2;
        list left, right;

        // Move first half to left
        for (size_t i = 0; i < mid; ++i) {
            left.push_back(lst.front());
            lst.pop_front();
        }

        // Move second half to right
        while (!lst.empty()) {
            right.push_back(lst.front());
            lst.pop_front();
        }

        // Recursively sort both halves
        left = merge_sort(left);
        right = merge_sort(right);

        // Merge the sorted halves
        left.merge(right);
        return left;
    }

public:
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;
        if (other.empty()) return;
        if (empty()) {
            *this = other;
            other.clear();
            return;
        }

        node *curr1 = head;
        node *curr2 = other.head;
        node *new_head = nullptr;
        node *new_tail = nullptr;

        while (curr1 != nullptr && curr2 != nullptr) {
            node *selected;
            if (*(curr2->data) < *(curr1->data)) {
                selected = curr2;
                curr2 = curr2->next;
            } else {
                selected = curr1;
                curr1 = curr1->next;
            }

            if (new_head == nullptr) {
                new_head = new_tail = selected;
                selected->prev = nullptr;
            } else {
                new_tail->next = selected;
                selected->prev = new_tail;
                new_tail = selected;
            }
        }

        node *remaining = (curr1 != nullptr) ? curr1 : curr2;
        if (new_tail != nullptr) {
            new_tail->next = remaining;
        }
        if (remaining != nullptr) {
            remaining->prev = new_tail;
        }

        head = new_head;
        tail = (new_tail != nullptr && new_tail->next != nullptr) ? new_tail->next : new_tail;
        if (tail == nullptr && head != nullptr) {
            node *temp = head;
            while (temp->next != nullptr) temp = temp->next;
            tail = temp;
        }

        list_size += other.list_size;

        other.head = other.tail = nullptr;
        other.list_size = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (size() < 2) return;

        node *curr = head;
        node *temp = nullptr;

        while (curr != nullptr) {
            temp = curr->prev;
            curr->prev = curr->next;
            curr->next = temp;
            curr = curr->prev;
        }

        temp = head;
        head = tail;
        tail = temp;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (size() < 2) return;

        node *curr = head;
        while (curr != nullptr && curr->next != nullptr) {
            if (*(curr->data) == *(curr->next->data)) {
                node *to_delete = curr->next;
                curr->next = to_delete->next;
                if (to_delete->next != nullptr) {
                    to_delete->next->prev = curr;
                } else {
                    tail = curr;
                }
                delete to_delete;
                list_size--;
            } else {
                curr = curr->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
