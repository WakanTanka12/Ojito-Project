#include <iostream>
#include "Horario_en_tiempo_real.h"
#include "Hora.h"
#include "inicio_sesion.h"
#include <random>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;
using namespace nlohmann;

string hora_actual;
mutex hora_mutex;
atomic<bool> mostrar_reloj(true); 


int Numero_Random() {
    int randomNumber = 0;
    random_device rd; 
    mt19937 gen(rd()); 

    uniform_int_distribution<> distrib(0, 5);

    randomNumber = distrib(gen);
    

    return randomNumber;
}

void Reloj() {
    while (mostrar_reloj) {
        time_t ahora = time(0);
        tm tiempoLocal;

        localtime_s(&tiempoLocal, &ahora);

        int horas = tiempoLocal.tm_hour;
        int minutos = tiempoLocal.tm_min;
        int segundos = tiempoLocal.tm_sec;

        stringstream ss;
        ss << (horas < 10 ? "0" : "") << horas << ":"
            << (minutos < 10 ? "0" : "") << minutos << ":"
            << (segundos < 10 ? "0" : "") << segundos;

        {
            lock_guard<mutex> lock(hora_mutex);
            hora_actual = ss.str();
        }

        cout << setw(22) <<"\rHora actual: " << hora_actual << flush;

        this_thread::sleep_for(chrono::seconds(1));
    }
}

void Mostrar_Lecturas_de_Posicion() {
    mostrar_reloj = true;
    int opcion = 0;
    json archivo1;
    json archivo2;
    ifstream abrir("data/datosDeAsistenciaDiaria/Asistencia.json");
    if (!abrir.is_open()) { 
        cout << setw(39) << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    abrir >> archivo1;
    abrir.close();

    ifstream abrir2("data/lugaresDeInteres/Lugares_de_Interes.json");
    if (!abrir2.is_open()) {
        cout << setw(39) << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    abrir2 >> archivo2;
    abrir2.close();

    while (true) {
        setColor(11);
        cout << endl << setw(58) << "--- HORARIO EN TIEMPO REAL ---" << endl;
        setColor(7);
        cout << setw(50) << "1. Revisar horarios de los empleados" << endl;
        cout << setw(41) << "0. Volver al menu principal" << endl;
        cout << setw(34) << "Ingrese una opcion: " << endl;

        if (!(cin >> opcion)) {
            cout << setw(65) << "Entrada no valida. Por favor, introduzca un numero.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (opcion == 1) { 
            string hora_capturada;
            {
                lock_guard<mutex> lock(hora_mutex); 
                hora_capturada = hora_actual; 
            } 

            cout << setw(31) << "Hora de lectura: " << hora_capturada << endl;

            int hora2 = horaEnSegundos(hora_capturada); 

            for (int i = 0; i < archivo1.size(); i++) {
                if (archivo1[i]["Estado"] == "Presente" || archivo1[i]["Estado"] == "Atraso") {
                    string hora5 = archivo1[i]["Hora_Entrada"];
                    int numero = Numero_Random(), hora1 = horaEnSegundos(hora5);
                    if (archivo1[i]["Estado"] == "Presente" && hora2 > hora1) {
                        cout << setw(35) << "Nombre del empleado: " << archivo1[i]["Nombre"] << ", estado: Presente" << endl;
                        cout << setw(25) << "Ubicacion: " << archivo2[numero]["Lugar_de_Interes"] << endl;
                    }
                    else
                        if (archivo1[i]["Estado"] == "Atraso" && hora2 > hora1) {
                            cout << setw(25) << "Nombre del empleado: " << archivo1[i]["Nombre"] << ", estado: Atraso" << endl;
                            cout << setw(25) << "Ubicacion: " << archivo2[numero]["Lugar_de_Interes"] << endl;
                        }
                }
                if (archivo1[i]["Estado"] == "Falta") 
                     cout << setw(35) << "Nombre del empleado: " << archivo1[i]["Nombre"] << ", estado: Falta" << endl;
            }

        }
        else if (opcion == 0) {
            mostrar_reloj = false;
            cout << setw(40) << "Volviendo a Menu Princiapl\n";
            break; // Salir del bucle y terminar el programa
        }
        else {
            cout << setw(50) << "Opcion invalida. Intente nuevamente.\n";
        }
    }
}


}

