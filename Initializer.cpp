//  Initializer.cpp
//  phwhat
//      This file has three functions:
//          1. Create the versions.h file, and add three very important macros for preprocessing
//          2. Add the external versions.h string, so it can be reached across other files.
//          3. Add a couple important files to the top of the library files list
//  Created by Curve on 5/27/15.
//  Copyright (c) 2015 RadiaxDevelopment. All rights reserved.
#include "Initializer.h"
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include "findname.h"
using namespace std;
void libfilesheader(string bbase){
    ofstream libsoutput("libfiles.txt");
    string important[3]={bbase+"/main/php_config.h",bbase+"/Zend/zend.h",bbase+"/main/php_compat.h"
    };
    for(int i=0; i<3; i++){
        libsoutput << important[i] << endl;
    }
}
void buildtestingfile(){
    ofstream samplee("sample.c", fstream::out | fstream::trunc);
    samplee << "int main(){}" << endl;

}
void initializer(string base){
    ofstream outt("versions.h", fstream::out | fstream::trunc);
    string filess[3]={base+"/Zend/zend_modules.h", base+"/Zend/zend_extensions.h", base+"/main/php.h"};
    string stringss[3]={"#define ZEND_MODULE_API_NO", "#define ZEND_EXTENSION_API_NO", "#define PHP_API_VERSION"};
    //enumerate the threee files
    for(int i =0; i <3; i++){
        ifstream fread(filess[i].c_str());
        if(fread.fail()){
            cout << "File failed to open: " << filess[i];
            exit(EXIT_FAILURE);
        }
        //Search for each specific macro
        bool continuee=true;
        string getlinerrr;
        while(getline(fread, getlinerrr) && continuee){
            cout << getlinerrr << endl;
            if(getlinerrr.rfind(stringss[i])!=string::npos){
                outt << getlinerrr << endl;
                continuee=false;
                cout << stringss[i] << " was found in " << filess[i] << ", adding to Versions.h" << endl;
            }
        }
    }
    libfilesheader(base); //Add a couple important library files to the top of the libfiles list.
    buildtestingfile();
    //Just a skeleton file, we'll use gcc to determine whether #include <>'s should be included with -imacro for compilation or not. Used in phphardener.
}
