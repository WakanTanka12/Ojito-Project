#include "Asistencia_del_dia.h"
#include "reportes.h"
#include "ajustes.h"
#include "inicio_sesion.h"
#include "Horario_en_tiempo_real.h"
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iomanip>
#include <thread>

using namespace std;
	
void menu_principal();

int main() {
	int opcion;
	thread reloj_thread;
	menu_inicio_sesion();

	while (true) {
		menu_principal();
		if (!(cin >> opcion)) {
			cout << "Entrada no valida. Por favor, introduzca un numero.\n";
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		switch (opcion) {
			case 1:
				menu_reportes();
				break;
			case 2:
				menu_ajustes();
				break;
			case 3:
				Actualizar_Asistencia();
				Mostrar_Asistencia();
				break;
			case 4:
				if (!reloj_thread.joinable()) {
					reloj_thread = thread(Reloj);
					Mostrar_Lecturas_de_Posicion();
					reloj_thread.detach();
				}
				break;
			case 0:
				return 0;
				break;
			default:
				cout << "Opcion invalida";
		}
	}
	return 0;
}

void menu_principal() {
	setColor(11);
	cout << endl << setw(53) << "--- MENU PRINCIPAL---" << endl;
	setColor(7);
	cout << setw(25) << "1. Reportes" << endl;
	cout << setw(24) << "2. Ajustes" << endl;
	cout << setw(40) << "3. Ver asistencias diarias" << endl;
	cout << setw(39) << "4. Horario en tiempo real" << endl;
	cout << setw(22) << "0. Salir" << endl;
	cout << setw(43) << "Ingrese la opcion que desee: ";
}
