#include "graph.h"
#include "searcher.h"

using std::vector;

std::vector<Vertex> AStarSearcher::V;

///BaseGraph========================================================
///Copy
BaseGraph& BaseGraph::copy(const BaseGraph& other)
    {
        V=other.V;
        iranyitott=other.iranyitott;
        for (size_t i=0; i<GetN(); i++)
        {
            for (Edge e : other.getNeigboursOf(i))
                insert_edge(e);
        }
        return (*this);
    }

///Összefüggö-e?
bool BaseGraph::isConnected(size_t k) const
{
    BFSSearcher bfsSearcher;
    return Search(0,bfsSearcher).GetN()==GetN();
}

Tree BaseGraph::Search(size_t kezdo_pont, Searcher& searcher) const
{
    if (kezdo_pont>=GetN()) throw std::out_of_range("Search_segfault");

    int elertuk[GetN()];
    for(size_t i=0; i<GetN(); i++)
    {
        elertuk[i]=-1;
    }

    int elert=0;
    elertuk[kezdo_pont]=elert++;
    Tree returnTree(V[kezdo_pont]);
    for (Edge edge : getNeigboursOf(kezdo_pont))
    {
        searcher.put(edge);
    }

    while (!searcher.exit())
    {
        Edge E=searcher.get();
        if (elertuk[E.to]==-1) //most érjuk el eloszor
        {
            elertuk[E.to]=elert++;

            for (Edge edge : getNeigboursOf(E.to))
            {
                if (elertuk[edge.to]==-1)
                    searcher.put(edge);
            }

            Edge e=E;
            e.from=elertuk[E.from];
            e.to=elertuk[E.to];
            returnTree.insert_edge_vertex(e,V[E.to]);

        }
    }

    return returnTree;
}

///Van-e él a-ból b-be
bool BaseGraph::areAdjacents(size_t a, size_t b) const
{
    for (Edge e : getNeigboursOf(a))
    {
        if (e.to==b) return true;
    }
    return false;
}


///Csúcsok kiírása
void BaseGraph::writeVertexListTo(std::ostream& out) const
{
    out << "Csucs lista:" << std::endl;
    for (size_t i=0; i<GetN(); i++)
    {
        out<< i << " : " ;
        for (double data : V[i].custom_data)
                out << data << " ";
        out << std::endl;
    }
    out<<std::endl;
}


///AdjacencyListGraph=================================================

///Konstruktor
AdjacencyListGraph::AdjacencyListGraph(unsigned int N, bool irany) : BaseGraph(N,irany) //alaposztály ctor hívás
{
    edgeSz=vector<vector<Edge>>(N);
}

///Csúcs beillesztése
void AdjacencyListGraph::insert_vertex(Vertex v)
{
    BaseGraph::insert_vertex(v);
    edgeSz.push_back(vector<Edge>());
}

///Él beillesztése
void AdjacencyListGraph::insert_edge(Edge edge)
{
    unsigned int a=edge.from;
    unsigned int b=edge.to;

    if (a>=GetN() || b >=GetN() )
    {
        //throw std::out_of_range("AdjacencyListGraph_insert_edge");
        for (unsigned int i=GetN(); i<=a || i<=b; i++)
        {
            insert_vertex({{(int)i}});
        }
    }


    if (areAdjacents(a,b)) return; //it cant contain more than one edge
    if (a==b) return; //if simple, cant have loops
    edgeSz[a].push_back(edge);
    if (!iranyitott)
        edgeSz[b].push_back(edge.reversed());
}

///Él törlése
void AdjacencyListGraph::erase_edge(size_t a, size_t b)
{
    if (a>=GetN() || b >=GetN() ) throw std::out_of_range("AdjacencyListGraph_erase_edge");
        auto newend=std::remove_if(edgeSz[a].begin(),edgeSz[a].end(),[b](auto edge){return edge.to==b;});
        edgeSz[a].erase(newend,edgeSz[a].end());
    if (!iranyitott)
    {
        auto newend=std::remove_if(edgeSz[b].begin(),edgeSz[b].end(),[a](auto edge){return edge.to==a;});
        edgeSz[b].erase(newend,edgeSz[b].end());
    }
}


///Csúcsból kiinduló éleket adja vissza egy vector<Edge>-ben
const std::vector<Edge>& AdjacencyListGraph::getNeigboursOf(unsigned int P) const
{
    return edgeSz[P];
}

///Szomszédsági lista kiírása
void AdjacencyListGraph::writeEdgeSzListTo(std::ostream& out) const
{
    out << "Szomszedsagi lista:" << std::endl;
    for (size_t sor=0; sor<GetN(); sor++)
    {
        out << sor << ":";
        for (Edge edge : edgeSz[sor])
        {
            out << " " << edge.to << "(" ;
            for (double data : edge.custom_data)
                out << data << " ";
            out <<")  ";
        }
        out<<std::endl;
    }
    out<<std::endl;
}

///EdgeListGraph==========================================================
///Konstruktor
EdgeListGraph::EdgeListGraph(unsigned int N, bool irany) : BaseGraph(N,irany) //alaposztály ctor hívás
{}

///Csúcs beillesztése
void EdgeListGraph::insert_vertex(Vertex v)
{
    BaseGraph::insert_vertex(v);
}

///Él beillesztése
void EdgeListGraph::insert_edge(Edge edge)
{
    unsigned int a=edge.from;
    unsigned int b=edge.to;

    if (a>=GetN() || b >=GetN() )
    {
        //throw std::out_of_range("AdjacencyListGraph_insert_edge");
        for (unsigned int i=GetN(); i<=a || i<=b; i++)
        {
            insert_vertex({{(int)i}});
        }
    }

    if (areAdjacents(a,b)) return; //it cant contain more than one edge
    if (a==b) return; //if simple, cant have loops
    edgeList.emplace_back(edge);

}

///Él törlése
void EdgeListGraph::erase_edge(size_t a, size_t b)
{
    if (a>=GetN() || b >=GetN() ) throw std::out_of_range("AdjacencyListGraph_erase_edge");

    auto newend=std::remove_if(edgeList.begin(),edgeList.end(),[a, b](Edge edge){return edge.from==a && edge.to==b;});
    edgeList.erase(newend,edgeList.end());

    if(!iranyitott)
    {
        auto newend=std::remove_if(edgeList.begin(),edgeList.end(),[a, b](Edge edge){return edge.from==b && edge.to==a;});
        edgeList.erase(newend,edgeList.end());
    }

}

///Él listához hozzáférés
const std::vector<Edge>& EdgeListGraph::getEdgeList() const
{
    return edgeList;
}

///Csúcsból kiinduló éleket adja vissza egy vector<Edge>-ben
const std::vector<Edge>& EdgeListGraph::getNeigboursOf(unsigned int P) const
{
    getNeigboursOfResult.clear();
    if (!iranyitott)
    {
        std::copy_if(edgeList.begin(),edgeList.end(),std::back_inserter(getNeigboursOfResult),[P](Edge e){return e.to==P;});
        std::transform(getNeigboursOfResult.begin(),getNeigboursOfResult.end(),getNeigboursOfResult.begin(),[](Edge e){return e.reversed();});
        //std::for_each(getNeigboursOfResult.begin(),getNeigboursOfResult.end(),[](Edge& e){e.reverse();});
    }
    std::copy_if(edgeList.begin(),edgeList.end(),std::back_inserter(getNeigboursOfResult),[P](Edge e){return e.from==P;});

    return getNeigboursOfResult;
}

bool EdgeListGraph::areAdjacents(size_t a, size_t b) const
{
    if (iranyitott)
        return std::find_if(edgeList.begin(),edgeList.end(),[a, b](Edge e){return e.from==a && e.to==b;})!=edgeList.end();
    else
        return std::find_if(edgeList.begin(),edgeList.end(),[a, b](Edge e){return (e.from==a && e.to==b) || (e.from==b && e.to==a);})!=edgeList.end();
}

///Él lista kiírása
 void EdgeListGraph::writeEdgeListTo(std::ostream& out) const
{
    out << "El lista:" << std::endl;
    for (Edge edge : edgeList)
    {
        out<< edge.from << " " << edge.to << " (" ;
        for (double data : edge.custom_data)
            out << data << " ";
        out <<")"<<std::endl;
    }
}

///Tree==============================================
///Konstruktor (izolált ponttal)
Tree::Tree(Vertex v)
{
    selfGraph.insert_vertex(v);
    apa.push_back(Edge{0,0});//placeholder az elejére. 0. csúcsnak nem lehet "apja".
}

bool Tree::areAdjacents(size_t a, size_t b) const
{
    if (apa[a].to==b) return true;
    if (apa[b].to==a) return true;
    return false;
}

void Tree::insert_edge_vertex(Edge edge, Vertex v) {
    if (edge.from>=selfGraph.GetN() || edge.to!=selfGraph.GetN())
        throw std::out_of_range("Unable to attach the parameters to the tree!");

    selfGraph.insert_vertex(v);
    selfGraph.insert_edge(edge);
    apa.emplace_back(edge);
}

///Távolság a gyökértöl
size_t Tree::tav(size_t k) const
{
    int i=0;
    while (k!=0){k=apa[k].from;i++;}
    return i;
}


///Élsúlyok összege
double Tree::sumOfEdges() const
{
    double sum=0;
    for (unsigned int i=1; i<GetN(); i++)
    {
        sum+=apa[i].custom_data[0];
    }
    return sum;
}


//Ez ránézésre valami rég írt C kód, de most ezt nem akartam teljesen átírni,
//Csak annyit javítottam rajta, hogy jó legyen a mostani környezethez...
void Tree::kirajzol(const char* filename)
{
    int tavmax=0;
    int szintmax=0;
    int szintek[GetN()];
    int sorban[GetN()];
    for (unsigned int i=0; i<GetN(); i++)
    {

        szintek[i]=0;
        sorban[i]=0;
    }

    for (unsigned int i=0; i<GetN(); i++)
    {
        int p=tav(i);
        if (p>tavmax) tavmax=p;
        szintek[p]++;
        sorban[i]=szintek[p];
        if (szintek[p]>szintmax) szintmax=szintek[p];
    }

    char f[130];
    sprintf(f,"%s%s",filename,".svg");
    FILE* fp;
    fp = fopen(f, "w");
    fprintf(fp,"<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n",100*szintmax,100*tavmax+100);
    for (unsigned int i=1; i<GetN(); i++)
    {
        //koordináták: tav(i), sorban[i]
        fprintf(fp,"<line x1=\"%d\" x2=\"%d\" y1=\"%d\" y2=\"%d\" stroke=\"orange\" fill=\"transparent\" stroke-width=\"2\"/>\n",100*szintmax*sorban[apa[i].from]/(szintek[tav(apa[i].from)]+1),100*szintmax*sorban[apa[i].to]/(szintek[tav(apa[i].to)]+1),25+tav(apa[i].from)*100,25+tav(apa[i].to)*100);
        if (apa[i].custom_data.size()>0)
        fprintf(fp,"<text x=\"%d\" y=\"%d\" >%.0f</text>\n",(100*szintmax*sorban[apa[i].from]/(szintek[tav(apa[i].from)]+1)+100*szintmax*sorban[apa[i].to]/(szintek[tav(apa[i].to)]+1))/2,(25+tav(apa[i].from)*100+25+tav(apa[i].to)*100)/2,apa[i].custom_data[0]);
    }
    for (unsigned int i=0; i<GetN(); i++)
    {
        //koordináták: tav(i), sorban[i]
        fprintf(fp,"<circle cx=\"%d\" cy=\"%d\" r=\"22\" stroke=\"red\" fill=\"white\" stroke-width=\"2\"/>\n",100*szintmax*sorban[i]/(szintek[tav(i)]+1),25+tav(i)*100);
        if (selfGraph.V[i].custom_data.size()>0)
        fprintf(fp,"<text x=\"%d\" y=\"%d\" >%d (%d)</text>\n",100*szintmax*sorban[i]/(szintek[tav(i)]+1)-12,25+tav(i)*100+8,i,selfGraph.V[i].custom_data[0]);
    }

    fprintf(fp,"</svg>\n");
    fclose(fp);
}

