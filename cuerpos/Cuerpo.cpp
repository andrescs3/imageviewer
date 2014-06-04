/* 
 * File:   Cuerpo.cpp
 * Author: DanyAlejandro
 * 
 * Created on 2 de junio de 2014, 05:30 PM
 */

#include "Cuerpo.h"

// Reinicializa todos los datos de este cuerpo
void Cuerpo::inicializar() {
	int i;
	
	// eliminamos los puntos del cuerpo de memoria y limpiamos el vector
    if (!puntos.empty()) {
		for (i = 0; i < puntos.size(); i++) {
			delete puntos[i];
		}
		puntos.clear();
	}
}

// Agrega un punto a este cuerpo
void Cuerpo::agregar(int x, int y) {
	puntos.push_back(new Point(x, y));
	
	// Actualizamos el MBR
	if (puntos.size() == 1) {
		x1 = x;
		x2 = x;
		y1 = y;
		y2 = y;
	}
	else {
		if (x < x1) x1 = x;
		else if (x > x2) x2 = x;
		
		if (y < y1) y1 = y;
		else if (y > y2) y2 = y;
	}
	//cout << "punto " << puntos.size() << ": ("<< x << "," << y << ") " << "[" << x1 << "," << y1 << "] - " << "[" << x2 << "," << y2 << "]" << endl;
}

int Cuerpo::numPuntos() {
	return puntos.size();
}

int Cuerpo::ancho() {
	return x2 - x1 + 1;
}

int Cuerpo::alto() {
	return y2 - y1 + 1;
}


Point *Cuerpo::elem(int n) {
	return puntos[n];
}

Cuerpo::Cuerpo() {
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
}

Cuerpo::Cuerpo(const Cuerpo& orig) {
}

Cuerpo::~Cuerpo() {
}

