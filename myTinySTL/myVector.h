//
// Created by 孙卓文 on 2020/3/18.
//

#ifndef MYTINYSTL_MYVECTOR_H
#define MYTINYSTL_MYVECTOR_H

#include <algorithm>
//vector
template <typename T>
class Vector
{
public:
    using value_type = T;
    using iterator = value_type*;
    using reference = value_type&;
    using size_type = size_t ;

protected:
    iterator start;                                                          //vector起始元素的迭代器
    iterator finish;                                                         //vector元素结束位置的下一个位置的迭代器
    iterator end_of_storage;                                                 //vector预留空间最后位置的下一个位置的迭代器
    void inset_axu(iterator position, const T &x);

    //配置空间并填满内容
    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = std::allocator<T>().allocate(n);
        std::uninitialized_fill_n(result, n, x);
        return result;
    }
    void fill_initialize(size_type n, const T& x) {
        start = allocate_and_fill(n, x);
        finish = start + n;
        end_of_storage = finish;
    }
    void fill(iterator first, iterator last, const T& x) {
        auto iter = first;
        for (; iter != last; ++iter) {
            *iter = x;
        }
    }

    void deallocate() {
        if (start) {
            std::allocator<T>().deallocate(start, end_of_storage - start);
        }
    }

public:
    //构造函数
    Vector():start(0), finish(0), end_of_storage(0){}
    Vector(size_type n, const T &x) {
        fill_initialize(n, x);
    }
    Vector(int n, const T& x) {
        fill_initialize(n, x);
    }
    Vector(long n, const T& x) {
        fill_initialize(n, x);
    }
    explicit Vector(size_type n) {
        fill_initialize(n, T());
    }

    //析构函数
    ~Vector() {
        auto iter = start;
        for (; iter != finish; iter++)
            std::allocator<T>().destroy(iter);
        deallocate();
    }

    iterator begin() {return start;}
    iterator end() {return finish;}
    size_type size() const {
        return size_type(finish - start);
    }
    size_type capacity() const {
        return size_type(end_of_storage - start);
    }
    bool empty() const {
        return start == finish;
    }
    reference operator[](size_type n) {
        return *(begin() + n);
    }
    reference front() {return *begin();}
    reference back() {return *(end()-1);}

void push_back(const T& x) {
    //拥有预留空间
    if (finish != end_of_storage) {
        std::allocator<T>().construct(finish, x);
        ++finish;
    }
    //没有预留空间
    else
        inset_axu(end(), x);
}
    void pop_back() {
        --finish;
        std::allocator<T>().destroy(finish);
    }
    iterator erase(iterator position) {
        if (position + 1 != end()) {
            std::copy(position + 1, finish, position);
        }
        --finish;
        std::allocator<T>().destroy(finish);
        return position;
    }
    iterator erase(iterator first, iterator last) {
        iterator i = std::copy(last, finish, first);
        auto iter_tmp = i;
        for (; iter_tmp != finish; ++iter_tmp)
            std::allocator<T>().destroy(iter_tmp);
        finish = finish - (last - first);
        return first;
    }
    void resize(size_type new_size, const T& x) {
        if (new_size < size())
            erase(begin()+new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
    void clear() {
        erase(begin(), end());
    }
    void insert(iterator position, size_type n, const T& x);

};

template<typename T>
void Vector<T>::inset_axu(Vector::iterator position, const T &x) {
    //Vector拥有备用空间
    if (finish != end_of_storage) {
        std::allocator<T>().construct(finish, *(finish-1));
        ++finish;
        T x_copy = x;
        std::copy_backward(position, finish-2, finish-1);
        *position = x_copy;

    }
    //Vector没有备用空间
    else {
        const size_type old_size = size();
        //当原来空间大小不为零时分配原来空间大小二倍的空间
        const size_type len = old_size == 0 ? 1 : 2 * old_size;
        iterator new_start = std::allocator<T>().allocate(len);
        iterator new_finish = new_start;
        try {
            //将原vector插入位置之前的内容拷贝到新vector
            new_finish = std::uninitialized_copy(start, position, new_start);
            //在新配置空间的finish位置设定要插入的对象
            std::allocator<T>().construct(new_finish, x);
            //更新新finish指向的位置
            ++new_finish;
            //将原vector插入位置之后的内容拷贝到新vector
            new_finish = std::uninitialized_copy(position, finish, new_finish);
        }
        //若分配新空间时出现异常，则析构并释放新空间
        catch(...) {
            auto iter = new_start;
            for (; iter != new_finish; ++iter)
                std::allocator<T>().destroy(iter);
            std::allocator<T>().deallocate(new_start, len);
            throw;
        }
        //析构并释放原vector
        auto iter = begin();
        for (; iter != end(); ++iter)
            std::allocator<T>().destroy(iter);
        deallocate();
        //调整迭代器
        start = new_start;
        finish = new_finish;
        end_of_storage = new_finish + len;

    }
}

template<typename T>
void Vector<T>::insert(Vector::iterator position, Vector::size_type n, const T &x) {
    if (n != 0) {
        if (size_type(end_of_storage - finish) >= n) {                                 //备用空间完全可以使用
            T x_copy = x;
            const size_type elements_after = finish - position;                        //插入点之后的元素个数
            iterator old_finish = finish;
            if (elements_after > n){                                                   //插入点之后的元素个数大于新增元素个数
                std::uninitialized_copy(finish - n, finish, finish);
                finish += n;
                std::copy_backward(position, old_finish - n, old_finish);
                fill(position, position+n, x_copy);
            }
            else {                                                                     //插入点之后的元素个数小于等于新增元素个数
                std::uninitialized_fill_n(finish, n-elements_after, x_copy);
                finish += n - elements_after;
                std::uninitialized_copy(position, old_finish, finish);
                finish += elements_after;
                fill(position, old_finish, x_copy);
            }
        }
        else {                                                                         //备用空间不够使用
            const size_type old_size = size();
            const size_type len = old_size + std::max(old_size, n);
            //下面配置新的Vector空间
            iterator new_start = std::allocator<T>().allocate(len);
            iterator new_finish = new_start;
            try {
                //先将旧Vector的插入点之前的元素复制到新空间里
                new_finish = std::uninitialized_copy(start, position, new_start);
                //再将新增元素填入新空间
                new_finish = std::uninitialized_fill_n(new_finish, n, x);
                //再将旧Vector的插入点之后的元素复制到新空间里
                new_finish = std::uninitialized_copy(position, finish, new_finish);
            }
            catch(...) {
                auto iter = new_start;
                for (; iter != new_finish; ++iter) {
                    std::allocator<T>().destroy(iter);
                }
                std::allocator<T>().deallocate(new_start, len);
                throw ;
            }
            //清除并释放旧的Vector
            auto iter = start;
            for (; iter != finish; ++iter) {
                std::allocator<T>().destroy(iter);
            }
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_finish + len;
        }
    }
}

#endif //MYTINYSTL_MYVECTOR_H
