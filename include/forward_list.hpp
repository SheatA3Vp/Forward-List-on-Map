#pragma once

#include <fmt/core.h>

#include <cstdlib>
#include <functional>
#include <iterator>
#include <utility>

#include "exceptions.hpp"

template <typename T>
class ForwardList {
private:
    class Node {
        friend class ForwardListIterator;
        friend class ForwardList;

        Node* next_;
        T value_;

        explicit Node(const T& value) : next_(nullptr), value_(value) {
        }
    };

public:
    class ForwardListIterator {
    public:
        // NOLINTNEXTLINE
        using value_type = T;
        // NOLINTNEXTLINE
        using reference_type = value_type&;
        // NOLINTNEXTLINE
        using pointer_type = value_type*;
        // NOLINTNEXTLINE
        using difference_type = std::ptrdiff_t;
        // NOLINTNEXTLINE
        using iterator_category = std::forward_iterator_tag;

        ForwardListIterator() : current_(nullptr) {
        }

        inline bool operator==(const ForwardListIterator& other) const {
            return current_ == other.current_;
        }

        inline bool operator!=(const ForwardListIterator& other) const {
            return current_ != other.current_;
        }

        inline reference_type operator*() const {
            return current_->value_;
        }

        ForwardListIterator& operator++() {
            current_ = current_->next_;
            return *this;
        }

        ForwardListIterator operator++(int) {
            ForwardListIterator temp = *this;
            ++(*this);
            return temp;
        }

        inline pointer_type operator->() const {
            return &(current_->value_);
        }

    private:
        explicit ForwardListIterator(Node* node) : current_(node) {
        }

    private:
        Node* current_;
        friend class ForwardList;
    };

public:
    ForwardList() : head_(nullptr), size_(0) {
    }

    explicit ForwardList(size_t sz) : ForwardList() {
        for (size_t i = 0; i < sz; ++i) {
            PushFront(T());
        }
    }

    ForwardList(const std::initializer_list<T>& values) : ForwardList() {
        for (auto it = values.end(); it != values.begin();) {
            PushFront(*--it);
        }
    }

    ForwardList(const ForwardList& other) : ForwardList() {
        Node* node = other.head_;
        Node* prev = nullptr;
        while (node != nullptr) {
            Node* new_node = new Node(node->value_);
            if (prev) {
                prev->next_ = new_node;
            } else {
                head_ = new_node;
            }
            prev = new_node;
            node = node->next_;
        }
        size_ = other.size_;
    }

    ForwardList& operator=(const ForwardList& other) {
        if (this == &other) {
            return *this;
        }
        Clear();
        Node* node = other.head_;
        Node* prev = nullptr;
        while (node != nullptr) {
            Node* new_node = new Node(node->value_);
            if (prev) {
                prev->next_ = new_node;
            } else {
                head_ = new_node;
            }
            prev = new_node;
            node = node->next_;
        }
        size_ = other.size_;
        return *this;
    }

    ForwardListIterator Begin() const noexcept {
        return ForwardListIterator(head_);
    }

    ForwardListIterator End() const noexcept {
        return ForwardListIterator(nullptr);
    }

    inline T& Front() const {
        if (IsEmpty()) {
            throw ListIsEmptyException("ForwardList is empty");
        }
        return head_->value_;
    }

    inline bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    inline size_t Size() const noexcept {
        return size_;
    }

    void Swap(ForwardList& other) {
        Node* temp_head = head_;
        size_t temp_size = size_;

        head_ = other.head_;
        size_ = other.size_;

        other.head_ = temp_head;
        other.size_ = temp_size;
    }

    void EraseAfter(ForwardListIterator pos) {
        if (IsEmpty()) {
            throw ListIsEmptyException("ForwardList is empty");
        }
        Node* node = pos.current_;
        if (node && node->next_) {
            Node* to_delete = node->next_;
            node->next_ = to_delete->next_;
            delete to_delete;
            --size_;
        }
    }

    void InsertAfter(ForwardListIterator pos, const T& value) {
        Node* node = new Node(value);
        Node* cur = pos.current_;
        if (cur) {
            node->next_ = cur->next_;
            cur->next_ = node;
        } else {
            head_ = node;
        }
        ++size_;
    }

    ForwardListIterator Find(const T& value) const {
        Node* node = head_;
        while (node != nullptr) {
            if (node->value_ == value) {
                return ForwardListIterator(node);
            }
            node = node->next_;
        }
        return End();
    }

    void Clear() noexcept {
        while (!IsEmpty()) {
            PopFront();
        }
    }

    void PushFront(const T& value) {
        Node* node = new Node(value);
        node->next_ = head_;
        head_ = node;
        ++size_;
    }

    void PopFront() {
        if (IsEmpty()) {
            throw ListIsEmptyException("ForwardList is empty");
        }
        Node* node = head_;
        head_ = head_->next_;
        delete node;
        --size_;
    }

    ~ForwardList() {
        Clear();
    }

private:
    Node* head_;
    size_t size_;
};

namespace std {
template <typename T>
void swap(ForwardList<T>& a, ForwardList<T>& b) {  // NOLINT
    a.Swap(b);
}
}  // namespace std