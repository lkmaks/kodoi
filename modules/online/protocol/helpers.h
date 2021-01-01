#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <sstream>
#include <cereal/archives/binary.hpp>


#define SERIALIZE(...)         \
  template <typename Archive>  \
  void serialize(Archive& a) { \
    a(__VA_ARGS__);            \
  };



struct Archives {
  using InputArchive = cereal::BinaryInputArchive;
  using OutputArchive = cereal::BinaryOutputArchive;
};


template <typename T>
std::string Serialize(const T& object) {
  std::stringstream output;
  {
    Archives::OutputArchive oarchive(output);
    oarchive(object);
  }  // archive goes out of scope, ensuring all contents are flushed

  auto str = output.str();
  return str;
}


template <typename T>
T Deserialize(const std::string& bytes) {
  // GlobalHeapScope g;

  T object;

  std::stringstream input(bytes);
  {
    Archives::InputArchive iarchive(input);
    iarchive(object);  // Read the data from the archive
  }

  return object;
}


#endif // HELPERS_H
