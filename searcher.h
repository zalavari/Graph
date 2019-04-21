#ifndef SEARCHER_H
#define SEARCHER_H

#include<queue>
#include<map>
#include<set>
#include<algorithm>
#include<iostream>
#include "graph_elements.h"


//Egy gráf bejárást/keresést befolyásoló osztály
//A bejárás/keresés úgy történik, hogy mindig egy aktuálisan választott csomópontot fejtünk ki,
//Az éleket hozzávesszük egy kollekcióhoz, majd választunk egyet belőlük
//Ha a kilépési feltétel teljesül, vége a bejárásnak.
class Searcher
{
public:
    virtual void put(Edge edge)=0;
    virtual Edge get()=0;
    virtual bool exit() const=0;
};

//Három példa a Searcher osztályra
class PrimSearcher : public Searcher
{
public:

    static constexpr auto f=[](const Edge& lhs, const Edge&rhs)->bool { return (lhs.custom_data[0]>rhs.custom_data[0]); };
    std::priority_queue<Edge,std::vector<Edge>, decltype(PrimSearcher::f)> prsor {PrimSearcher::f};

    void put(Edge edge) override
    {
        prsor.emplace(edge);
    }

    Edge get() override
    {
        Edge e=prsor.top();
        prsor.pop();
        return e;
    }

    bool exit() const override
    {
       return prsor.empty();
    }
};
constexpr decltype(PrimSearcher::f) PrimSearcher::f;


class BFSSearcher : public Searcher
{
    std::queue<Edge> S;
    void put(Edge edge) override
    {
        S.emplace(edge);
    }

    Edge get() override
    {
        Edge e=S.front();
        S.pop();
        return e;
    }

    bool exit() const override
    {
       return S.empty();
    }
};


class AStarSearcher : public Searcher
{
    //A hasonlító függvénynek el kell érni a csúcspontokról szóló információt is. Erre több workaround is lehet, egyik sem a legszebb.
    //Itt azt csináljuk, hogy statikusként beállítunk egy Vertex vektort statikusan,
    //Majd az eredményt is innen nyerjük ki
    //Jobb lenne, ha a gráfot direkt tudnánk módosítani, de az jóval munkásabb lenne, már csak a const megkötések miatt is.
    //Vertex.custom_data[0] == azonosito
    //Vertex.custom_data[1] == megtett ut
    //Vertex.custom_data[2] ==  heurisztika
public:
    static std::vector<Vertex> V;

    static constexpr auto f=[](const Edge& lhs, const Edge&rhs)->bool
    {return (lhs.custom_data[0]+V[lhs.from].custom_data[1]+V[lhs.to].custom_data[2])>(rhs.custom_data[0]+V[rhs.from].custom_data[1]+V[rhs.to].custom_data[2]); };
    std::priority_queue<Edge,std::vector<Edge>, decltype(AStarSearcher::f)> prsor {AStarSearcher::f};
    bool finished=false;

    void put(Edge edge) override
    {
        prsor.emplace(edge);
    }

    Edge get() override
    {
        Edge e=prsor.top();
        prsor.pop();
        if (V[e.to].custom_data[2]==0) finished=true;
        V[e.to].custom_data[1]=V[e.from].custom_data[1]+e.custom_data[0];
        return e;
    }

    bool exit() const override
    {
       return prsor.empty() || finished;
    }

};
constexpr decltype(AStarSearcher::f) AStarSearcher::f;


class MaxVisszaSearcher : public Searcher
{

public:
    std::vector<int> visszafok;
    std::vector<bool> elert;
    bool vege=false;
    std::vector<int> legkorabbiOs;

    MaxVisszaSearcher(int n)
    {
        legkorabbiOs=std::vector<int>(n);
        visszafok=std::vector<int>(n);
        elert=std::vector<bool>(n);
        for (int i=0; i<n; i++)
        {
            legkorabbiOs[i]=0;
            visszafok[i]=0;
            elert[i]=false;
        }
    }


    void put(Edge edge) override
    {
            elert[edge.from]=true;


        if (!elert[edge.to])
            if ((visszafok[edge.to]++)==0) //bit tricky
                legkorabbiOs[edge.to]=edge.from;
    }

    Edge get() override
    {

        int maxi=0;
        int db=0;
        for (auto i = 0; i <visszafok.size(); i++)
            if (!elert[i])
            {
                if (visszafok[maxi] < visszafok[i])
                   maxi=i;
            }
            else
            {
                db++;
            }

        if (db==visszafok.size()) vege=true;

        elert[maxi]=true;
        return Edge(legkorabbiOs[maxi],maxi);
    }

    bool exit() const override
    {
       return vege;
    }

};

#endif // SEARCHER_H
