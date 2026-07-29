#include <cassert>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#include "UniquePtr.hpp"

int main(int, char**) {
  struct Person {
    Person(std::string _name, uint64_t _age, std::vector<std::string> _kids)
        : name{_name}, age{_age}, kids{_kids} {};
    std::string name;
    uint64_t age;
    std::vector<std::string> kids;
  };

  struct CustomDeleter {
    void operator()(Person* person) {
      if (!person) {
        return;
      }
      std::println("Removing person: {}", person->name);
      delete person;
    }
  };

  using UniquePtr = jstl::UniquePtr<Person, CustomDeleter>;
  // Error: I hit an error code "No matching constructor for initialization of
  // 'UniquePtr'". The issue was the vector wasn't explicitly
  // declared.
  //  Before: UniquePtr uptr(std::string{"Josiah Sam"}, 26u,
  //  {"Grace", "Boaz", "Joseph", "Thomas"});
  UniquePtr movedFromPtr = jstl::MakeUnique<Person, CustomDeleter>(
      std::string{"Josiah Sam"}, 26u,
      std::vector<std::string>{"Grace", "Boaz", "Joseph", "Thomas"});
  assert(movedFromPtr);
  UniquePtr movedToPtr(std::move(movedFromPtr));
  assert(!movedFromPtr);
  assert(movedToPtr);
  UniquePtr assignedPtr;
  assert(!movedFromPtr);
  assert(movedToPtr);
  assert(!assignedPtr);
  assignedPtr = std::move(movedToPtr);
  assert(!movedFromPtr);
  assert(!movedToPtr);
  assert(assignedPtr);

  // Test for memory leaks
  UniquePtr assignedWithValue = jstl::MakeUnique<Person, CustomDeleter>(
      std::string{"Albert Einstein"}, 147u,
      std::vector<std::string>{"Lieserl", "Hans Albert", "Edmund"});
  assert(assignedWithValue);
  // asan will catch any memory leaks here
  assignedWithValue = std::move(assignedPtr);
  assert(!assignedPtr);
  assert(assignedWithValue);
  // Self Move
  assignedWithValue = std::move(assignedWithValue);
  assert(assignedWithValue);
  assert(sizeof(assignedWithValue) ==
         sizeof(Person*));  // Empty Base Class Optimization
}
