#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <thread>

using namespace std;

bool load_graph(vector<vector<unsigned int>>& graph, string plik)
{
    ifstream file(plik);

    if (!file.is_open())
    {
        cout << "Nie udalo sie otworzyc pliku " << endl;
        return 0;
    }

    int vertex_counter, edges;
    file >> vertex_counter >> edges; // wczytaj liczbę wierzchołków i krawędzi
    graph.resize(vertex_counter);  // przypisanie vectorowi graph ilosc wierzcholkow
    for (int i = 0; i < edges; i++)
    {
        unsigned int vertex, adjacent;
        file >> vertex >> adjacent; // wczytaj kolejną krawędź
        graph[vertex].push_back(adjacent);
        graph[adjacent].push_back(vertex); // dodaj krawędź do grafu
    }

    file.close();
    return 1;
}


vector<int> bfs(vector<vector<unsigned int>>& graph, unsigned int start, int which_graph)
{
    size_t size = graph.size(); // sprawdzamy liczbe wierzcholkow zmienna typu size_t
    vector<bool> visited(size, false); // deklarujemy vertex_counter wierzcholkow i przypisujemy im status "nieodwiedzonych"
    queue<int> q;
    vector <int> output;

    visited[start] = true; // punkt poczatkowy skanowania grafu 
    q.push(start);  // dodajemy do kolejki element poczatkowy

    while (!q.empty()) // dopoki mamy wierzcholki w kolejce
    {
        int current_vertex = q.front(); // pobieramy pierwszy wierzcholek z kolejki
        q.pop(); // usuwamy go z kolejki

      
        output.push_back(current_vertex);
        

        for (size_t i = 0; i < graph[current_vertex].size(); ++i) // petla ktora wykona sie tyle razy ile sasaiadow ma dany wierzcholek
        {
            int current_adjacent = graph[current_vertex][i]; // zmienna ktorej przypisujemy konkretnego sasiada sprawdzanego wierzcholka
            if (!visited[current_adjacent]) // jezeli nie odwiedzilismy tego wierzcholka
            {
                visited[current_adjacent] = true; // przypisz status odwiedzonego

                q.push(current_adjacent); // dodaj do kolejki
            }
        }
    }
    return output;
}

int main()
{
    vector<vector<unsigned int>> graph1;
    vector<vector<unsigned int>> graph2;
    vector<int> output1, output2; 

    if (!load_graph(graph1, "input1.txt") or !load_graph(graph2, "input2.txt"))
        return 0;

    unsigned int start;
    cout << "Upewnij sie, ze podales dane we wlasciwym formacie [patrz komentarz na dole kodu] \n" << endl;
    cout << "Podaj poczatkowy wierzcholek, od ktorego zacznie sie wykonywac algorytm BFS: ";
    do 
    {
        cin >> start;
    } while (start >= graph1.size() or start >= graph2.size() or start < 0);


    thread thread1([&graph1, start, &output1]() { output1 = bfs(graph1, start, 1); });
    thread thread2([&graph2, start, &output2]() { output2 = bfs(graph2, start, 2); });

    thread1.join();
    thread2.join();

    for (size_t i = 0; i < output1.size(); i++)
        cout << output1[i] << " ";
    cout << endl;
    for (size_t i = 0; i < output1.size(); i++)
        cout << output2[i] << " ";

    return 0;

}
/*
Dane wejsciowe podajemy wedle nastepujacego schematu:
    liczba wierzcholkow     liczba krawedzi
    wierzcholek1    wierzcholek2
    wierzcholek1    wierzcholek2
    wierzcholek1    wierzcholek2 <--- polaczone ze soba wierzcholki [graf nieskierowany, czyli podajac 1 0,
              itd...                          automatycznie dodajemy do grafu polaczenie 0 1]
    
    
*/