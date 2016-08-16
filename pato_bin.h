#ifndef PATO_BIN_H
#define PATO_BIN_H
/* 
 The MIT License (MIT)

 Copyright (c) 2016 Nicolas Daniel Engels

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/
#include <array>

using byte_t = unsigned char;
class big_tag_t { };
class little_tag_t { };

// 15/08/2016
// Name and idea of detection get from howardhinnant.github.io/endian.html 
struct endian
{
  static constexpr big_tag_t big { };
  static constexpr little_tag_t little { };  
};

// Endian detection
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
using native_tag_t = little_tag_t;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
using native_tag_t = big_tag_t;  
#else
#error "mixed endian"  
#endif

// General template declaration (no definition)
template <class T, class E = native_tag_t>
constexpr std::array<byte_t, sizeof(T)> to_bin(const T&, E = {});

template <class T, class E = native_tag_t>
constexpr T bin_to(const std::array<byte_t, sizeof(T)>&, E = {});  

// Specialization for all signed/unsigned fixed-width integral types
template <>
constexpr std::array<byte_t, 1> to_bin(const int8_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i) };
}  

template <>
constexpr std::array<byte_t, 1> to_bin(const int8_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i) };
}

template <>
constexpr std::array<byte_t, 1> to_bin(const uint8_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i) };
}  

template <>
constexpr std::array<byte_t, 1> to_bin(const uint8_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i) };
}

template <>
constexpr std::array<byte_t, 2> to_bin(const int16_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i), static_cast<byte_t>(i >> 8) };
}  

template <>
constexpr std::array<byte_t, 2> to_bin(const int16_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i >> 8),  static_cast<byte_t>(i) };
}

template <>
constexpr std::array<byte_t, 2> to_bin(const uint16_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i), static_cast<byte_t>(i >> 8) };
}  

template <>
constexpr std::array<byte_t, 2> to_bin(const uint16_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i >> 8),  static_cast<byte_t>(i) };
}

template <>
constexpr std::array<byte_t, 4> to_bin(const int32_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i),       static_cast<byte_t>(i >> 8), 
             static_cast<byte_t>(i >> 16), static_cast<byte_t>(i >> 24) };
}  

template <>
constexpr std::array<byte_t, 4> to_bin(const int32_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i >> 24), static_cast<byte_t>(i >> 16), 
             static_cast<byte_t>(i >> 8),  static_cast<byte_t>(i) };
}

template <>
constexpr std::array<byte_t, 4> to_bin(const uint32_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i),       static_cast<byte_t>(i >> 8), 
             static_cast<byte_t>(i >> 16), static_cast<byte_t>(i >> 24) };
}  

template <>
constexpr std::array<byte_t, 4> to_bin(const uint32_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i >> 24), static_cast<byte_t>(i >> 16), 
             static_cast<byte_t>(i >> 8),  static_cast<byte_t>(i) };
}

template <>
constexpr std::array<byte_t, 8> to_bin(const int64_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i),       static_cast<byte_t>(i >> 8), 
             static_cast<byte_t>(i >> 16), static_cast<byte_t>(i >> 24),
             static_cast<byte_t>(i >> 32), static_cast<byte_t>(i >> 40),               
             static_cast<byte_t>(i >> 48), static_cast<byte_t>(i >> 56) };
}  

template <>
constexpr std::array<byte_t, 8> to_bin(const int64_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i >> 56), static_cast<byte_t>(i >> 48),
             static_cast<byte_t>(i >> 40), static_cast<byte_t>(i >> 32),
             static_cast<byte_t>(i >> 24), static_cast<byte_t>(i >> 16), 
             static_cast<byte_t>(i >> 8),  static_cast<byte_t>(i) };
}

template <>
constexpr std::array<byte_t, 8> to_bin(const uint64_t& i, little_tag_t)
{
    return { static_cast<byte_t>(i),       static_cast<byte_t>(i >> 8), 
             static_cast<byte_t>(i >> 16), static_cast<byte_t>(i >> 24),
             static_cast<byte_t>(i >> 32), static_cast<byte_t>(i >> 40),               
             static_cast<byte_t>(i >> 48), static_cast<byte_t>(i >> 56) };
}  

template <>
constexpr std::array<byte_t, 8> to_bin(const uint64_t& i, big_tag_t)
{
    return { static_cast<byte_t>(i >> 56), static_cast<byte_t>(i >> 48),
             static_cast<byte_t>(i >> 40), static_cast<byte_t>(i >> 32),
             static_cast<byte_t>(i >> 24), static_cast<byte_t>(i >> 16), 
             static_cast<byte_t>(i >> 8),  static_cast<byte_t>(i) };
}

template <>
constexpr int8_t bin_to(const std::array<byte_t, 1>& a, little_tag_t)
{
    return { static_cast<int8_t>(a[0]) };
}


template <>
constexpr int8_t bin_to(const std::array<byte_t, 1>& a, big_tag_t)
{
    return { static_cast<int8_t>(a[0]) };
}

template <>
constexpr uint8_t bin_to(const std::array<byte_t, 1>& a, little_tag_t)
{
    return { static_cast<uint8_t>(a[0]) };
}


template <>
constexpr uint8_t bin_to(const std::array<byte_t, 1>& a, big_tag_t)
{
    return { static_cast<uint8_t>(a[0]) };
}

template <>
constexpr int16_t bin_to(const std::array<byte_t, 2>& a, little_tag_t)
{
    return static_cast<int16_t>(a[0] + (a[1] << 8));
}


template <>
constexpr int16_t bin_to(const std::array<byte_t, 2>& a, big_tag_t)
{
    return static_cast<int16_t>(a[1] + (a[0] << 8));
}

template <>
constexpr uint16_t bin_to(const std::array<byte_t, 2>& a, little_tag_t)
{
    return static_cast<uint16_t>(a[0] + (a[1] << 8));
}

template <>
constexpr uint16_t bin_to(const std::array<byte_t, 2>& a, big_tag_t)
{
    return static_cast<uint16_t>(a[1] + (a[0] << 8));
}

template <>
constexpr int32_t bin_to(const std::array<byte_t, 4>& a, little_tag_t)
{
    return static_cast<int32_t>( a[0] + (a[1] << 8) + (a[2] << 16) + 
                                (a[3] << 24));
}


template <>
constexpr int32_t bin_to(const std::array<byte_t, 4>& a, big_tag_t)
{
    return static_cast<int32_t>( a[3] + (a[2] << 8) + (a[1] << 16) + 
                                (a[0] << 24));
}

template <>
constexpr uint32_t bin_to(const std::array<byte_t, 4>& a, little_tag_t)
{
    return static_cast<uint32_t>( a[0] + (a[1] << 8) + (a[2] << 16) + 
                                 (a[3] << 24));
}

template <>
constexpr uint32_t bin_to(const std::array<byte_t, 4>& a, big_tag_t)
{
    return static_cast<uint32_t>( a[3] + (a[2] << 8) + (a[1] << 16) + 
                                 (a[0] << 24));
}

template <>
constexpr int64_t bin_to(const std::array<byte_t, 8>& a, little_tag_t)
{
    return {  a[0] + (a[1] << 8) + (a[2] << 16) + (a[3] << 24) +
             (int64_t { a[4] } << 32) + (int64_t { a[5] } << 40) + 
             (int64_t { a[6] } << 48) + (int64_t { a[7] } << 56) };
}

template <>
constexpr int64_t bin_to(const std::array<byte_t, 8>& a, big_tag_t)
{
    return {  a[7] + (a[6] << 8) + (a[5] << 16) + (a[4] << 24) +
             (int64_t { a[3] } << 32) + (int64_t { a[2] } << 40) + 
             (int64_t { a[1] } << 48) + (int64_t { a[0] } << 56) };
}

template <>
constexpr uint64_t bin_to(const std::array<byte_t, 8>& a, little_tag_t)
{
    return {  a[0] + (a[1] << 8) + (a[2] << 16) + (a[3] << 24) +
             (uint64_t { a[4] } << 32) + (uint64_t { a[5] } << 40) + 
             (uint64_t { a[6] } << 48) + (uint64_t { a[7] } << 56) };
}

template <>
constexpr uint64_t bin_to(const std::array<byte_t, 8>& a, big_tag_t)
{
    return {  a[7] + (a[6] << 8) + (a[5] << 16) + (a[4] << 24) +
             (uint64_t { a[3] } << 32) + (uint64_t { a[2] } << 40) + 
             (uint64_t { a[1] } << 48) + (uint64_t { a[0] } << 56) };
}

#endif
// PATO_BIN_H