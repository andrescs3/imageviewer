#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "limpieza.h"

using namespace cv;
using namespace std;

int numAislados1 = 0;
int numAislados2 = 0;

// Devuelve el código de la vecindad de un pixel
uchar limpia::getCodVecindad(Mat &img, int x, int y) {
	uchar vecindad[7];
	// iniciamos arriba y rotamos en sentido horario
	vecindad[0] = (img.at<uchar>(y-1,	x) == 255) ? 1 : 0;
	vecindad[1] = (img.at<uchar>(y-1,	x+1) == 255) ? 1 : 0;
	vecindad[2] = (img.at<uchar>(y,		x+1) == 255) ? 1 : 0;
	vecindad[3] = (img.at<uchar>(y+1,	x+1) == 255) ? 1 : 0;
	vecindad[4] = (img.at<uchar>(y+1,	x) == 255) ? 1 : 0;
	vecindad[5] = (img.at<uchar>(y+1,	x-1) == 255) ? 1 : 0;
	vecindad[6] = (img.at<uchar>(y,		x-1) == 255) ? 1 : 0;
	vecindad[7] = (img.at<uchar>(y-1,	x-1) == 255) ? 1 : 0;

	// aplicamos codificación Greenlee (1987)
	uchar codVecindad = 128 * int(vecindad[0]) + int(vecindad[1]) + 2 * int(vecindad[2])
			+ 4 * int(vecindad[3]) + 8 * int(vecindad[4]) + 16 * int(vecindad[5])
			+ 32 * int(vecindad[6]) + 64 * int(vecindad[7]);
	return codVecindad;
}

// true si es el código greenlee de un punto final de línea
bool limpia::codPuntoFinal(uchar codVecindad) {
	return (codVecindad == 1 || codVecindad == 2 || codVecindad == 4 || codVecindad == 8
			|| codVecindad == 16 || codVecindad == 32 || codVecindad == 64 || codVecindad == 128);
}


// Colorea de blanco un vector de Point en una Mat binaria
void limpia::marcarTrazo(vector <Point *> vec, Mat &mat) {
	Point *pun;
	for(vector<Point *>::iterator it = vec.begin(); it != vec.end(); ++it) {
		pun = *it;
		mat.at<uchar>(pun->y, pun->x) = 255;
	}
}

uchar limpia::trazoAislado(Mat &img, int x, int y) {
	uchar codVecindad = getCodVecindad(img, x, y);
	
	if (codPuntoFinal(codVecindad)) {
		// Si marca un punto final de línea...
		
		// Marcamos el trazo mientras se conserve delgado (1px)
		bool continuar = true, marcar = false;
		uchar dir = 0;
		uchar codSig;
		int i, j;
		int xAnt = x, yAnt = y, xSig, ySig;
		int xMin = x, yMin = y, xMax = x, yMax = y;
		int numMarcas = 0, numNegros = 0;
		vector <Point *> vecTrazo;
		
		while (continuar) {
			switch (codVecindad) {
				case 1: dir = 1; xSig=xAnt+1; ySig=yAnt-1; break;
				case 2: dir = 2; xSig=xAnt+1; ySig=yAnt; break;
				case 4: dir = 3; xSig=xAnt+1; ySig=yAnt+1; break;
				case 8: dir = 4; xSig=xAnt; ySig=yAnt+1; break;
				case 16: dir = 5; xSig=xAnt-1; ySig=yAnt+1; break;
				case 32: dir = 6; xSig=xAnt-1; ySig=yAnt; break;
				case 64: dir = 7; xSig=xAnt-1; ySig=yAnt-1; break;
				case 128: dir = 0; xSig=xAnt; ySig=yAnt-1; break;
				default: cout << "(" << xAnt << "," << yAnt << ") codigo " << (int)codVecindad << endl; return 0; break;
			}
			
			vecTrazo.push_back(new Point(xAnt, yAnt));
			
			// Actualizamos bounding box
			if (xSig < xMin) xMin = xSig;
			else if (xSig > xMax) xMax = xSig;
			if (ySig < yMin) yMin = ySig;
			else if (ySig > yMax) yMax = ySig;
			
			codSig = getCodVecindad(img, xSig, ySig);
			if (codPuntoFinal(codSig)) {
				// el trazo terminó
				vecTrazo.push_back(new Point(xSig, ySig));
				
				// contamos el número de pixeles marcados y no marcados en la vecindad
				for (i = (xMin - 1); i <= (xMax + 1); i++) {
					if (img.at<uchar>(yMin - 1, i) == 255) {
						//(imgMarcas.at<uchar>(yMin - 1, i) == 255) ? numMarcas++ : numNegros++;
					}
					if (img.at<uchar>(yMax + 1, i) == 255) {
						//(imgMarcas.at<uchar>(yMax + 1, i) == 255) ? numMarcas++ : numNegros++;
					}
				}
				for (j = yMin; j <= yMax; j++) {
					if (img.at<uchar>(j, xMin - 1) == 255) {
						//(imgMarcas.at<uchar>(j, xMin - 1) == 255) ? numMarcas++ : numNegros++;
					}
					if (img.at<uchar>(j, xMax + 1) == 255) {
						//(imgMarcas.at<uchar>(j, xMax + 1) == 255) ? numMarcas++ : numNegros++;
					}
				}
				
				//cout << "(" << xMin << "," << yMin << ")-(" << xMax << "," << yMax << "): n" << numNegros << "/ m" << numMarcas << endl;
				
				if (numMarcas == 0 && numNegros == 0) { marcar = true; }
				else if (numMarcas > numNegros) { marcar = true; }
				
				if (marcar) {
					//marcarTrazo(vecTrazo);
				}
				
				continuar = false;
			}
			else {
				// la línea no termina aún. Borrar pixel y seguir la línea
				img.at<uchar>(yAnt, xAnt) = 0;
				yAnt = ySig;
				xAnt = xSig;
				codVecindad = getCodVecindad(img, xSig, ySig);
			}
		}
		
		vecTrazo.clear();
		return 1; // TODO: contar pixeles totales marcados
	}
	return 0;
}


// true si el pixel se encuentra aislado
uchar limpia::pixelAislado(Mat &img, int x, int y) {
	uchar codVecindad = getCodVecindad(img, x, y); // aplicamos codificación Greenlee (1987)
    cout << x << "," << y << " " << (int)codVecindad << endl;
	return (codVecindad == 0);
}

// Resibe una imagen en escala de grises
// Remueve los pixeles aislados
// Devuelve en imgDest
int limpia::exec_limpiarPxAislados(Mat &imgOrigi, Mat &imgDest) {
	Mat imgBinaria;

    threshold(imgOrigi, imgBinaria, 200, 255, CV_THRESH_BINARY_INV); // filtro de umbral para obtener la versión binaria

    //cv::bitwise_xor(imgOrigi, cv::Scalar(255, 255, 255), imgBinaria);
    copyMakeBorder(imgBinaria, imgBinaria, 1, 1, 1, 1, BORDER_CONSTANT, 0); // agregamos borde de 1px
	
	int imgAlto = imgBinaria.rows;
	int imgAncho = imgBinaria.cols;
	int x, y, numPxMarcados = 0;
	uchar* fila;
	uchar intensidad;
	
	imgDest = imgOrigi.clone();

    //namedWindow("A", WINDOW_AUTOSIZE);
   // imshow("A", imgBinaria);
    //waitKey(0);
    imwrite("c:/img/imgBinaria.png", imgBinaria);
    cout<<"iniciando limpieza"<<endl;
    for (y = 1 ; y < (imgAlto - 1) ; ++y) {
        fila = imgBinaria.ptr<uchar>(y);
		for (x = 1 ; x < (imgAncho - 1) ; ++x) {
            intensidad = fila[x];
            cout<<(int) intensidad<<endl;
			if (intensidad == 255) {
                if (pixelAislado(imgBinaria, x, y)) {
                    imgDest.at<uchar>(y-1, x-1) = 255;
					numPxMarcados++;
				}
			}
		}
	}
    cout<<numPxMarcados<<endl;
	return numPxMarcados;
}


int limpia::metodoTrazoAislado(Mat &imgBinaria) {
	Mat imgBinTrazos = imgBinaria.clone();
	int imgAlto = imgBinaria.rows;
	int imgAncho = imgBinaria.cols;
	int x, y, numTrazosMarcados = 0;
	uchar* fila;
	uchar intensidad, a;
	
	for (y = 1 ; y < (imgAlto - 1) ; ++y) {
		fila = imgBinaria.ptr<uchar>(y);
		for (x = 1 ; x < (imgAncho - 1) ; ++x) {
			intensidad = fila[x]; // valor de intensidad del pixel
			if (intensidad == 255) {
				if ((y > 0) && (y < imgAlto - 1) && (x > 0) && (x < imgAncho - 1)) {
					a = trazoAislado(imgBinTrazos, x, y);
					if (a > 0) { numTrazosMarcados++; }
				}
			}
		}
	}
	
	cout << "metodoTrazoAislado marco " << numTrazosMarcados << " trazos." << endl;
	return numTrazosMarcados;
}
