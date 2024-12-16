#include "Asistencia_del_dia.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

using namespace std;
using namespace nlohmann;

int horaEnSegundos(const string& hora) {
    int horas = stoi(hora.substr(0, 2));
    int minutos = stoi(hora.substr(3, 2));
    int segundos = stoi(hora.substr(6, 2));
    return horas * 3600 + minutos * 60 + segundos;
}

string segundosAHora(int segundosTotales) {
    int horas = segundosTotales / 3600;
    segundosTotales %= 3600;
    int minutos = segundosTotales / 60;
    int segundos = segundosTotales % 60;

    string resultado = "00:00:00";

    if (horas >= 0 && horas < 10) {
        resultado.at(0) = '0';
        resultado.at(1) = '0' + horas; // Convierte el número a carácter
    }
    else if (horas >= 10 && horas < 24) {
        resultado.at(0) = '0' + (horas / 10);
        resultado.at(1) = '0' + (horas % 10);
    }

    if (minutos >= 0 && minutos < 10) {
        resultado.at(3) = '0';
        resultado.at(4) = '0' + minutos;
        }
    else if (minutos >= 10 && minutos < 60) {
        resultado.at(3) = '0' + (minutos / 10);
        resultado.at(4) = '0' + (minutos % 10);
    }

    if (segundos >= 0 && segundos < 10) {
        resultado.at(6) = '0';
        resultado.at(7) = '0' + segundos;
    }
    else if (segundos >= 10 && segundos < 60) {
        resultado.at(6) = '0' + (segundos / 10);
        resultado.at(7) = '0' + (segundos % 10);
    }

    return resultado;
}

string calcularDiferencia(const string& hora1, const string& hora2) {

    int segundosHora1 = horaEnSegundos(hora1);
    int segundosHora2 = horaEnSegundos(hora2);

    int diferencia = abs(segundosHora1 - segundosHora2); // Diferencia absoluta
    return segundosAHora(diferencia);
}


void Mostrar_Asistencia() {
    ifstream abrir("data/ajustes/ajustes.json");
    if (!abrir.is_open()) {
        cout << setw(49) << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    json tolerancia;
    abrir >> tolerancia;
    abrir.close();

    string nombre, aux1 = "", hora= " ", horaTolerancia = tolerancia["hora_tolerancia"].get<string>(); //Cambiar por una variable global que configure la tolerancia de asistencia
    cout << setw(46) << "Ingrese el nombre del empleado: ";
    cin.ignore();
    getline(cin, nombre);
    json archivo;
    abrir.open("data/datosDeAsistenciaDiaria/Asistencia.json");
    if (!abrir.is_open()) { // Validacion
        cout << setw(39) << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    abrir >> archivo;
    for (int i = 0; i < 4; i++) {
        if (archivo[i]["Nombre"] == nombre && archivo [i]["Estado"] == "Presente") {
            cout << setw(22) << "Estado: " << archivo[i]["Estado"].get<string>() << endl;
            cout << setw(20) << "Hora: " << archivo[i]["Hora_Entrada"].get<string>() << endl;
            break;
        }
        else if (archivo[i]["Nombre"] == nombre && archivo[i]["Estado"] == "Atraso") {
            cout << setw(22) << "Estado: " << archivo[i]["Estado"].get<string>() << endl;
            cout << setw(20) << "Hora: " << archivo[i]["Hora_Entrada"].get<string>() << endl;
            hora = archivo[i]["Hora_Entrada"].get<string>();
            aux1 = calcularDiferencia(horaTolerancia, hora);
            cout << setw(42) << "El empleado se retraso por: " << aux1;
            break;
        }
        else if (archivo[i]["Nombre"] == nombre && archivo[i]["Estado"] == "Falta") {
            cout << setw(22) << "Estado: " << archivo[i]["Estado"].get<string>() << endl;
            cout << setw(42) << "Este empleado esta con falta" << endl;
            break;
        } else if (i == 3 && archivo[i]["Nombre"] != nombre)
            cout << setw(63) << "Nombre de empleado no encontrado intente de nuevo" << endl;
    }
    abrir.close();
}

void Actualizar_Asistencia() {

    // Cargar la hora de entrada en HoraEntrada
    ifstream abrir("data/ajustes/ajustes.json");

    if (!abrir.is_open()) { // Validacion
        cout << setw(39) << "No se pudo crear / abrir el archivo";
        exit(1);
    }

    json hora_entrada;
    abrir >> hora_entrada;
    abrir.close();

    string HoraEntrada = hora_entrada["hora_entrada"];

    // Cargar el archivo JSON
    fstream archivo("data/datosDeAsistenciaDiaria/Asistencia.json", ios::in | ios::out); // Modo lectura y escritura
    if (!archivo.is_open()) {
        cout << setw(57) << "No se pudo abrir el archivo Asistencia.json" << endl;
        return;
    }

    // Leer el contenido del archivo en un objeto JSON
    json data;
    archivo >> data;

    // Calcular la hora de entrada en segundos
    int Hora1 = horaEnSegundos(HoraEntrada);

    // Editar los estados según la hora de entrada
    for (auto& empleado : data) {
        if (empleado.contains("Hora_Entrada")) {
            string aux = empleado["Hora_Entrada"];
            if (!aux.empty()) {
                int Hora2 = horaEnSegundos(aux);
                int diferencia = Hora1 - Hora2;

                if (diferencia < 0) {
                    empleado["Estado"] = "Atraso";
                }
                else {
                    empleado["Estado"] = "Presente";
                }
            }
        }
    }

    archivo.clear(); // Limpiar los flags del flujo
    archivo.seekp(0); // Posicionar el puntero de escritura al inicio
    archivo << data.dump(4); // Sobrescribir el archivo con sangría de 4 espacios

    cout << setw(67) << "Estados actualizados correctamente en Asistencia.json" << endl;

    archivo.close();

}
