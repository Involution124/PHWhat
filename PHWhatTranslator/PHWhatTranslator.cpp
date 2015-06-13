#include <unistd.h>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
using namespace std;
string filetoopen;
string findmeaname(string funcname){
        funcname.erase(remove_if(funcname.begin(),funcname.end(),  (int(*)(int))isspace) , funcname.end());
        ifstream filepointer(filetoopen.c_str());
	if(filepointer.fail()){
		cout << "Couldn't open " << filetoopen << endl;
		exit(EXIT_FAILURE);
	}
        string lineholder, newname;
        while(getline(filepointer, lineholder)){
                if(lineholder.compare(funcname)==0){ 
			getline(filepointer, lineholder);
			return lineholder; 
		}
		getline(filepointer, lineholder); // advance the file position forward either way. We won't be matching abc's
	}
        return funcname;
}

int tokenerr(string source){
	string sourceout=source+".o";
	ifstream phpsource(source.c_str());
	ofstream phpsourceout(sourceout.c_str());
	string a, name;
	while(getline(phpsource, a)){
		int counter = 1;
		size_t locationholder=a.find("(");
		while(locationholder!=string::npos){
			if(locationholder>=1){
				size_t finder = locationholder-1;
				char cc = a.at(finder);
				bool continuee = true;
				while(isalnum((int) cc ) || cc=='_' ){
					if(finder==0){
						cc ='&';
						continuee=false;
					} else {
					cc=a.at(--finder);
					}
				}
				if(continuee){ finder++; };
				bool findorno = true;
				/*if(finder>=9 && (a.substr(finder-9, 9)).compare("function ")==0){
					findorno=false;
				} */
				if(findorno){
					if(locationholder-finder>1){
						string name =(a.substr(finder, locationholder-finder));
						name = findmeaname(name);
						a.replace(finder, locationholder-finder, name);
					}
				}
			}
			locationholder=0;
			for(int i =0; i<counter; i++){
				locationholder=a.find("(", locationholder+1);
			}
			counter++;
		}
		phpsourceout << a << endl;
	}
	remove(source.c_str());
	rename(sourceout.c_str(), source.c_str());
	cout << "Got it -> " << source << endl;
}
int main(int argc, char*argv[]){
	if(argc==3){
		filetoopen=argv[2];
		string sourcecode=argv[1];
		tokenerr(sourcecode);
	}
}
