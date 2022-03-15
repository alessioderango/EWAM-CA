#include <string>
#include <iostream>
using namespace std;

class NodeResult
{
  public:
    NodeResult(string id_,
               double demand_, 
               double head_,
               double pressure_,
               double quality_,
               string deltat_) :
               id(id_), 
               demand(demand_), 
               head(head_),
               pressure(pressure_),
               quality(quality_),
               deltat(deltat_)
    {
    }

    string id;
    double demand;
    double head;
    double pressure;
    double quality;
    string deltat;
};


