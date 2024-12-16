#include "reportes.h"
#include "inicio_sesion.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <sstream>
#include <filesystem>
#include <iomanip>	

using namespace std;
using namespace nlohmann; // Como se usa namespace std para no usar std:: cada rato, lo mismo pero para no usar nlohmann:: a cada rato
					      // Y asi usar solo json
namespace fs = filesystem;

string obtenerTimestamp() { // Funcion para obtener timstamp, no me pregunten
	auto ahora = chrono::system_clock::now();
	time_t tiempo_actual = chrono::system_clock::to_time_t(ahora);
	tm tiempo_local;
	localtime_s(&tiempo_local, &tiempo_actual);

	stringstream ss;
	ss << put_time(&tiempo_local, "%Y%m%d-%H%M%S");
	return ss.str();
}

void crear_reporte() { 

	ifstream lectura("data/datosDeAsistenciaGenerales/datosDeAsistenciaGenerales.json"); // Abrimos el archivo donde están los datos de los clientes
	if (!lectura.is_open()) { // Validacion
		cout << setw(40) << "No se pudo leer el archivo";
		exit(1);
	}
	json datos; // Creamos un objeto json
	lectura >> datos; // Guardamos los datos del json en el objeto
	lectura.close(); // Una vez guardados, cerramos el archivo

	string timestamp = obtenerTimestamp(); // Donde se guardará la timestamp
	string nombre = "data/Reportes/Reporte_" + timestamp + ".txt"; // Variable que guardará el nombre del archivo incluyendo la timestamp

	ofstream escritura(nombre); // Creamos el archivo para escribir el reporte con la variable nombre porque ahi esta guardado el nombre + timestamp
	if (!escritura.is_open()) { // Validacion
		cout << setw(49) << "No se pudo crear / abrir el archivo";
		exit(1);
	}

	// Encabezados
	escritura << left << setw(25) << "Nombre" 
		      << setw(10) << "Horas" 
		      << setw(15) << "Horas Extra"
		      << setw(10) << "Atrasos"
		      << setw(10) << "Faltas" 
		      << setw(15) << "Licencias" << endl;
	
	// Dato de cada trabajador
	for (int i = 0; i < datos.size(); i++) {
		escritura << left << setw(25) << datos[i]["Nombre"].get<string>()
				  << setw(10) << datos[i]["Horas"].get<int>()
			      << setw(15) << datos[i]["Extra"].get<int>()
			      << setw(10) << datos[i]["Atrasos"].get<int>()
			      << setw(10) << datos[i]["Faltas"].get<int>()
			      << setw(15) << datos[i]["Licencias"].get<int>() << endl;
	}
	escritura.close();
	cout << setw(38) << "Reporte creado con exito" << endl;

}

void abrir_reporte() {

	int j = 1;  // Contador para mostrar los numeros de archivos xd
	vector<string> archivos;  // Vector donde se guardarán las direcciones de los archivos

	// Menu de eleccion
	// Iniciamos desde el primer archivo, hasta el ultimo archivo
	for (fs::directory_iterator i("data/Reportes"); i != fs::directory_iterator(); i++) {
		string directorio = i->path().string();
		archivos.push_back(directorio);
		cout << endl << setw(15) << j << ". " << directorio.substr(14, directorio.length() - 1) << endl;
		j++;
	}
	cout << endl << setw(39) << "0. Volver a MENU REPORTES" << endl;

	int opcion;
	cout << setw(46) << "Elija que reporte quiere abrir: ";
	cin >> opcion;
	while (opcion < 0 || opcion > archivos.size()) {
		cout << setw(47) << "Opcion invalida, intente de nuevo";
		cin >> opcion;
	}

	if (opcion == 0)
		return;
	// String donde se guardará la dirección del archivo seleccionado (sacada del vector archivos)
	string archivoSeleccionado; 
	archivoSeleccionado = archivos[opcion - 1];

	// Abrimos el archivo con la direccion guardada en archivoSeleccionado
	ifstream apertura(archivoSeleccionado);
	if (!apertura.is_open()) {
		cout << setw(42) << "No se pudo abrir el archivo." << endl;
		exit(1);
	}

	string linea = "1"; // Numero random para que el while funcione
	cin.ignore(); // Vaciar el buffer porque despues uso getline

	cout << setw(39) << "El reporte es: " << endl;
	while (!linea.empty()) {
		getline(apertura, linea);
		cout << linea << endl;
	}
}

void menu_reportes() {
	while (true) {
		setColor(11);
		cout << endl << setw(55) << "--- MENU DE REPORTES ---" << endl;
		setColor(7);
		cout << setw(30) << "1. Crear reporte" << endl;
		cout << setw(40) << "2. Ver reportes anteriores" << endl;
		cout << setw(40) << "0. Volver a MENU PRINCIPAL" << endl;
		cout << setw(33) << "Ingrese su opcion: ";
		int opcion;
		if (!(cin >> opcion)) {
			cout << setw(65) << "Entrada no valida. Por favor, introduzca un numero." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		switch (opcion) {
		case 0:
			return;
			break;
		case 1:
			crear_reporte();
			break;
		case 2:
			abrir_reporte();
			break;
		default:
			cout << setw(47) << "Opcion invalida, intente de nuevo";
			break;
		}
	}
}
