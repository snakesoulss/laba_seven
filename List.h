#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include <stdexcept>
#include <iterator>

template <typename T>
class List
{
protected:
     struct Node
    {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    size_t list_size;
    void ProtectedCopyFrom(const List& other);

public:
    List();
    List(const List& other);
    List(List&& other) noexcept;
    virtual ~List();

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    virtual void Push(const T& value) = 0;
    virtual void Pop() = 0;
    virtual T& GetFront() = 0;
    virtual const T& GetFront() const = 0;
    virtual bool IsEmpty() const = 0;
    virtual size_t Size() const = 0;


    class Const_iterator;

    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        Iterator(Node* p = nullptr): p(p){}

        reference operator*()const {return p->data;}
        pointer operator->() const {return &(p->data);}

        Iterator& operator ++(){p=p->next; return *this;}
        Iterator operator ++(int) {Iterator t = *this; ++(*this); return t;}

        bool operator == (const Iterator& other) const {return p==other.p;}
        bool operator != (const Iterator& other) const {return p!=other.p;}

        bool operator==(const Const_iterator& other) const { return p == other.p; }
        bool operator!=(const Const_iterator& other) const { return p != other.p; }

    private:
        Node* p;

    };


class Const_iterator{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = const T*;
    using reference = const T&;

    Const_iterator(Node* p = nullptr) : p(p) {}
    Const_iterator(const Iterator& i) : p(i.p) {}

    reference operator*() const { return p->data; }
    pointer operator->() const { return &(p->data);}

    Const_iterator& operator++(){p = p->next; return *this;}
    Const_iterator operator++(int){Const_iterator t = *this; ++(*this);return t;}

    bool operator==(const Const_iterator& other) const {return p == other.p;}
    bool operator!=(const Const_iterator& other) const {return p != other.p;}

    bool operator == (const Iterator& other) const {return p==other.p;}
    bool operator != (const Iterator& other) const {return p!=other.p;}

private:
    Node* p;
};

    virtual Iterator end() = 0;
    virtual Iterator begin() = 0;
    virtual Const_iterator cend() const = 0;
    virtual Const_iterator cbegin() const = 0;
    virtual Const_iterator end() const = 0;
    virtual Const_iterator begin() const = 0;



private:
    void Clear();
    void CopyFrom(const List& other);

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& list)
{
    for (auto i = list.cbegin(); i != list.cend(); ++i)
    {
        os << *i << " ";
    }
    return os;
}


template <typename T>
List<T>::List() : head(nullptr), list_size(0) {}

template <typename T>
List<T>::List(const List& other) : head(nullptr), list_size(0)
{
    CopyFrom(other);
}

template <typename T>
List<T>::List(List&& other) noexcept : head(other.head), list_size(other.list_size)
{
    other.head = nullptr;
    other.list_size = 0;
}

template <typename T>
List<T>::~List()
{
    Clear();
}

template <typename T>
List<T>& List<T>::operator=(const List& other)
{
    if (this != &other)
    {
        Clear();
        CopyFrom(other);
    }
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List&& other) noexcept
{
    if (this != &other)
    {
        Clear();
        head = other.head;
        list_size = other.list_size;
        other.head = nullptr;
        other.list_size = 0;
    }
    return *this;
}

template <typename T>
void List<T>::CopyFrom(const List& other)
{
    if (other.head == nullptr) return;
    Node* temp = other.head;
    Node* new_node = new Node(temp->data);
    head = new_node;
    temp = temp->next;

    while (temp)
    {
        new_node->next = new Node(temp->data);
        new_node = new_node->next;
        temp = temp->next;
    }
    list_size = other.list_size;
}

template <typename T>
void List<T>::ProtectedCopyFrom(const List& other)
{
    CopyFrom(other);
}

template <typename T>
void List<T>::Clear()
{
    while (head)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    list_size = 0;
}

template <typename T>
class Stack : public List<T>
{
public:
    Stack();
    ~ Stack();
    Stack(const Stack& other);
    Stack(Stack&& other) noexcept;
    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other) noexcept;

    void Push(const T& value) override;
    void Pop() override;
    T& GetFront() override;
    const T& GetFront() const override;
    bool IsEmpty() const override;
    size_t Size() const override;

    typename List<T>::Iterator begin() override {
        return typename List<T>::Iterator(this->head);}

    typename List<T>::Iterator end() override {
        return typename List<T>::Iterator(nullptr);}

    typename List<T>::Const_iterator cbegin() const override {
        return typename List<T>::Const_iterator(this->head);}

    typename List<T>::Const_iterator cend() const override {
        return typename List<T>::Const_iterator(nullptr);}

    typename List<T>::Const_iterator begin() const override {
        return typename List<T>::Const_iterator(this->head);}

    typename List<T>::Const_iterator end() const override {
        return typename List<T>::Const_iterator(nullptr);}

private:
    using List<T>::head;
    using List<T>::list_size;
    void CopyFrom(const Stack& other);
    void Clear();

};

template <typename T>
class Queue : public List<T>
{
public:
    Queue();
    ~Queue();
    Queue(const Queue& other);
    Queue(Queue&& other) noexcept;
    Queue& operator=(const Queue& other);
    Queue& operator=(Queue&& other) noexcept;

    void Push(const T& value) override;
    void Pop() override;
    T& GetFront() override;
    const T& GetFront() const override;
    bool IsEmpty() const override;
    size_t Size() const override;

    typename List<T>::Iterator begin() override {
        return typename List<T>::Iterator(this->head);}

    typename List<T>::Iterator end() override {
        return typename List<T>::Iterator(nullptr);}

    typename List<T>::Const_iterator cbegin() const override {
        return typename List<T>::Const_iterator(this->head);}

    typename List<T>::Const_iterator cend() const override {
        return typename List<T>::Const_iterator(nullptr);}

    typename List<T>::Const_iterator begin() const override {
        return typename List<T>::Const_iterator(this->head);}

    typename List<T>::Const_iterator end() const override {
        return typename List<T>::Const_iterator(nullptr);}

private:
    typename List<T>::Node* tail;
    using List<T>::head;
    using List<T>::list_size;

    void CopyFrom(const Queue<T>& other);
    void Clear();
};


//Stack
template <typename T>
Stack<T>::Stack() : List<T>() {}

template <typename T>
Stack<T>::Stack(const Stack& other) : List<T>(other) {}

template <typename T>
Stack<T>::Stack(Stack&& other) noexcept : List<T>(std::move(other)) {}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other)
{
    if (this != &other)
    {
        List<T>::operator=(other);
    }
    return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept
{
    if (this != &other)
    {
        List<T>::operator=(std::move(other));
    }
    return *this;
}

template <typename T>
Stack<T>::~Stack() {}

template <typename T>
void Stack<T>::Push(const T& value)
{
    typename List<T>::Node* new_node = new typename List<T>::Node(value);
    new_node->next = this->head;
    this->head = new_node;
    this->list_size++;
}

template <typename T>
void Stack<T>::Pop()
{
    if (IsEmpty())
    {
        throw std::runtime_error("Stack is empty");
    }
    typename List<T>::Node* temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->list_size--;
}

template <typename T>
T& Stack<T>::GetFront()
{
    if (IsEmpty()) throw std::runtime_error("Stack is empty");
    return this->head->data;
}

template <typename T>
const T& Stack<T>::GetFront() const
{
    if (IsEmpty()) throw std::runtime_error("Stack is empty");
    return this->head->data;
}

template <typename T>
bool Stack<T>::IsEmpty() const
{
    return this->head == nullptr;
}

template <typename T>
size_t Stack<T>::Size() const
{
    return this->list_size;
}



//Queue
template <typename T>
Queue<T>::Queue() : List<T>(), tail(nullptr){}

template <typename T>
Queue<T>::Queue(const Queue& other) : List<T>(other), tail(nullptr)
{
    if (other.head == nullptr)
    {
        return;
    }
    typename List<T>::Node* temp = this->head;
    while (temp->next)
    {
        temp = temp->next;
    }
    tail = temp;
}

template <typename T>
Queue<T>::Queue(Queue&& other) noexcept : List<T>(std::move(other)), tail(other.tail)
{
    other.tail = nullptr;
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& other)
{
    if (this != &other)
    {
        List<T>::operator=(other);

        typename List<T>::Node* temp = this->head;
        while (temp && temp->next)
        {
            temp = temp->next;
        }
        tail = temp;
    }
    return *this;
}

template <typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept
{
    if (this != &other)
    {
        List<T>::operator=(std::move(other));
        tail = other.tail;
        other.tail = nullptr;
    }
    return *this;
}

template <typename T>
Queue<T>::~Queue() {}

template <typename T>
void Queue<T>::Push(const T& value)
{
    typename List<T>::Node* new_node = new typename List<T>::Node(value);
    if (this->IsEmpty())
    {
        this->head = new_node;
    }
    else
    {
        tail->next = new_node;
    }
    tail = new_node;
    this->list_size++;
}

template <typename T>
void Queue<T>::Pop()
{
    if (IsEmpty())
    {
        throw std::runtime_error("Queue is empty");
    }
    typename List<T>::Node* temp = this->head;
    this->head = this->head->next;
    delete temp;

    if (this->head == nullptr)
    {
        tail = nullptr;
    }

    this->list_size--;
}

template <typename T>
T& Queue<T>::GetFront()
{
    if (IsEmpty()) throw std::runtime_error("Queue is empty");
    return this->head->data;
}
template <typename T>
const T& Queue<T>::GetFront() const
{
    if (IsEmpty()) throw std::runtime_error("Queue is empty");
    return this->head->data;
}

template <typename T>
bool Queue<T>::IsEmpty() const
{
    return this->head == nullptr;
}

template <typename T>
size_t Queue<T>::Size() const
{
    return this->list_size;
}


template <typename T>
void Queue<T>::CopyFrom(const Queue<T>& other)
{
    this->ProtectedCopyFrom(other);
    typename List<T>::Node* temp = this->head;
    while (temp && temp->next)
    {
        temp = temp->next;
    }
    tail = temp;
}


#endif // LIST_H_INCLUDED

