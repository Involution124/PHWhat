//
//  recursive.cpp
//  phwhat
//
//  Created by Curve on 5/28/15.
//  Copyright (c) 2015 RadiaxDevelopment. All rights reserved.
//

#include "recursive.h"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string>
#include <cstring>
#include <unistd.h>
using namespace std;
void buildfilelist(string path, string base, bool libfile){
    ofstream thebase;
    if(!base.empty()){
	string prepinc = base+"/preprocessorincludes.txt";
	thebase.open(prepinc.c_str(), fstream::out | fstream::app);	
    }
    ofstream phpdirlist("files.txt", fstream::app | fstream::out);
    ofstream allfiles("allfiles.txt", fstream::app | fstream::out);
    ofstream libfiles("libfiles.txt", fstream::app | fstream::out);
    if(phpdirlist.fail() || allfiles.fail() || libfiles.fail()){
        cout << "Opening files.txt, libfiles.txt, or allfiles.txt failed" << endl;
        exit(-1);
    }
    DIR *directory;
    struct dirent* directoryentry;
    //These directories have no business being preprocessed or edited, as they are library files included in the extensions source tree of php.
    string notallowed[SIZER] = {"oniguruma", "libgd", "libflatfile", "lib", "libinifile", "libcdb", "libmbfl","libsqlite", "regex", "libbcmath", "pcrelib", "libmagic", "libxmlrpc", "libsqlite"};
    //Lets not edit tests, documentation, or examples of code either, as they are useless.
    string dontenter[] = {"tests", "docs", "examples"};
    directory = opendir(path.c_str());
    if(directory==NULL){
        cout << "Directory couldn't be opened. Make sure you typed the correct path" << endl;
        exit(EXIT_FAILURE);
    }
    while((directoryentry=readdir(directory))!=NULL){
        string newpath = path+"/"+(directoryentry->d_name);
        if(directoryentry->d_type==DT_REG){
            //If it is a regular file
            size_t al = (newpath.find_last_of("."));
            if(al!=string::npos){
                if((libfile)&&((newpath.substr(al).compare(".c")==0)||(newpath.substr(al).compare(".h")==0))){
                    //if the library flag is set, and its .c or .h, add it to libfile list
                    libfiles << newpath << endl;
                } else if((newpath.substr(al)).compare(".c")==0){
                    //We want to do every operation to .c files
                    phpdirlist << newpath << endl;
                    allfiles << newpath << endl;
		    thebase << " -imacros " << newpath << " " << endl;
                } else if((newpath.substr(al)).compare(".h")==0){
                    //We don't want to preprocess .h files however
                    allfiles << newpath << endl;
		    thebase << " -imacros " << newpath << " " << endl;
                }
            }
        } else if(directoryentry->d_type==DT_DIR){ // IF its another directory
            if((directoryentry->d_name[0]!='.'||\
                (strcmp(directoryentry->d_name, ".pc")==0))&&\
                (strcmp(directoryentry->d_name, dontenter[0].c_str())!=0)&&\
                (strcmp(directoryentry->d_name, dontenter[1].c_str())!=0)&&\
                (strcmp(directoryentry->d_name, dontenter[2].c_str())!=0)){
                //Its doesn't start with ., unless its .pc. And its not equal to tests/docs/examples.
                if(libfile){
                    buildfilelist(newpath,"arbitrary",true);
                } else {
                    bool harden=true; //Is the file in the notallowed?
                    for(int looper=0; looper < SIZER; looper++){
                        if(strcmp(directoryentry->d_name, notallowed[looper].c_str())==0){
                            harden=false;
                        }
                    }
                    if(harden){     //If not, harden it.
			string basedirr;
                        if(base==""){ //Only do this for a file depth of 1,
                            phpdirlist << "BASE" << endl << newpath << endl;
                            allfiles << "BASE" << endl << newpath << endl;
                            //Base output is used to recognize base directories
			    basedirr=newpath;
                        } else {
			    basedirr=base; //we want a consistent /preprocessorincludes file through each dir with a depth of one
			}
                        buildfilelist(newpath, basedirr);
                    } else {
                        //If you don't want to harden it, still recursive search, with lib set libfiles.
                        buildfilelist(newpath, "arbitrary", true);
                    }
                }
            }
        }
    }
}
