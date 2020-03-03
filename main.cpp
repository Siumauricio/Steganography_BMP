#include <iostream>
#include "Bitmap.h"
#include <bitset>
#include <string>
int main (int argc,char ** argv) {
    string palabra;
    for (int i = 0; i < argc; ++i){
        std::string word = argv[i];
        if(word=="-o"){
            for (int j = i+2; j < argc; ++j) {
                palabra+=argv[j];
                palabra+=" ";
            }
            Bitmap bmp (argv[2],palabra, 1);
        }else if(word=="-e"){
            Bitmap bmp (argv[2],palabra, 2);
        }
}
    return 0;
}
