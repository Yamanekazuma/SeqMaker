#pragma once

#include <array>
#include <stdexcept>
#include <utility>
#include <vector>

namespace seq {

template <typename T>
class NGram {
 public:
  NGram(const std::vector<T>& data, std::size_t n);

  NGram(NGram&&) = delete;
  NGram(const NGram&) = delete;
  NGram operator=(NGram&&) = delete;
  NGram operator=(const NGram&) = delete;

  inline explicit operator std::string() const noexcept { return string(); }
  const std::string string() const noexcept;

 private:
  std::size_t n_;
  std::vector<std::vector<T>> ngram_;
};

template <typename T>
NGram<T>::NGram(const std::vector<T>& data, std::size_t n) : n_{n}, ngram_{} {
  if (data.size() < n) {
    throw std::runtime_error("入力データが短すぎます．");
  }

  for (std::size_t i = 0; i < data.size() - (n - 1); ++i) {
    std::vector<T> vec{};
    vec.reserve(n);
    for (std::size_t j = 0; j < n; ++j) {
      vec.emplace_back(data[i + j]);
    }
    ngram_.emplace_back(std::move(vec));
  }
}

template <typename T>
const std::string NGram<T>::string() const noexcept {
  std::string str{"{\n"};
  for (const auto& vec : ngram_) {
    str += '(';
    for (std::size_t i = 0; i < n_ - 1; ++i) {
      str += static_cast<std::string>(vec[i]);
      str += ", ";
    }
    str += static_cast<std::string>(vec[n_ - 1]);
    str += ")\n";
  }
  str += "}";

  return str;
}

}  // namespace seq
