#include <string>
#include <iostream>
using namespace std;

class Junction
{
  public:
    Junction(string id_, double elv_, double demand_) : id(id_), elevation(elv_), demand(demand_)
    {
    }

    string getId() const
    {
        return id;
    }

    double getElevation() const
    {
        return elevation;
    }

    double getDemand() const
    {
        return demand;
    }

  private:
    string id;
    double elevation;
    double demand;
};

