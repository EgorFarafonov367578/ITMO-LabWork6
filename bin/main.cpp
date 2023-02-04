#include "lib/parser.h"
#include <iostream>

int main(int, char**) {
  std::string data = "key = \n\"value\"";


  const auto root = omfl::parse(data);

  omfl::PrintSection(root);
  std::cout << root.valid() << std::endl;
  return 0;
}
