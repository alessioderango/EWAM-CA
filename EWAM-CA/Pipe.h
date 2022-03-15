#include <string>
#include <iostream>
using namespace std;

class Pipe
{
  public:
    Pipe(string id_,
         string node1_,
         string node2_,
         double length_,
         double diameter_,
         int roughness_,
         int minorLoss_,
         string status_) : id(id_), node1(node1_), node2(node2_), length(length_), diameter(diameter_), roughness(roughness_), minorLoss(minorLoss_), status(status_)
    {
    }

    string id;
    string node1;
    string node2;
    double length;
    double diameter;
    int roughness;
    int minorLoss;
    string status;
};


