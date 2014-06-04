/* 
 * File:   cuerpos.h
 * Author: DanyAlejandro
 *
 * Created on 2 de junio de 2014, 03:19 PM
 */

#ifndef CUERPOS_H
#define	CUERPOS_H

#include <queue>
#include "opencv2/core/core.hpp"

#include "Cuerpo.h"

using namespace cv;
using namespace std;

namespace cuerpos {
	void getVecindad(Mat &img, int x, int y, uchar *vecindad);
	uchar getCodVecindad(Mat &img, int x, int y);
	bool codPuntoFinal(uchar codVecindad);
	void marcarCuerpo(Cuerpo *cuerpo, int nColor, Mat &img, Mat &imgMarcas);
	void analizarPunto(Mat &imgBin, int x, int y, queue <Point *> &cola, Cuerpo *objCuerpo);
	void explorarCuerpo(Mat &imgBin, int x, int y, Cuerpo *objCuerpo);
	
	void exec_limpiarCuerpos(Mat &imgIn, Mat &imgOut);
	
}

#endif	/* CUERPOS_H */

