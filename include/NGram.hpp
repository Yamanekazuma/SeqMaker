#pragma once

#include <array>
#include <stdexcept>
#include <utility>
#include <vector>

namespace NGram {

template <typename T, std::size_t N>
class NGram {
public:
  NGram(const std::vector<T>& data) : ngram_{} {
    if (data.size() < N) {
      throw std::runtime_error("入力データが短すぎます．");
    }

    for (std::size_t i = 0; i < data.size() - (N - 1); ++i) {
      std::array<T, N> arr{};
      for (std::size_t j = 0; j < N; ++j) {
        arr[j] = data[i + j];
      }
      ngram_.emplace_back(std::move(arr));
    }
  }

  NGram(NGram&&) = delete;
  NGram(const NGram&) = delete;
  NGram operator=(NGram&&) = delete;
  NGram operator=(const NGram&) = delete;

  inline explicit operator std::string() const noexcept { return toString(); }
  inline const std::string toString() const noexcept {
    std::string str{"{\n"};
    for (const auto& arr : ngram_) {
      str += '(';
      for (std::size_t i = 0; i < N - 1; ++i) {
        str += static_cast<std::string>(arr[i]);
        str += ", ";
      }
      str += static_cast<std::string>(arr[N - 1]);
      str += ")\n";
    }
    str += "}\n";

    return str;
  }

private:
  std::vector<std::array<T, N>> ngram_;
};

}  // namespace NGram
