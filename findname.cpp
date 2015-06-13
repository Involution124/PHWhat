//
//  findname.cpp
//  phwhat
//
//  Created by Involution on 5/31/15.
//  Copyright (c) 2015 RadiaxDevelopment. All rights reserved.
#include <algorithm> //remove_if
#include <cctype>
#include "Initializer.h"
#include "findname.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
//#include <cstdlib>
#include <cctype>
using namespace std;
int countt=0;
string aarandom(){
	string retr = "abc"+to_string(++countt);
	char alnumer[62]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	srand(time(NULL));
	for(int i=0; i<6; i++){
		retr=retr+alnumer[rand()%62];
	}
	return retr;
}
string makename(string funcname){
    static int countt;
    ofstream filepointer("Randomname.txt", fstream::out | fstream::app);
    string newname=  aarandom();
    filepointer << funcname << endl;
    string phporc[22] = {"((void*","(void*", "NULL",  "fname", "name", "func" , "v", "__construct", "__destruct", "__call", "__callStatic", "__get", "__set", "__isset", "__unset", "__sleep","__wakeup", "__toString", "__invoke", "__set_state", "__clone","__debugInfo"};
    for(int i=0; i<22; i++){
        if(funcname.compare(phporc[i])==0){
            newname=phporc[i];
        }
    }
    filepointer << newname << endl;
    return newname;
}

string findmeaname(string funcname){
    //First check to see if a replacement already exists
    funcname.erase(remove_if(funcname.begin(),funcname.end(),  (int(*)(int))isspace) , funcname.end());
    ifstream filepointer("Randomname.txt");
    string lineholder;
    while(getline(filepointer, lineholder)){
        int condition = (lineholder.compare(funcname));
        getline(filepointer, lineholder);
        if(lineholder.compare(funcname)==0){
            return funcname; }
        if(condition==0){
            return lineholder;
        }
    }// if it isn't found, ...
    
    string newname = makename(funcname);
    return newname;
}
