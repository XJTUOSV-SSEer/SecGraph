#include <vector>

// 在这里包括适当的密码学库的头文件，以便表示 Element 类型
#include "../pbcwrapper/PBC.h" // 以 PBC 为例

class OXTTSetTuple {
public:
    unsigned char* e;
    // 使用密码学库的 Element 类型代替 Element y
    Zr y; // 以 PBC 为例
};