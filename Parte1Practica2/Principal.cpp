
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


int betaEntero = 0;
double betaDecimal = 0;
int k = 3, kImpar = 3; //filtro de la mediana

void funcionBeta(int valor, void *data){
    betaDecimal = (double) betaEntero;
    betaDecimal/=10;
}

void funcionK(int valor, void *data){
    if(k%2!=0){
        kImpar = k;
    }
}

int hmin = 0, hmax = 33, smin = 49, smax = 255, vmin = 255, vmax = 255;

void eventoTrack(int valor, void *data){
    //cout << "Valor: " << valor << endl;
    //cout << "hmin: " << valor << endl;
}

//metodo investigado 
const int MAX_FRAMES = 1000;
const int THRESH = 60;
const int ASSIGN_VALUE = 255;
Mat background;
bool isFirstFrame = true;

int main(int argc, char* args[]) {
    
    VideoCapture video(0);

    
    if (video.isOpened()) {
        namedWindow("Video", WINDOW_FREERATIO);
        //trackbar
        createTrackbar("HMin", "Video", &hmin, 180, eventoTrack, NULL);
        createTrackbar("SMin", "Video", &smin, 255, eventoTrack, NULL);
        createTrackbar("VMin", "Video", &vmin, 255, eventoTrack, NULL);
        createTrackbar("HMax", "Video", &hmax, 180, eventoTrack, NULL);
        createTrackbar("SMax", "Video", &smax, 255, eventoTrack, NULL);
        createTrackbar("VMax", "Video", &vmax, 255, eventoTrack, NULL);

        namedWindow("Motion Mask", WINDOW_FREERATIO);
        namedWindow("Background", WINDOW_FREERATIO);

        Mat frame;
        Mat binaria;
        Mat imgHSV;
        Mat imgLab;
        Mat movimiento;

        Mat frameGray, diff, motionMask;


        double inicio = static_cast<double>(getTickCount());
        int count = 0;


        while (true) {
            video >> frame;

            
            resize(frame, frame, Size(), 0.5, 0.5);
            Mat imgOscura = Mat::zeros(frame.size(), CV_8UC1);
            Mat imgEcualizada = Mat::zeros(frame.size(), CV_8UC1);
            Mat imgCLAHE = Mat::zeros(frame.size(), CV_8UC1);
            cvtColor(frame, imgOscura, COLOR_BGR2GRAY);
            cvtColor(frame, imgHSV, COLOR_BGR2HSV);
            cvtColor(frame, imgLab, COLOR_BGR2Lab);
            


            // en un cuadro vamos a poner los fps
            putText(frame, "FPS: " + to_string(static_cast<int>(round(count * getTickFrequency() / (static_cast<double>(getTickCount()) - inicio)))),
            Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);


            // equalizada y CLAHE
            equalizeHist(imgOscura, imgEcualizada);
            Ptr<CLAHE> clahe = createCLAHE();
            clahe->apply(imgOscura, imgCLAHE);
            clahe->setTilesGridSize(Size(24,24));


            //movimiento AND
            inRange(imgHSV, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), binaria);
            movimiento = Mat::zeros(frame.size(), CV_8UC3);
            bitwise_and(frame, frame, movimiento, binaria);

            //investigado  Background Substraction
            frameGray = imgOscura; 
            if (isFirstFrame) {
                background = frameGray.clone(); 
                isFirstFrame = false;
            } else {
                absdiff(background, frameGray, diff);
                threshold(diff, motionMask, THRESH, ASSIGN_VALUE, THRESH_BINARY);

                imshow("Motion Mask", motionMask);
                imshow("Background", background);
            }
            


            imshow("Video", frame);
            imshow("Ecualizada", imgEcualizada);
            imshow("CLAHE", imgCLAHE);
            imshow("Binario", binaria);



            if (waitKey(23) == 27)
                break;

            count++;
        }

        video.release();
        destroyAllWindows();
    }

    return 0;
}
