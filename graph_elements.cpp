#include "graph_elements.h"

Edge::Edge(unsigned int from,unsigned int to, std::initializer_list<double> init_list)
:to(to), from(from) {
    custom_data= std::vector<double>(init_list);
}

Edge Edge::reversed() const
{
    Edge e=(*this);
    std::swap(e.from,e.to);
    return e;
}

Edge& Edge::reverse()
{
    std::swap(from,to);
    return (*this);
}
