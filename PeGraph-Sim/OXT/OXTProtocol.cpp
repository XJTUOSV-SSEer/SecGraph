#include "OXTProtocol.h"
#include <openssl/rand.h>
#include "../Util/Utils.h"

OXT::OXT(const char* dataset){
    file.open(dataset);
    if(!file.is_open()){
        std::cerr << "无法打开文件" << std::endl;
    }
    //读文件
	double ratio = 0.4;
    int lineNumber = 0;
    std::string line;
    int i=0;
    Fileline = GetFileLine(dataset);
    while (std::getline(file, line)) {
		if(lineNumber>=Fileline*ratio){
			break;
		}
        // 使用字符串流将每行分割成元素item
        std::istringstream iss(line);
        std::string item;
        std::vector<std::string> items;
        while (iss >> item) {
            items.push_back(item);
        }
		std::string sw = "fd"+items[0];
		std::string sid = items[1];
        if(DB.find(sw) != DB.end()){
            DB[sw].push_back(sid);
        }else{
            vector<string> ids;
            ids.push_back(sid);
            DB[sw] = ids;
        }
        lineNumber++;
    }
    // pbc_param_t param;
    // size_t count;
    // char *params = NULL;
    // 读取参数文件
    // FILE *fp = fopen("../pbcwrapper/pairing.param", "r");
    // fseek(fp, 0, SEEK_END);
    // count = ftell(fp);
    // rewind(fp);
    // params = (char *)malloc(count * sizeof(char));
    // fread(params, 1, count, fp);
    // fclose(fp);

    // // 初始化PBC库
    // pbc_param_init_set_str(param, params);
    // pbc_param_init_a_gen(param, 160, 512);
    // pairing_init_pbc_param(pairing, param);

    // // 随机生成G1群元素
    // element_init_G1(g, pairing);
    // element_random(g);

    const char *paramFileName = "pbcwrapper/pairing.param";
    FILE *sysParamFile = fopen(paramFileName, "r");
    if (sysParamFile == NULL) {
        cerr<<"Can't open the parameter file " << paramFileName << "\n";
        return;
    }
    p = new Pairing(sysParamFile);
    cout<<"Is symmetric? "<< p->isSymmetric()<< endl;
    cout<<"Is pairing present? "<< p->isPairingPresent()<< endl;  

    // Pairing e(sysParamFile);
    // cout<<"Is symmetric? "<< e.isSymmetric()<< endl;
    // cout<<"Is pairing present? "<< e.isPairingPresent()<< endl;  
    // unsigned char K[ENC_KEY_SIZE];
    // RAND_bytes(K,ENC_KEY_SIZE); 
    // unsigned char xind[5 + AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
    // int xind_len = enc_aes_gcm((unsigned char*)"12345", 5, K, xind);
    // Zr xind_Zr(e,(void*)xind,xind_len);

    g = G1(*p,false);
    fclose(sysParamFile);
}

OXTEDB OXT::SetUp(){
    //cout<<DB.size()<<endl;
    OXTEDB oxtedb;

    //生成密钥
    RAND_bytes(oxtedb.K_I,ENC_KEY_SIZE); 
    RAND_bytes(oxtedb.K_Z,ENC_KEY_SIZE); 
    RAND_bytes(oxtedb.K_T,ENC_KEY_SIZE); 
    RAND_bytes(oxtedb.K_S,ENC_KEY_SIZE); 
    RAND_bytes(oxtedb.K_X,ENC_KEY_SIZE);
    RAND_bytes(oxtedb.K_E,ENC_KEY_SIZE); 

    for(auto iter = DB.begin(); iter!=DB.end(); iter++) {
        string sw = iter->first;
        int c = 1;
        for(int i=0;i<iter->second.size();i++){
            string sid = iter->second[i];

            //Exact item
            const char* w = sw.c_str();
            size_t w_len = sw.length();
            const char* id = sid.c_str();
            size_t id_len = sid.length();
            
            //加密xind
            unsigned char xind[id_len + AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
            int xind_len = enc_aes_gcm((unsigned char*)id, id_len, oxtedb.K_I, xind);

            unsigned char z[to_string(c).length() + AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
            int z_len = enc_aes_gcm((unsigned char*)to_string(c).c_str(), to_string(c).length(), oxtedb.K_Z, z);
            //cout<<"test1"<<endl;
            Zr xind_Zr(*p,(void*)xind,xind_len);
            Zr z_Zr(*p,(void*)z,z_len);
            Zr y(*p,true);
            y = xind_Zr/z_Zr;
            //cout<<"test2"<<endl;
            // vector<uint8_t> vxind(xind,xind+xind_len);
            // HashResultZr xindhash(pairing,vxind); 
            // element_t resultXind;
            // resultXind[0] = xindhash.element[0];

            // vector<uint8_t> vz(z,z+z_len);
            // HashResultZr zhash(pairing,vz);
            // element_t resultZ;
            // resultZ[0] = zhash.element[0];
            
            // element_t y;
            // element_init_Zr(y,pairing);
            // element_div(y,resultXind,resultZ);

            unsigned char stag[w_len+ AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
            int stag_len = enc_aes_gcm((unsigned char*)w, w_len, oxtedb.K_T, stag);

            unsigned char e[id_len + AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
            int e_len = enc_aes_gcm((unsigned char*)id, id_len, oxtedb.K_E, e);

            OXTTSetTuple tuple;
            tuple.e = e;
            tuple.y = y;

            string sstag((char*)stag,stag_len);

            if(oxtedb.TSet.find(sstag) != oxtedb.TSet.end()){
                oxtedb.TSet[sstag].push_back(tuple);
            }else{
                vector<OXTTSetTuple> tuples;
                tuples.push_back(tuple);
                oxtedb.TSet[sstag] = tuples;
            }

            unsigned char kxw[w_len+ AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
            int kxw_len = enc_aes_gcm((unsigned char*)w, w_len, oxtedb.K_X, kxw);

            //cout<<"test3"<<endl;
            Zr kxw_Zr(*p,(void*)kxw,kxw_len);
            // kxw_Zr.dump(stdout,"kxw_Zr",10);
            // xind_Zr.dump(stdout,"xind_Zr",10);
            G1 xtag(g,false); //这里一定要传入g表示与g在同一个群内，否则结果为0，true表示特定的群，false表示随机群
            xtag = g ^ (kxw_Zr * xind_Zr);
            //g.dump(stdout,"g is               ",16);
            xtag.dump(stdout,"xtag is               ",16);
            //cout<<"test4"<<endl;

            // vector<uint8_t> vkxw(kxw,kxw + kxw_len);
            // HashResultZr kxwhash(pairing,vkxw);
            // element_t resultkxw;
            // resultkxw[0] = kxwhash.element[0];

            // element_t xtag;
            // element_init_GT(xtag, pairing);
            // element_mul(xtag, resultkxw, resultXind);
            // element_pow_zn(xtag, g, xtag);
            oxtedb.XSet.insert(xtag.toString(true));
            c++;
        }
    }
    cout<<"finished"<<endl;
    file.close();
    return oxtedb;
}

bool OXT::MembershipCheck(string xtag, OXTEDB oxtedb){
    if(oxtedb.XSet.find(xtag) != oxtedb.XSet.end()){
        return true;
    }
    return false;
}

void OXT::Search(vector<string> w,OXTEDB oxtedb){
    cout<<oxtedb.XSet.size()<<endl; //XSet大小为1？
    uint64_t set_up_start_time =  timeSinceEpochMillisec();
    //cout<<oxtedb.TSet.size()<<endl;
    vector<unsigned char*> res;
    //获取每个关键词w的stag
    vector<string> stags;
    int leastIndex = 0;
    int leastNumber = INT_MAX;
    for(int i=0;i<w.size();i++){
        unsigned char stag[w[i].length()+ AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
        int stag_len = enc_aes_gcm((unsigned char*)w[i].c_str(), w[i].length(), oxtedb.K_T, stag);
        string sstag((char*)stag,stag_len);
        stags.push_back(sstag);
        if(oxtedb.TSet[sstag].size() < leastNumber){
            leastIndex = i;
            leastNumber = oxtedb.TSet[sstag].size();
        }
    }

    //get t
    vector<OXTTSetTuple> t = oxtedb.TSet[stags[leastIndex]];
    cout<<t.size()<<endl;
    vector<Zr> z_c;
    for(int i=0;i<t.size();i++){
        unsigned char z[to_string(i).length() + AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
        int z_len = enc_aes_gcm((unsigned char*)to_string(i).c_str(), to_string(i).length(), oxtedb.K_Z, z);
        Zr z_Zr(*p,(void*)z,z_len);
        z_c.push_back(z_Zr);
    }

    //gen xtoken
    vector<vector<G1>> xtokens(t.size());
    for(int c = 0; c<z_c.size(); c++){
        for(int i=0;i<w.size();i++){
            if(i != leastIndex){
                unsigned char kxw[w[i].length()+ AESGCM_MAC_SIZE + AESGCM_IV_SIZE];
                int kxw_len = enc_aes_gcm((unsigned char*)w[i].c_str(), w[i].length(), oxtedb.K_X, kxw);
                Zr kxw_Zr(*p,(void*)kxw,kxw_len);
                G1 xtrap(g,true);
                xtrap = g^kxw_Zr;
                G1 xtoken(g,true);
                xtoken = xtrap^(z_c[c]);
                xtokens[c].push_back(xtoken);
            }
        }
    }

    int count = 0;
    //membershipcheck
    for(int c=0;c<t.size();c++){
        int flag = 1;
        for(int i=0;i<xtokens[c].size();i++){
            G1 xtag(*p);
            xtag = xtokens[c][i] ^ t[c].y;
            if(oxtedb.XSet.find(xtag.toString(true)) == oxtedb.XSet.end()){
                count++;
                flag = 0;
            }
        }
        if(flag == 1){
            res.push_back(t[c].e);
        }
    }
    cout<<"count:"<<count<<endl;
    uint64_t set_up_end_time =  timeSinceEpochMillisec();
    cout << "Search time " << set_up_end_time - set_up_start_time << " ms" <<endl;

    return;
}


