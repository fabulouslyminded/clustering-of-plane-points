#include "header.h"

wgraph::wgraph(const size_t n)
{
    nodes = n;
    matrix = new double*[nodes];
    for (size_t i = 0; i < nodes; ++i)
        matrix[i] = new double[nodes];
}

void wgraph::set(const point points[])
{
    double weight;
    for (size_t i = 0; i < nodes; ++i)
        for (size_t j = 0; j < nodes; ++j)
        {
            weight = sqrt(pow((points[i].x - points[j].x), 2) + pow((points[i].y - points[j].y), 2));
            matrix[i][j] = weight;
        }
}

void wgraph::tree(edge edges[]) const
{
    double min;
    int current;
    int* array = new int[nodes];
    array[0] = -1;
    for (size_t i = 1; i < nodes; ++i)
        array[i] = 0;
    for (size_t i = 1; i < nodes; ++i)
    {
        min = INF;
        current = 0;
        for (size_t j = 1; j < nodes; ++j)
            if (array[j] != -1 && min > matrix[j][array[j]])
            {
                current = j;
                min = matrix[j][array[j]];
            }
        if (!current)
            return;
        edges[i - 1].a = current;
        edges[i - 1].b = array[current];
        edges[i - 1].weight = min;
        array[current] = -1;
        for (size_t j = 1; j < nodes; ++j)
            if (array[j] != -1 && matrix[j][array[j]] > matrix[j][current])
                array[j] = current;
    }
    delete [] array;
}

wgraph::~wgraph()
{
    for (size_t i = 0; i < nodes; ++i)
        delete [] matrix[i];
    delete [] matrix;
}


mgraph::mgraph(const edge edges[], const size_t n, const size_t c)
{
    nodes = n;
    clusters = c;
    quantity = 0;
    components = nullptr;
    matrix = create(edges);
}

bool** mgraph::create(const edge edges[]) const
{
    bool** temp = new bool*[nodes];
    for (size_t i = 0; i < nodes; ++i)
        temp[i] = new bool[nodes];
    for (size_t i = 0; i < nodes - clusters; ++i)
    {
        temp[edges[i].a][edges[i].b] = true;
        temp[edges[i].b][edges[i].a] = true;
    }
    for (size_t i = 0; i < nodes; ++i)
        for (size_t j = 0; j < nodes; ++j)
            if (!temp[i][j])
                temp[i][j] = false;
    return temp;
}

size_t* mgraph::selection()
{
    components = new size_t[nodes];
    for (size_t i = 0; i < nodes; ++i)
        components[i] = 0;
    for (size_t i = 0; i < nodes; ++i)
        if (!components[i])
        {
            ++quantity;
            search(i);
        }
    return components;
}

void mgraph::search(const size_t current)
{
    components[current] = quantity;
    for (size_t i = 0; i < nodes; ++i)
        if (matrix[current][i] && !components[i])
            search(i);
}

void mgraph::centroid(const point points[]) const
{
    size_t start = 0;
    bool first = true;
    point* centroid = new point[clusters];
    std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> ret;
    std::multimap<size_t, size_t> tree;
    for (size_t i = 0; i < clusters; ++i)
    {
        for (size_t j = start; j < nodes; ++j)
        {
            if (i + 1 != components[j])
            {
                if (first)
                {
                    start = j;
                    first = false;
                }
                continue;
            }
            tree.insert(std::pair<size_t, size_t>(i + 1, j + 1));
        }
        ret = tree.equal_range(i + 1);
        for (std::multimap<size_t, size_t>::iterator it = ret.first; it != ret.second; ++it)
        {
            centroid[i].x += points[it -> second - 1].x / tree.count(i + 1);
            centroid[i].y += points[it -> second - 1].y / tree.count(i + 1);
        }
        first = true;
        std::cout << "Centroid of the " << i + 1 << " component: "
                  << "(" << centroid[i].x << ", " << centroid[i].y << ")" << std::endl;
    }
}

mgraph::~mgraph()
{
    for (size_t i = 0; i < nodes; ++i)
        delete [] matrix[i];
    delete [] matrix;
    delete [] components;
}

std::ostream & operator<<(std::ostream & os, const mgraph & object)
{
    os << std::setw(15) << "adjacency matrix";
    for (size_t i = 0; i < object.nodes; i++)
        os << std::setw(3) << "| "<< i + 1;
    os << std::endl;
    for (size_t i = 0; i < object.nodes; i++)
    {
        os << std::setw(16) << i + 1;
        for (size_t j = 0; j < object.nodes; j++)
            os << std::setw(3) << "| " << object.matrix[i][j];
        os << std::endl;
    }
    for (size_t i = 0; i < object.nodes; ++i)
        os << i + 1 << " node contains in " << object.components[i] << " component" << std::endl;
    return os;
}

void input(size_t* n, size_t* c)
{
    std::cout << "Enter the number of points on the plane: ";
    while (!(std::cin >> *n) || *n > 1000)
    {
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Incorrect input or too many points on the plane "
                  << "(must be less than or equal to 1000). Try again: ";
    }
    std::cout << "Enter the number of clusters: ";
    while (!(std::cin >> *c) || *c > *n)
    {
        std::cin.clear();
        while (std::cin.get() != '\n');
        std::cout << "Incorrect input or too many clusters "
                  << "(must be less than or equal to the number of points on the plane). Try again: ";
    }
}

void randomize(point points[], const size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        points[i].x = (rand() - rand()) % 20;
        points[i].y = (rand() - rand()) % 20;
    }
}

void count(const size_t components[], const size_t n)
{
    std::map<int, int> pair;
    for (size_t i = 0; i < n; ++i)
        pair[components[i]]++;
    for (auto i = pair.begin(); i != pair.end(); ++i)
    {
        std::cout << i -> first << " component includes " << i -> second;
        i -> second == 1 ? std::cout << " node" << std::endl : std::cout << " nodes" << std::endl;
    }
}

void clean(point points[], edge edges[])
{
    delete [] points;
    delete [] edges;
}
