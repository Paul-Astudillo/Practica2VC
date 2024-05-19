
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <cstring>

// Librerías de OpenCV
//#include <opencv2/opencv.hpp>

#include <opencv2/imgproc.hpp> // Librería que contiene operaciones para realizar procesamiento de imágenes
#include <opencv2/imgcodecs.hpp> // Librería contiene los métodos para cargar imágenes de acuerdo a un códec
#include <opencv2/core.hpp> // Librería que contiene las estructuras y funciones base para representar las imágenes como matrices
#include <opencv2/video.hpp> // Librería para realizar lectura de vídeos
#include <opencv2/videoio.hpp> // Librería para escritura de vídoeos y streaming
#include <opencv2/highgui.hpp> // Librería para crear interfaces básicas de usuario


using namespace std;
using namespace cv;

int main(int argc, char* args[]){

    Mat torax = imread("torax.jpeg");
    resize(torax, torax, Size(), 0.3, 0.3);
    cvtColor(torax , torax , COLOR_BGR2GRAY);
    Mat totalTorax = Mat::zeros(Size(torax.cols * 5, torax.rows), CV_8UC1); 
    
    
    

    Mat dientes =imread("dientes.png");
    resize(dientes, dientes, Size(), 0.2, 0.2);
    cvtColor(dientes , dientes , COLOR_BGR2GRAY);
    Mat totalDientes = Mat::zeros(Size(dientes.cols * 5, dientes.rows), CV_8UC1); 
    
    

    Mat cerebro =imread("cerebro.jpg");
    cvtColor(cerebro , cerebro , COLOR_BGR2GRAY);
    Mat totalCerebro = Mat::zeros(Size(cerebro.cols * 5, cerebro.rows), CV_8UC1); 
    
    

    // cout <<"Canales Torax : " << torax.channels() << endl; 
    // cout <<"Canales dientes : " << dientes.channels() << endl; 
    // cout <<"Canales cerebro : " << cerebro.channels() << endl; 


    // Kernel
    Size kernel_size(37, 37);


    //torax
    Mat toraxErosion, toraxDilatacion, toraxTopHat, toraxBlackHat, toraxFinal;
    morphologyEx(torax, toraxErosion, MORPH_ERODE, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(torax, toraxDilatacion, MORPH_DILATE, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(torax, toraxTopHat, MORPH_TOPHAT, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(torax, toraxBlackHat, MORPH_BLACKHAT, getStructuringElement(MORPH_RECT, kernel_size));
    addWeighted(torax, 1, toraxTopHat - toraxBlackHat, 1, 0, toraxFinal);

    toraxErosion.copyTo(totalTorax(Rect(0,0 ,toraxErosion.cols ,toraxErosion.rows )));
    toraxDilatacion.copyTo(totalTorax(Rect(torax.cols,0 ,toraxDilatacion.cols ,toraxDilatacion.rows )));
    toraxTopHat.copyTo(totalTorax(Rect(torax.cols*2,0 ,toraxTopHat.cols ,toraxTopHat.rows )));
    toraxBlackHat.copyTo(totalTorax(Rect(torax.cols*3,0 ,toraxBlackHat.cols ,toraxBlackHat.rows )));
    toraxFinal.copyTo(totalTorax(Rect(torax.cols*4,0 ,toraxFinal.cols ,toraxFinal.rows )));

    //dientes
    Mat dientesErosion, dientesDilatacion, dientesTopHat, dientesBlackHat, dientesFinal;
    morphologyEx(dientes, dientesErosion, MORPH_ERODE, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(dientes, dientesDilatacion, MORPH_DILATE, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(dientes, dientesTopHat, MORPH_TOPHAT, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(dientes, dientesBlackHat, MORPH_BLACKHAT, getStructuringElement(MORPH_RECT, kernel_size));
    addWeighted(dientes, 1, dientesTopHat - dientesBlackHat, 1, 0, dientesFinal);

    dientesErosion.copyTo(totalDientes(Rect(0,0 ,dientesErosion.cols ,dientesErosion.rows )));
    dientesDilatacion.copyTo(totalDientes(Rect(dientes.cols,0 ,dientesDilatacion.cols ,dientesDilatacion.rows )));
    dientesTopHat.copyTo(totalDientes(Rect(dientes.cols*2,0 ,dientesTopHat.cols ,dientesTopHat.rows )));
    dientesBlackHat.copyTo(totalDientes(Rect(dientes.cols*3,0 ,dientesBlackHat.cols ,dientesBlackHat.rows )));
    dientesFinal.copyTo(totalDientes(Rect(dientes.cols*4,0 ,dientesFinal.cols ,dientesFinal.rows )));

    //cerebro
    Mat cerebroErosion, cerebroDilatacion, cerebroTopHat, cerebroBlackHat, cerebroFinal;
    morphologyEx(cerebro, cerebroErosion, MORPH_ERODE, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(cerebro, cerebroDilatacion, MORPH_DILATE, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(cerebro, cerebroTopHat, MORPH_TOPHAT, getStructuringElement(MORPH_RECT, kernel_size));
    morphologyEx(cerebro, cerebroBlackHat, MORPH_BLACKHAT, getStructuringElement(MORPH_RECT, kernel_size));
    addWeighted(cerebro, 1, cerebroTopHat - cerebroBlackHat, 1, 0, cerebroFinal);

    cerebroErosion.copyTo(totalCerebro(Rect(0,0 ,cerebroErosion.cols ,cerebroErosion.rows )));
    cerebroDilatacion.copyTo(totalCerebro(Rect(cerebro.cols,0 ,cerebroDilatacion.cols ,cerebroDilatacion.rows )));
    cerebroTopHat.copyTo(totalCerebro(Rect(cerebro.cols*2,0 ,cerebroTopHat.cols ,cerebroTopHat.rows )));
    cerebroBlackHat.copyTo(totalCerebro(Rect(cerebro.cols*3,0 ,cerebroBlackHat.cols ,cerebroBlackHat.rows )));
    cerebroFinal.copyTo(totalCerebro(Rect(cerebro.cols*4,0 ,cerebroFinal.cols ,cerebroFinal.rows )));


    namedWindow("OriginalTORAX" , WINDOW_AUTOSIZE);
    namedWindow("OriginalDientes" , WINDOW_AUTOSIZE);
    namedWindow("OriginalCerebro" , WINDOW_AUTOSIZE);

    namedWindow("Torax" , WINDOW_AUTOSIZE);
    namedWindow("Dientes" , WINDOW_AUTOSIZE);
    namedWindow("Cerebro", WINDOW_AUTOSIZE);

    imshow("OriginalTORAX", torax);
    imshow("OriginalDientes", dientes);
    imshow("OriginalCerebro", cerebro);

    imshow("Torax", totalTorax);
    imshow("Dientes", totalDientes);
    imshow("Cerebro", totalCerebro);


    waitKey(0);

    return 0;
    
}