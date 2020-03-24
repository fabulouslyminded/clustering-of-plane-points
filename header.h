#ifndef HEADER_H
#define HEADER_H
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include <iomanip>
#define INF 1e30
struct point
{
    double x, y;
};

struct edge
{
    size_t a, b;
    double weight;
};

class wgraph
{
private:
    double** matrix;
    size_t nodes;
public:
    wgraph(const size_t n);
    ~wgraph();
    void set(const point points[]);
    void tree(edge edges[]) const;
};

class mgraph
{
private:
    size_t nodes;
    size_t clusters;
    size_t quantity;
    unsigned* components;
    bool** matrix;
    bool** create(const edge edges[]) const;
    void search(const size_t current);
public:
    mgraph(const edge edges[], const size_t n, const size_t c);
    ~mgraph();
    size_t* selection(void);
    void centroid(const point points[]) const;
    friend std::ostream & operator<<(std::ostream & os, const mgraph & object);
};

void input(size_t* n, size_t* c);
void randomize(point points[], const size_t n);
void count(const size_t components[], const size_t n);
void clean(point points[], edge edges[]);
#endif // HEADER_H
