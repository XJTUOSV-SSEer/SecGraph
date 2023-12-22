#include <vector>
#include <string>
#include <set>
#include "../common/data_type.h"
#include "OXTTSetTuple.h"

class OXTEDB {
public:
    unsigned char K_S[ENC_KEY_SIZE];
    unsigned char K_X[ENC_KEY_SIZE];
    unsigned char K_I[ENC_KEY_SIZE];//KF密钥
    unsigned char K_Z[ENC_KEY_SIZE];
    unsigned char K_T[ENC_KEY_SIZE];
    unsigned char K_E[ENC_KEY_SIZE];



    std::unordered_map<string, vector<OXTTSetTuple>> TSet; //stag->(e,y)
    std::set<std::string> XSet;
};