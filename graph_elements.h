#ifndef GRAPH_ELEMENTS_H
#define GRAPH_ELEMENTS_H

#include<vector>

struct Edge{
    unsigned int to, from;

    //Algoritmusfüggő adatok tárolására
    //Jobb lenne templatesíteni, vagy double helyett any típus, de egyelőre érjük be ezzel.
    //Tipikus felhasználása: élsúlyozás, kapacitásfüggvény, multiplicitás...
    std::vector<double> custom_data;

    Edge(unsigned int from,unsigned int to, std::initializer_list<double> init_list={});

    Edge reversed() const;

    Edge& reverse();
};


struct Vertex{
    //Algoritmusfüggő adatok tárolására
    //Jobb lenne templatesíteni, vagy int helyett any típus, de egyelőre érjük be ezzel.
    //Tipikus felhasználás: 0. elemként egy azonosítót tárolunk
    std::vector<int> custom_data;

    Vertex(std::initializer_list<int> init_list={}) : custom_data(init_list) {};
};

#endif // GRAPH_ELEMENTS_H
