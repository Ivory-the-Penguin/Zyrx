#include "Lexer.hpp"
#include <iostream>

int main() {
  Lexer lexer;

  lexer.GetTokensFromLine("\\\\ This is a comment!");
  lexer.GetTokensFromLine("main <- proc() int {");

  for (auto token : lexer.GetTokens()) {
    token.Output();
  }
  std::cout << '\n';

  return 0;
}