#include <iostream>
#include <queue>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Cuerpo.h"
#include "cuerpos.h"

// Devuelve la vecindad de un pixel en la referencia del parametro
void cuerpos::getVecindad(Mat &img, int x, int y, uchar vecindad[8]) {
    // iniciamos arriba y rotamos en sentido horario
    vecindad[0] = (img.at<uchar>(y-1,	x) == 255) ? 1 : 0;
    vecindad[1] = (img.at<uchar>(y-1,	x+1) == 255) ? 1 : 0;
    vecindad[2] = (img.at<uchar>(y,		x+1) == 255) ? 1 : 0;
    vecindad[3] = (img.at<uchar>(y+1,	x+1) == 255) ? 1 : 0;
    vecindad[4] = (img.at<uchar>(y+1,	x) == 255) ? 1 : 0;
    vecindad[5] = (img.at<uchar>(y+1,	x-1) == 255) ? 1 : 0;
    vecindad[6] = (img.at<uchar>(y,		x-1) == 255) ? 1 : 0;
    vecindad[7] = (img.at<uchar>(y-1,	x-1) == 255) ? 1 : 0;
}

// Devuelve el código de la vecindad de un pixel
uchar cuerpos::getCodVecindad(Mat &img, int x, int y) {
    uchar vecindad[8];
    getVecindad(img, x, y, vecindad);

    // aplicamos codificación Greenlee (1987)
    uchar codVecindad = 128 * int(vecindad[0]) + int(vecindad[1]) + 2 * int(vecindad[2])
                    + 4 * int(vecindad[3]) + 8 * int(vecindad[4]) + 16 * int(vecindad[5])
                    + 32 * int(vecindad[6]) + 64 * int(vecindad[7]);
    return codVecindad;
}

// true si es el código greenlee de un punto final de línea
bool cuerpos::codPuntoFinal(uchar codVecindad) {
    return (codVecindad == 1 || codVecindad == 2 || codVecindad == 4 || codVecindad == 8
                    || codVecindad == 16 || codVecindad == 32 || codVecindad == 64 || codVecindad == 128);
}


// Colorea un vector de Point en una Mat
void cuerpos::marcarCuerpo(Cuerpo *cuerpo, int nColor, Mat &img, Mat &imgMarcas) {
    Point *pun;
    Vec3b color;

    if (nColor < 0) {
        nColor = rand() % 6;
    }

    switch (nColor) {
            case 0: color = Vec3b(214, 43, 169); break; // morado
            case 1: color = Vec3b(211, 122, 18); break; // azul
            case 2: color = Vec3b(0, 242, 142); break; // verde
            case 3: color = Vec3b(21, 186, 249); break; // amarillo
            case 4: color = Vec3b(67, 12, 191); break; // rojo
            case 5: color = Vec3b(255, 0, 255); break;
        }

    vector<Point *> vec = cuerpo->puntos;
    for(vector<Point *>::iterator it = vec.begin(); it != vec.end(); ++it) {
        pun = *it;
        img.at<Vec3b>((pun->y) - 1, (pun->x) - 1) = color;
        imgMarcas.at<uchar>(pun->y - 1, pun->x - 1) = 255;
    }
}


void cuerpos::analizarPunto(Mat &imgBin, int x, int y, queue <Point *> &cola, Cuerpo *objCuerpo) {
    if (imgBin.at<uchar>(y,x) == 0) return; // No analizar pixeles vacíos si aparecen en la cola

    //cout << "Analizando (" << x << "," << y << ")";

    // obtenemos la vecindad (binaria) inmediata del pixel
    uchar vecindad[8];
    getVecindad(imgBin, x, y, vecindad);

    objCuerpo->agregar(x, y); // almacenamos el punto actual
    imgBin.at<uchar>(y, x) = 0; // eliminamos el punto para que el algoritmo no se devuelva

    if (vecindad[0] == 0 && vecindad[1] == 0 && vecindad[2] == 0 && vecindad[3] == 0 && vecindad[4] == 0 && vecindad[5] == 0 && vecindad[6] == 0 && vecindad[7] == 0) {
        // es un pixel aislado; registramos pero no analizamos vecindad
        return;
    }
    else {
        // no es un pixel aislado: comenzamos a recorrer el cuerpo
        // agregamos los pixeles negros contiguos a la cola de análisis
        if (vecindad[0] == 1) cola.push(new Point(x, y-1));
        if (vecindad[1] == 1) cola.push(new Point(x+1, y-1));
        if (vecindad[2] == 1) cola.push(new Point(x+1, y));
        if (vecindad[3] == 1) cola.push(new Point(x+1, y+1));
        if (vecindad[4] == 1) cola.push(new Point(x, y+1));
        if (vecindad[5] == 1) cola.push(new Point(x-1, y+1));
        if (vecindad[6] == 1) cola.push(new Point(x-1, y));
        if (vecindad[7] == 1) cola.push(new Point(x-1, y-1));
    }
}

// recibimos un punto de un cuerpo, lo analizamos y se agregan los contiguos a la cola
void cuerpos::explorarCuerpo(Mat &imgBin, int x, int y, Cuerpo *objCuerpo) {
    cout << "Explorando cuerpo en (" << x << "," << y << ")" << endl;
    queue <Point *> cola; // Cola de puntos pendientes por analizar

    analizarPunto(imgBin, x, y, cola, objCuerpo);

    // Analizamos todos los puntos en la cola con recurrencia
    Point *p;
    int tamCola = cola.size();
    while (tamCola > 0) {
        p = cola.front();
        cola.pop();
        analizarPunto(imgBin, p->x, p->y, cola, objCuerpo);
        tamCola = cola.size();
    }
}

void cuerpos::exec_limpiarCuerpos(Mat &imgOrigi, Mat &imgOut) {
    Mat imgBinaria, imgMarcas, imgCuerpos;
    cvtColor(imgOrigi, imgCuerpos, CV_GRAY2BGR);

    threshold(imgOrigi, imgBinaria, 200, 255, CV_THRESH_BINARY_INV); // filtro de umbral para obtener la versión binaria
    copyMakeBorder(imgBinaria, imgBinaria, 1, 1, 1, 1, BORDER_CONSTANT, 0); // agregamos borde de 1px

    int imgAlto = imgBinaria.rows;
    int imgAncho = imgBinaria.cols;

    imgMarcas = Mat(imgAlto, imgAncho, CV_8U, Scalar(0));
    imgOut = imgOrigi.clone();

    int y, x;
    uchar* fila;
    uchar intensidad;
    int ancho, alto, area, numPuntos;
    float densidad, aspecto;
    Mat imgRoi;
    int marcar;

    Cuerpo *objCuerpo;

    for (y = 1 ; y < (imgAlto - 1) ; ++y) {
        fila = imgBinaria.ptr<uchar>(y);
        for (x = 1 ; x < (imgAncho - 1) ; ++x) {
            intensidad = fila[x];
            if (intensidad == 255) {
                objCuerpo = new Cuerpo();
                explorarCuerpo(imgBinaria, x, y, objCuerpo);

                // Evaluamos el cuerpo; debemos marcarlo?
                ancho = objCuerpo->ancho();
                alto = objCuerpo->alto();
                area = ancho * alto;
                numPuntos = objCuerpo->numPuntos();
                densidad = (float)numPuntos / (float)area; // [0:1] relación de pixeles ocupados a libres
                aspecto = (float)ancho / (float)alto;
                marcar = -1; // -1 = no marcar. 0 en adelante: marcar de color...

                //cout << "(" << x << ", " << y << ") ancho: " << ancho << ", alto: " << alto << ", area: " << area << ", puntos: " << numPuntos << ", densidad: " << densidad << ", aspecto: " << aspecto << endl;

                // Condiciones de marcado

                // Alta densidad y relación de aspecto media (figura no achatada o alargada)
                if (densidad > 0.3 && aspecto > 0.5 && aspecto < 2) marcar = 0;

                // Letras i, I, l:
                else if (densidad > 0.35 && (aspecto > 0.15 && aspecto < 0.55) && (alto <= 40) && (area > 100 && area < 700)) marcar = 1;

                // Cuerpos demasiado pequeños (se considera ruido)
                else if (area <= 60) marcar = 2;

                // Filtro agresivo de área reducida
                else if (area <= 2000) marcar = 3;


                if (marcar >= 0) {
                    marcarCuerpo(objCuerpo, marcar, imgCuerpos, imgOut);
                }
                else {
                    cout << "Cuerpo no marcado" << endl;
                }
            }
        }
    }

    /*namedWindow("Cuerpos", WINDOW_AUTOSIZE);
    imshow("Cuerpos", imgCuerpos);
    imwrite( "D:/cuerpos.bmp", imgCuerpos);
    waitKey(0);*/
}
