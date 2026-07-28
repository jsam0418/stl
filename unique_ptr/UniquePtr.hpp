namespace jstl {
template <typename T> class UniquePtr {
public:
  template <typename... Args> UniquePtr(Args... args) { ptr = new T(args...); }

  UniquePtr() = default;
  ~UniquePtr() {
    // RAII - When this goes out of scope, clean up the ptr
    if (ptr) {
      delete ptr;
    }
  }

  // Open Q: Why is noexcept needed?
  // Answer: We need this to be noexcept because if an exception is thrown
  // mid-move, we have no idea what state both the moved from and the moved to
  // objects are in. This would probably result in a memory leak since the
  // resource can't be cleaned up.
  UniquePtr(UniquePtr &&movedFromPtr) noexcept {
    ptr = movedFromPtr.ptr;
    movedFromPtr.ptr = nullptr;
  };
  UniquePtr &operator=(UniquePtr &&movedFromPtr) noexcept {
    ptr = movedFromPtr.ptr;
    movedFromPtr.ptr = nullptr;
    return *this;
  };

  // Explicitly deleting these even though the compiler does that when you
  // define any of the move functions. We can't copy a unique ptr because its
  // the singular owner of the resource.
  UniquePtr(const UniquePtr &) = delete;
  UniquePtr &operator=(const UniquePtr &) = delete;

  explicit operator bool() { return ptr != nullptr; }
  T &operator*() { return *ptr; }
  T *operator->() { return ptr; } // Open Q: Compiler applies -> recursively?

private:
  T *ptr{nullptr};

  // Open Q: How does std::make_unique interface with this class?
  // Open Q: Contextual Conversion and safe bool idiom pre C++11.
};
} // namespace jstl
