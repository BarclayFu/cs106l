#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(std::string& source) {
  auto spaces = find_all(source.begin(), source.end(), ::isspace);

  std::set<Token> tokens;

  std::transform(
      spaces.begin(), spaces.end() - 1,
      spaces.begin() + 1,
      std::inserter(tokens, tokens.end()),
      [&source](auto it1, auto it2) { return Token{source, it1, it2}; });

  std::erase_if(tokens, [](const Token& t) { return t.content.empty(); });

  return tokens;
}

std::set<Misspelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  namespace rv = std::ranges::views;

  auto view = source
      | rv::filter([&dictionary](const Token& token) {
          return !dictionary.contains(token.content);
        })
      | rv::transform([&dictionary](const Token& token) {
          auto suggestions_view = dictionary
              | rv::filter([&token](const std::string& word) {
                  return levenshtein(token.content, word) == 1;
                });
          std::set<std::string> suggestions(
              suggestions_view.begin(), suggestions_view.end());
          return Misspelling{token, suggestions};
        })
      | rv::filter([](const Misspelling& m) {
          return !m.suggestions.empty();
        });

  return std::set<Misspelling>(view.begin(), view.end());
};

/* Helper methods */

#include "utils.cpp"