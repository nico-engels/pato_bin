          _
       __(.)<
     __\___)__
      0   1 1
       0 0 1

# C++ Pato_bin
## Utilitário Conversor Binário de/para Tipos Integrais

* Licença MIT
* C++11
* Header-only
* constexpr

## Referência

A biblioteca define dois conjuntos de funções sobrecarregadas através de templates especializados. Eles
dependem em um tipo auxiliar para representar os dados binários conceitualmente:

```c++
using byte_t = unsigned char;
```

O alias é apenas para coveniência, enquanto C++ não possui um tipo padrão para representar
dados binários, ao usar o tipo é mais simples de imprimir e expressar. Usando o tipo como retorno
é a função da biblioteca que converte tipos integrais para um array de dados binários:

```c++
template <class T, class E = native_tag_t>
constexpr std::array<byte_t, sizeof(T)> to_bin(const T&, E = {});
```

E outra para converter um array de dados binários em um tipo integral:

```c++
template <class T, class E = native_tag_t>
constexpr T bin_to(const std::array<byte_t, sizeof(T)>&, E = {});
```

As funções genéricas são especializadas para todos os tipos definidos no
`cstdint`: `int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `int32_t`, `uint32_t`,
`int64_t` and `uint64_t`.

## Casos de Uso

### Exemplo completo - Serialização de Strings:

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

    // Converte o tamanho na endianess da máquina
    for (auto b : to_bin<int8_t>(a.size()))
      stream.push_back(b);

    // Adiciona o texto da string
    for (auto ch : a)
      stream.push_back(ch);

    // Imprime o resultado
    cout << hex << setfill('0') << uppercase;
    for (auto b : stream)
      cout << setw(2) << +b << ' ';
    cout << dec << '\n';

    for (auto c : stream)
      cout << ' ' << static_cast<char>((isprint(c) ? c : ' ')) << ' ';
}
```

Imprime:
```
fixed-length str!!!

13 66 69 78 65 64 2D 6C 65 6E 67 74 68 20 73 74 72 21 21 21
    f  i  x  e  d  -  l  e  n  g  t  h     s  t  r  !  !  !
```

Alterando o `to_bin<int8_t>(a.size())` para `to_bin<int64_t>(a.size())` imprime
(little-endian nesse exemplo):

```
fixed-length str!!!

13 00 00 00 00 00 00 00 66 69 78 65 64 2D 6C 65 6E 67 74 68 20 73 74 72 21 21 21
                         f  i  x  e  d  -  l  e  n  g  t  h     s  t  r  !  !  !
```

### Endianness Explícita

```c++
    // ...
    int32_t i = 0x1A2B3C4D;                  // 0x1A'2B'3C'4D

    auto binl_i = to_bin(i, endian::little); // array<byte_t, 4> { 0x4D, 0x3C, 0x2B, 0x1A };
    auto binb_i = to_bin(i, endian::big);    // array<byte_t, 4> { 0x1A, 0x2B, 0x3C, 0x4D };

    // ...

    // Reverter
    int32_t rl_i = bin_to<int32_t>(binl_i, endian::little); // rl_i == i
    int32_t rb_i = bin_to<int32_t>(binb_i, endian::big);    // rb_i == i
    // ...
```