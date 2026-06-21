#pragma once
#include <stdexcept>

template <class T>
class LinkedList
{
private:
    struct Node
    {
        T data;
        Node* next;

        Node(const T& data, Node* next = nullptr)
        {
            this->data = data;
            this->next = next;
        }
    };

    Node* head;
    Node* tail;
    int length;

public:
    LinkedList()
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    LinkedList(T* items, int count)
    {
        head = nullptr;
        tail = nullptr;
        length = 0;

        for (int i = 0; i < count; i++)
        {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& other)
    {
        head = nullptr;
        tail = nullptr;
        length = 0;

        Node* current = other.head;

        while (current != nullptr)
        {
            Append(current->data);
            current = current->next;
        }

    }

    ~LinkedList()
    {
        Node* current = head;

        while (current != nullptr)
        {
            Node* next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
        tail = nullptr;
    }

    T GetFirst() const
    {
        if (head == nullptr)
        {
            throw std::out_of_range("List is empty");
        }

        return head->data;
    }

    T GetLast() const
    {
        if (head == nullptr)
        {
            throw std::out_of_range("List is empty");
        }

        return tail->data;
    }

    T Get(int index) const
    {
        if (index < 0 || index >= length)
        {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head;

        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }

        return current->data;
    }

    int GetLength() const
    {
        return length;
    }

    void Set(int index, const T& value)
    {
        if (index < 0 || index >= length)
        {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head;

        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }

        current->data = value;
    }

    void Append(const T& item)
    {
        Node* newNode = new Node(item);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }

        length++;
    }

    void Prepend(const T& item)
    {
        Node* newNode = new Node(item, head);

        head = newNode;
        if (tail == nullptr)
        {
            tail = newNode;
        }
        
        length++;
    }

    void InsertAt(const T& item, int index)
    {
        if (index < 0 || index > length)
        {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0)
        {
            Prepend(item);
            return;
        }

        if (index == length)
        {
            Append(item);
            return;
        }

        Node* current = head;

        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }

        Node* newNode = new Node(item, current->next);
        current->next = newNode;

        length++;
    }

    void RemoveAt(int index)
    {
        if (index < 0 || index >= length)
        {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0)
        {
            Node* temp = head;
            head = head->next;

            if (head == nullptr)
            {
                tail = nullptr; 
            }
            delete temp;
            length--;
            return;
        }

        Node* current = head;

        for (int i = 0; i < index - 1; i++)
        {
            current = current->next;
        }

        Node* toDelete = current->next;
        current->next = toDelete->next;

        if (toDelete == tail)
        {
            tail = current;
        }

        delete toDelete;
        length--;
    }
};