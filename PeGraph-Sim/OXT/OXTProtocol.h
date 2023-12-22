#ifndef OXT_H
#define OXT_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "OXTEDB.h"
#include <chrono>
#include "../pbcwrapper/PBC.h"

using namespace std;

class OXT{
    public:
        OXT(const char* dataset);
        int GetFileLine(const char* path){
            int lineCount = 0;
            std::ifstream file(path);
            std::string line;
            while (std::getline(file, line)) {
                lineCount++;
            }
            file.close();
            return lineCount;
        }
        uint64_t timeSinceEpochMillisec() {//截取以纪元时间为单位获取当前时间戳，以毫秒为单位

            using namespace std::chrono;
            return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        }
        OXTEDB SetUp();

        void Search(vector<string> w, OXTEDB oxtedb);
        //vector<vector<G1>> GenXtoken(vector<string> w, int);
        bool MembershipCheck(string xtag, OXTEDB oxtedb);
        G1 g;
        Pairing* p;
    private:
        std::ifstream file;
        int Fileline = 0; 
        unordered_map<string,vector<string>> DB;
};

#endif