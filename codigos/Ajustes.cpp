#include "ajustes.h"
#include "inicio_sesion.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using namespace nlohmann;

void ajustar_horarios() {
	ifstream leer("data/ajustes/ajustes.json");
	if (!leer.is_open()) {
		cout << setw(41) << "No se pudo abrir el archivo";
		exit(1);
	}
	json ajustes;
	leer >> ajustes;
	leer.close();
	cout << endl << setw(29) << "Ajuste actual: " << ajustes["hora_entrada"].get<string>() << endl;
	string linea;
	cout << setw(75) << "Ingrese la hora de entrada (en formato HH:MM:SS y 24 horas): ";
	cin.ignore();
	getline(cin, linea);
	ajustes["hora_entrada"] = linea;

	ofstream escribir("data/ajustes/ajustes.json");
	escribir << ajustes.dump(4);
	escribir.close();

	ifstream leer1("data/ajustes/ajustes.json");
	if (!leer1.is_open()) {
		cout << setw(41) << "No se pudo abrir el archivo";
		exit(1);
	}
	json ajustes1;
	leer1 >> ajustes1;
	leer1.close();
	cout << endl << setw(29) << "Ajuste actual: " << ajustes1["hora_tolerancia"].get<string>() << endl;
	string linea1;
	cout << setw(78) << "Ingrese la hora de tolerancia (en formato HH:MM:SS y 24 horas): ";
	cin.ignore();
	getline(cin, linea1);
	ajustes1["hora_tolerancia"] = linea1;

	ofstream escribir1("data/ajustes/ajustes.json");
	escribir1 << ajustes1.dump(4);
	escribir1.close();
}

void ajustar_frecuencia_reportes() {
	ifstream leer("data/ajustes/ajustes.json");
	if (!leer.is_open()) {
		cout << setw(41) << "No se pudo abrir el archivo";
		exit(1);
	}
	json ajustes;
	leer >> ajustes;
	leer.close();
	cout << endl << setw(29) << "Ajuste actual: " << endl
		<< setw(21) << "anhos: " << ajustes["anho_frecuencia"].get<int>() << endl
		<< setw(21) << "meses: " << ajustes["mes_frecuencia"].get<int>() << endl
		<< setw(20) << "dias: " << ajustes["dia_frecuencia"].get<int>() << endl;

	int anhos, meses, dias;
	cout << setw(35) << "Cada cuantos anhos?: ";
	cin >> anhos;
	cout << setw(35) << "Cada cuantos meses?: ";
	cin >> meses;
	cout << setw(34) << "Cada cuantos dias?: ";
	cin >> dias;

	ajustes["anho_frecuencia"] = anhos;
	ajustes["mes_frecuencia"] = meses;
	ajustes["dia_frecuencia"] = dias;

	ofstream escribir("data/ajustes/ajustes.json");
	escribir << ajustes.dump(4);
	escribir.close();

}

void ajustar_licencias() {

	cout << setw(83) << "Hola!, En este apartado podras ajustar las licencias de tus empleados" << endl;
	ifstream abrir("data/datosDeAsistenciaGenerales/datosDeAsistenciaGenerales.json");
	if (!abrir.is_open()) {
		cout << setw(41) << "No se pudo abrir el archivo";
		exit(1);
	}
	json datos;
	abrir >> datos;
	abrir.close();

	string nombre;
	int aux;
	cout << setw(85) << "Ingrese el nombre de la persona a la que quiere ajustar las licencias: ";
	cin.ignore();
	getline(cin, nombre);

	bool encontrado = false;
	for (int i = 0; i < datos.size(); i++) {
		if (nombre == datos[i]["Nombre"].get<string>()) {
			cout << setw(52) << "Ingrese la cantidad de licencias para " << nombre << ": ";
			cin >> aux;
			datos[i]["Licencias"] = datos[i]["Licencias"].get<int>() + aux;
			encontrado = true;
		}
	}

	if (encontrado) {
		ofstream escribir("data/datosDeAsistenciaGenerales/datosDeAsistenciaGenerales.json");
		if (!escribir.is_open()) {
			cout << setw(41) << "No se pudo abrir el archivo";
			exit(1);
		}
		escribir << datos.dump(4);
		escribir.close();
	}
	else {
		cout << setw(55) << "No se encontro un empleado con ese nombre";
	}
}

void ajustar_sensores() {
	int opcion;
	while (true) {
		cout << endl << setw(65) << "Hola!, En este apartado podras ajustar los sensores" << endl;
		cout << setw(46) << "1. Ajustar distancia de muestreo" << endl;
		cout << setw(47) << "2. Ajustar frecuencia de muestreo" << endl;
		cout << setw(38) << "0. Volver a Menu Ajustes" << endl;
		cout << setw(34) << "Ingrese una opcion: ";

		if (!(cin >> opcion)) {
			cout << setw(65) << "Entrada no valida. Por favor, introduzca un numero." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		ifstream leer("data/ajustes/ajustes.json");
		if (!leer.is_open()) {
			cout << setw(41) << "No se pudo abrir el archivo";
			continue;
		}
		json datos;
		leer >> datos;
		leer.close();

		switch (opcion) {
			case 1: {
				int aux;
				cout << setw(29) << "Ajuste actual: " << datos["distancia_muestreo"].get<int>() << endl;
				cout << setw(44) << "Ingrese la distancia deseada: ";
				cin >> aux;
				while (aux < 0) {
					cout << setw(66) << "Por favor ingrese una distancia valida (mayor a 0): ";
					cin >> aux;
				}
				datos["distancia_muestreo"] = aux;
				ofstream ajuste("data/ajustes/ajustes.json");
				if (!ajuste.is_open()) {
					cout << setw(56) << "No se pudo abrir el archivo";
					continue;
				}
				ajuste << datos.dump(4);
				ajuste.close();
				break;
			}
			case 2: {
				string aux;
				cout << setw(29) << "Ajuste actual: " << datos["frecuencia_muestreo"].get<string>() << endl;
				cout << setw(69) << "Ingrese la frecuencia de muestreo en formato HH:MM:SS: ";
				cin.ignore();
				getline(cin, aux);
				datos["frecuencia_muestreo"] = aux;
				ofstream ajuste("data/ajustes/ajustes.json");
				if (!ajuste.is_open()) {
					cout << setw(40) << "No se pudo abrir el archivo";
					continue;
				}
				ajuste << datos.dump(4);
				ajuste.close();
				break;
			}
			case 0: {
				return;
				break;
			}
			default:
					cout << setw(39) << "Ingrese una opcion valida";
		}
	}
}

void menu_ajustes() {
	int opcion;
	while (true) {
		setColor(11);
		cout << endl << setw(52) << "--- AJUSTES ---" << endl;
		setColor(7);
		cout << setw(33) << "1. Ajustar Horarios" << endl;
		cout << setw(46) << "2. Editar frecuencia de reportes" << endl;
		cout << setw(38) << "3. Administrar licencias" << endl;
		cout << setw(40) << "4. Ajustes de los sensores" << endl;
		cout << setw(23) << "0. Volver" << endl;
		cout << setw(33) << "Ingrese su opcion: ";

		if (!(cin >> opcion)) {
			cout << setw(65) << "Entrada no valida. Por favor, introduzca un numero.\n";
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		switch (opcion) {
		case 1:
			ajustar_horarios();
			break;
		case 2:
			ajustar_frecuencia_reportes();
			break;
		case 3:
			ajustar_licencias();
			break;
		case 4:
			ajustar_sensores();
			break;
		case 0:
			return;
			break;
		default:
			cout << setw(49) << "Opcion invalida, intentelo de nuevo";
			break;
		}
	}
}
