#include <iostream>
#include "Bitmap.h"
#include <bitset>
#include <string>
int main (int argc,char ** argv) {
    string palabra;
    if(argc>=3){
        string word = argv[1];
        if (word == "-o"&& argc>=5) {
            std::string word2 = argv[3];
            if (word2 == "-m") {
                for (int j = 4; j < argc; ++j) {
                    palabra += argv[j];
                }
                Bitmap bmp(argv[2], palabra, 1, 1);
            } else if (word2 == "-f") {
                Bitmap bmp(argv[2], argv[4], 1, 2);
            }

        }
        if(word=="-e"){
            if(argc==5){
                string word2 = argv[3];
                if(word2=="-f"){
                    Bitmap bmp(argv[2], argv[4], 2, 2);
                }
            }else if(argc==3){
                Bitmap bmp(argv[2], palabra, 2, 1);
            }
        }

        if(word=="-t"&&argc==3){
            Bitmap bmp(argv[2], "", 3, 1);
        }
        if(word=="-v"&& argc==3){
            Bitmap bmp(argv[2], "", 4, 1);
        }
    }
    cout<<endl;
    return 0;
}
