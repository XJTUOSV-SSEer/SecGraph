#include "OXTProtocol.h"
#include <sys/time.h>
#include <iostream>
#include <chrono>
using namespace std;

uint64_t timeSinceEpochMillisec() {//截取以纪元时间为单位获取当前时间戳，以毫秒为单位

  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main(){
    uint64_t set_up_start_time =  timeSinceEpochMillisec();
    OXT oxt("dataset/Youtube");
    //OXT oxt("dataset/Email-Enron-new.txt");
    OXTEDB oxtedb = oxt.SetUp();
    uint64_t set_up_end_time =  timeSinceEpochMillisec();
    cout << "SetUp time " << (set_up_end_time - set_up_start_time)*2 << " ms" <<endl;

    // id: 263: count: 131
	// id: 284: count: 131
	// id: 489: count: 131
	// id: 492: count: 131
	// id: 498: count: 131
	// id: 462: count: 132
	// id: 504: count: 133
	// id: 560: count: 133
	// id: 261: count: 134
	// id: 1478: count: 134

    //vector<string> ids = {"263","284","489","492","498","462","504","560","261","1478"};
    vector<std::string> ids = {"134","461","2234","2386","3870","4264","6681","9874","11870","14439"};
    //vector<string> ids = {"1568","10119","1698","1709","753","5205","5208","5272","6009","568"};
    vector<string> query_str;
    for(int i=0;i<10;i+=2){
        set_up_start_time =  timeSinceEpochMillisec();
        query_str.push_back("fd"+ids[i]);
        query_str.push_back("fd"+ids[i+1]);
        oxt.Search(query_str,oxtedb); 
    }

    return 0;
}