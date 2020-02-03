#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <fstream>

using namespace std;

int binary_to_int(deque<bool> liczba);
deque<bool> int_to_binary(int liczba);

class Kompresja {
private:
	int p_length; //ilosc bitow potrzebna na zapisanie pozycji
	int c_length; //ilosc bitow potrzebna na zapisanie ilosci znakow
	int s_length; //ilosc bitow potrzebna na zapisanie symbolu
	unsigned int rozmiar_bufora_slownikowego; //
	unsigned int rozmiar_bufora_wejsciowego; //128 bajtow, staly
	deque<int> text_raw; //kopia wprowadzonych danych, latwiej uzywac, dopasowanie kontenera
	deque<int> bufor_slownikowy;
	deque<int> bufor_wejsciowy;
	struct PCS {
		unsigned int p;
		unsigned int c;
		unsigned int s;
	};
	deque<PCS> tablica_trojek;
	PCS szukaj(); // zwraca trojke
	void przesun_bufory_o(int c);

public:
	vector<bool>output;

	Kompresja(vector<int> dane, int p_length, int c_length, int s_length);//konwertuj tekst, okresl rozmiar i wypelnij bufory
	bool check_if_input_empty(); //konczy kompresje
	void read_iteration(); // czytaj text, szukaj w buforze, zapisz trójkê do tablicy, przesuñ bufory
	void utworz_output();
};

vector<bool> LZ77_kompresuj(int opcja, vector<int> dane);


//////////////////////////////////////////////////////////////////////////////////


class Dekompresja {
private:
	int p_length; //ilosc bitow potrzebna na zapisanie pozycji
	int c_length; //ilosc bitow potrzebna na zapisanie ilosci znakow
	int s_length; //ilosc bitow potrzebna na zapisanie symbolu
	unsigned int rozmiar_bufora_slownikowego; //
	unsigned int rozmiar_bufora_wejsciowego; //128 bajtow, staly
	deque<int> input_raw; //kopia wprowadzonych danych, latwiej uzywac, dopasowanie kontenera
	deque<int> bufor_slownikowy;
	deque<int> bufor_wejsciowy;
	struct PCS {
		unsigned int p;
		unsigned int c;
		unsigned int s;
	};
	deque<PCS> tablica_trojek;

	PCS czytaj_trojke();


public:
	Dekompresja(vector<bool> dane, int p_length, int c_length, int s_length);
	void przesun_bufory_wypisz();
	vector<int> output;
	bool check_if_input_empty();
	void read_iteration();
};



vector<int> LZ77_dekompresuj(int opcja, vector<bool> dane);