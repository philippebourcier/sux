/*
   ________ ______  ___
  /  ___/  ||  \  \/  /
 _\___ \|  |/  />    <
/______>______//__/\_ \
                     \/  v1.01
        written by Philippe Bourcier
	  with input from Serge Danzanvilliers
          with fixes by Pierre Ynard

  This code is published under the BSD License:
  http://www.opensource.org/licenses/bsd-license.php

  The main idea is to replace "... | sort | uniq -c [ | sort -(r)(n) ] [ | head -N ]"
  This program is (way) faster and less memory consuming than the above commands.

  Sample use:

   # cat file.txt | awk ... | sort | uniq -c | sort -rn | head -n 20
   will be replaced by:
   # cat file.txt | awk ... | sux -rn 20

   # cat file.txt | awk ... | sort | uniq -c | sort -n
   will be replaced by:
   # cat file.txt | awk ... | sux -s

   # cat file.txt | awk ... | sort | uniq -c
   will be replaced by:
   # cat file.txt | awk ... | sux

   # cat file.txt | awk ... | sort | uniq -c | head -n 20
   will be replaced by:
   # cat file.txt | awk ... | sux -n 20

*/

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <stdlib.h>
#include <unistd.h>

using namespace std;

typedef pair<std::string, int> mypair;

struct intcmp {
  bool operator()(const mypair &lhs, const mypair &rhs) {
    return lhs.second < rhs.second;
  }
};

struct rintcmp {
  bool operator()(const mypair &lhs, const mypair &rhs) {
    return lhs.second > rhs.second;
  }
};

void usage(string p) {
cout  << "Usage: " << p << " [OPTION]... [FILE]..." << endl <<
          "Write sorted concatenation of all FILE(s) to standard output." << endl << endl <<
          "Ordering options:" << endl <<
          "-s             sort the result (sort -n)" << endl <<
          "-r             reverse sorting order (sort -rn)" << endl <<
          "-nN           limit output to the first N lines" << endl << endl <<
          "Report bugs to <philippe@sysctl.org>." << endl; }

int main(int argc, char *argv[]) {

map<string,int> c;
string line;
bool s, r, l;
s = r = l = false;
char *n=NULL;
int opt;
int z=-1;

  // Getting arguments
  while ((opt=getopt(argc,argv,"hsrn:"))!=-1) {
    switch (opt) {
      case 's':
        s = true;
        break;
      case 'r':
        r = true;
        break;
      case 'n':
        l = true;
        n = optarg;
        z=atoi(n);
        break;
      case 'h':
        usage(argv[0]);
        return 0;
      case '?':
        cout << "Try '" << argv[0] << " -h' for more information." << endl;
        return 1;
      default:
        return 0;
    }
  }

  if(argv[optind]) {
         ifstream file(argv[optind]);
         if(!file.is_open()) {
                 cout << "Unable to open file : " << argv[(argc-1)] << endl;
                 return 1;
         }
         while(file.good()) {
                 getline(file,line);
                 if (!file.eof()) c[line]++;
         }
         file.close();
  } else {
         if(!cin) return 1;
         while(cin) {
                 getline(cin,line);
                 if (!cin.eof()) c[line]++;
         }
  }

   // Sorting
  if(s || r) {
    vector<mypair> myvec(c.begin(),c.end());
    if(r) partial_sort(myvec.begin(),myvec.begin()+c.size(),myvec.end(),rintcmp());
    else partial_sort(myvec.begin(),myvec.begin()+c.size(),myvec.end(),intcmp());
    for (int i=0;i<int(c.size());++i) {
         cout << myvec[i].second << " " << myvec[i].first << endl;
         if(l) {
                 z--;
                 if(z==0) break;
         }
    }
  } else {
   // No Sorting
    for (map<string,int>::iterator i=c.begin();i!=c.end();i++) {
         cout << i->second << " " << i->first << endl;
         if(l) {
                 z--;
                 if(z==0) break;
         }
    }
}

return 0;

}


