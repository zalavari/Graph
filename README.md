# Graph Framework

This program was a homework project for the course C++11 Programming in 2018.
The task was to write a demo program, which uses  >=C++11 elements, the more the better.

The goal of this specific project was to make a framework, where one can create and modify graphs easily, and run calculations on them.
Since graphs can be stored several ways, and this influences the effectivness of the algorithms, it is cruical that separate graph implementations may exist, but a common interface is also needed, so algorithms don't have to know the concrete implementation. To sum it up, I had to separate the graph storage (implementation) from the usage and algorithms (interface). To demonstrate this, I added two different graph implementation, with a common anchestor. One of the implementations has one adjacency list for every vertex, the other stores only the edges in a large list.

Another interesting aspect is that, how trees can be managed. They are a subset of graphs in mathematical sense, but we might want to behave them separately in our program. We might would want to make some inheritance pattern, but it would violate Liskov-Substituion Principle, so in the final run, it became a completely different class from the class for graphs. Still we want to make it e.g. easily convertable to any graph implementation, which produces new interesting questions.

A considerable extension to this project, that a general searcher algorithm is implemented on the graph. By implementing the abstract Searcher class, which requires only a couple of simple function to override, we can make various searches, traversal and much more. To demonstrate this, I added BFS, A-Star, Prim-algorithm, and an algorithm, which produces the legal ordering of the vertices.

Thanks to [IIT](https://www.iit.bme.hu/) for the memtrace and gtest_lite.

