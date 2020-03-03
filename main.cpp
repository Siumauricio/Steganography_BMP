#include <iostream>
#include "Bitmap.h"
#include <bitset>
int main (int argc, char** argv) {
    bool cerrar = true;
    int opcion = 0;
    cout << "You have entered " << argc
        << " arguments:" << "\n";

    for (int i = 0; i < argc; ++i)
        cout << argv[i] << "\n";

    do {
        cin >> opcion;
        if (opcion==1) {
            Bitmap bmp ("C://Users//Mauricio//Desktop//Imagenes//cuadro.bmp", 1);
        } else if (opcion == 2) {
            Bitmap bmp ("C://Users//Mauricio//Desktop//Imagenes//cuadro.bmp", 2);

        } else if (opcion == -1) {
            break;
        }
    } while (cerrar);
   // cout<<bmp.getTexto ();
    return 0;
}
