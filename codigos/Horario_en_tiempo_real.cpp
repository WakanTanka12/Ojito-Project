#include <iostream>
#include "Horario_en_tiempo_real.h"
#include "Hora.h"
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

        cout << "\rHora actual: " << hora_actual << flush;

        this_thread::sleep_for(chrono::seconds(1));
    }
}

void Mostrar_Lecturas_de_Posicion() {
    int opcion = 0;
    json archivo1;
    json archivo2;
    ifstream abrir("data/datosDeAsistencia/Asistencia.json");
    if (!abrir.is_open()) { 
        cout << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    abrir >> archivo1;
    abrir.close();

    ifstream abrir2("data/lugaresDeInteres/Lugares_de_Interes.json");
    if (!abrir2.is_open()) {
        cout << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    abrir2 >> archivo2;
    abrir2.close();

    while (true) {

        cout << "\t --- Menú de Opciones ---" << endl;
        cout << "1. Revisar horarios de los empleados" << endl;
        cout << "0. Salir" << endl;
        cout << "Ingrese una opción: " << endl;

        cin >> opcion;

        if (opcion == 1) { 
            string hora_capturada;
            {
                lock_guard<mutex> lock(hora_mutex); 
                hora_capturada = hora_actual; 
            } 

            cout << "\tHora de lectura: " << hora_capturada << endl;

            int hora2 = horaEnSegundos(hora_capturada); 

            for (int i = 0; i < archivo1.size(); i++) {
                if (archivo1[i]["Estado"] == "Presente" || archivo1[i]["Estado"] == "Atraso") {
                    string hora5 = archivo1[i]["Hora_Entrada"];
                    int numero = Numero_Random(), hora1 = horaEnSegundos(hora5);
                    if (archivo1[i]["Estado"] == "Presente" && hora2 > hora1) {
                        cout << "Nombre del empleado: " << archivo1[i]["Nombre"] << ", estado: Presente" << endl;
                        cout << "Ubicacion: " << archivo2[numero]["Lugar_de_Interes"] << endl;
                    }
                    else
                        if (archivo1[i]["Estado"] == "Atraso" && hora2 > hora1) {
                            cout << "Nombre del empleado: " << archivo1[i]["Nombre"] << ", estado: Atraso" << endl;
                            cout << "Ubicacion: " << archivo2[numero]["Lugar_de_Interes"] << endl;
                        }
                }
                if (archivo1[i]["Estado"] == "Falta") 
                     cout << "Nombre del empleado: " << archivo1[i]["Nombre"] << ", estado: Falta" << endl;
            }

        }
        else if (opcion == 0) {
            mostrar_reloj = false;
            cout << "Saliendo del programa...\n";
            break; // Salir del bucle y terminar el programa
        }
        else {
            cout << "Opción inválida. Intente nuevamente.\n";
        }
    }


}

