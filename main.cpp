#include "header.h"

int main()
{
    size_t n, c;
    std::srand(std::time(nullptr));
    input(&n, &c);
    point* points = new point[n];
    edge* edges = new edge[n - 1];
    randomize(points, n);
    wgraph x(n);
    x.set(points);
    x.tree(edges);
    std::sort(edges, edges + (n - 1), [](const edge & a, const edge & b) { return a.weight < b.weight; });
    mgraph y(edges, n, c);
    size_t* components = y.selection();
    if (n <= 10)
        std::cout << y;
    count(components, n);
    y.centroid(points);
    clean(points, edges);
    return 0;
}
