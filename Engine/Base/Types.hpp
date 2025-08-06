#pragma once

#include <expected>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

// -- Numeric Types --
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

// -- String Types --
using String = std::string;
using StringView = std::string_view;

// -- Container Types --
template <typename T, std::size_t N>
using Array = std::array<T, N>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Span = std::span<T>;

template <typename K, typename V>
using Map = std::map<K, V>;

template <typename K, typename V>
using UnorderedMap = std::unordered_map<K, V>;

template <typename T, typename U>
using Pair = std::pair<T, U>;

template <typename T, typename U>
Pair<T, U> MakePair(T &&first, U &&second) {
  return std::make_pair<T, U>(std::forward<T>(first), std::forward<U>(second));
}

// -- Optional Type --
template <typename T>
using Optional = std::optional<T>;

// -- Expected Type --
template <typename T, typename E>
using Expected = std::expected<T, E>;

// -- Function Types --
template <typename... Args>
using Function = std::function<Args...>;

// -- File System Types --
namespace fs = std::filesystem;
using Path = fs::path;

// -- Memory Types --
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
Scope<T> MakeScope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Ref<T> MakeRef(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using WeakRef = std::weak_ptr<T>;

// -- Engine Types --
using Id = u32;