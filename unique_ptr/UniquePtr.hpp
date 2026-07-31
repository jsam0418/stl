#pragma once

#include <concepts>
#include <memory>

namespace jstl {
template <typename T, typename Deleter = std::default_delete<T>>
  requires std::invocable<Deleter, T*>
class UniquePtr {
 public:
  explicit UniquePtr(T* _ptr, Deleter&& _deleter = Deleter{})
      : ptr{_ptr}, deleter{std::forward<Deleter>(_deleter)} {}
  UniquePtr() = default;
  ~UniquePtr() {
    // RAII - When this goes out of scope, clean up the ptr
    if (ptr) {
      deleter(ptr);
    }
  }

  // Open Q: Why is noexcept needed?
  // Answer: We need this to be noexcept because if an exception is thrown
  // mid-move, we have no idea what state both the moved from and the moved to
  // objects are in. This would probably result in a memory leak since the
  // resource can't be cleaned up.
  UniquePtr(UniquePtr&& movedFromPtr) noexcept {
    ptr = movedFromPtr.ptr;
    movedFromPtr.ptr = nullptr;
    deleter = std::move(movedFromPtr.deleter);
  }
  UniquePtr& operator=(UniquePtr&& movedFromPtr) noexcept {
    /*
if (ptr) {
  delete ptr;
}
ptr = movedFromPtr.ptr;
movedFromPtr.ptr = nullptr;
return *this;
    */

    // In order to solve the self assignment issue, we could do a few things.
    // 1. We could put the ptr in a temporary variable, clear the outside
    // object, then put our ptr back
    // 2. We could swap ptr values. If objects are the same, no-op. If objects
    // are different, the moved
    //    from object now cleans up our resource when it expires.

    /*
        std::swap(movedFromPtr.ptr, ptr);
      */
    reset(movedFromPtr.release());
    deleter = std::move(movedFromPtr.deleter);
    return *this;
  }

  // Explicitly deleting these even though the compiler does that when you
  // define any of the move functions. We can't copy a unique ptr because its
  // the singular owner of the resource.
  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  [[nodiscard]] explicit operator bool() const noexcept {
    return ptr != nullptr;
  }
  [[nodiscard]] T& operator*() const noexcept { return *ptr; }
  [[nodiscard]] T* operator->() const noexcept {
    return ptr;
  }  // Open Q: Compiler applies -> recursively?

  [[nodiscard]] T* release() noexcept {
    T* ret = ptr;
    ptr = nullptr;
    return ret;
  }

  void reset(T* newPtr = nullptr) noexcept {
    T* toDelete = ptr;
    ptr = newPtr;
    if (toDelete) {
      deleter(toDelete);
    }
  }

  [[nodiscard]] T* get() const noexcept { return ptr; }
  [[nodiscard]] const Deleter& getDeleter() const noexcept { return deleter; }

 private:
  T* ptr{nullptr};
  [[no_unique_address]] Deleter deleter;

  // Open Q: How does std::make_unique interface with this class?
  // Open Q: Contextual Conversion and safe bool idiom pre C++11.
};

// Deviation: The standard does not allow make_unique to take a custom deleter.
// You must operate on the unique_ptr directly. Make unique's main purpose is to
// remove the "every call to new must have a delete" breakage that unique_ptr
// would otherwise add. However, a custom deleter is most likely going to be
// used by different resource types, like a file or socket. The same new/delete
// paradigm doesn't exists there.
// TODO: Figure out how to pass a stateful Deleter to our unique ptr.
template <typename T, typename Deleter = std::default_delete<T>,
          typename... Args>
auto MakeUnique(Args&&... args) {
  // Open Q: Why does std::forward<Args&&> and std::forward<Args> produce the
  // same results? Hint: reference collapsing.
  return jstl::UniquePtr<T, Deleter>(new T(std::forward<Args>(args)...));
}
}  // namespace jstl
