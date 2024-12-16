#pragma once
#ifndef INICIO_SESION_H
#define INICIO_SESION_H

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

void registrarUsuario();
void iniciarSesion();
void menu_inicio_sesion();
void setColor(int color);

#endif
