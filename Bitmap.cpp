//
// Created by siumauricio on 28/2/20.


#include "Bitmap.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <dirent.h>
#include <string.h>
#include <sstream>
#include <stdlib.h>

#include <bitset>

#include <stdio.h>
using namespace std;

Bitmap::Bitmap () {}

Bitmap::Bitmap (string Filename) {
    cout<<"\n";
    ObtenerBmp_Header(Filename);
    ObtenerBmp_InfoHeader(Filename);
}

void Bitmap::ObtenerBmp_Header (string Filename) {
    ifstream File;
    File.open (Filename, ifstream::in | ifstream::binary);
    if (File.fail ()) {
        return;
    }
    char* Header_Posiciones = new char[sizeof (BMP_Header)];
    File.read (Header_Posiciones, sizeof (BMP_Header));
    char informacion[4];

    copy (Header_Posiciones + 0, Header_Posiciones + 2, informacion);
    memcpy (&Header.Tipo, informacion, 2);

    copy (Header_Posiciones + 2, Header_Posiciones + 6, informacion);
    memcpy (&Header.Tamano, informacion, 4);

    copy (Header_Posiciones + 10, Header_Posiciones + 14, informacion);
    memcpy (&Header.OffsetData, informacion, 4);

    cout << "==== BMP HEADER ====" << endl;
    cout << "+ Tipo: " << Header.Tipo[0] << Header.Tipo[1] << endl;
    cout << "+ Tamano Archivo: " << Header.Tamano << endl;
    cout << "+ Espacio Reservado: " << sizeof (Header.Reservado) << endl;
    cout << "+ DataOffset: " << Header.OffsetData << endl;

    cout << endl;
    File.close ();
}

void Bitmap::ObtenerBmp_InfoHeader (string Filename) {
    ifstream File;
    File.open (Filename, ifstream::in | ifstream::binary);
    if (File.fail ()) {
        return;
    }
    char* Info_Posiciones = new char[sizeof (BMP_InfoHeader) + sizeof (BMP_Header)];
    File.read (Info_Posiciones, sizeof (BMP_InfoHeader) + sizeof (BMP_Header));
    char informacion[4];

    copy (Info_Posiciones + 18, Info_Posiciones + 22, informacion);
    memcpy (&InfoHeader.Anchura, informacion, 4);

    copy (Info_Posiciones + 22, Info_Posiciones + 26, informacion);
    memcpy (&InfoHeader.Altura, informacion, 4);

    copy (Info_Posiciones + 26, Info_Posiciones + 28, informacion);
    memcpy (&InfoHeader.Planes, informacion, 2);

    copy (Info_Posiciones + 28, Info_Posiciones + 30, informacion);
    memcpy (&InfoHeader.ContadorBits, informacion, 2);

    copy (Info_Posiciones + 30, Info_Posiciones + 34, informacion);
    memcpy (&InfoHeader.Compresion, informacion, 4);

    copy (Info_Posiciones + 34, Info_Posiciones + 38, informacion);
    memcpy (&InfoHeader.TamanoImagen, informacion, 4);

    copy (Info_Posiciones + 38, Info_Posiciones + 42, informacion);
    memcpy (&InfoHeader.Pixeles_X, informacion, 4);

    copy (Info_Posiciones + 42, Info_Posiciones + 46, informacion);
    memcpy (&InfoHeader.Pixeles_Y, informacion, 4);

    copy (Info_Posiciones + 46, Info_Posiciones + 50, informacion);
    memcpy (&InfoHeader.Colores_Usados, informacion, 4);

    copy (Info_Posiciones + 50, Info_Posiciones + 54, informacion);
    memcpy (&InfoHeader.Colores_Importantes, informacion, 4);

    cout << "==== BMP INFOHEADER ====" << endl;
    cout << "+ Tamano: " << 40<< endl;
    cout << "+ Anchura: " << InfoHeader.Anchura << endl;
    cout << "+ Altura: " << InfoHeader.Altura << endl;
    cout << "+ Planos: " << (int)InfoHeader.Planes[0] << endl;
    cout << "+ Contador Bits: " << (int)InfoHeader.ContadorBits[0] << endl;
    cout << "+ Compresion: " << InfoHeader.Compresion << endl;
    cout << "+ Tamano Imagen: " << InfoHeader.TamanoImagen << endl;
    cout << "+ Pixel X: " << InfoHeader.Pixeles_X << endl;
    cout << "+ Pixel Y: " << InfoHeader.Pixeles_Y << endl;
    cout << "+ Colores U: " << InfoHeader.Colores_Usados << endl;
    cout << "+ Colores I: " << InfoHeader.Colores_Importantes << endl;
    File.seekg (Header.OffsetData, File.beg);
    ofstream File2(Filename, ifstream::in | ifstream::binary);
    File2.seekp(Header.OffsetData, File.beg);
    int contador=0;
    int posV=0;
    vector<string>cadenas=stringtoBinary("AO");
    cout<<"\n";
        for (int row = 0; row < InfoHeader.Altura; row++){
            for (int col = 0; col < InfoHeader.Anchura; col++) {
                RGB24B  Pixel;
                File.read (reinterpret_cast<char*>(&Pixel), sizeof (RGB24B));
               Colores24.push_back (Pixel);
            }
            File.seekg(InfoHeader.Anchura % 4, std::ios::cur);
        }
        int i=0;

    for (int row = 0; row < InfoHeader.Altura; row++){
        for (int col = 0; col < InfoHeader.Anchura; col++) {
            string R=decimaltoBinary((int)Colores24[i].r);
            string G=decimaltoBinary((int)Colores24[i].g);
            string B=decimaltoBinary((int)Colores24[i].b);

            if(posCadena!=8){
                Swap_RGB_String_R(R,cadenas[posV]);
                int R_=( int) getDecimalFromBinary(R);
                File2.write(reinterpret_cast<char*>(&R_),1);

            }
            if(posCadena!=8){
                Swap_RGB_String_G(G,cadenas[posV]);

                int G_=( int) getDecimalFromBinary(G);
                File2.seekp(Header.OffsetData+(++contador),File.beg);
                File2.write(reinterpret_cast<char*>(&G_),1);
            }else{
                cout<<"FInal "<<posCadena<<endl;
                return;
            }

            if(posCadena!=8){
                Swap_RGB_String_B(B,cadenas[posV]);
                int B_=( int) getDecimalFromBinary(B);
                File2.seekp(Header.OffsetData+(++contador),File.beg);
                File2.write(reinterpret_cast<char*>(&B_),1);
                File2.seekp(Header.OffsetData+(++contador),File.beg);
            }else{
                posCadena=0;
                Swap_RGB_String_B(B,cadenas[++posV]);
                int B_=( int) getDecimalFromBinary(B);
                File2.seekp(Header.OffsetData+(++contador),File.beg);
                File2.write(reinterpret_cast<char*>(&B_),1);
                File2.seekp(Header.OffsetData+(++contador),File.beg);
                //return;
            }
            i++;
        }
        i++;
        }

    File.close ();
}

string Bitmap::decimaltoBinary(int decimal){
    std::bitset<8> bin_x(decimal);
    string digito=bin_x.to_string();
    return digito;
}

int Bitmap::getTipo() {
    int bit = InfoHeader.ContadorBits[0];
    return bit;
}

vector<string> Bitmap::stringtoBinary(string myString) {
    vector<string>palabras;
    for (std::size_t i = 0; i < myString.size(); ++i)
    {
        palabras.push_back(bitset<8>(myString[i]).to_string());
    }
    return palabras;
}



uint8_t Bitmap::getDecimalFromBinary(string s) {
    std::bitset<sizeof(int) * 8> b(s);
    uint8_t x=b.to_ulong() ;
    return (int)x;
}

void Bitmap::Swap_RGB_String_R(string &R,string palabra) {
    R[R.length()-1]=palabra.at(posCadena++);
}

void Bitmap::Swap_RGB_String_G(string & G, string palabra) {
    G[G.length()-1]=palabra.at(posCadena++);
}
void Bitmap::Swap_RGB_String_B(string &B, string palabra) {
    B[B.length()-1]=palabra.at(posCadena++);
}



/*
                cout<<(int)Pixel.r<<" "<<(int)Pixel.g<<" "<<(int)Pixel.b<<endl;

                string R=decimaltoBinary((int)Pixel.r);
                string G=decimaltoBinary((int)Pixel.g);
                string B=decimaltoBinary((int)Pixel.b);



                if(posCadena!=8){
                    cout<<File2.tellp()<<endl;

                    Swap_RGB_String_R(R,cadenas[posV]);
                    int R_=( int) getDecimalFromBinary(R);
                    cout<<R_<<endl;
                    File2.write(reinterpret_cast<char*>(&R_),1);

                }
                if(posCadena!=8){
                    Swap_RGB_String_G(G,cadenas[posV]);

                    int G_=( int) getDecimalFromBinary(G);
                    File2.seekp(Header.OffsetData+(++contador),File.beg);

                    File2.write(reinterpret_cast<char*>(&G_),1);

                }

                if(posCadena!=8){
                    Swap_RGB_String_B(B,cadenas[posV]);
                    int B_=( int) getDecimalFromBinary(B);
                    File2.seekp(Header.OffsetData+(++contador),File.beg);
                    File2.write(reinterpret_cast<char*>(&B_),1);
                    File2.seekp(Header.OffsetData+(++contador),File.beg);
                }else{
                    return;
                }*/
