          _
       __(.)<
     __\___)__
      0   1 1
       0 0 1

# C++ Pato_bin
## Utilitário Conversor Binário de/para Tipos Integrais

* Licença MIT
* C++14
* Header-only
* constexpr

## Motivação

Em C++, sempre quando necessitamos realizar alguma tarefa que envolva alguma
representação em binário de tipos com mais de um byte, é preciso recorrer a
operações bit-a-bit para converter os dados em determinada ordenação (endianess).
Um desenvolvedor, por exemplo, poderia escrever um software para comunicação TLS:

```c++
    // ...
    vector<unsigned char> stream { 0x16, 0x03, 0x01 }; // Protocolo TLSv1
                                                       // Registro que contêm as mensagens
                                                       // 0x16 - Handshake
                                                       // 0x0301 - SSL versão (TLS 1.0)

    vector<unsigned char> handshake_packet = make_client_hello();

    // Após o cabeçalho tem 2 bytes em big-endian para informar o tamanho do registro
    int16_t packet_size = static_cast<int16_t>(handshake_packet.size());
    stream.push_back(packet_size >> 8);
    stream.push_back(packet_size & 0x00FF);

    // Faz o registro completo e envia os dados via rede
    stream.insert(stream.end(), handshake_packet.begin(), handshake_packet.end());
    send_over_wire(stream);
    // ...
```

Não é claro, sem o comentário, o que está sendo feito na conversão para os 2
bytes do tamanho do registro. Qual a ordenação (endianess)? Quais conversões
as operações shift e e-lógico em bits acarretam?

Pensando nessa necessidade de clareza e comodidade foi desenvolvida a biblioteca
que possibilita deixar o intuido explícito, sem perder otimizações:

```c++
    // ...
    // Após o cabeçalho tem 2 bytes em big-endian para informar o tamanho do registro
    for (auto b : to_bin<int16_t>(handshake_packet.size(), endian::big))
      stream.push_back(b);
    // ...
```

## Referência

A biblioteca define dois conjuntos de funções sobrecarregadas através de templates especializados. Eles
dependem em um tipo auxiliar para representar os dados binários conceitualmente:

```c++
using byte_t = unsigned char;
```

O alias é apenas para coveniência, enquanto C++ não possui um tipo padrão para representar
dados binários, ao usar o tipo é mais simples de imprimir e expressar. Usando o tipo como retorno
é a função que converte tipos integrais para um array de dados binários:

```c++
template <class T>
constexpr std::array<byte_t, sizeof(T)> to_bin(T, endian = endian::native);
```

E outra para converter um array de dados binários em um tipo integral:

```c++
template <class T>
constexpr T bin_to(const std::array<byte_t, sizeof(T)>&, endian = endian::native);
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
    constexpr int32_t i = 0x1A2B3C4D;                  // 0x1A'2B'3C'4D

    constexpr auto binl_i = to_bin(i, endian::little); // array<byte_t, 4> { 0x4D, 0x3C, 0x2B, 0x1A };
    constexpr auto binb_i = to_bin(i, endian::big);    // array<byte_t, 4> { 0x1A, 0x2B, 0x3C, 0x4D };

    // ...

    // Reverter
    constexpr int32_t rl_i = bin_to<int32_t>(binl_i, endian::little); // rl_i == i
    constexpr int32_t rb_i = bin_to<int32_t>(binb_i, endian::big);    // rb_i == i
    // ...
    
    // Tudo é constexpr    
    static_cast(i == rl_i, "igual");
    static_cast(i == rb_i, "igual");
```