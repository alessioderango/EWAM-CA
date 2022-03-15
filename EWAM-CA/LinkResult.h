#include <string>
#include <iostream>
using namespace std;

class LinkResult
{
  public:
    LinkResult(string id_,
               double flow_, 
               double velocityunit_,
               double headloss_,
               string status_,
               string deltat_,
               string idLink_) :
               id(id_), 
               flow(flow_), 
               velocityunit(velocityunit_),
               headloss(headloss_),
               status(status_),
               deltat(deltat_),
               idLink(idLink_)

    {
    }

    string id;
    string idLink;
    double flow;
    double velocityunit;
    double headloss;
    string status;
    string deltat;
};


