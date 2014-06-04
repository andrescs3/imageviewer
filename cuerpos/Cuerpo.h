/* 
 * File:   Cuerpo.h
 * Author: DanyAlejandro
 *
 * Created on 2 de junio de 2014, 05:30 PM
 */

#ifndef CUERPO_H
#define	CUERPO_H

#include <iostream>
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

class Cuerpo {
public:
	vector <Point *> puntos; // Puntos detectados en el cuerpo actual
	int x1, y1; // Coordenadas iniciales del MBR
	int x2, y2; // Coordenadas finales del MBR
	
	Cuerpo();
	Cuerpo(const Cuerpo& orig);
	virtual ~Cuerpo();
	
	void inicializar();
	void agregar(int x, int y);
	int numPuntos();
	int ancho();
	int alto();
	
	Point *elem(int n);
private:

};

#endif	/* CUERPO_H */

