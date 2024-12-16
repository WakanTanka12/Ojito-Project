#pragma once
#ifndef HORA_H
#define HORA_H
#include <string>

using namespace std;

int horaEnSegundos(const string& hora);

string segundosAHora(int segundosTotales);

string calcularDiferencia(const string& hora1, const string& hora2);

#endif
