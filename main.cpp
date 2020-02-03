#include <cstdlib>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <deque>
#include <vector>
#include<bits/stdc++.h>
#include "LZ77.h"
using namespace std;


SDL_Surface *screen;
SDL_Surface *obraz;
SDL_Surface *tlo;
SDL_Rect tloDane;
SDL_Rect tloCel;
SDL_Surface *przycisk[3];
int width = 900;
int height = 600;
char const* tytl = "projektGKIM";
char *sygnatura = "bzzs";
SDL_Rect przyciskDane;
SDL_Rect przyciskCel[3];
int myszkaX, myszkaY;
int wybor_palety;
int wybor_rozmiaru_bufora;

struct SDL_4bitColor{
    Uint8 r;
    Uint8 g;
    Uint8 b;
};
SDL_Color *PaletaD = new SDL_Color [16]; // dedykowana paleta

SDL_4bitColor PaletaKolorow[16]=
{
{0,0,0}, //0
{0,0,1}, //1
{0,1,0}, //2
{0,1,1}, //3
{0,2,0}, //4
{0,2,1}, //5
{0,3,0}, //6
{0,3,1}, //7
{1,0,0}, //8
{1,0,1}, //9
{1,1,0}, //10
{1,1,1}, //11
{1,2,0}, //12
{1,2,1}, //13
{1,3,0}, //14
{1,3,1} //15
};



void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);
SDL_4bitColor przesuniecieBitowe(int x, int y);
void set4bitPixel(int x, int y, SDL_4bitColor bit4);
SDL_4bitColor poszukiwanieSomsiada(int x, int y);
void ladujBMP(char const *nazwa, int x, int y);

void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void listaKolorowDelete(SDL_Color *tab){
    delete [] tab;
}
void bit4paleta(){

    int kolor=0;
    for(int i=1;i<width/2;i++){
        for(int j=0;j<height/2;j++){
                set4bitPixel(i,j,PaletaKolorow[kolor]);

        }
        if(i%16 == 0)
                    kolor++;
    }

}
SDL_Color *listaKolorow(int width, int height){
    int arrayLength = width*height;
    SDL_Color *tab = new SDL_Color [arrayLength];
    for(int i = 0; i < width/2; i++){
        for (int j = 0; j < height/2; j++){
            tab[i*height/2 + j] = getPixel(i, j);
        }
    }
    return tab;
}

SDL_Color srednia (SDL_Color kolor1, SDL_Color kolor2){
    SDL_Color srednia;
    int srR = kolor1.r+kolor2.r/2;
    int srG = kolor1.g+kolor2.g/2;
    int srB = kolor1.b+kolor2.b/2;
    srednia.r = srR;
    srednia.g = srG;
    srednia.b = srB;
    return srednia;
}

void sortowanieListyKolorow(SDL_Color *listaKolor, int *lista, int arrayLength){
    int k;
    unsigned int R = 0, srR = 0;
    unsigned int G = 0, srG = 0;
    unsigned int B = 0, srB = 0;
    for(int i = 0; i < arrayLength; i++){
            R += listaKolor[i].r;
            G += listaKolor[i].g;
            B += listaKolor[i].b;
    }
    srR = R/arrayLength;
    srG = G/arrayLength;
    srB = B/arrayLength;
    for(int i = 0; i < arrayLength; i++) lista[i] = (srR*listaKolor[i].r) + (srG*listaKolor[i].g) + (srB*listaKolor[i].b);

    for(int i = 0; i < arrayLength; i++){
        k = i;
        for(int j = i+1; j<arrayLength; j++){
            if(lista[j]>lista[k]) k=j;
        }
        swap(lista[k], lista[i]);
        swap(listaKolor[k], listaKolor[i]);
    }
}

void showSortedTab (SDL_Color *listaKolor){
    unsigned int temp = 0;
    for (int i = 0; i < width/2; i++){
        for (int j = 0; j < height/2; j++){
            temp++;
            setPixel(i+450, j, listaKolor[temp-1].r, listaKolor[temp-1].g, listaKolor[temp-1].b);
        }
    }
}

void wpiszObrazekDoTablicy (SDL_Color *listaKolor, int arrayLength){
    unsigned int temp = 0;
    for (int i = 0; i < width/2; i++){
        for (int j = 0; j < height/2; j++){
            listaKolor[temp] = getPixel (i+450, j);
            temp++;
        }
    }
}

void podzielNaBloki (int poczatek, SDL_Color *listaKolor, int koniec){
    unsigned int R, G, B, elem;
    unsigned int srR, srG, srB;
    for (int j = 0; j < 32; j++){
        R=0, G=0, B=0, elem=0, srR=0, srG=0, srB=0;
        for (int i = 16*j; i < 16*j+16; i++){
                R += listaKolor[i].r;
                G += listaKolor[i].g;
                B += listaKolor[i].b;
                elem++;
        }
        srR = R/elem;
        srG = G/elem;
        srB = B/elem;
        for (int i = 16*j; i < 16*j+16; i++){
            listaKolor[i].r = srR;
            listaKolor[i].g = srG;
            listaKolor[i].b = srB;
        }
    }
}



vector<bool> naglowek() {
	vector<bool> output;
	deque<bool> temp;

	//sygnatura
	for (int i = 0; i < 4; i++) {
		temp = (int_to_binary(static_cast<int>(sygnatura[i])));
		while (temp.size() < 8) {
			temp.push_front(0);
		}
		for (int i = 0; i < temp.size(); i++) {
			output.push_back(temp[i]);
		}
		temp.clear();
	}
	//szerokosc
	temp = int_to_binary(width);

	while (temp.size() < 8*4) {
		temp.push_front(0);
	}
	for (int i = 0; i < temp.size(); i++) {
		output.push_back(temp[i]);
	}
	temp.clear();
	//wysokosc
	temp = int_to_binary(height);

	while (temp.size() < 8*4) {
		temp.push_front(0);
	}
	for (int i = 0; i < temp.size(); i++) {
		output.push_back(temp[i]);
	}
	temp.clear();
	//wybor palety
	temp = int_to_binary(wybor_palety);

	while (temp.size() < 8) {
		temp.push_front(0);
	}
	for (int i = 0; i < temp.size(); i++) {
		output.push_back(temp[i]);
	}
	temp.clear();
	//wybor rozmiaru bufora
	temp = int_to_binary(wybor_rozmiaru_bufora);
	while (temp.size() < 8*4) {
		temp.push_front(0);
	}
	for (int i = 0; i < temp.size(); i++) {
		output.push_back(temp[i]);
	}
	temp.clear();
	//paleta dedykowana
	for (int i = 0; i < 16; i++) {
        temp = int_to_binary(PaletaD[i].r);
        	for (int i = 0; i < temp.size(); i++) {
		output.push_back(temp[i]);
	}
        temp.clear();
        temp = int_to_binary(PaletaD[i].g);
        	for (int i = 0; i < temp.size(); i++) {
		output.push_back(temp[i]);
	};
        temp.clear();
        temp = int_to_binary(PaletaD[i].b);
        	for (int i = 0; i < temp.size(); i++) {
		output.push_back(temp[i]);
	}
        temp.clear();
    }


    return output;
}
void wypisz_plik(vector<bool> naglowek, vector<bool> dane) {
    fstream plik;
    plik.open("obrazek.bssz",  ios::binary | ios::out);
    for (int i = 0; i < naglowek.size(); i++) {
        plik << naglowek[i]<<endl;
    }
    for (int i = 0; i < dane.size(); i++) {
        plik << dane[i]<<endl;
    }
    plik.close();
}
bool saveToBmp(){
    //SDL_Surface* outputBMP = SDL_CreateRGBSurfaceFrom(tablica_pikseli, width, height, 8, 8*width, 0, 0, 0, 0);
    //SDL_SaveBMP(outputBMP, "obrazek_po_dekompresji.bmp");
}
vector<bool> odczytaj_plik() {
    vector<bool> dane;
    deque<bool> naglowek;
    deque<bool> temp;

    fstream plik;
    string c;
    plik.open("obrazek.bssz", ios::binary | ios::in);
    for(int i = 0 ;i<142*8 ; i ++){
    getline(plik,c);
    naglowek.push_back(c[0]);
    }
    //sygnatura
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8 ; j++) {
            temp.push_back(naglowek.front());
            naglowek.pop_front();
        }
       // sygnatura[i] = (binary_to_int(temp));
    }
    temp.clear();

    //szerokosc
    for (int i = 0; i < 8 * 4; i++) {
        temp.push_back(naglowek.front());
        naglowek.pop_front();
    }
    width = binary_to_int(temp);
    temp.clear();

    //wysokosc
    for (int i = 0; i < 8 * 4; i++) {
        temp.push_back(naglowek.front());
        naglowek.pop_front();
    }
    height = binary_to_int(temp);
    temp.clear();
    //wybor palety
    for (int i = 0; i < 8; i++) {
        temp.push_back(naglowek.front());
        naglowek.pop_front();
    }
    wybor_palety = binary_to_int(temp);
    temp.clear();
    //wybor rozmiaru bufora
    for (int i = 0; i < 8; i++) {
        temp.push_back(naglowek.front());
        naglowek.pop_front();
    }
    wybor_rozmiaru_bufora = binary_to_int(temp);
    temp.clear();
    //paleta dedykowana
 for (int i = 0; i < 16; i++) {
        for (int i = 0; i < 8; i++) {
            temp.push_back(naglowek.front());
            naglowek.pop_front();
        }
        PaletaD[i].r = binary_to_int(temp);
        for (int i = 0; i < 8; i++) {
            temp.push_back(naglowek.front());
            naglowek.pop_front();
        }
        PaletaD[i].g = binary_to_int(temp);
        for (int i = 0; i < 8; i++) {
            temp.push_back(naglowek.front());
            naglowek.pop_front();
        }
        PaletaD[i].b = binary_to_int(temp);
    }
    plik.close();
    return dane;
}

SDL_4bitColor przesuniecieBitowe(int x, int y){
    //RRGGB
    int R, G, B;
    SDL_4bitColor bit4;
    SDL_Color kolor;
    kolor = getPixel(x,y);
    R = kolor.r; G = kolor.g; B=kolor.b;
            // R
  if(R <= 127)
        bit4.r = 0;
    else if(R >= 128)
        bit4.r = 1;

    if(G <= 63)
        bit4.g = 0;
    else if(G >= 64 && G <= 127)
        bit4.g= 1;
    else if(G >= 128 && G <= 191)
        bit4.g = 2;
    else if(G >= 192 && G <= 255)
        bit4.g = 3;

    if(B <= 127)
        bit4.b = 0;
    else if(B >= 128)
        bit4.b = 1;

    return bit4;
}

int przesuniecieBitoweBW(int x, int y){
    //RRGGB
    double R, G, B;
    SDL_4bitColor bit4;
    SDL_Color kolor;
    kolor = getPixel(x,y);
    R = kolor.r; G = kolor.g; B=kolor.b;

    double BW = (0.299*R) + (0.587*G) + (0.114*B);

    double bit4BW= BW/16;
    return bit4BW;
}

SDL_4bitColor poszukiwanieSomsiada(int x, int y){
    //RRGGB
    int R, G, B;
    SDL_4bitColor bit4;
    SDL_Color kolor;
    kolor = getPixel(x,y);
    R = kolor.r; G = kolor.g; B=kolor.b;
            // R
    // 0 85 170 255 //

   if(0+R < 255 -R)
        bit4.r = 0;
    else
        bit4.r = 1;

    if(0+G <= 85-G)
        bit4.g = 0;
    else if(G-85 <= 170-G)
        bit4.g= 1;
    else if(G-170 <= 255-G)
        bit4.g = 2;
    else if(G-170 >= 255-G)
        bit4.g = 3;

    if(0+B < 255 -B)
        bit4.b = 0;
    else
        bit4.b = 1;

    return bit4;
}

void set4bitPixel(int x, int y, SDL_4bitColor bit4){

    int R, G, B;

   if(bit4.r == 0)
        R = 0;
    else if(bit4.r == 1)
        R = 255;

    if(bit4.g == 0)
        G = 0;
    else if(bit4.g == 1)
        G = 85;
    else if(bit4.g == 2)
        G = 170;
    else if(bit4.g == 3)
        G = 255;

    if(bit4.b == 0)
        B = 0;
    else if(bit4.b == 1)
        B = 255;

    setPixel(x,y,R,G,B);
}
void set4bitPixelBW(int x, int y, int BW){
    setPixel(x,y,BW*16,BW*16,BW*16);
}

int RGBnaINT4(SDL_4bitColor bit4){
    int index=0;

    while(bit4.r!=0)
        index+=8, bit4.r--;
    while(bit4.g!=0)
        index+=2, bit4.g--;
    while(bit4.b!=0)
        index+=1, bit4.b--;

    return index;
}
int RGBnaINT4BW(SDL_Color bit4){
    int index=0;

    while(bit4.r!=0)
        index+=8, bit4.r--;
    while(bit4.g!=0)
        index+=2, bit4.g--;
    while(bit4.b!=0)
        index+=1, bit4.b--;

    return index;
}
SDL_Color findColor(SDL_Color* listaKolor, SDL_Color pixel, int &minimum){
    for (int i = 1; i < 16; i++) if (sqrt((pixel.r-listaKolor[i].r)*(pixel.r-listaKolor[i].r)+
                                          (pixel.g-listaKolor[i].g)*(pixel.g-listaKolor[i].g)+
                                          (pixel.b-listaKolor[i].b)*(pixel.b-listaKolor[i].b)) <
                                     sqrt((pixel.r-listaKolor[minimum].r)*(pixel.r-listaKolor[minimum].r)+
                                          (pixel.g-listaKolor[minimum].g)*(pixel.g-listaKolor[minimum].g)+
                                          (pixel.b-listaKolor[minimum].b)*(pixel.b-listaKolor[minimum].b)) )
                                            minimum = i;
    return listaKolor[minimum];
}
int RGB_Distance(SDL_Color color, SDL_Color palette){

    return sqrt(pow(color.r - palette.r,2) + pow(color.g - palette.g,2) + pow(color.b - palette.b, 2));
}

int Find_Nearest(SDL_Color color, SDL_Color palette[])
{
    int dist;
    int minimum = RGB_Distance(color,palette[0]);
    int indx = 0;

    for(int i=1; i<16; i++){

        dist = RGB_Distance(color,palette[i]);

        if(dist <= minimum)
        {
            indx = i;
            minimum = dist;
        }
    }

    return indx;
}

SDL_4bitColor ColorTo4bitColor(SDL_Color kolor) {      // dla 4bit pixeli

	SDL_4bitColor kolor4bit;
	int actualR;
	int actualG;
	int actualB;
			actualR = kolor.r;

			actualG = kolor.g;

			actualB = kolor.b;

			if (actualR < 128) {           //R
				kolor4bit.r = 0;
			}
			else {
				kolor4bit.r = 1;
			}

			if (actualG < 64) {           //G
				kolor4bit.g = 0;
			}
			else if (actualG < 128) {
				kolor4bit.g = 1;
			}
			else if (actualG < 192) {
				kolor4bit.g = 2;
			}
			else {
				kolor4bit.g = 3;
			}

			if (actualB < 128) {           //B
				kolor4bit.b = 0;
			}
			else {
				kolor4bit.b = 1;
			}

			return kolor4bit;
}

void ditheringInGreyScale4bitColor(vector<int> &wektorek, int startY, int startX){

  SDL_Color kolor ;
    int accurateBlackWhite ;
    float errors[(width/2) + 2][(height/2) + 2] ; // zeby nie pisac warunkow przekroczenia tablicy
    memset(errors, 0, sizeof(errors)) ;
    int BW = 0;
    int blad = 0 ;
    int przesuniecie = 1 ; // aby nie wyjsc pponizej (-1) tabeli bladow
        for(int x=startX; x< startX + 450; x++){
    for(int y=startY; y<startY + 300; y++){
           kolor = getPixel(x,y) ;
           accurateBlackWhite = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
           accurateBlackWhite += errors[(x - startX) + przesuniecie][y - startY] ;

           for(int i=0; i<=16; i++){
                if(i == 16) {
                    setPixel(x + 450, y, 255, 255, 255) ;
                    blad = accurateBlackWhite - 255 ;
                    break ;
                }
                else if(accurateBlackWhite < 3 + (4 * i) ){
                    setPixel(x + 450, y, 4 * i, 4 * i, 4 * i );
                    blad = accurateBlackWhite - (4 * i) ;
                    break ;
                }
           }
           wektorek.push_back(RGBnaINT4(ColorTo4bitColor(kolor)));
            errors[(x - startX)+przesuniecie + 1][(y - startY)    ] += (blad * 7.0 / 16.0 ) ;
            errors[(x - startX)+przesuniecie + 1][(y - startY) + 1] += (blad * 1.0 / 16.0 ) ;
            errors[(x - startX)+przesuniecie    ][(y - startY) + 1] += (blad * 5.0 / 16.0 ) ;
            errors[(x - startX)+przesuniecie -1 ][(y - startY) + 1] += (blad * 3.0 / 16.0 ) ;
        }
    }
}                   // dithering 2 - > paleta dedykowana

void set4bitPixelForcedColors(int x, int y, SDL_4bitColor bit4, int *tableRed, int *tableGreen, int *tableBlue){

    int R, G, B;

    if(bit4.r == 0)
        R = tableRed[0];
    else if(bit4.r == 1)
        R = tableRed[1];

    if(bit4.g == 0)
        G = tableGreen[0];
    else if(bit4.g == 1)
        G = tableGreen[1];
    else if(bit4.g == 2)
        G = tableGreen[2];
    else if(bit4.g == 3)
        G = tableGreen[3];

    if(bit4.b == 0)
        B = tableBlue[0];
    else if(bit4.b == 1)
        B = tableBlue[1];

    setPixel(x,y,R,G,B);
}

void ditheringInForcedScale(vector <int> &wektor, int startX, int startY, int *tableRed, int *tableGreen, int *tableBlue){      // dla 4bit pixeli

    SDL_Color kolor ;
    SDL_4bitColor kolor4bit ;
    int actualR ;
    int actualG ;
    int actualB ;

    float errorsR[(width/2) + 2][(height/2) + 2] ; //
    float errorsG[(width/2) + 2][(height/2) + 2] ; //
    float errorsB[(width/2) + 2][(height/2) + 2] ; //
    memset(errorsR, 0, sizeof(errorsR)) ;
    memset(errorsG, 0, sizeof(errorsG)) ;
    memset(errorsB, 0, sizeof(errorsB)) ;

    int bladR = 0 ;
    int bladG = 0 ;
    int bladB = 0 ;
    int przesuniecie = 1 ; // aby nie wyjsc pponizej (-1) tabeli bladow
     for(int x=startX; x< startX + 450; x++){
    for(int y=startY; y<startY + 300; y++){
           kolor = getPixel(x,y) ;

            actualR = kolor.r ;
            actualR += errorsR[(x - startX) + przesuniecie][y - startY] ;

            actualG = kolor.g ;
            actualG += errorsG[(x - startX) + przesuniecie][y - startY] ;

            actualB = kolor.b ;
            actualB += errorsB[(x - startX) + przesuniecie][y - startY] ;

          if(actualR < 128){           //R
                kolor4bit.r = 0 ;
                bladR = actualR - tableRed[0];
            }
            else {
                kolor4bit.r = 1 ;
                bladR = actualR - tableRed[1] ;
            }

            if(actualG < 64){           //G
                kolor4bit.g = 0 ;
                bladG = actualG - tableGreen[0];
            }
            else if(actualG < 128){
                kolor4bit.g = 1 ;
                bladG = actualG - tableGreen[1];
            }
            else if(actualG < 192){
                kolor4bit.g = 2 ;
                bladG = actualG - tableGreen[2];
            }
            else{
                kolor4bit.g = 3 ;
                bladG = actualG - tableGreen[3];
            }

            if(actualB < 128){           //B
                kolor4bit.b = 0 ;
                bladB = actualB - tableBlue[0];
            }
            else {
                kolor4bit.b = 1 ;
                bladB = actualB - tableBlue[1] ;
            }
            wektor.push_back(RGBnaINT4(kolor4bit));
            set4bitPixelForcedColors(x+450, y, kolor4bit, tableRed, tableGreen, tableBlue);
            errorsR[(x - startX)+przesuniecie + 1][(y - startY)    ] += (bladR * 7.0 / 16.0 ) ;
            errorsR[(x - startX)+przesuniecie + 1][(y - startY) + 1] += (bladR * 1.0 / 16.0 ) ;
            errorsR[(x - startX)+przesuniecie    ][(y - startY) + 1] += (bladR * 5.0 / 16.0 ) ;
            errorsR[(x - startX)+przesuniecie -1 ][(y - startY) + 1] += (bladR * 3.0 / 16.0 ) ;

            errorsG[(x - startX)+przesuniecie + 1][(y - startY)    ] += (bladG * 7.0 / 16.0 ) ;
            errorsG[(x - startX)+przesuniecie + 1][(y - startY) + 1] += (bladG * 1.0 / 16.0 ) ;
            errorsG[(x - startX)+przesuniecie    ][(y - startY) + 1] += (bladG * 5.0 / 16.0 ) ;
            errorsG[(x - startX)+przesuniecie -1 ][(y - startY) + 1] += (bladG * 3.0 / 16.0 ) ;

            errorsB[(x - startX)+przesuniecie + 1][(y - startY)    ] += (bladB * 7.0 / 16.0 ) ;
            errorsB[(x - startX)+przesuniecie + 1][(y - startY) + 1] += (bladB * 1.0 / 16.0 ) ;
            errorsB[(x - startX)+przesuniecie    ][(y - startY) + 1] += (bladB * 5.0 / 16.0 ) ;
            errorsB[(x - startX)+przesuniecie -1 ][(y - startY) + 1] += (bladB * 3.0 / 16.0 ) ;
        }
    }
}

void ditheringInOriginalScale(vector<int> &wektorek, int startX, int startY) {      // dla 4bit pixeli

	SDL_Color kolor;
	SDL_4bitColor kolor4bit;
	SDL_Color pixel1, pixel2;
	int actualR;
	int actualG;
	int actualB;
	//int act

	float errorsR[(width / 2) + 2][(height / 2) + 2]; //
	float errorsG[(width / 2) + 2][(height / 2) + 2]; //
	float errorsB[(width / 2) + 2][(height / 2) + 2]; //
	memset(errorsR, 0, sizeof(errorsR));
	memset(errorsG, 0, sizeof(errorsG));
	memset(errorsB, 0, sizeof(errorsB));

	int bladR = 0;
	int bladG = 0;
	int bladB = 0;
	int przesuniecie = 1; // aby nie wyjsc pponizej (-1) tabeli bladow
	for (int x = startX; x < startX + 450; x++) {
	for (int y = startY; y < startY + 300; y++) {
			kolor = getPixel(x, y);

			actualR = kolor.r;
			actualR += errorsR[(x - startX) + przesuniecie][y - startY];

			actualG = kolor.g;
			actualG += errorsG[(x - startX) + przesuniecie][y - startY];

			actualB = kolor.b;
			actualB += errorsB[(x - startX) + przesuniecie][y - startY];

			if (actualR < 128) {           //R
				kolor4bit.r = 0;
				bladR = actualR;
			}
			else {
				kolor4bit.r = 1;
				bladR = actualR - 255;
			}

			if (actualG < 64) {           //G
				kolor4bit.g = 0;
				bladG = actualG;
			}
			else if (actualG < 128) {
				kolor4bit.g = 1;
				bladG = actualG - 85;
			}
			else if (actualG < 192) {
				kolor4bit.g = 2;
				bladG = actualG - 170;
			}
			else {
				kolor4bit.g = 3;
				bladG = actualG - 255;
			}

			if (actualB < 128) {           //B
				kolor4bit.b = 0;
				bladB = actualB;
			}
			else {
				kolor4bit.b = 1;
				bladB = actualB - 255;
			}
			pixel1.r = actualR;
			pixel1.g = actualG;
			pixel1.b = actualB;
			int index = 0;
            pixel2 = findColor(PaletaD, pixel1,index);
            wektorek.push_back(index);
			set4bitPixel(x + 450, y, kolor4bit);
			errorsR[(x - startX) + przesuniecie + 1][(y - startY)] += (bladR * 7.0 / 16.0);
			errorsR[(x - startX) + przesuniecie + 1][(y - startY) + 1] += (bladR * 1.0 / 16.0);
			errorsR[(x - startX) + przesuniecie][(y - startY) + 1] += (bladR * 5.0 / 16.0);
			errorsR[(x - startX) + przesuniecie - 1][(y - startY) + 1] += (bladR * 3.0 / 16.0);

			errorsG[(x - startX) + przesuniecie + 1][(y - startY)] += (bladG * 7.0 / 16.0);
			errorsG[(x - startX) + przesuniecie + 1][(y - startY) + 1] += (bladG * 1.0 / 16.0);
			errorsG[(x - startX) + przesuniecie][(y - startY) + 1] += (bladG * 5.0 / 16.0);
			errorsG[(x - startX) + przesuniecie - 1][(y - startY) + 1] += (bladG * 3.0 / 16.0);

			errorsB[(x - startX) + przesuniecie + 1][(y - startY)] += (bladB * 7.0 / 16.0);
			errorsB[(x - startX) + przesuniecie + 1][(y - startY) + 1] += (bladB * 1.0 / 16.0);
			errorsB[(x - startX) + przesuniecie][(y - startY) + 1] += (bladB * 5.0 / 16.0);
			errorsB[(x - startX) + przesuniecie - 1][(y - startY) + 1] += (bladB * 3.0 / 16.0);
		}
	}
}
void showDecoded1(vector <int> decoding){
    int k=0;
    for(int i=0;i<width/2;i++){
        for(int j=0;j<height/2;j++){
            set4bitPixel(i+450,j+300,PaletaKolorow[decoding[k]]);
            k++;
        }
    }
}
void showDecoded2(vector <int> decoding){
    int k=0;
    int BW=0;
    for(int i=0;i<width/2;i++){
        for(int j=0;j<height/2;j++){
            BW=decoding[k];
            set4bitPixelBW(i+450,j+300,BW);
            k++;
        }
    }
}
void showDecoded3(vector <int> decoding){
    int k=0;
    int index=0;
    for(int i=0;i<width/2;i++){
        for(int j=0;j<height/2;j++){
            index=decoding[k];
            setPixel(i+450,j+300,PaletaD[index].r,PaletaD[index].g,PaletaD[index].b);
            k++;
        }
    }
}
void Funkcja1(vector <int> &word) {
    ladujBMP("obrazek1.bmp",0,0);
    SDL_4bitColor kolor;
    cout<<"Wybierz: " <<endl;
    cout<<"1. Bez ditheringu"<<endl;
    cout<<"2. Z ditheringiem"<<endl;
    int wybor = 0;
    cin>>wybor;
    switch(wybor){
        case 1:{
    for(int i=0;i<width/2;i++){
        for(int j=0;j<height/2;j++){
             kolor = przesuniecieBitowe(i,j);
             word.push_back(RGBnaINT4(kolor));
             set4bitPixel(i,j+300, kolor);
        }
    }
     wybor_palety = 0;
        }
        break;
        case 2:{
    int *tableRed = new int[4] ;

    tableRed[0] = 15 ;
    tableRed[1] = 240 ;

    int *tableGreen = new int[4] ;

    tableGreen[0] = 15 ;
    tableGreen[1] = 95 ;
    tableGreen[2] = 160 ;
    tableGreen[3] = 240 ;

    int *tableBlue = new int[2] ;

    tableBlue[0] = 15 ;
    tableBlue[1] = 240 ;

    ditheringInForcedScale(word,0,0, tableRed, tableGreen, tableBlue);
     wybor_palety = 3;
        }
        break;
        default:
            break;
    }
}

void Funkcja2(vector <int> &word) {
    	bit4paleta();
		SDL_Flip(screen);
}

void Funkcja3(vector <int> &word) {
        ladujBMP("obrazek1.bmp",0,0);
cout<<"Wybierz: " <<endl;
    cout<<"1. Bez ditheringu"<<endl;
    cout<<"2. Z ditheringiem"<<endl;
    int wybor = 0;
    cin>>wybor;
    switch(wybor){
        case 1:{
    int BW=0;
    for(int i=0;i<width/2;i++){
        for(int j=0;j<height/2;j++){
             BW = przesuniecieBitoweBW(i,j);
             word.push_back(BW);
             set4bitPixelBW(i,j+300, BW);
              wybor_palety = 2;
        }
    }
        }
        break;
        case 2:{
    ditheringInGreyScale4bitColor(word,0,0);
     wybor_palety = 5;
        }
        break;
          default:
            break;
    }
}


void Funkcja4(vector <int> &word, SDL_Color PaletaD[]) {
		SDL_Color pixel1, pixel2;
		ladujBMP("obrazek1.bmp",0,0);
		SDL_Flip(screen);
		cout<<"Wybierz: " <<endl;
    cout<<"1. Bez ditheringu"<<endl;
    cout<<"2. Z ditheringiem"<<endl;
    int wybor = 0;
    cin>>wybor;
		int* tempTab = new int [16];
		SDL_Color* tablica = listaKolorow(width/2, height/2);
		sortowanieListyKolorow(tablica, tempTab, 16);
		podzielNaBloki(0, tablica, 15);
		delete [] tempTab;
		for(int i = 0; i < 16; i++) PaletaD[i] = tablica[i*2110];
		   switch(wybor){
        case 1:{
		for(int i = 0; i < width/2; i++){
			for(int j = 0; j < height/2; j++){
				int index=0;
				pixel1 = getPixel(i, j);
				pixel2 = findColor(PaletaD, pixel1,index);
				word.push_back(index);
				setPixel(i, j+300, pixel2.r, pixel2.g, pixel2.b);
			}
		}

    wybor_palety = 1;
        }
        break;
        case 2:{
        		listaKolorowDelete(tablica);
          ditheringInOriginalScale(word,0,0) ;
          wybor_palety = 4;
          SDL_Flip(screen);
        }
            break;
              default:
            break;
    }

}


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<width) && (y>=0) && (y<height))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru pixela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informacji ile bajtów zajmuje jeden pixel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres pixela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość pixela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;

    }
         /* update the screen (aka double buffering) */
  }
}
void zapiszDoPliku(){
}
void ladujBMP(char const* nazwa, int x, int y)
{
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        width = bmp->w*2;
        height = bmp->h*2;
        SDL_Rect dstrect;
        dstrect.x = x;
        dstrect.y = y;
        SDL_BlitSurface(bmp, 0, screen, &dstrect);
        SDL_Flip(screen);
        SDL_FreeSurface(bmp);
    }

}
void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_Flip(screen);

}

SDL_Color getPixel (int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<width) && (y>=0) && (y<height)) {
        //determine position
        char* pPosition=(char*)screen->pixels ;
        //offset by y
        pPosition+=(screen->pitch*y) ;
        //offset by x
        pPosition+=(screen->format->BytesPerPixel*x);
        //copy pixel data
        memcpy(&col, pPosition, screen->format->BytesPerPixel);
        //convert color
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}


int main ( int argc, char** argv )
{


    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(width, height, 32,SDL_HWSURFACE|SDL_DOUBLEBUF);

    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }

    SDL_WM_SetCaption( tytl , NULL );

    vector <int> word;
    vector <bool> outPut;
    vector <int> decoding;

    bool done = false;
    tloDane.w = 900;
    tloDane.h = 600;
    tloCel.x = 0;
    tloCel.y = 0;
    przyciskCel[0].x = 650;
    przyciskCel[0].y = 120;
    przyciskCel[1].x = 650;
    przyciskCel[1].y = przyciskCel[0].y + 144;
    przyciskCel[2].x = 650;
    przyciskCel[2].y = przyciskCel[1].y + 144;
    przyciskDane.w = 200;
    przyciskDane.h = 124;
    tlo = SDL_LoadBMP("tlo.bmp");
    przycisk[0] = SDL_LoadBMP("przycisk1.bmp");
    przycisk[1] = SDL_LoadBMP("przycisk2.bmp");
    przycisk[2] = SDL_LoadBMP("przycisk3.bmp");
    SDL_Event event;

     while (!done)
    {

        SDL_FillRect(screen, NULL, 0 );
        SDL_BlitSurface(tlo, &tloDane, screen, &tloCel);
        //SDL_BlitSurface()
        SDL_BlitSurface( przycisk[0], & przyciskDane, screen, & przyciskCel[0] );
        SDL_BlitSurface( przycisk[1], & przyciskDane, screen, & przyciskCel[1] );
        SDL_BlitSurface( przycisk[2], & przyciskDane, screen, & przyciskCel[2] );
        SDL_Flip(screen);

        while( SDL_PollEvent( &event ) )
        {

            if( event.type == SDL_MOUSEMOTION )
            {
                myszkaX = event.motion.x;
                myszkaY = event.motion.y;
            }
            if((myszkaX > przyciskCel[0].x && myszkaX < (przyciskCel[0].x + przyciskDane.w)) && (myszkaY > przyciskCel[0].y && myszkaY < (przyciskCel[0].y + przyciskDane.h)))
                przycisk[0] = SDL_LoadBMP( "przycisk1hover.bmp" );
            else
                przycisk[0] = SDL_LoadBMP( "przycisk1.bmp" );
            if((myszkaX > przyciskCel[1].x && myszkaX < (przyciskCel[1].x + przyciskDane.w)) && (myszkaY > przyciskCel[1].y && myszkaY < (przyciskCel[1].y + przyciskDane.h)))
                przycisk[1] = SDL_LoadBMP( "przycisk2hover.bmp" );
            else
                przycisk[1] = SDL_LoadBMP( "przycisk2.bmp" );
                   if((myszkaX > przyciskCel[2].x && myszkaX < (przyciskCel[2].x + przyciskDane.w)) && (myszkaY > przyciskCel[2].y && myszkaY < (przyciskCel[2].y + przyciskDane.h)))
                przycisk[2] = SDL_LoadBMP( "przycisk3hover.bmp" );
            else
                przycisk[2] = SDL_LoadBMP( "przycisk3.bmp" );
            if( event.type == SDL_MOUSEBUTTONDOWN )
            {
                if( event.button.button == SDL_BUTTON_LEFT  && myszkaX > przyciskCel[0].x && myszkaX < (przyciskCel[0].x + przyciskDane.w) && myszkaY > przyciskCel[0].y && myszkaY < (przyciskCel[0].y + przyciskDane.h))
                {
                    int opcja = 0;
                    cout<<"Podaj wartosc dla buffora slownikowego(0-9)"<<endl;
                    int wartosc = 0;
                    cin>>wartosc;
                    opcja += wartosc*10;
                    cout<<"Podaj wartosc dla buffora wejsciowego(0-9)"<<endl;
                    int wartosc2 = 0;
                    cin>>wartosc2;
                    opcja += wartosc2;
                    wybor_rozmiaru_bufora = opcja;

                    tlo = SDL_LoadBMP("tlo1.bmp");
                    SDL_BlitSurface(tlo, &tloDane, screen, &tloCel);
                    Funkcja1(word);
                    decoding = LZ77_dekompresuj(opcja,LZ77_kompresuj(opcja,word));
                    wypisz_plik(naglowek(),LZ77_kompresuj(opcja,word));
                    cout<<"Zapisano do pliku bssz!"<<endl;
                    showDecoded1(decoding);
                    SDL_Flip(screen);
                    //odczytaj_plik();
                    //cout<<height<<","<<width<<endl;
                    //LZ77_dekompresuj(opcja,odczytaj_plik());
                    done = true;
                }
                if( event.button.button == SDL_BUTTON_LEFT  && myszkaX > przyciskCel[1].x && myszkaX < (przyciskCel[1].x + przyciskDane.w) && myszkaY > przyciskCel[1].y && myszkaY < (przyciskCel[1].y + przyciskDane.h))
                {

                    int opcja = 0;
                    cout<<"Podaj wartosc dla buffora slownikowego(0-9)"<<endl;
                    int wartosc = 0;
                    cin>>wartosc;
                    opcja += wartosc*10;
                    cout<<"Podaj wartosc dla buffora wejsciowego(0-9)"<<endl;
                    int wartosc2 = 0;
                    cin>>wartosc2;
                    opcja += wartosc2;
                    wybor_rozmiaru_bufora = opcja;

                    tlo = SDL_LoadBMP("tlo1.bmp");
                    SDL_BlitSurface(tlo, &tloDane, screen, &tloCel);
                    Funkcja3(word);
                    decoding = LZ77_dekompresuj(opcja,LZ77_kompresuj(opcja,word));
                     wypisz_plik(naglowek(),LZ77_kompresuj(opcja,word));
                    cout<<"Zapisano do pliku bssz!"<<endl;
                    showDecoded2(decoding);
                    SDL_Flip(screen);
                    done = true;
                }
            if( event.button.button == SDL_BUTTON_LEFT  && myszkaX > przyciskCel[2].x && myszkaX < (przyciskCel[2].x + przyciskDane.w) && myszkaY > przyciskCel[2].y && myszkaY < (przyciskCel[2].y + przyciskDane.h))
                {
                    int opcja = 0;
                    cout<<"Podaj wartosc dla buffora slownikowego(0-9)"<<endl;
                    int wartosc = 0;
                    cin>>wartosc;
                    opcja += wartosc*10;
                    cout<<"Podaj wartosc dla buffora wejsciowego(0-9)"<<endl;
                    int wartosc2 = 0;
                    cin>>wartosc2;
                    opcja += wartosc2;
                    wybor_rozmiaru_bufora = opcja;
                   tlo = SDL_LoadBMP("tlo1.bmp");
                    SDL_BlitSurface(tlo, &tloDane, screen, &tloCel);
                    Funkcja4(word, PaletaD);
                    decoding = LZ77_dekompresuj(opcja,LZ77_kompresuj(opcja,word));
                     wypisz_plik(naglowek(),LZ77_kompresuj(opcja,word));
                    cout<<"Zapisano do pliku bssz!"<<endl;
                    showDecoded3(decoding);
                    SDL_Flip(screen);
                    done = true;
                }
            }
            if( event.key.keysym.sym == SDLK_ESCAPE ) done = true;

        }
    }

    SDL_FreeSurface(przycisk[0]);
    SDL_FreeSurface(przycisk[1]);
    SDL_FreeSurface(przycisk[2]);
    done=false;
    while (!done){
        while( SDL_PollEvent( &event ) ){
            if( event.type == SDL_MOUSEBUTTONDOWN )
                done=true;
        }
    }
    printf("Exited cleanly\n");
    return 0;
}
