#pragma once
#ifndef BMP_H
#define BMP_H
#include <vector>
using namespace std;
#include <string>
#include <iostream>

#pragma pack(push,1)
class BMP_Header {
public:
    char Tipo[2];
    int Tamano;
    int Reservado;
    int OffsetData;
};
#pragma pack(pop)

#pragma pack(push,1)
class BMP_InfoHeader {
public:
    int Tamano;
    int Anchura;
    int Altura;
    char Planes[2];
    char ContadorBits[2];
    int Compresion;
    int TamanoImagen;
    int Pixeles_X;
    int Pixeles_Y;
    int Colores_Usados;
    int Colores_Importantes;
};
#pragma pack(pop)

struct RGB24B {
    uint8_t r, g, b;
};
class Bitmap {
public:
    BMP_Header Header;
    BMP_InfoHeader InfoHeader;
    Bitmap ();
    Bitmap (string,int);
    void ObtenerBmp_Header (string);
    void ObtenerBmp_InfoHeader (string,int);

    vector<RGB24B>Colores24;
    vector<char>Cadena;
    int posCadena = 0;
    int posTexto = 0;
    string decimaltoBinary (int);
    vector<string> stringtoBinary (string);
    void Swap_RGB_String_R (string&, string);
    void Swap_RGB_String_G (string&, string);

    void Swap_RGB_String_B (string&, string);
    uint8_t getDecimalFromBinary (string);

    void getLastIndex (string);
    string getTextFromBinary (string);
    void EncryptMessage (string, string);
    void DecryptMessage ();


    int getTipo ();
};
#endif // !BITMAP_H
