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
  // TODO: Implement these
  // UniquePtr(UniquePtr &&) noexcept;
  // UniquePtr &operator=(UniquePtr &&) noexcept;

  // Explicitly deleting these even though the compiler does that when you
  // define any of the move functions. We can't copy a unique ptr because its
  // the singular owner of the resource.
  UniquePtr(const UniquePtr &) = delete;
  UniquePtr &operator=(const UniquePtr &) = delete;

  explicit operator bool() { return ptr != nullptr; }
  T &operator*() { return *ptr; }
  T *operator->() { return ptr; } // Compiler applies -> recursively?

private:
  T *ptr{nullptr};

  // Open Q: How does std::make_unique interface with this class?
  // Open Q: Contextual Conversion and safe bool idiom pre C++11.
};
} // namespace jstl
