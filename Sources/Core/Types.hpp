#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// -- Numeric Types --
using i8 = signed char;
using i16 = short;
using i32 = int;
using i64 = long long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

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

template <typename K, typename V>
using Map = std::map<K, V>;

template <typename K, typename V>
using UnorderedMap = std::unordered_map<K, V>;

template <typename T, typename U>
using Pair = std::pair<T, U>;

// -- Optional Type --
template <typename T>
using Optional = std::optional<T>;

// -- File System Types --
namespace fs = std::filesystem;
using Path = fs::path;
