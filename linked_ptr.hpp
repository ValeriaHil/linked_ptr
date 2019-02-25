#pragma once

#include <cassert>

namespace smart_ptr {

template <typename T>
class linked_ptr;

namespace details {

struct linked_ptr_base {
  constexpr linked_ptr_base() noexcept {
    _left = this;
    _right = this;
  };

  void swap(linked_ptr_base& other) noexcept {
    if (unique() && other.unique()) {
      return;
    }
    if (unique() && !other.unique()) {
      _right = other._right;
      _left = other._left;
      _right->_left = _left->_right = this;
    } else if (!unique() && other.unique()) {
      other._right = _right;
      other._left = _left;
      other._right->_left = other._left->_right = &other;
    } else {
      std::swap(_right, other._right);
      std::swap(_left, other._left);
      _right->_left = _left->_right = this;
      other._right->_left = other._left->_right = &other;
    }
  }
 protected:
  void insert_after(linked_ptr_base& rhs) noexcept {
    _right = rhs._right;
    rhs._right = this;
    _left = &rhs;
    _right->_left = this;
  }

  void erase() noexcept {
    _right->_left = _left;
    _left->_right = _right;
    _right = _left = this;
  }

  bool unique() const noexcept {
    return _left == this && _right == this;
  }

  linked_ptr_base* _left;
  linked_ptr_base* _right;
};

} // namespace details

template <typename T>
class linked_ptr : public details::linked_ptr_base {
  template <typename Y> friend class linked_ptr;

 public:
  using elemnt_type = T;

 private:
  template <typename Y>
  using _Compatible = std::enable_if_t<std::is_convertible<Y*, T*>::value>;
  using _Base = details::linked_ptr_base;

 public:
  constexpr linked_ptr() noexcept = default;
  constexpr explicit linked_ptr(std::nullptr_t) noexcept : linked_ptr() {}
  ~linked_ptr() {
    reset();
  }

  template <typename Y, typename = _Compatible<Y>>
  explicit linked_ptr(Y* ptr) noexcept : _ptr(static_cast<T*>(ptr)) {}

  linked_ptr(const linked_ptr& rhs) noexcept {
    insert_after(const_cast<linked_ptr&>(rhs));
    _ptr = rhs.get();
  }
  template <typename Y, typename = _Compatible<Y>>
  linked_ptr(const linked_ptr<Y>& rhs) noexcept {
    insert_after(const_cast<linked_ptr<Y>&>(rhs));
    _ptr = static_cast<T*>(rhs.get());
  }

  linked_ptr& operator=(const linked_ptr& rhs) noexcept {
    reset(rhs.get());
    insert_after(const_cast<linked_ptr&>(rhs));
    return *this;
  }
  template <typename Y, typename = _Compatible<Y>>
  linked_ptr& operator=(const linked_ptr<Y>& rhs) noexcept {
    reset(rhs.get());
    insert_after(const_cast<linked_ptr<Y>&>(rhs));
    return *this;
  }

  bool unique() const noexcept {
    return _Base::unique();
  }

  void reset() noexcept {
    if (unique()) {
      delete _ptr;
    } else {
      erase();
    }
    _ptr = nullptr;
  }

  template <typename Y, typename = _Compatible<Y>>
  void reset(Y* ptr) {
    if (unique()) {
      delete _ptr;
    } else {
      erase();
    }
    _ptr = ptr;
  }

  T* get() const noexcept {
    return _ptr;
  }

  void swap(linked_ptr& other) noexcept {
    if (_ptr == other._ptr) {
      return;
    }
    _Base::swap(other);
    std::swap(_ptr, other._ptr);
  }

  T& operator*() const noexcept {
    return *_ptr;
  }

  T* operator->() const noexcept {
    return _ptr;
  }

  explicit operator bool() const noexcept {
    return _ptr != nullptr;
  }

 private:
  T* _ptr = nullptr;
};

template <typename T, typename Y>
bool operator==(const linked_ptr<T>& lhs, const linked_ptr<Y>& rhs) noexcept {
  return lhs.get() == rhs.get();
}

template <typename T, typename Y>
bool operator!=(const linked_ptr<T>& lhs, const linked_ptr<Y>& rhs) noexcept {
  return !(lhs == rhs);
}

template <typename T, typename Y>
bool operator<(const linked_ptr<T>& lhs, const linked_ptr<Y>& rhs) noexcept {
  return std::less<>()(
      static_cast<void *>(lhs.get()),
      static_cast<void *>(rhs.get())
  );
}

template <typename T, typename Y>
bool operator>(const linked_ptr<T>& lhs, const linked_ptr<Y>& rhs) noexcept {
  return !(lhs < rhs);
}

template <typename T, typename Y>
bool operator<=(const linked_ptr<T>& lhs, const linked_ptr<Y>& rhs) noexcept {
  return !(lhs > rhs);
}

template <typename T, typename Y>
bool operator>=(const linked_ptr<T>& lhs, const linked_ptr<Y>& rhs) noexcept {
  return !(lhs < rhs);
}

} // namespace smart_ptr
