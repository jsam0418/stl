#include "UniquePtr.hpp"
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

int main(int, char **) {
  struct Person {
    Person(std::string _name, uint64_t _age, std::vector<std::string> _kids)
        : name{_name}, age{_age}, kids{_kids} {};
    std::string name;
    uint64_t age;
    std::vector<std::string> kids;
  };

  // Error: I hit an error code "No matching constructor for initialization of
  // 'jstl::UniquePtr<Person>'". The issue was the vector wasn't explicitly
  // declared.
  //  Before: jstl::UniquePtr<Person> uptr(std::string{"Josiah Sam"}, 26u,
  //  {"Grace", "Boaz", "Joseph", "Thomas"});
  jstl::UniquePtr<Person> movedFromPtr(
      std::string{"Josiah Sam"}, 26u,
      std::vector<std::string>{"Grace", "Boaz", "Joseph", "Thomas"});
  assert(movedFromPtr);
  jstl::UniquePtr<Person> movedToPtr(std::move(movedFromPtr));
  assert(!movedFromPtr);
  assert(movedToPtr);
  jstl::UniquePtr<Person> assignedPtr;
  assert(!movedFromPtr);
  assert(movedToPtr);
  assert(!assignedPtr);
  assignedPtr = std::move(movedToPtr);
  assert(!movedFromPtr);
  assert(!movedToPtr);
  assert(assignedPtr);

  // Test for memory leaks
  jstl::UniquePtr<Person> assignedWithValue(
      std::string{"Albert Einstein"}, 147u,
      std::vector<std::string>{"Lieserl", "Hans Albert", "Edmund"});
  assert(assignedWithValue);
  // asan will catch any memory leaks here
  assignedWithValue = std::move(assignedPtr);
  assert(assignedPtr); // This still evaluates to true now because of our swap.
                       // Maybe that is a bug?
  assert(assignedWithValue);
  // Self Move
  assignedWithValue = std::move(assignedWithValue);
  assert(assignedWithValue);
}
