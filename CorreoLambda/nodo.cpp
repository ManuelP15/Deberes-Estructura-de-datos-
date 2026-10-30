#include <iostream>
#include <cctype>
#include <fstream>
#include <sstream>
#include <functional>
#include "nodo.h"
using namespace std;

Nodo::Nodo() {
    cedula = string();
    nombre = string();
    apellido = string();
    correo = string();
    siguiente = nullptr;
    anterior = nullptr;
}

Nodo::Nodo(string cedula, string nombre, string apellido) {
    this->cedula = cedula;
    this->nombre = nombre;
    this->apellido = apellido;
    this->correo = string();
    siguiente = nullptr;
    anterior = nullptr;
}

Nodo::Nodo(string cedula, string nombre, string apellido, string correo) {
    this->cedula = cedula;
    this->nombre = nombre;
    this->apellido = apellido;
    this->correo = correo;
    siguiente = nullptr;
    anterior = nullptr;
}

Nodo::~Nodo() {

}

string Nodo::getCedula() { 
    return cedula; 
}
void Nodo::setCedula(string newCedula) { 
    cedula = newCedula; 
}
string Nodo::getNombre() { 
    return nombre; 
}
void Nodo::setNombre(string newNombre) { 
    nombre = newNombre; 
}
string Nodo::getApellido() { 
    return apellido;
 }
void Nodo::setApellido(string newApellido) { 
    apellido = newApellido; 
}
string Nodo::getCorreo() {
    return correo;
}
void Nodo::setCorreo(string newCorreo) {
    correo = newCorreo;
}
Nodo* Nodo::getSiguiente() { 
    return siguiente; 
}
void Nodo::setSiguiente(Nodo* newSiguiente) { 
    siguiente = newSiguiente; 
}
Nodo* Nodo::getAnterior() {
    return anterior;
}

void Nodo::setAnterior(Nodo* newAnterior) {
    anterior = newAnterior;
}

bool Nodo::guardarEnArchivo(string ruta) {
    ofstream archivo(ruta, ios::app);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo " << ruta << endl;
        return false;
    }
    archivo << cedula << " - " << nombre << " " << apellido << " - " << correo << endl;
    archivo.close();
    return true;
}
bool Nodo::eliminarDeArchivo(string ruta) {
    ifstream archivoEntrada(ruta);
    if (!archivoEntrada.is_open()) {
        cout << "No se pudo abrir el archivo " << ruta << endl;
        return false;
    }

    stringstream buffer;
    string linea;
    bool encontrado = false;
    string prefijo = cedula + " - ";

    while (getline(archivoEntrada, linea)) {
        if (cedula.empty() || linea.compare(0, prefijo.size(), prefijo) != 0) {
            buffer << linea << endl;
        } else {
            encontrado = true;
        }
    }
    archivoEntrada.close();

    ofstream archivoSalida(ruta);
    if (!archivoSalida.is_open()) {
        cout << "No se pudo abrir el archivo " << ruta << endl;
        return false;
    }

    archivoSalida << buffer.str();
    archivoSalida.close();

    return encontrado;
}

ListaDoble::ListaDoble() {
    cabeza = nullptr;
    cola = nullptr;
}

ListaDoble::~ListaDoble() {
    while (cabeza != nullptr) {
        Nodo* aux = cabeza;
        cabeza = cabeza->getSiguiente();
        delete aux;
    }
    cola = nullptr;
}

Nodo* ListaDoble::getCola() { 
    return cola;
 }
void ListaDoble::setCola(Nodo* newCola) { 
    cola = newCola;
 }
Nodo* ListaDoble::getCabeza() { 
    return cabeza; 
}
void ListaDoble::setCabeza(Nodo* newCabeza) { 
    cabeza = newCabeza;
}

void ListaDoble::insertar(string cedula, string nombre, string apellido) {
    string correo = generarCorreo(nombre, apellido);
    Nodo* nuevo = new Nodo(cedula, nombre, apellido, correo);
    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        nuevo->setAnterior(cola);
        cola->setSiguiente(nuevo);
        cola = nuevo;
    }
    nuevo->guardarEnArchivo("registros.txt");
}

void ListaDoble::imprimir() {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        cout << aux->getCedula() << " - "
             << aux->getNombre() << " "
             << aux->getApellido() << " - "
             << aux->getCorreo() << endl;
        aux = aux->getSiguiente();
    }
}
Nodo* ListaDoble::buscar(string cedula) {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        if (aux->getCedula() == cedula) return aux;
        aux = aux->getSiguiente();
    }
    return nullptr;

}

bool ListaDoble::borrar(string cedula) {
    Nodo* aux = buscar(cedula);

    if (aux == nullptr) {
        return false;
    }
    aux->eliminarDeArchivo("registros.txt");

    if (aux->getAnterior() != nullptr) {
        aux->getAnterior()->setSiguiente(aux->getSiguiente());
    } else {
        cabeza = aux->getSiguiente();
    }

    if (aux->getSiguiente() != nullptr) {
        aux->getSiguiente()->setAnterior(aux->getAnterior());
    } else {
        cola = aux->getAnterior();
    }

    delete aux;
    return true;
}
void ListaDoble::imprimirReversa() {
    Nodo* aux = cola;

    while (aux != nullptr) {
        cout << aux->getCedula() << " - "
             << aux->getNombre() << " "
             << aux->getApellido() << " - "
             << aux->getCorreo() << endl;

        aux = aux->getAnterior();
    }
}
static string convertirMinusculas(string texto) {
    for (size_t i = 0; i < texto.length(); i++) {
        texto[i] = tolower((unsigned char)texto[i]);
    }
    return texto; 

}
static string primerApellido(string apellido) {
    size_t posEspacio = apellido.find(' ');
    return (posEspacio == string::npos) ? apellido : apellido.substr(0, posEspacio);
}
static bool coincideApellido(string apellidoGuardado, string buscado) {
    apellidoGuardado = convertirMinusculas(apellidoGuardado);
    if (apellidoGuardado == buscado) return true;

    stringstream ss(apellidoGuardado);
    string palabra;
    while (ss >> palabra) {
        if (palabra == buscado) return true;
    }
    return false;
}

int ListaDoble::buscarPorApellido(string apellido){
    int encontrados = 0;
    string buscado = convertirMinusculas(apellido);
    Nodo* aux = cabeza;
    while (aux != nullptr){
        if(coincideApellido(aux->getApellido(), buscado)){
            cout << aux->getCedula() << " - " << aux->getNombre() << " " << aux->getApellido() << endl;
            encontrados++;
            
        }
        aux = aux->getSiguiente();
    }

    return encontrados;
}

bool ListaDoble::modificarRegistro(string cedulaActual, string nuevaCedula, string nuevoNombre, string nuevoApellido) {
    Nodo* nodo = buscar(cedulaActual);
    if (nodo == nullptr) {
        return false;
    }

    if (nuevaCedula != cedulaActual && buscar(nuevaCedula) != nullptr) {
        return false;
    }

    bool cambioNombre = (nodo->getNombre() != nuevoNombre || nodo->getApellido() != nuevoApellido);

    nodo->eliminarDeArchivo("registros.txt");

    nodo->setCedula(nuevaCedula);
    nodo->setNombre(nuevoNombre);
    nodo->setApellido(nuevoApellido);

    if (cambioNombre) {
        nodo->setCorreo("");
        nodo->setCorreo(generarCorreo(nuevoNombre, nuevoApellido));
    }
    nodo->guardarEnArchivo("registros.txt");

    return true;
}

bool ListaDoble::existeCorreo(string correo) {
    Nodo* aux = cabeza;
    while (aux != nullptr) {
        if (aux->getCorreo() == correo) return true;
        aux = aux->getSiguiente();
    }
    return false;
}

string ListaDoble::generarCorreo(string nombre, string apellido) {
    if (nombre.empty() || apellido.empty()) {
        return "";
    }

    string base = string(1, nombre[0]);

    size_t espacio = nombre.find(' ');
    if (espacio != string::npos && espacio + 1 < nombre.length()) {
        base += nombre[espacio + 1];
    }
    base += primerApellido(apellido);

    function<string(const string&, size_t)> alternar =
        [&alternar](const string& texto, size_t i) -> string {
            if (i >= texto.size()) {           
                return "";
            }

            const int DIFERENCIA = 'a' - 'A';  
            char letra = texto[i];

            if (i % 2 == 0) {                   
                if (letra >= 'a' && letra <= 'z') letra = letra - DIFERENCIA;
            } else {                            
                if (letra >= 'A' && letra <= 'Z') letra = letra + DIFERENCIA;
            }

            return string(1, letra) + alternar(texto, i + 1);   
        };

    base = alternar(base, 0);

    string correo = base + "@espe.edu.ec";

    int contador = 0;
    while (existeCorreo(correo)) {
        contador++;
        correo = base + to_string(contador) + "@espe.edu.ec";
    }

    return correo;
}
