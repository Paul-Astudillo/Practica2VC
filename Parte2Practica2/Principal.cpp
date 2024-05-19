
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

int valorSal = 0, valorPimienta = 0;

void eventoTrack(int valor, void *data){
    //cout << "Valor: " << valor << endl;
    //cout << "hmin: " << valor << endl;
}

void agregarRuidoSalYPimienta(Mat& imagen, int porcentajeSal, int porcentajePimienta) {
    default_random_engine generador;
    uniform_int_distribution<int> distribucion(0, 100); //aleatroio

    for (int y = 0; y < imagen.rows; y++) {
        for (int x = 0; x < imagen.cols; x++) {
            int numero_aleatorio = distribucion(generador);

            if (numero_aleatorio < porcentajeSal) {
                int valor_r = rand() % 256;
                int valor_g = rand() % 256;
                int valor_b = rand() % 256;

                imagen.at<Vec3b>(y, x) = Vec3b(valor_b, valor_g, valor_r);
            }
            //pimienta
            else if (numero_aleatorio > (100 - porcentajePimienta)) {
                imagen.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
            }
        }
    }
}

void aplicarFiltroMediana(Mat& imagen, int tamano_mascara) {
    medianBlur(imagen, imagen, tamano_mascara);
}

void aplicarFiltroBlur(Mat& imagen, int tamano_mascara) {
    blur(imagen, imagen, Size(tamano_mascara, tamano_mascara));
}

void aplicarFiltroGaussiano(Mat& imagen, int tamano_mascara, double desviacion) {
    GaussianBlur(imagen, imagen, Size(tamano_mascara, tamano_mascara), desviacion);
}



void compararResultadosFiltros(Mat& frameOriginal, Mat& frameFiltro1, Mat& frameFiltro2, Mat& frameFiltro3) {

    Mat resultado= Mat::zeros(Size(frameOriginal.cols * 4, frameOriginal.rows), CV_8UC3);

    frameOriginal.copyTo(resultado(Rect(0, 0, frameOriginal.cols, frameOriginal.rows)));
    frameFiltro1.copyTo(resultado(Rect(frameOriginal.cols, 0, frameOriginal.cols, frameOriginal.rows)));
    frameFiltro2.copyTo(resultado(Rect(frameOriginal.cols * 2, 0, frameOriginal.cols, frameOriginal.rows)));
    frameFiltro3.copyTo(resultado(Rect(frameOriginal.cols * 3, 0, frameOriginal.cols, frameOriginal.rows)));

    imshow("Comparación de Resultados de Filtros", resultado);
}

void deteccionBordes(Mat& imagen, bool usarFiltro) {
    Mat bordes;

    if (usarFiltro) {
        medianBlur(imagen, imagen, 3); // Filtro de mediana para suavizar la imagen antes de la detección de bordes
    }

    Canny(imagen, bordes, 50, 150); 

    imshow("Detección de Bordes Canny", bordes);
}
void deteccionBordesSobel(Mat& imagen) {
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Mat grad;

    // gradiente en x
    Sobel(imagen, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);

    // gradiente y
    Sobel(imagen, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);

    // unir
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    imshow("Detección de Bordes Sobel", grad);
}





int main(int argc, char* args[]){

    // Manejo de Video
    VideoCapture video("/home/paul/Escritorio/VisionArtificial/Practica2/Parte2Practica2/The Breathtaking Beauty of Nature HD (720p).mp4");


    
    if (video.isOpened()){
        //namedWindow("Video",WINDOW_FREERATIO);
         namedWindow("Completo" , WINDOW_FREERATIO);
         namedWindow("Median" , WINDOW_FREERATIO);
         namedWindow("Blur" , WINDOW_FREERATIO);
         namedWindow("Gauss" , WINDOW_FREERATIO);
         namedWindow("Detección de Bordes Canny", WINDOW_FREERATIO);
         namedWindow("Detección de Bordes Sobel", WINDOW_FREERATIO);
         namedWindow("Comparación de Resultados de Filtros", WINDOW_FREERATIO);


         createTrackbar("Sal", "Completo", &valorSal, 30, eventoTrack, NULL);
         createTrackbar("Pimienta", "Completo", &valorPimienta, 30, eventoTrack, NULL);

        Mat frame;
        Mat completo ;
        Mat sal;
        Mat mediaBlur;
        Mat blur;
        Mat gauss;

        while(3==3){
            video >> frame;
            

            resize(frame, frame, Size(), 0.6, 0.6);
            sal=frame.clone();
            mediaBlur=frame.clone();
            blur=frame.clone();
            gauss=frame.clone();


            completo=Mat::zeros(Size(2*frame.cols, frame.rows), CV_8UC3);
            frame.copyTo(completo(Rect(0, 0, frame.cols, frame.rows)));
            


            //sal y pimiuenta
            agregarRuidoSalYPimienta(sal , valorSal , valorPimienta );
            sal.copyTo(completo(Rect(frame.cols,0 ,frame.cols , frame.rows)));


            //median OJO  numeros impares
           aplicarFiltroMediana(mediaBlur , 33);

           //blur
           aplicarFiltroBlur(blur, 15); 

            //gauss
           aplicarFiltroGaussiano(gauss, 15, 0);

           // sobel
            deteccionBordesSobel(frame);


          //imshow("Video", frame);
          imshow("Completo", completo);
          imshow("Median",mediaBlur);
          imshow("Blur",blur);
          imshow("Gauss",gauss);


          compararResultadosFiltros(frame, mediaBlur, blur, gauss);

          // con y sin suavizado
          //  deteccionBordes(frame, true); //con suavizado
           deteccionBordes(frame, false); //sin suavizado



            if(waitKey(23)==27)
                break;
        }
        video.release();
        destroyAllWindows();
    }

    return 0;
}