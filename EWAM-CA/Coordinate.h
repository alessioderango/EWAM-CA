#include <string>
#include <iostream>
using namespace std;

class Coordinate
{
  public:
    Coordinate(string id_, double elv_, double demand_) : id(id_), XCoord(elv_), YCoord(demand_)
    {
    }

    string getId() const
    {
        return id;
    }

    double getXCoord() const
    {
        return XCoord;
    }

    double getYCoord() const
    {
        return YCoord;
    }

  private:
    string id;
    double XCoord;
    double YCoord;
};


