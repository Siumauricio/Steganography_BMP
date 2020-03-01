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

Bitmap::Bitmap (string Filename,int opcion) {
    cout << "\n";
    ObtenerBmp_Header (Filename);
    ObtenerBmp_InfoHeader (Filename,opcion);
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

    copy (Header_Posiciones + 6, Header_Posiciones + 10, informacion);
    memcpy (&Header.Reservado, informacion, 4);

    copy (Header_Posiciones + 10, Header_Posiciones + 14, informacion);
    memcpy (&Header.OffsetData, informacion, 4);

    cout << "==== BMP HEADER ====" << endl;
    cout << "+ Tipo: " << Header.Tipo[0] << Header.Tipo[1] << endl;
    cout << "+ Tamano Archivo: " << Header.Tamano << endl;
    cout << "+ Espacio Reservado: " << (Header.Reservado) << endl;
    cout << "+ DataOffset: " << Header.OffsetData << endl;

    cout << endl;
    File.close ();
}

void Bitmap::ObtenerBmp_InfoHeader (string Filename,int opcion) {
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
    cout << "+ Tamano: " << 40 << endl;
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

    cout << "\n";
    if (opcion == 1) {
        for (int row = 0; row < InfoHeader.Altura; row++) {
            for (int col = 0; col < InfoHeader.Anchura; col++) {
                RGB24B  Pixel;
                File.read (reinterpret_cast<char*>(&Pixel), sizeof (RGB24B));
                Colores24.push_back (Pixel);
            }
            File.seekg (InfoHeader.Anchura % 4, std::ios::cur);

        }
        string texto = "Chile continental, onas geográficas. La primera de ellas, Chile continental, comprende una franja en la coss. La primera de ellas, Chile continental, comprende una franja  ";
        EncryptMessage (Filename, texto);
    }else{
        cout << "*************************************" << endl;
        cout << "Su Texto se esta desencryptando...!" << endl;
        cout << "*************************************\n\n" << endl;

        for (int row = 0; row < InfoHeader.Altura; row++) {
            for (int col = 0; col < InfoHeader.Anchura; col++) {
                RGB24B  Pixel;
                File.read (reinterpret_cast<char*>(&Pixel), sizeof (RGB24B));
                getLastIndex (decimaltoBinary ((int)Pixel.r));
                getLastIndex (decimaltoBinary ((int)Pixel.g));
                getLastIndex (decimaltoBinary ((int)Pixel.b));
                posTexto+=3;
                if (posTexto>=(Header.Reservado*8)) {
                    DecryptMessage ();
                    return;
                }
                Colores24.push_back (Pixel);
            }
            File.seekg (InfoHeader.Anchura % 4, std::ios::cur);
        }
    }
    File.close ();
}

string Bitmap::decimaltoBinary (int decimal) {
    std::bitset<8> bin_x (decimal);
    string digito = bin_x.to_string ();
    return digito;
}

int Bitmap::getTipo () {
    int bit = InfoHeader.ContadorBits[0];
    return bit;
}

vector<string> Bitmap::stringtoBinary (string myString) {
    vector<string>palabras;
    for (std::size_t i = 0; i < myString.size (); ++i) {
        palabras.push_back (bitset<8> (myString[i]).to_string ());
    }
    return palabras;
}

uint8_t Bitmap::getDecimalFromBinary (string s) {
    std::bitset<sizeof (int) * 8> b (s);
    uint8_t x = b.to_ulong ();
    return (int)x;
}

void Bitmap::Swap_RGB_String_R (string& R, string palabra) {
    R[R.length () - 1] = palabra.at (posCadena++);
}

void Bitmap::Swap_RGB_String_G (string& G, string palabra) {
    G[G.length () - 1] = palabra.at (posCadena++);
}
void Bitmap::Swap_RGB_String_B (string& B, string palabra) {
    B[B.length () - 1] = palabra.at (posCadena++);
}

void Bitmap::EncryptMessage (string path, string texto) {
  
    if (((InfoHeader.Altura* InfoHeader.Anchura)/8)<(double)texto.length()) {
        cout << "No hay suficientes pixeles para representar su mensaje completo" << endl;
        cout << "Elimine: " << (double)texto.length () - ((InfoHeader.Altura * InfoHeader.Anchura) / 8) << " Caracteres" << endl;
        double total_pixels = ((InfoHeader.Altura * InfoHeader.Anchura) / 8) * (double)texto.length ();
        double div1 = ((InfoHeader.Altura * InfoHeader.Anchura) / 8);
        cout << "Se necesita una imagen de " << total_pixels / div1 << " total pixeles para representar su mensaje completamente" << endl;
        return;
    }

    int canT = texto.length ();
    vector<string>cadenas = stringtoBinary (texto);
    int contador = 0;
    int posV = 0;
    int i = 0;
    ofstream File2 (path, ifstream::in | ifstream::binary);
    File2.seekp (6,File2.beg);
    File2.write (reinterpret_cast<char*>(&canT), 4);
    File2.seekp (Header.OffsetData, File2.beg);
    for (int row = 0; row < InfoHeader.Altura; row++) {
        for (int col = 0; col < InfoHeader.Anchura; col++) {
            string R = decimaltoBinary ((int)Colores24[i].r);
            string G = decimaltoBinary ((int)Colores24[i].g);
            string B = decimaltoBinary ((int)Colores24[i].b);

            if (posCadena != 8) {
                Swap_RGB_String_R (R, cadenas[posV]);
                int R_ = (int)getDecimalFromBinary (R);
                File2.write (reinterpret_cast<char*>(&R_), 1);

            } else {
                if ((posV + 1) < cadenas.size ()) {
                    posCadena = 0;
                    Swap_RGB_String_R (R, cadenas[++posV]);
                    int R_ = (int)getDecimalFromBinary (R);
                    File2.write (reinterpret_cast<char*>(&R_), 1);
                } else {
                    return;
                }

            }
            if (posCadena != 8) {
                Swap_RGB_String_G (G, cadenas[posV]);
                int G_ = (int)getDecimalFromBinary (G);
                File2.seekp (Header.OffsetData + (++contador), File2.beg);
                File2.write (reinterpret_cast<char*>(&G_), 1);
            } else {
                if ((posV + 1) < cadenas.size ()) {
                    posCadena = 0;
                    Swap_RGB_String_G (G, cadenas[++posV]);
                    int G_ = (int)getDecimalFromBinary (G);
                    File2.seekp (Header.OffsetData + (++contador), File2.beg);
                    File2.write (reinterpret_cast<char*>(&G_), 1);
                } else {
                    return;
                }

            }

            if (posCadena != 8) {
                Swap_RGB_String_B (B, cadenas[posV]);
                int B_ = (int)getDecimalFromBinary (B);
                File2.seekp (Header.OffsetData + (++contador), File2.beg);
                File2.write (reinterpret_cast<char*>(&B_), 1);
                File2.seekp (Header.OffsetData + (++contador), File2.beg);
            } else {
                if ((posV + 1) < cadenas.size ()) {
                    posCadena = 0;
                    Swap_RGB_String_B (B, cadenas[++posV]);
                    int B_ = (int)getDecimalFromBinary (B);
                    File2.seekp (Header.OffsetData + (++contador), File2.beg);
                    File2.write (reinterpret_cast<char*>(&B_), 1);
                    File2.seekp (Header.OffsetData + (++contador), File2.beg);

                } else {
                    return;
                }
                //return;
            }
            i++;
        }
    }
    cout << "Su texto ha sido Encryptado Correctamente....!" << endl;
    File2.close ();
}

void Bitmap::DecryptMessage () {
    int can = 0;
    for (int i = 8; i < Cadena.size (); i += 8) {
        string a (&Cadena[can], &Cadena[i]);
        string texto = getTextFromBinary (a);
        if (texto[0] == '#') {
           return;
        }
        for (int j = 0; j < a.length () - 7; j++) {
            cout << texto[j];
        }
        can += 8;
    }
    cout << "\n";

}

void Bitmap::getLastIndex (string data) {
    std::bitset<8> bits (data[data.length () - 1]);
    Cadena.push_back (char (bits.to_ullong ()));
}

string Bitmap::getTextFromBinary (string data) {
    std::stringstream sstream (data);
    std::string output;
    while (sstream.good ()) {
        std::bitset<8> bits;
        sstream >> bits;
        char c = char (bits.to_ulong ());
        output += c;


    }

    return output;

}

/*
bool isBitSet (char ch, int pos) {
    //7 6 5 4 3 2 1 0
    //0 0 0 0 0 0 0 1
    ch = ch >> pos;
    if (ch & 1)
        return true;
    return false;
}*/
