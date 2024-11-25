#include "Asistencia_del_dia.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <iomanip>

string fecha = "07:15:00";

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
    string nombre, aux1 = "", hora= " ", horaTolerancia = "07:35:00"; //Cambiar por una variable global que configure la tolerancia de asistencia
    cout << "Ingrese el nombre del empleado: "<<endl;
    getline(cin, nombre);
    json archivo;
    ifstream abrir("Asistencia.json");
    if (!abrir.is_open()) { // Validacion
        cout << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    abrir >> archivo;
    for (int i = 0; i < 4; i++) {
        if (archivo[i]["Nombre"] == nombre && archivo [i]["Estado"] == "Presente") {
            cout << "Estado: " << archivo[i]["Estado"].get<string>() << endl;
            cout << "Hora: " << archivo[i]["Hora_Entrada"].get<string>();
            break;
        }
        else if (archivo[i]["Nombre"] == nombre && archivo[i]["Estado"] == "Atraso") {
            cout << "Estado: " << archivo[i]["Estado"].get<string>() << endl;
            cout << "Hora: " << archivo[i]["Hora_Entrada"].get<string>() << endl;
            hora = archivo[i]["Hora_Entrada"].get<string>();
            aux1 = calcularDiferencia(horaTolerancia, hora);
            cout << "El empleado se retraso por: " << aux1;
            break;
        }
        else if (archivo[i]["Nombre"] == nombre && archivo[i]["Estado"] == "Falta") {
            cout << "Estado: " << archivo[i]["Estado"].get<string>() << endl;
            cout << "Este empleado esta con falta";
            break;
        } else if (i == 3 && archivo[i]["Nombre"] != nombre)
            cout << "Nombre de empleado no encontrado intente de nuevo";
    }
    abrir.close();
}

void Registrar_Asistencia() {
    string aux = "", HoraEntrada = "07:20:00"; //Cambiar la variable HoraEntrada por una variable global
    int Hora1 = horaEnSegundos(HoraEntrada), Hora2 = 0, resultado = 0;
    json archivo;
    ifstream abrir("Asistencia.json");
    if (!abrir.is_open()) { // Validacion
        cout << "No se pudo crear / abrir el archivo";
        exit(1);
    }
    abrir >> archivo;
    for (int i = 0; i < 4; i++) {
        aux = archivo[i]["Hora_Entrada"].get<string>();
        Hora2 = horaEnSegundos(aux);
        resultado = Hora1 - Hora2;
        if (resultado < 0) {
            archivo[i]["Estado"];
        }
    }

}

int main() {
    string fecha = "07:15:00";
    Mostrar_Asistencia();

    return 0;
}
