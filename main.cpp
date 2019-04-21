#include <iostream>
#include "gtest_lite.h" //Szebi féle teszt-keretrendszer
//#include "memtrace.h"
#include "searcher.h"
#include "graph.h"


using namespace std;

int main()
{
   // int* a= new int[3];

//Ezeket elég egyszer létrehozni.
//Helyes használat esetén alapállapotba visszaállnak a belső változóik.
PrimSearcher primSearcher;
BFSSearcher bfsSearcher;


TEST(SzomszedsagiLista_Iranyitatlan, grafepites) {

    AdjacencyListGraph G(false);

        EXPECT_EQ(0,G.GetN());

    G.insert_vertex({{1}});
    G.insert_vertex({{2}});

        EXPECT_EQ(2,G.GetN());

    G.insert_edge({5,6});

        EXPECT_EQ(7,G.GetN());
} END

TEST(SzomszedsagiLista_Iranyitatlan, grafletrehozas) {

    AdjacencyListGraph G(5,false);

        EXPECT_EQ(5,G.GetN());

    AdjacencyListGraph G2=G;

        EXPECT_EQ(5,G2.GetN());

    G.insert_edge({1,2});
    G.insert_edge({3,2});
    G.insert_edge({3,5});

        EXPECT_EQ(6,G.GetN());

    G2=G;

        EXPECT_EQ(6,G2.GetN());

    G2.insert_vertex({{(int)G2.GetN()}});

} END


TEST(SzomszedsagiLista_Iranyitatlan, grafepitesbontas) {

    AdjacencyListGraph G(5,false); //irányítatlan 5 csúcsú
    Tree F=G.Search(0,bfsSearcher); //izolált pontból álló fa

        EXPECT_EQ(false,G.isConnected());

    G.insert_edge({1,2});
    G.insert_edge({1,0});
    G.insert_edge({1,2});

        EXPECT_EQ(1,G.getNeigboursOf(2).size());
        EXPECT_EQ(2,G.getNeigboursOf(1)[0].to);
        EXPECT_EQ(0,G.getNeigboursOf(1)[1].to);

    G.insert_edge({0,2});
    G.erase_edge(1,2);
    G.erase_edge(1,2);
    G.insert_edge({3,4});
        EXPECT_EQ(false,G.isConnected());
    G.insert_edge({4,2});
        EXPECT_EQ(true,G.isConnected());
    G.erase_edge(0,2);
        EXPECT_EQ(false,G.isConnected());

    AdjacencyListGraph G2{F};
    G=F;
} END

TEST(Ellista_Iranyitatlan, grafepites) {

    EdgeListGraph G(false);

        EXPECT_EQ(0,G.GetN());

    G.insert_vertex({{1}});
    G.insert_vertex({{2}});

        EXPECT_EQ(2,G.GetN());

    G.insert_edge({5,6});

        EXPECT_EQ(7,G.GetN());

} END

TEST(Ellista_iranyitatlan, grafletrehozas) {

    EdgeListGraph G(5,false);

        EXPECT_EQ(5,G.GetN());

    EdgeListGraph G2=G;

        EXPECT_EQ(5,G2.GetN());

    G.insert_edge({1,2});
    G.insert_edge({3,2});
    G.insert_edge({3,5});

        EXPECT_EQ(6,G.GetN());

    G2=G;

        EXPECT_EQ(6,G2.GetN());

    G2.insert_vertex({{(int)G2.GetN()}});

    G2.writeEdgeListTo(cout);

} END



TEST(Ellista_iranyitatlan, grafepitesbontas) {

    EdgeListGraph G(5,false); //irányítatlan 5 csúcsú
    Tree F=G.Search(0,bfsSearcher); //izolált pontból álló fa

        EXPECT_EQ(false,G.isConnected());

    G.insert_edge({1,2});
    G.insert_edge({1,0});
    G.insert_edge({1,2});

        EXPECT_EQ(1,G.getNeigboursOf(2).size());
        EXPECT_EQ(2,G.getNeigboursOf(1)[0].to);
        EXPECT_EQ(0,G.getNeigboursOf(1)[1].to);


    G.insert_edge({0,2});
    G.erase_edge(1,2);
    G.erase_edge(1,2);
    G.insert_edge({3,4});
        EXPECT_EQ(false,G.isConnected());
    G.insert_edge({4,2});
        EXPECT_EQ(true,G.isConnected());
    G.erase_edge(0,2);
        EXPECT_EQ(false,G.isConnected());

    EdgeListGraph G2{F};
    G=F;

} END

TEST (Fa, Fa_epites) {

    Tree F; //izolált pontból álló fa

        EXPECT_EQ(1,F.GetN());

    F.insert_edge_vertex({0,1},{{1}});
    F.insert_edge_vertex({1,2},{{2}});
    F.insert_edge_vertex({0,3},{{3}});

        EXPECT_THROW(F.insert_edge_vertex({2,1},{}),out_of_range);
        EXPECT_THROW(F.insert_edge_vertex({2,6},{}),out_of_range);
        EXPECT_THROW(F.insert_edge_vertex({4,2},{}),out_of_range);

        EXPECT_EQ(4,F.GetN());

}END


TEST (Fa, Grafbol_fa) {

    AdjacencyListGraph G(5,false); //irányítatlan 5 csúcsú

    G.insert_edge({0,1});
    Tree F=G.Search(0,bfsSearcher); //két pontból álló fa

        EXPECT_EQ(2,F.GetN());
        EXPECT_EQ(0,F.Apa(1).from);

    G.insert_edge({1,2});
    G.insert_edge({1,0});
    G.insert_edge({1,2});
    F=G.Search(3,bfsSearcher);

        EXPECT_EQ(0,F.Apa(0).from);
        EXPECT_EQ(1,F.GetN());


}END

TEST (Fa, Fabol_graf) {

    AdjacencyListGraph G(5,false); //irányítatlan 5 csúcsú

    G.insert_edge({0,1});
    Tree F=G.Search(0,bfsSearcher); //két pontból álló fa


        EXPECT_EQ(2,F.GetN());
        EXPECT_EQ(0,F.Apa(1).from);

    G.insert_edge({1,2});
    G.insert_edge({1,0});
    G.insert_edge({1,2});
    F=G.Search(3,bfsSearcher);

        EXPECT_EQ(0,F.Apa(0).from);
        EXPECT_EQ(1,F.GetN());

}END


TEST(Iranyitott,fabol_graf){

    AdjacencyListGraph G{6,true};
    G.insert_edge({3,4});
    G.insert_edge({3,2});
    G.insert_edge({2,1});
    G.insert_edge({2,0});
    G.insert_edge({0,1});
    G.insert_edge({5,4}); //Ez az él és az ötös csúcs kiesik, ha a négyes felől érkezünk a kereséssel

    Tree F=G.Search(3,bfsSearcher);

    AdjacencyListGraph G1{F};
    G1=F;

    AdjacencyListGraph G2=G;

        EXPECT_EQ(6,G.GetN());
    G=G2=F;

        EXPECT_EQ(5,G.GetN());
        EXPECT_EQ(5,F.GetN());

}END

TEST(Komplex_pelda, Iranyitatlan_Prim) {
    AdjacencyListGraph G(8);
    G.insert_edge({0,1,{10}});
    G.insert_edge({0,2,{4}});
    G.insert_edge({0,4,{6}});
    G.insert_edge({0,5,{8}});
    G.insert_edge({1,2,{5}});
    G.insert_edge({1,3,{6}});
    G.insert_edge({2,3,{4}});
    G.insert_edge({2,4,{9}});
    G.insert_edge({2,7,{3}});
    G.insert_edge({3,4,{6}});
    G.insert_edge({3,7,{4}});
    G.insert_edge({4,5,{12}});
    G.insert_edge({4,6,{1}});
    G.insert_edge({4,7,{13}});
    G.insert_edge({6,7,{2}});

    Tree F=G.Search(0,primSearcher);

       EXPECT_EQ(27,F.sumOfEdges());
       EXPECT_EQ(0,F.tav(0));
       EXPECT_EQ(1,F.tav(1));
       EXPECT_EQ(2,F.tav(2));
       EXPECT_EQ(2,F.tav(6));
       EXPECT_EQ(4,F.tav(4));

    F.kirajzol("fa1");
}END

TEST(Komplex_pelda, Iranyitott_Prim) {
    AdjacencyListGraph G=AdjacencyListGraph(8,true);
    G.insert_edge({0,1,{10}});
    G.insert_edge({0,2,{4}});
    G.insert_edge({0,4,{6}});
    G.insert_edge({0,5,{8}});
    G.insert_edge({1,2,{5}});
    G.insert_edge({1,3,{6}});
    G.insert_edge({2,3,{4}});
    G.insert_edge({2,4,{9}});
    G.insert_edge({2,7,{3}});
    G.insert_edge({3,4,{6}});
    G.insert_edge({3,7,{4}});
    G.insert_edge({4,5,{12}});
    G.insert_edge({4,6,{1}});
    G.insert_edge({4,7,{13}});
    G.insert_edge({6,7,{2}});

    Tree F=G.Search(0,primSearcher);

    F.kirajzol("fa2");
        EXPECT_EQ(36,F.sumOfEdges());

}END

TEST(Komplex_pelda, AStar) {
    AdjacencyListGraph G=AdjacencyListGraph(8,true);

    //kezdeti távolságok beállítása, ezt majd az algoritmus piszkálja
    for (unsigned int i=0; i<8; i++)
        G.V[i].custom_data.push_back(0);

    //Heurisztikák hozzáadása
    G.V[0].custom_data.push_back(7);
    G.V[1].custom_data.push_back(6);
    G.V[2].custom_data.push_back(3);
    G.V[3].custom_data.push_back(3);
    G.V[4].custom_data.push_back(1);
    G.V[5].custom_data.push_back(10);
    G.V[6].custom_data.push_back(0); //Ez lesz a célállapot
    G.V[7].custom_data.push_back(2);


    //Súlyozott élek hozzáadáása
    G.insert_edge({0,1,{10}});
    G.insert_edge({0,2,{4}});
    G.insert_edge({0,4,{6}});
    G.insert_edge({0,5,{8}});
    G.insert_edge({1,2,{5}});
    G.insert_edge({1,3,{6}});
    G.insert_edge({2,3,{4}});
    G.insert_edge({2,4,{9}});
    G.insert_edge({2,7,{3}});
    G.insert_edge({3,4,{6}});
    G.insert_edge({3,7,{4}});
    G.insert_edge({4,5,{12}});
    G.insert_edge({4,6,{1}});
    G.insert_edge({4,7,{13}});
    G.insert_edge({6,7,{2}});

    //Ebből mindig új példány kell, és a statikus adattagjait is manuálisan kell menedzselni
    AStarSearcher myAStarSearcher;
    myAStarSearcher.V=G.V;

    Tree F=G.Search(0,myAStarSearcher); //Visszaadja az algoritmus által bejárt keresési fát
    G.V=myAStarSearcher.V; //Ha mindent jól csináltunk ez frissíti a kifejtett csomópontokon a távolságot

    G.writeVertexListTo(cout);
    G.writeEdgeSzListTo(cout);

    F.kirajzol("fa3");

}END


TEST(Komplex_pelda, LegalOrdering) {
    AdjacencyListGraph G=AdjacencyListGraph(8,false);


    //Súlyozott élek hozzáadáása
    G.insert_edge({0,1,{10}});
    G.insert_edge({0,2,{4}});
    G.insert_edge({0,4,{6}});
    G.insert_edge({0,5,{8}});
    G.insert_edge({1,2,{5}});
    G.insert_edge({1,3,{6}});
    G.insert_edge({2,3,{4}});
    G.insert_edge({2,4,{9}});
    G.insert_edge({2,7,{3}});
    G.insert_edge({3,4,{6}});
    G.insert_edge({3,7,{4}});
    G.insert_edge({4,5,{12}});
    G.insert_edge({4,6,{1}});
    G.insert_edge({4,7,{13}});
    G.insert_edge({6,7,{2}});

    //Ebből mindig új példány kell, és a statikus adattagjait is manuálisan kell menedzselni
    MaxVisszaSearcher maxVisszaSearcher(G.GetN());


    Tree F=G.Search(0,maxVisszaSearcher); //Visszaadja


    F.kirajzol("fa4");

}END

   return 0;
}
