#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;
#include "nodo.h"

const size_t MAX_TEXTO = 50;

struct FinEntrada {};

string recortar(const string& texto) {
    size_t inicio = 0;
    while (inicio < texto.size() && isspace((unsigned char)texto[inicio])) inicio++;
    size_t fin = texto.size();
    while (fin > inicio && isspace((unsigned char)texto[fin - 1])) fin--;
    return texto.substr(inicio, fin - inicio);
}

void leerLinea(string& destino) {
    if (!getline(cin, destino)) {
        throw FinEntrada();
    }
    destino = recortar(destino);
}

bool esOpcionValida(const string& texto) {
    if (texto.empty() || texto.size() > 2) return false;
    for (char c : texto) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

bool validarCedula(string cedula) {
    if(cedula.length() != 10) {
        return false;
    }

    for (char digito : cedula) {
        if (!(digito >= '0' && digito <= '9')) {
            return false;
        }
    }

    int provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
    if (!((provincia >= 1 && provincia <= 24) || provincia == 30)) {
        return false;
    }

    if (cedula[2] - '0' > 5) {
        return false;
    }

    int codigoVerificador = cedula[9] - '0';
    int suma = 0, digito, codigoVerificadorCalculado;
    for(int i = 0; i < 9; i++){
        digito = cedula[i] - '0';
        if(i % 2 == 0){
            digito *= 2;
            if(digito > 9){
                digito -= 9;
            }
        }
        suma += digito;
    }

    codigoVerificadorCalculado = suma%10;
    if(codigoVerificadorCalculado != 0){
        codigoVerificadorCalculado = 10 - codigoVerificadorCalculado;
    }


    return codigoVerificador == codigoVerificadorCalculado;
}

bool validarLetras(string texto) {
    if (texto.empty()) {
        return false;
    }

    for (char letra : texto) {
        if (!((letra >= 'A' && letra <= 'Z') ||
              (letra >= 'a' && letra <= 'z'))) {
            return false;
        }
    }

    return true;
}

bool validarNombres(string texto) {
    if (texto.empty() || texto.size() > MAX_TEXTO || texto.front() == ' ' || texto.back() == ' ') {
        return false;
    }

    bool espacioPrevio = false;
    for (char letra : texto) {
        if (letra == ' ') {
            if (espacioPrevio) return false;
            espacioPrevio = true;
        } else if ((letra >= 'A' && letra <= 'Z') || (letra >= 'a' && letra <= 'z')) {
            espacioPrevio = false;
        } else {
            return false;
        }
    }

    return true;
}

int main() {
    ListaDoble lista;
    int opcion;
    string cedula, nombre, apellido;

    try {
    do {
        cout << "\n===== MENU =====" << endl;
        cout << "1. Insertar" << endl;
        cout << "2. Imprimir" << endl;
        cout << "3. Buscar" << endl;
        cout << "4. Borrar" << endl;
        cout << "5. Imprimir Reversa" << endl;
        cout << "6. Buscar por Apellido" << endl;
        cout << "7. Modificar registro" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";

        string entradaOpcion;
        leerLinea(entradaOpcion);
        if (!esOpcionValida(entradaOpcion)) {
            cout << "Error, ingresar solo numeros" << endl;
            opcion = -1;
            continue;
        }
        opcion = stoi(entradaOpcion);
        
        switch (opcion) {
        case 1:
        do {
            cout << "Cedula: ";
            leerLinea(cedula);
            if (!validarCedula(cedula)) {
                cout << "Error, ingrese una cedula valida" << endl;
            }else if(lista.buscar(cedula) != nullptr) {
                cout << "Error. esa cedula ya esta registrada" << endl;
            }
        }while (!validarCedula(cedula) || lista.buscar(cedula) != nullptr);
            do {
                cout << "Nombres: ";
              leerLinea(nombre);

                if (!validarNombres(nombre)) {
                    cout << "Error, solo letras sin tildes ni enie (maximo 50), separadas por un espacio" << endl;
                }

            } while (!validarNombres(nombre));
            do {
                cout << "Apellido: ";  
              leerLinea(apellido);
               if (!validarNombres(apellido)) {
                    cout << "Error, solo letras sin tildes ni enie (maximo 50), separadas por un espacio" << endl;
                }

            } while (!validarNombres(apellido));

            lista.insertar(cedula, nombre, apellido);
            cout << "Registrado correctamente." << endl;
            cout << "Correo generado: " << lista.getCola()->getCorreo() << endl;
            break;

            case 2:
                if (lista.getCabeza() == nullptr)
                    cout << "La lista esta vacia." << endl;
                else
                    lista.imprimir();
                break;

            case 3: {
                if(lista.getCabeza()== nullptr){
                    cout << "La lista esta vacia."<< endl;
                    break;
                }
                cout << "Cedula a buscar: ";
                 leerLinea(cedula);
                 if(!validarCedula(cedula)){
                    cout <<"Cedula Invalida"<<endl;
                    break;
                 }
                Nodo* encontrado = lista.buscar(cedula);
                if (encontrado != nullptr)
                    cout << "Encontrado: " << encontrado->getNombre()
                         << " " << encontrado->getApellido() << endl;
                else
                    cout << "No existe esa cedula." << endl;
                break;
            }

            case 4:
            if (lista.getCabeza() == nullptr) {              
                    cout << "La lista esta vacia." << endl;
                    break;
                }
                cout << "Cedula a borrar: ";
              leerLinea(cedula);
              if (!validarCedula(cedula)) {                    
                    cout << "Cedula invalida." << endl;
                    break;
                }

                if (lista.borrar(cedula))
                    cout << "Borrado." << endl;
                else
                    cout << "No existe esa cedula." << endl;
                    

                break;
            case 5:
                if (lista.getCabeza() == nullptr) {
                    cout << "La lista esta vacia." << endl;
                } else {
                    cout << "\nLista en reversa:" << endl;
                    lista.imprimirReversa();
                }
                break;
            case 6:{
                int total = 0;
                if(lista.getCabeza()== nullptr){
                    cout << "La lista esta vacia" << endl;
                    break;
                }
                cout << "Apellido a buscar : "; 
                leerLinea(apellido);
                if (!validarNombres(apellido)) {
                    cout << "Apellido invalido, ingrese solo letras sin tildes ni enie." << endl;
                    break;
                }
                total = lista.buscarPorApellido(apellido);
                if (total == 0){
                    cout << "No hay personas con ese apellido registrado" << endl;
                }else {
                    cout << "Total encontrados: " << total << endl;
                }
                break;
            }
            case 7: {
                if (lista.getCabeza() == nullptr) {
                    cout << "La lista esta vacia." << endl;
                    break;
                }
                cout << "Cedula del registro a modificar: ";
                leerLinea(cedula);
                if (!validarCedula(cedula)) {
                    cout << "Cedula invalida." << endl;
                    break;
                }
                Nodo* actual = lista.buscar(cedula);
                if (actual == nullptr) {
                    cout << "No existe esa cedula." << endl;
                    break;
                }

                cout << "Registro actual: " << actual->getCedula() << " - "
                     << actual->getNombre() << " " << actual->getApellido() << endl;
                cout << "(Presione Enter para dejar un campo sin cambios)" << endl;

                string nuevaCedula, nuevoNombre, nuevoApellido;

                while (true) {
                    cout << "Nueva cedula: ";
                    leerLinea(nuevaCedula);
                    if (nuevaCedula.empty()) {
                        nuevaCedula = cedula;
                        break;
                    }
                    if (!validarCedula(nuevaCedula)) {
                        cout << "Error, ingrese una cedula valida" << endl;
                    } else if (nuevaCedula != cedula && lista.buscar(nuevaCedula) != nullptr) {
                        cout << "Error. esa cedula ya esta registrada" << endl;
                    } else {
                        break;
                    }
                }

                while (true) {
                    cout << "Nuevo nombre: ";
                    leerLinea(nuevoNombre);
                    if (nuevoNombre.empty()) {
                        nuevoNombre = actual->getNombre();
                        break;
                    }
                    if (validarNombres(nuevoNombre)) break;
                    cout << "Error, solo letras sin tildes ni enie (maximo 50), separadas por un espacio" << endl;
                }

                while (true) {
                    cout << "Nuevo apellido: ";
                    leerLinea(nuevoApellido);
                    if (nuevoApellido.empty()) {
                        nuevoApellido = actual->getApellido();
                        break;
                    }
                    if (validarNombres(nuevoApellido)) break;
                    cout << "Error, solo letras sin tildes ni enie (maximo 50), separadas por un espacio" << endl;
                }

                if (lista.modificarRegistro(cedula, nuevaCedula, nuevoNombre, nuevoApellido))
                    cout << "Registro modificado correctamente." << endl;
                else
                    cout << "No se pudo modificar el registro." << endl;
                break;
            }
            case 0:
                cout << "Salir" << endl;
                break;

            default:
                cout << "Opcion invalida." << endl;
        }
    } while (opcion != 0);
    } catch (const FinEntrada&) {
        cout << "\nSe cerro la entrada. Saliendo del programa." << endl;
    }

    return 0;
}
