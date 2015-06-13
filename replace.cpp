//
//  replace.cpp
//  phwhat
//
//  Created by Curve on 5/30/15.
//  Copyright (c) 2015 RadiaxDevelopment. All rights reserved.
//

#include "replace.h"
#include "findname.h"
#include <iostream>
#include <unistd.h>
#include <string>
using namespace std;
void replace(string &a, string arguments, size_t incrementer){
    int skipfunc=-1; //used for skipping functions
    for(int i =0; i<arguments.length(); i++){
        size_t firstind = a.find("(", incrementer), secondind = a.find(",", incrementer), testind = a.find(")", incrementer);
        //Testind is used to see if a ) is found before a comma
        if(secondind==string::npos || testind < secondind){
            secondind = a.find(")", incrementer);
        }
        char b = arguments[i];
        int value = b - '0';
        //parse arguments, same as above buttt loop...
        for(int ii = 1; ii<value; ii++){
            firstind = secondind;
            secondind = a.find(",", firstind+1);
            testind = a.find(")", firstind+1);
            if(secondind==string::npos || testind < secondind){
                secondind = a.find(")", firstind+1);
                if(skipfunc==ii){
                    size_t holder = secondind+1;
                    secondind = a.find(",", holder);
                    testind = a.find(",", holder);
                    if(secondind==string::npos || testind < secondind){
                        secondind =a.find(")", holder);
                        //simple
                    }
                }
                
            }
            size_t foundfunc = a.find("(", firstind+1);
            if(foundfunc<secondind && foundfunc!=string::npos && skipfunc==-1){
                firstind=foundfunc;
                skipfunc=ii;
            }
        }
        //Find a new name within the barrier, replace it.
        string newstr = findmeaname(a.substr(firstind+1, secondind-firstind-1));
        a.replace(firstind+1, secondind-firstind-1, newstr.c_str());
    }
}