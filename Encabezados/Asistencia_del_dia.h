#pragma once
#ifndef ASISTENCIADELDIA_H
#define ASISTENCIADELDIA_H
#include <string>

using namespace std;

void Mostrar_Asistencia();

int horaEnSegundos(const string& hora);

string segundosAHora(int segundosTotales);

string calcularDiferencia(const string& hora1, const string& hora2); 

void Registrar_Asistencia();

#endif
