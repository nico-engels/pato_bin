          _
       __(.)<
     __\___)__
      0   1 1
       0 0 1

# C++ Pato_bin
## Binary Utility Converter from/to Integral Types

UNDER CONSTRUCTION!!!

* MIT-licensed
* C++11
* Header-only
* constexpr

## Motivation

In C++, when we need to do some task that envolves some binary representation
of types with more than one byte, is necessary the use of bitwise operator for
convert the data in some endianess.
One developer, for example, could be writing some software for TLS communication:

```c++
    // ...
    vector<unsigned char> stream { 0x16, 0x03, 0x01 }; // Protocolo TLSv1
                                                       // Header frame
                                                       // 0x16 - Handshake
                                                       // 0x0301 - SSL version (TLS 1.0)

    vector<unsigned char> handshake_packet = make_client_hello();

    // After header will have 2 big-endian bytes with the size of the frame
    int16_t packet_size = static_cast<int16_t>(handshake_packet.size());
    stream.push_back(packet_size >> 8);
    stream.push_back(packet_size & 0x00FF);

    // Merge the data and send over network
    stream.insert(stream.end(), handshake_packet.begin(), handshake_packet.end());
    send_over_wire(stream);
    // ...
```

It is not clear, without the comments, what is been doing in the convertion to the
2 bytes of packet size. What is the endianess? What is the implict convertions of
bitwise operations make?

Thinking in this need of reability and easy of development was made the lib
witch permits made the convertion explicit, without losing performace.

```c++
    // ...
    // After header will have 2 big-endian bytes with the size of the frame
    for (auto b : to_bin<int16_t>(handshake_packet.size(), endian::big))
      stream.push_back(b);
    // ...
```

## Reference

The library defines two set of overload functions throught template
specializations. One converts integral types to a array of binary data:

```c++
template <class T, class E = native_tag_t>
constexpr std::array<byte_t, sizeof(T)> to_bin(const T&, E = {});
```

And another to convert a array of binary data to a integral type:

```c++
template <class T, class E = native_tag_t>
constexpr T bin_to(const std::array<byte_t, sizeof(T)>&, E = {});
```

The functions template are specialized for all integral types defined in
`cstdint`: `int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `int32_t`, `uint32_t`,
`int64_t` and `uint64_t`.

## Use Cases

### Complete example - String Serialization:

```c++
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "pato_bin.h"
using namespace std;
using namespace pato_bin;

int main()
{
    string a { "fixed-length strings!!!" };
    cout << a << "\n\n";

    vector<byte_t> stream;

    // Write size in the machine endian
    for (auto b : to_bin<int8_t>(a.size()))
      stream.push_back(b);

    // Write string
    for (auto ch : a)
      stream.push_back(ch);

    // Output the result
    cout << hex << setfill('0') << uppercase;
    for (auto b : stream)
      cout << setw(2) << +b << ' ';
    cout << dec << '\n';

    for (auto c : stream)
      cout << ' ' << static_cast<char>((isprint(c) ? c : ' ')) << ' ';
}
```

Prints:
```
fixed-length str!!!

13 66 69 78 65 64 2D 6C 65 6E 67 74 68 20 73 74 72 21 21 21
    f  i  x  e  d  -  l  e  n  g  t  h     s  t  r  !  !  !
```

Changing the `to_bin<int8_t>(a.size())` to `to_bin<int64_t>(a.size())` prints
(little-endian in this example):

```
fixed-length str!!!

13 00 00 00 00 00 00 00 66 69 78 65 64 2D 6C 65 6E 67 74 68 20 73 74 72 21 21 21
                         f  i  x  e  d  -  l  e  n  g  t  h     s  t  r  !  !  !
```

### Explicit Endianness

```c++
    // ...
    int32_t i = 0x1A2B3C4D;                  // 0x1A'2B'3C'4D

    auto binl_i = to_bin(i, endian::little); // array<byte_t, 4> { 0x4D, 0x3C, 0x2B, 0x1A };
    auto binb_i = to_bin(i, endian::big);    // array<byte_t, 4> { 0x1A, 0x2B, 0x3C, 0x4D };

    // ...

    // Revert
    int32_t rl_i = bin_to<int32_t>(binl_i, endian::little); // rl_i == i
    int32_t rb_i = bin_to<int32_t>(binl_i, endian::big);    // rb_i == i
    // ...
```