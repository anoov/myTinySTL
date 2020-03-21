//
// Created by 孙卓文 on 2020/3/20.
//

#ifndef MYTINYSTL_MYLIST_H
#define MYTINYSTL_MYLIST_H

//list的节点结构，是一个双向链表
template <typename T>
struct list_node {
    using node_pointer = list_node*;
    node_pointer prev;
    node_pointer next;
    T data;
};

//list的迭代器
template <typename T, typename Ref, typename Ptr>
struct list_iterator {
    using iterator = list_iterator<T, T&, T*>;
    using self = list_iterator<T, Ref, Ptr>;

    using value_type = T;
    using pointer = Ptr;
    using reference = Ref;
    using link_type = list_node<T>*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    link_type node;                             //迭代器内有一个普通指针指向list的节点

    //list迭代器的构造函数
    list_iterator(link_type x): node(x) {}
    list_iterator() {}
    list_iterator(const iterator& x) : node(x.node) {}

    reference operator*() const {return node->data;}
    pointer operator->() const {return &(operator*());}

    bool operator==(const self& x) const {return node == x.node;}
    bool operator!=(const self& x) const {return node != x.node;}

    self& operator++() {
        node = (link_type)(node->next);
        return *this;
    }
    self operator++(int) {
        auto tmp = *this;
        ++ *this;
        return tmp;
    }

    self& operator--() {
        node = (link_type)(node->prev);
        return *this;
    }
    self operator--(int) {
        auto tmp = *this;
        -- *this;
        return tmp;
    }
};

template <typename T>
class myList {
public:
    using size_type = size_t;
    using link_type = list_node<T>*;
    using iterator = list_iterator<T, T&, T*>;
    using reference = T&;
    using pointer = T*;

protected:
    using list_node = list_node<T>;
    link_type node;                         //一个头指针便可以表示整个双向链表

public:
    iterator begin() {return (link_type)(node->next);}
    iterator end() {return node;}

    bool empty() const {return node->next == node;}
    size_type size() const {
        size_type result = 0;
        std::distance(begin(), end(), result);
        return result;
    }
    reference front() {return *begin();}
    reference back() {return *(-end());}


protected:
};

#endif //MYTINYSTL_MYLIST_H
