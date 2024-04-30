#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include "Parser.h"

/**
 * Check for an exit string in the input.
 * Exits the program.
 */
void check_exit(std::string val) {
  if (val == "quit" || val == "exit") {
    std::cout << "Exiting...\n";
    exit(0);
  }
}

/**
 * Main loop.
 */
int main(int argc, char* argv[]){
  if (argc <= 1) {
    std::cout << "Please enter an expression.\n";
    exit(1);
  } else if (argc > 2) {
    std::cout << "Please enter only one expression.\n";
    exit(1);
  }

  Parser parser;
  std::string input = argv[1];

  try {
    check_exit(input);
    parser.tokens = tokenize(input);
    bool val = parser.parse();
    const std::string final_val = (val) ? "True" : "False";
    std::cout << final_val << "\n";

  } catch (const char *error) {
    std::cout << error << "\n";
  } catch (const std::out_of_range error) {
    std::cout << "Encountered mismatched parenthesis.\n";
  }

  return 0;
}
