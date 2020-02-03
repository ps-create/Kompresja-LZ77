# Projekt-Grafika-GKIM-2020-Zespol11
całość programu w folderze zip umieszczonym w tym repozytorium!

# Treść zadania:
Należy utworzyć specyfikację rastrowego pliku graficznego rejestrującego obraz kolorowy (z wykorzystaniem 16 narzuconych oraz 16 dedykowanych barw) i w 16 stopniowej skali szarości we wszystkich przypadkach opierającego się na kompresji LZ77 ​ (Lempel-Ziv 77) ​ . Alfabet wejściowy to wartość 4 bitowa. 
Należy napisać aplikacje, które zgodnie ze stworzoną specyfikacją dokonają filtracji danych wejściowych (przystosowanie danych do alfabetu wejściowego) i konwersji z pliku BMP do nowego rodzaju pliku graficznego oraz z nowego rodzaju pliku do formatu BMP. Użytkownik powinien mieć możliwość m.in. wyboru jednego z trzech trybów barwnych (paleta narzucona, paleta dedykowana lub skala szarości). 
 Zaimplementować dithering przy użyciu algorytmu Floyda-Steinberga. 
 
# Kompresja LZ77:
Praca algorytmu zaczyna się od określenia rozmiarów buforów: słownikowego i wejściowego. Rozmiary tych buforów muszą zostać zapisane, ponieważ ich rozmiar musi być identyczny przy dekompresji.
Następnie bufor słownikowy jest wypełniany pierwszym symbolem kodowanego tekstu, a bufor wejściowy kolejnymi symbolami tekstu. Pierwszy symbol jest bezpośrednio wypisywany na wyjście. Dopóki w buforze wejściowym są jakieś dane:
- algorytm wyszukuje w słowniku najdłuższy podciąg odpowiadający podciągowi bufora wejściowego (zaczynającemu się na jego początku)
- na wyjście zapisywana jest trójka PCS, gdzie P to pozycja początku ciągu w buforze słownikowym, C to liczba elementów pasującego ciągu a S to pierwszy symbol z bufora wejściowego następujący po ciągu (nie pasujący).
Jeżeli zadany symbol nie zostanie znaleziony w buforze, jest wypisywany jako trójka z C = 0.
- Okno (czyli bufor słownikowy i bufor wejściowy) jest przesuwany o C+1 symboli. To oznacza, że na C+1 symboli z bufora wejściowego jest wpisywane na ostanie pola bufora słownikowego, natomiast do bufora wejściowego dopisywane jest C+1 symboli z wejścia. Jest to zachowanie świetnie wykorzystujące bufory cykliczne.
Stopień kompresji jest zależny od wielkości buforów, głównie słownikowego, jednak zwiększenie jego rozmiaru znacznie wydłuża czas kompresji.
Żeby dekompresja się powiodła, rozmiary obu buforów muszą być takie same jak przy kompresji. Pierwszy symbol jest wczytywany z tekstu, wypełniamy nim bufor słownikowy i wypisujemy go na wyjście.
Dopóki w tekście są jakieś trójki, czytamy je, znajdujemy pierwszy wyraz danego ciągu w buforze słownikowym a następnie czytamy C kolejnych znaków. Przeczytany ciąg uzupełniamy o symbol S i zapisujemy do bufora wejściowego oraz na wyjście. Następnie przesuwamy okno o C+1, czyli dopóki bufor wejściowy nie będzie pusty. To powoduje automatyczne odtworzenie słownika.

# Projekt był projektem grupowym!
Created by: Paweł/Mikołaj/Marek/Kamil
