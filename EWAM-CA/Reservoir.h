#include <string>
#include <iostream>
using namespace std;

class Reservoir
{
  public:
    Reservoir(string id_, string head_) : id(id_), head(head_)
    {
    }

    string getId() const
    {
        return id;
    }

    string getHead() const
    {
        return head;
    }


  private:
    string id;
    string head;
};

