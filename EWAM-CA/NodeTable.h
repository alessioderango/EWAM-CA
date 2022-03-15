#include <string>
#include <iostream>
using namespace std;

class NodeTable
{
  public:
    NodeTable(string id_, string startNode_, string endNode_, double length_, double diameter_) :
     id(id_), startNode(startNode_), endNode(endNode_), length(length_), diameter(diameter_)
    {
    }

    string id;
    string startNode;
    string endNode;
    double length;
    double diameter;
};


