//
//  phphardener.cpp
//  phwhat
//
//  Created by Curve on 5/30/15.
//  Copyright (c) 2015 RadiaxDevelopment. All rights reserved.
//
#include "phphardener.h"
#include <string>
#include "replace.h"
#include "Initializer.h"
#include <algorithm> //remove_if
#include <iostream>
#include <fstream>
#include <cctype>
#include <unistd.h>

using namespace std;
void constructsample(string macron){

	ofstream sample("sample.c", fstream::out | fstream::trunc);
	sample << "#include <" << macron << ">" << endl;
	sample << "int main(){}";
}
void phphardener(string a, string base, int i, string extra){
    string aout = a+".output", getlinerrr;
    ifstream fileread(a.c_str());
    if(fileread.fail()){
        cout << "Couldn't open " << a << " from phphardener function" << endl;
        return;
    }
    ofstream filewrite(aout.c_str(), fstream::out | fstream::trunc);
    if(i==0){
        while(getline(fileread, getlinerrr)){
            if(getlinerrr.find("#")!=string::npos){
                string cleaner=getlinerrr;
                cleaner.erase(remove_if(cleaner.begin(),cleaner.end(),  (int(*)(int))isspace) , cleaner.end()); //Get all that whitespace out of it to simplify manipulation
                if(cleaner.find("#include")!=string::npos || cleaner.find("#error")!=string::npos){
                    if(!base.empty()){
                        if(cleaner.find("#include<")!=string::npos){
                            size_t first=cleaner.find("<");
                            size_t second=cleaner.find(">");
                            string macroname=cleaner.substr(first+1, second-first-1);
			    constructsample(macroname);
                            string command = "gcc sample.c -I"+extra+"/ext/date/lib -I"+extra+"/ext/ereg/regex -I/usr/include/libxml2 -I"+Basepath+"/ext/sqlite3/libsqlite 2>/dev/null";
                            if(system(command.c_str())==0){
                                ofstream includeprep(a+"/preprocessorincludes.txt", fstream::out | fstream::app);
                                includeprep << "-imacros " << macroname << endl;
                            }
                        }
                    }
                    getlinerrr.insert(0, "//unique");

                }
	}
                filewrite << getlinerrr << endl;
            }
            remove(a.c_str());
            rename(aout.c_str(), a.c_str());
   } else if(i==1){
        // --------Certain files do not need to be preprocessed
        string notallowed[6]={"hash/hash_ripemd.c","sqlite3/sqlite3.c", "hash/hash_sha.c", "fileinfo/data_file.c", "xml/compat.c", "standard/filestat.c"};
        string comparable=a.substr(a.find_last_of("/", a.find_last_of("/")-1)+1);
        bool hardorno=true;
        for(int q=0; q<6; q++){
            if(comparable.compare(notallowed[q])==0){
                hardorno=false;
            }
        }
        // --------------
        // --------- Certain files also need extra parameters
	if(hardorno){
	        string extraa = "";
	        /* phar still needs a lot of work, will need much more then this.
	         if(comparable.compare("phar/func_interceptors.c")==0){
	         extra=" -D'PHAR_MOD_INT(func, phname)=PHAR_G(orig_##func) = NULL;if (SUCCESS == zend_hash_find(CG(function_table), #func, sizeof(#func), (void **)&orig)) { PHAR_G(orig_##func) = orig->internal_function.handler; orig->internal_function.handler = phname; } ' ";
	         phphardener(filelist, base, 4, extra);
	         } else*/
	        if(a.substr(a.find_last_of("/", a.find_last_of("/")-1)+1).compare("/usr/include/sqlite3.h")==0){
	             extraa = " -U _SQLITE3_H_ -imacros /usr/src/php/ext/sqlite3/libsqlite/sqlite3.c ";
	         } else if(a.substr(a.find_last_of("/", a.find_last_of("/")-1)+1,3).compare("dom")==0 || a.substr(a.find_last_of("/", a.find_last_of("/")-1)+1).compare("simplexml/simplexml.c")==0){
	             extraa = " -D LIBXML2_NEW_BUFFER -D LIBXML_XPATH_ENABLED ";
	         } else {
	         }
	        // -------------
	        // ------- Add on preprocessing directives
		char curdir[1024];
	        getcwd(curdir, 1024);
	        string versions(curdir);
		string globalvars=" -imacros "+versions+"/versions.h"+" -imacros "+extra+"/main/php_version.h -I"+extra+"/ext/date/lib -I"+extra+"/ext/ereg/regex -I/usr/include/libxml2 -I"+extra+"/ext/sqlite3/libsqlite -imacros "+extra+"/Zend/zend.h -I/usr/include/apache2 -imacros "+extra+"/main/php_config.h ";
		cout << "\t[+]Executing: " << a << endl;
	        string  executer = "sudo gcc -w -E -P -C -dD -o " + aout + " " + a + globalvars;
	        ifstream includeprep(base+"/preprocessorincludes.txt");
	        while(getline(includeprep, getlinerrr)){
		if(getlinerrr.compare(a)!=0){
	                executer+=" " +getlinerrr+" ";
	            }
	        }
	        // ------------
	        executer+=globalvars+extraa;
	        int success = system(executer.c_str()); //success on the system call = 0
	        fileread.close();
	        fileread.clear();
		filewrite.close();
	filewrite.clear();
	        remove(a.c_str());
	        rename(aout.c_str(), a.c_str());
	        if(success!=0){	// if it didn't compile, touch the file so it doesnt cause errors >:)
	            cout << "WARNING: " << a << "Couldn't compile >:| " << endl;
	            string ex = "touch " +a;
	            system(ex.c_str());
	        } else {
	            fileread.open(a.c_str());
	            filewrite.open(aout.c_str());
	            bool starter=false;
	            // --------- Don't start copying till you hit the copywrite, preprocess adds a lot of junk.
	            while(getline(fileread, getlinerrr)){
	                if(!starter){
	                    if(getlinerrr.find("/* Copyright (C) 1991-2014 Free Software Foundation, Inc.")!=string::npos){
	                        starter=true;
	                        filewrite << getlinerrr << endl;
	                    }
	                } else {
	                    filewrite << getlinerrr << endl; //Once its been found, copy every line.
	                }
	            }
	    
	            fileread.close();
	            filewrite.close();
	            remove(a.c_str());
	            rename(aout.c_str(), a.c_str());
       		 }
	}
    } else if (i==2){

        cout << "\t[+] Cleaning " << a << endl;
        while(getline(fileread, getlinerrr)){
            if(getlinerrr.find("//unique")!=string::npos){
                getlinerrr.erase(0,8);
            }
            filewrite << getlinerrr << endl;
        }
        remove(a.c_str());
        rename(aout.c_str(), a.c_str());
    } else if (i==3){
        size_t incremental = 0;
        cout << "\t[+]Hardening " << a << endl;
        while(getline(fileread, getlinerrr)){
            if(getlinerrr[0]=='#'){ //If this is a directive, don't harden it
                while(getlinerrr.back()=='\\'){ //In case it spans multiple lines
                    filewrite << getlinerrr << endl;
                    getline(fileread, getlinerrr);
                }
            } else {
                string a1[] = {"PHP_FUNCTION(","ZEND_FUNCTION(","PHP_NAMED_FUNCTION(","ZEND_NAMED_FUNCTION(","ZEND_FN(","PHP_FN(","ZEND_BEGIN_ARG_INFO", "FileFunction("}; //7
		string a2[] = {"PHP_METHOD(", "ZEND_METHOD("}; //2
                string a12[] = {"PHP_FE(","ZEND_DEP_FE(","PHP_DEP_FE(","ZEND_FE("}; //4
                string a123[] = {"PHP_ME_MAPPING(","ZEND_ME_MAPPING(","ZEND_FENTRY(","ZEND_RAW_FENTRY(","ZEND_RAW_NAMED_FE(","PHP_RAW_NAMED_FE(","ZEND_NAMED_FE(","PHP_NAMED_FE(","PHP_DEP_FALIAS(","ZEND_DEP_FALIAS(","ZEND_NAMED_ME(","PHP_NAMED_ME(","ZEND_FALIAS(","PHP_FALIAS("};
                string a23[]={"PHP_ME(","ZEND_ME(","ZEND_ABSTRACT_ME(","PHP_ABSTRACT_ME("};
                string a234[]={"ZEND_MALIAS(","PHP_MALIAS("};
                for(int i = 0; i < 8; i++){
                    if(getlinerrr.find(a1[i])!=string::npos){
                        incremental=getlinerrr.find(a1[i]);
                        replace(getlinerrr, "1", incremental);
			i=10;
                    }
                }
		for(int i = 0; i < 2; i++){
		    if(getlinerrr.find(a2[i])!=string::npos){
			incremental=getlinerrr.find(a2[i]);
			replace(getlinerrr, "2", incremental);
			i=10;
		    }
		}
                for(int i = 0; i < 4; i++){
                    if(getlinerrr.find(a12[i])!=string::npos){
                        incremental=getlinerrr.find(a12[i]);
                        replace(getlinerrr, "12", incremental);
			i=10;
                    }
                }
                for(int i = 0; i < 14; i++){
                    if(getlinerrr.find(a123[i])!=string::npos){
                        incremental=getlinerrr.find(a123[i]);
                        replace(getlinerrr, "123", incremental);
			i==10;
                    }
                }
                for(int i = 0; i < 4; i++){
                    if(getlinerrr.find(a23[i])!=string::npos){
                        incremental=getlinerrr.find(a23[i]);
                        replace(getlinerrr, "23", incremental);
       			i=10; 
	            }
                }
                for(int i = 0; i < 2; i++){
                    if(getlinerrr.find(a234[i])!=string::npos){
                        incremental=getlinerrr.find(a234[i]);
                        replace(getlinerrr, "234", incremental);
                   	i=10;
			 }
                }
            }
            filewrite << getlinerrr << endl;
        }
        remove(a.c_str());
        rename(aout.c_str(), a.c_str());
	}/*else if(i==4){
	size_t incremental=0;
	cout << "Getting spl" << endl;
	while(getline(fileread,getlinerrr)){
	incremental = getlinerrr.find("PharFileFunction(")!=string::npos;
	if(incremental!=string::npos){
		replace(getlinerrr, "1", incremental);
	} else if(getlinerrr.find("PHAR_INTERCEPT(")){
		string name = getlinerrr.substr(getlinerrr.find("(")+1, getlinerrr.find(")")-getlinerrr.find("(")-1);
		getlinerrr="PHAR_MOD_INT(



}*/
}
