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

Bitmap::Bitmap () {}

Bitmap::Bitmap (string Filename,string txt,int opcion, int subopc) {
    string Path="//home//siumauricio//Escritorio//Imagenes//"+Filename;
    ObtenerBmp_Header (Path);
    ObtenerBmp_InfoHeader (Path,txt,opcion,subopc);
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

    File.close ();
}

void Bitmap::ObtenerBmp_InfoHeader (string Filename,string txt,int opcion,int subop) {
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
    File.seekg (Header.OffsetData, File.beg);

    if(opcion==4){
        if((int)InfoHeader.ContadorBits[0]!=24){
            cout<<"100: Imagen no valida para ocultar el mensaje!"<<endl;

        }if(Header.Reservado!=0){
           cout<<"101: Imagen ya contiene un mensaje de "<< Header.Reservado<<" caracteres"<<endl;
        }
        return;
    }
    if((int)InfoHeader.ContadorBits[0]!=24){
        cout<<"IMAGEN NO VALIDA!"<<endl;
        return;
    }

    if (opcion == 1) {
        if(Header.Reservado==0) {
            for (int row = 0; row < InfoHeader.Altura; row++) {
                for (int col = 0; col < InfoHeader.Anchura; col++) {
                    RGB24B  Pixel;
                    File.read (reinterpret_cast<char*>(&Pixel), sizeof (RGB24B));
                    Colores24.push_back (Pixel);
                }
            }
            if(subop==1){
                cout<<Colores24.size()<<endl;
                EncryptMessage (Filename,txt);

            }else{
                if (getTexto(txt)!="VACIO"){
                    EncryptMessage (Filename,getTexto(txt));
                    return;
                }else{
                    return;
                }
            }

        }else{
            cout<<"Error Este Archivo Posee un Texto!"<<endl;
            return;
        }


    }else if(opcion==2){
        if(Header.Reservado!=0) {
            if (subop == 1) {
                for (int row = 0; row < InfoHeader.Altura; row++) {
                    for (int col = 0; col < InfoHeader.Anchura; col++) {
                        RGB24B Pixel;
                        File.read(reinterpret_cast<char *>(&Pixel), sizeof(RGB24B));
                        getLastIndex(decimaltoBinary((int) Pixel.r));
                        getLastIndex(decimaltoBinary((int) Pixel.g));
                        getLastIndex(decimaltoBinary((int) Pixel.b));
                        posTexto += 3;
                        if (posTexto >= (Header.Reservado * 8)) {
                            pos = 0;
                            DecryptMessage();
                            return;
                        }
                        Colores24.push_back(Pixel);
                    }
                }
            } else {
                for (int row = 0; row < InfoHeader.Altura; row++) {
                    for (int col = 0; col < InfoHeader.Anchura; col++) {
                        RGB24B Pixel;
                        File.read(reinterpret_cast<char *>(&Pixel), sizeof(RGB24B));
                        getLastIndex(decimaltoBinary((int) Pixel.r));
                        getLastIndex(decimaltoBinary((int) Pixel.g));
                        getLastIndex(decimaltoBinary((int) Pixel.b));
                        posTexto += 3;
                        if (posTexto >= (Header.Reservado * 8)) {
                            pos = 1;
                            DecryptMessage();
                            EscribirEnArchivo(txt);
                            return;
                        }
                        Colores24.push_back(Pixel);
                    }
                }
            }
        }else {
            cout<<"LA IMAGEN ESTA VACIA!"<<endl;
        }
    } else if (opcion==3){
        cout<<cantidadPalabras()<<endl;
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
    return x;
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
    if ((texto.length()* 8) > (InfoHeader.Anchura *InfoHeader.Altura)*3) {
        cout<<"EL MENSAJE EXCEDE LO PERMITIDO"<<endl;
        return;
    }

    int canT = texto.length ();
    vector<string>cadenas = stringtoBinary (texto);
    int contador = 0;

    int posV = 0;
    ofstream File2 (path, ifstream::in | ifstream::binary);
    File2.seekp (6,File2.beg);
    File2.write (reinterpret_cast<char*>(&canT), 4);
    File2.seekp (Header.OffsetData, File2.beg);
    for (int i= 0; i < Colores24.size(); ++i) {

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
            }
    }
    File2.close ();
}

void Bitmap::DecryptMessage () {
    int can = 0;
    for (int i = 8; i < Cadena.size (); i += 8) {
        string a (&Cadena[can], &Cadena[i]);
        string texto = getTextFromBinary (a);
        for (int j = 0; j < a.length () - 7; j++) {
            if(pos==0){
                cout <<(char) texto[j];
            }else{
                txtArchivo+=texto[j];
            }
        }
        can += 8;
    }
}

string Bitmap::getTexto (string nombre) {
    if(nombre.substr(nombre.find_last_of(".") + 1) == "txt") {
    } else {
        return "VACIO";
    }
    ifstream a ("//home//siumauricio//Escritorio//"+nombre);

    std::string content((std::istreambuf_iterator<char>(a)), std::istreambuf_iterator<char>());
    a.close();
    return content;
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

void Bitmap::EscribirEnArchivo(string filename) {
    ofstream myfile;
    myfile.open (filename);
    myfile << txtArchivo;
    myfile.close();
}

double Bitmap::cantidadPalabras() {
    return (int )(InfoHeader.Anchura*InfoHeader.Altura*3)/8;
}
