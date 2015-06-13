//
//  fixfile.cpp
//  phwhat
//
//  Created by Curve on 5/31/15.
//  Copyright (c) 2015 RadiaxDevelopment. All rights reserved.
//

#include "fixfile.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;
void fixfile(string filename, string searchfor, string value ){
    string out = filename+".x", getlinerrr;
    ifstream ina(filename.c_str());
    ofstream ino(out.c_str());;
    cout << "\t[+] Fixing: " << searchfor << endl;
    while(getline(ina, getlinerrr)){
        size_t pos = 0;
        int counter = 1;
        pos = getlinerrr.find(searchfor, pos);
        while(pos!=string::npos){
            cout << " found " << endl;
            getlinerrr.replace(pos, searchfor.size(), value);
            pos = 0;
            for(int i=0; i < counter; i++){
                cout << counter << endl;
                pos = getlinerrr.find(searchfor, pos);
            }
            counter++;
        }
        ino << getlinerrr << endl;
    }
    remove(filename.c_str());
    rename(out.c_str(), filename.c_str());
}

