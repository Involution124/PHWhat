#include <iostream>
#include <string>
#include <fstream>
#include "Initializer.h"
#include "recursive.h"
#include <unistd.h>
#include "findname.h"
#include "fixfile.h"
#include "phphardener.h"
using namespace std;
void usage(){
    cout << "./phphardener <string:php-base-path> <bool:enablebacktickoperator>" << endl;
    cout << "WARNING: backtick operator is very highly recommended to be set to false";
    cout << "ex: ./phphardener /usr/src/php false" << endl;
}
void cleanup(){
	remove("files.txt");
	remove("libfiles.txt");
	remove("allfiles.txt");
	remove("versions.h");
	remove("sample.c");
	remove("a.out");
}
int main(int argc, char* argv[]){
    if(argc!=3){
        usage();
        return 0;
    }
	string backorno(argv[2]);
    bool backticks = false;
    if(backorno.compare("true")==0){
        backticks=true;
    }
    string Basepath = argv[1], Extpath = Basepath+"/ext", Pcpath=Basepath+"/.pc";
    initializer(Basepath);
    buildfilelist(Extpath);
    buildfilelist(Pcpath);
    ifstream dirlist("files.txt");
    ifstream libfiles("libfiles.txt");
    ifstream alldirlist("allfiles.txt");
    string filelist, base;
    //These files cause problems when preprocessed, and don't require preprocessing.
    for(int i=0; i<3; i++){
        //Include .h and .c files
        if(i!=1){
            while(getline(libfiles, filelist)){
		//library files don't need to be preprocessed, but may still aid in the preprocessing stage so need to be sent through stages 0 and 2
                phphardener(filelist, "", i, Basepath);
            }
            while(getline(alldirlist, filelist)){
                while(filelist.compare("BASE")==0){ //BASE indicates base dir next line
                    getline(alldirlist, filelist);
                    base=filelist; 
                    getline(alldirlist, filelist);
                }
	        
        	if(!filelist.empty()){    
		    phphardener(filelist, base, i, Basepath);
		}	
            }
            libfiles.clear();
            libfiles.seekg(0);
            alldirlist.clear();
            alldirlist.seekg(0);
        } else {
            //Just .c files
            while(getline(dirlist, filelist)){
                while(filelist.compare("BASE")==0){
                    getline(dirlist, filelist);
                    base=filelist;
                    getline(dirlist, filelist);
                }
		if(!filelist.empty()){
		phphardener(filelist, base, 1, Basepath);
            	}
	    }
            dirlist.clear();
            dirlist.seekg(0);
       }
    if(backticks){
	//Change the backticks to the respectable system call
        fixfile(Basepath+"/Zend/zend_compile.c", "\"shell_exec\"", "\""+findmeaname("shell_exec")+"\"");
    } else {
        fixfile(Basepath+"/Zend/zend_compile.c", "\"shell_exec\"", "\"\"");
    }
    fixfile(Basepath+"/Zend/zend_builtin_functions.c", "display_disabled_function", findmeaname("display_disabled_function")); 
    fixfile(Basepath+"/ext/reflection/php_reflection.c", "display_disabled_function", findmeaname("display_disabled_function"));
	//Something tells me a lot more fixfile calls will need to be implemented.
    }
	buildfilelist(Basepath);
	ifstream all("allfiles.txt");
	while(getline(all, filelist)){
		while(filelist.compare("BASE")==0){
			getline(all, filelist);
			base=filelist;
			getline(all, filelist);
		}
		if(!filelist.empty()){
			phphardener(filelist, "", 3, "");
		}
	}
	cleanup(); // this should be disabled for debugging.
}
