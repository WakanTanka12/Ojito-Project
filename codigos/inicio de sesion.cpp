#include "inicio_sesion.h"
#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iomanip>

using namespace std;
using namespace nlohmann;

// Funcion para imprimir caracteres unicode
void printUnicodeText(const wchar_t* text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteConsoleW(hConsole, text, wcslen(text), &written, NULL);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// Función para registrar un nuevo usuario
void registrarUsuario() {
    ifstream entrada("data/datosDeUsuarios/usuarios.json");
    if (!entrada.is_open()) {
        cout << setw(41) << "No se pudo abrir el archivo";
        exit(1);
    }
    json usuarios;
    entrada >> usuarios;
    entrada.close();

    // Pedir el nombre completo
    string nombre;
    cout << setw(42) << "Ingrese su nombre completo: ";
    cin.ignore();
    getline(cin, nombre);

    // Pedir el usuario a registrar y verificar si el usuario ya existe
    string usuario;
    bool existe = true;
    while (existe) {
        cout << setw(44) << "Ingrese un nombre de usuario: ";
        getline(cin, usuario);
        existe = false;
        for (int i = 0; i < usuarios.size(); i++) {
            if (usuarios[i]["Usuario"].get<string>() == usuario) {
                cout << setw(34) << "El usuario ya existe" << endl;
                existe = true;
                break;
            }
        }
    }
    
    // Pedir la contraseña
    string contrasena;
    cout << setw(38) << "Ingrese una contrasena: ";
    getline(cin, contrasena);
    // Si la contraseña es menor a 8 caracteres
    while (contrasena.length() < 8) {
        cout << setw(83) << "La contrasena debe ser mayor a 8 caracteres. Ingrese una nuevamente: ";
        getline(cin, contrasena);
    }
    // Registrar el usuario
    json nuevo_elemento = {
        {"Nombre", nombre},
        {"Usuario", usuario},
        {"Contrasena", contrasena}
    };

    usuarios.push_back(nuevo_elemento);
    ofstream salida("data/datosDeUsuarios/usuarios.json");
    salida << usuarios.dump(4);
    salida.close();
    
    cout << endl << setw(43) << "Usuario registrado con exito.";
}

// Función para iniciar sesión
void iniciarSesion() {
    string usuario, contrasena;

    ifstream entrada("data/datosDeUsuarios/usuarios.json");
    if (!entrada.is_open()) {
        cout << setw(41) << "No se pudo abrir el archivo";
        exit(1);
    }

    json usuarios;
    entrada >> usuarios;
    bool incorrecto = true;
    while (incorrecto) {
        cout << setw(44) << "Ingrese su nombre de usuario: ";
        cin >> usuario;
        cout << setw(37) << "Ingrese su contrasena: ";
        cin >> contrasena;
        bool encontrado = false;
        for (int i = 0; i < usuarios.size(); i++) {
            if (usuarios[i]["Usuario"].get<string>() == usuario && usuarios[i]["Contrasena"].get<string>() == contrasena) {
                cout << setw(53) << "Inicio de sesion exitoso. ¡Bienvenido, " << usuario << "!" << endl;
                incorrecto = false;
                encontrado = true;
                return;
            }
        }
        if (!encontrado)
            cout << setw(47) << "Usuario o contrasenha incorrectos" << endl;
    }
}

// Función para manejar el menú completo
void menu_inicio_sesion() {
    int opcion;

    while (true) {
        setColor(11);
        wstringstream wss;
        wss << setw(63) << endl << endl;
        wss << setw(63) << L" ██████╗      ██╗██╗████████╗ ██████╗ \n";
        wss << setw(63) << L"██╔═══██╗     ██║██║╚══██╔══╝██╔═══██╗\n";
        wss << setw(63) << L"██║   ██║     ██║██║   ██║   ██║   ██║\n";
        wss << setw(63) << L"██║   ██║██   ██║██║   ██║   ██║   ██║\n";
        wss << setw(63) << L"╚██████╔╝╚█████╔╝██║   ██║   ╚██████╔╝\n";
        wss << setw(63) << L" ╚═════╝  ╚════╝ ╚═╝   ╚═╝    ╚═════╝ \n";

        // Imprimir el texto en la consola
        wstring ws = wss.str();
        const wchar_t* text = ws.c_str();
        printUnicodeText(text);

        setColor(11);
        cout << endl << setw(55) << "--- INICIO DE SESION ---" << endl << endl;
        setColor(7);
        cout << setw(34) << "1. Registrar usuario"
             << setw(23) << "2. Iniciar sesion"
             << setw(14) << "0. Salir" << endl
             << setw(37) << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            // Si la entrada no es un entero, manejar el error
            cout << setw(67) << "Entrada no valida. Por favor, introduzca un numero.\n";
            cin.clear(); // Limpiar el estado de error
            cin.ignore(10000, '\n'); // Descartar la entrada restante
            continue;
        }

        switch (opcion) {
        case 1:
            registrarUsuario();
            break;
        case 2:
            iniciarSesion();
            return;
            break;
        case 0:
            cout << setw(36) << "Saliendo del programa." << endl;
            exit(0);
        default:
            cout << setw(51) << "Opcion no valida. Intente nuevamente." << endl;
        }
    }
}


