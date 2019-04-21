#ifndef AdjacencyListGraph_H
#define AdjacencyListGraph_H

#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <stdexcept>


#include "graph_elements.h"
#include "searcher.h"

class Tree;

class BaseGraph
{
private:
protected:
    bool iranyitott;

    ///Arra kell, hogy tetszőleges implementációjú gráf konvertálható legyen másikra
    ///Saját típus közötti másolásra pedig a beépített és alapértelmezett megfelelő
    BaseGraph& copy(const BaseGraph& other);
public:
    std::vector<Vertex> V;

    ///Megadja a csucsok számát
    size_t GetN() const { return V.size(); }

    ///Destruktor
    virtual ~BaseGraph()=default;

    ///Másoló konstruktor, értékadás és társaira mind jó a default,
    ///ezért ezeket végül mind kivettem. (A leszármazottakból is)

    ///Konstruktor
    ///@param N - csucspontok száma kezdetben
    ///@param iranyitott - iranyitott-e a gráf
    //N nagyobb nulla értékre mindegyik csúcspontnak ad egy azonosítót,
    //ezzel gyorsítva a munkát, mivel ez általában kell, de elegendő is
    BaseGraph(unsigned int N=0,bool iranyitott=false) :iranyitott(iranyitott)
    {
        for (unsigned int i=0; i<N; i++)
            V.push_back({{(int)i}});
    }

    ///Manipulációs függvények
    ///Csúcs beillesztése
    virtual void insert_vertex(Vertex v) {
        V.emplace_back(v);
    }

    ///Él beillesztése
    virtual void insert_edge(Edge edge)=0;

    ///Él törlése
    virtual void erase_edge(size_t a, size_t b)=0;

    ///Lekérdező függvények
    ///Csúcsból kiinduló éleket adja vissza egy vector<Edge>-ben
    virtual const std::vector<Edge>& getNeigboursOf(unsigned int P) const=0;

    ///Van-e él a kettő között
    virtual bool areAdjacents(size_t a, size_t b) const ;

    ///Összefüggõ-e?
    bool isConnected(size_t k=0) const;

    ///Kereso algoritmus
    Tree Search(size_t kezdo_pont, Searcher& searcher) const;

    ///Csúcsok kiírása
    void writeVertexListTo(std::ostream& out) const;
};

class AdjacencyListGraph : public BaseGraph
{
    private:

        std::vector<std::vector<Edge>> edgeSz;

    public:
        ///Konstruktor
        AdjacencyListGraph(unsigned int N=0, bool irany=false);

        ///Konstruktor más gráf implementációból
        AdjacencyListGraph(const BaseGraph& other) : AdjacencyListGraph()
        {
            copy(other);
        }

        ///Értékadás más gráf implementációval
        AdjacencyListGraph& operator=(const BaseGraph& other)
        {
            copy(other);
            return (*this);
        };

        ///Manipulációs függvények...
        ///Csúcs beillesztése
        void insert_vertex(Vertex v) override;

        ///Él beillesztése
        void insert_edge(Edge edge) override;

        ///Él törlése
        void erase_edge(size_t a, size_t b) override;


        ///Lekérdező függvények...
        ///Csúcsból kiinduló éleket adja vissza egy vector<Edge>-ben
        const std::vector<Edge>& getNeigboursOf(unsigned int P) const override;

        ///Van-e él a-ból b-be
        //bool areAdjacents(size_t a, size_t b) const override;
        //Jó a megörökölt

        ///Debug célokra...
        ///Szomszédsági lista kiírása
        void writeEdgeSzListTo(std::ostream& out) const;

};

class EdgeListGraph : public BaseGraph
{
    std::vector<Edge> edgeList;
    mutable std::vector<Edge> getNeigboursOfResult;
public:
        ///Konstruktor
        ///@param irany - iranyitott legyen-e
        EdgeListGraph(unsigned int N=0, bool irany=false);

        ///Konstruktor más gráf implementációból
        EdgeListGraph(const BaseGraph& other) : EdgeListGraph()
        {
            copy(other);
        }

        ///Értékadás más gráf implementációval
        EdgeListGraph& operator=(const BaseGraph& other)
        {
            copy(other);
            return (*this);
        };

        ///Manipulációs függvények...
        ///Csúcs beillesztése
        void insert_vertex(Vertex v) override;

        ///Él beillesztése
        void insert_edge(Edge edge) override;

        ///Él törlése
        void erase_edge(size_t a, size_t b) override;


        ///Lekérdező függvények...
        ///Csúcsból kiinduló éleket adja vissza egy vector<Edge>-ben
        const std::vector<Edge>& getNeigboursOf(unsigned int P) const override;

        ///Van-e él a-ból b-be
        bool areAdjacents(size_t a, size_t b) const override;

        ///Főleg debug célokra...
        ///Él lista kiírása
        void writeEdgeListTo(std::ostream& out) const;

        ///Él listához hozzáférés
        const std::vector<Edge>& getEdgeList() const;

};

class Tree
{
    private:
        std::vector<Edge> apa;
        AdjacencyListGraph selfGraph{0,true};
    public:

        size_t GetN() const {return selfGraph.GetN();}
        ///Konstruktor
        Tree(Vertex v={{0}});

        ///Destruktor
        ~Tree() = default;

        /// Copy konstruktor
        Tree(const Tree& tc) =default;

        ///Értékadás
        Tree& operator=(const Tree& tc) =default;

        ///Gráffá konvertálás
        operator const AdjacencyListGraph&() const { return selfGraph; }


        ///Apja?
        Edge Apa(unsigned int csucs) const
        {
            if (csucs>=GetN()) throw std::out_of_range("Non-existent vertex cant have \"apa\"");
            return apa[csucs];
        }

        ///Él beillesztése
        void insert_edge_vertex(Edge edge, Vertex v);

        ///Csúcsból kiinduló éleket adja vissza egy vector<Edge>-ben
        const std::vector<Edge>& getNeigboursOf(int P) const
        {
            return selfGraph.getNeigboursOf(P);
        }

        ///Van-e él a-ból b-be
        bool areAdjacents(size_t a, size_t b) const;

        ///Távolság a gyökértől
        ///@param k - kérdéses pont
        ///@return - az ut hossza
        size_t tav(size_t k) const;

        ///Élsúlyok összege
        ///Összeadja az élek custom_data nulladik elemeit, és azt visszadja
        ///@return - a fa értéke
        double sumOfEdges() const;

        ///A fát egy svg fájlba rajzolja
        ///@param filename - a fájl neve
        void kirajzol(const char* filename);

};

#endif // AdjacencyListGraph_H
