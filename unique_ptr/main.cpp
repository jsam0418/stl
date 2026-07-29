
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

  // Error: I hit an error code "No matching constructor for initialization of
  // 'jstl::UniquePtr<Person>'". The issue was the vector wasn't explicitly
  // declared.
  //  Before: jstl::UniquePtr<Person> uptr(std::string{"Josiah Sam"}, 26u,
  //  {"Grace", "Boaz", "Joseph", "Thomas"});
  struct CustomDeleter {
    void operator()(Person* person) {
      std::println("Removing person: {}", person->name);
      delete person;
    }
  };

  jstl::UniquePtr<Person, CustomDeleter> uniquePtr =
      jstl::MakeUnique<Person, CustomDeleter>(
          std::string{"Josiah Sam"}, 26u,
          std::vector<std::string>{"Grace", "Boaz", "Joseph", "Thomas"});
  assert(uniquePtr);
}
