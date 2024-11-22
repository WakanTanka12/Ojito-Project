#include "reportes.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <chrono>
#include <sstream>
#include <iomanip>	

using namespace std;
using json = nlohmann::json; // Como se usa namespace std para no usar std:: cada rato, lo mismo pero para no usar nlohmann::json a cada rato
							 // Y asi usar solo json

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
	string timestamp = obtenerTimestamp(); // Donde se guardará la timestamp
	string nombre = "D:/Proyectos/Visual/Visual-Assistant-Project/data/Reportes/Reporte_" + timestamp + ".txt"; // Variable que guardará el nombre del archivo incluyendo la timestamp

	ifstream lectura("D:/Proyectos/Visual/Visual-Assistant-Project/data/datosClientes/DatosdeClientes.json"); // Abrimos el archivo donde están los datos de los clientes
	if (!lectura.is_open()) { // Validacion
		cout << "No se pudo leer el archivo";
		exit(1);
	}
	json datos; // Creamos un objeto json
	lectura >> datos; // Guardamos los datos del json en el objeto
	lectura.close(); // Una vez guardados, cerramos el archivo

	ofstream escritura(nombre); // Creamos el archivo para escribir el reporte con la variable nombre porque ahi esta guardado el nombre + timestamp
	if (!escritura.is_open()) { // Validacion
		cout << "No se pudo crear / abrir el archivo";
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

}