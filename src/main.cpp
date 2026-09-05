#include "Lexer.hpp"
#include <iostream>

int main() {
  Lexer lexer;

  lexer.GetTokensFromLine("\\\\ This is a comment!");
  lexer.GetTokensFromLine("main <- proc() int {");
  lexer.GetTokensFromLine("\treturn 0");
  lexer.GetTokensFromLine("}");

  for (auto token : lexer.GetTokens()) {
    token.Output();
  }
  std::cout << '\n';

  return 0;
}