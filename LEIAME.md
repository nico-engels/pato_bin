         _
      __(.)<
    __\___)__  
      0   1 1
       0 0 1

# C++ Pato_bin
## Utilitário Conversor Binário de/para Tipos Integrais

EM CONSTRUÇÃO!!!
                                                                             
* Licença MIT                                                                      
* C++14
* Header-only 
* constexpr       

Trecho de código de exemplo:                                                                        

```c++
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "pato_bin.h"
using namespace std;

int main()
{
    string a { "fixed-length strings!!!" };    
    cout << a << "\n\n";
    
    vector<byte_t> stream;
    for (auto b : to_bin<int8_t>(a.size()))
      stream.push_back(b);
    
    for (auto ch : a)
      stream.push_back(ch);
    
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

Alterando o `to_bin<int8_t>(a.size())` para `to_bin<int64_t>(a.size())` imprime:

```
fixed-length str!!!

13 00 00 00 00 00 00 00 66 69 78 65 64 2D 6C 65 6E 67 74 68 20 73 74 72 21 21 21
                         f  i  x  e  d  -  l  e  n  g  t  h     s  t  r  !  !  !
```
