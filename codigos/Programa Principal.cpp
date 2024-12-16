#include "Asistencia_del_dia.h"
#include "reportes.h"
#include "ajustes.h"
#include "inicio_sesion.h"
#include "Horario_en_tiempo_real.h"
#include <iostream>
#include <thread>

using namespace std;

void menu_principal();

int main() {
	int opcion;
	thread reloj_thread;
	menu_inicio_sesion();
	
	while (true) {
		menu_principal();
		cin >> opcion;
		switch(opcion) {
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
		}
	}
	return 0;
}

void menu_principal() {
	cout << "\t -- MENU PRINCIPAL --" << endl;
	cout << "1. Reportes" << endl;
	cout << "2. Ajustes" << endl;
	cout << "3. Ver asistencias diarias" << endl;
	cout << "4. Horario en tiempo real" << endl;
	cout << "0. Salir" << endl;
	cout << "Ingrese la opcion que desee: ";
}