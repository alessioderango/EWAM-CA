#ifndef CAWDN_H
#define CAWDN_H
#include <string>


#include "ewamca.h"
#include "Junction.h"
#include "Coordinate.h"
#include "Pipe.h"
#include "NodeTable.h"
#include "NodeResult.h"
#include "LinkResult.h"
#include "Reservoir.h"

#include <QTime>
#include <QCoreApplication>
#include <iterator>
#include <math.h>
#include <QKeyEvent>

//class EdgeAC
//{
//private:
//    string idNode1;
//    string idNode2;
//    int direction;
//};

enum CONVENZIONE{START=0, END=1};

class Volume
{
public:
    Volume(string _id,int _state)
    {
        id = _id;
        state = _state;
    }

    string id;
    int state;
};
class NodeAC;
class LinkAC
{
public:
    LinkAC(string id)
    {
        this->id = id;
        massa.clear();
        concentrazione.clear();
    }
                           // ni --> nj
    vector<NodeAC*> nodes; //  [ni,nj]
                           //  [0, 1]
    double v; // volume del volumetto
    vector<double> massa;
    vector<double> concentrazione; // array concentrazione
    double concentrazionePerVolume;
    double flow; //portata Litri per secondo
    double volume;
    double length; // metri
    double diameter; // metri
    double ni;
    string id;
};

class NodeAC
{
public:
    NodeAC(string _id,int _state, bool reservoir, double concentrazione = 0)
    {
        id = _id;
        state = _state;
        this->reservoir =reservoir;
        this->concentrazione = concentrazione;
    }

    vector<NodeAC*> neighborhood;
    vector<LinkAC*> link_entranti;
    vector<LinkAC*> link_uscenti;
    string id;
    int state;
    bool reservoir;
    double concentrazione;
};



class CAWDN
{
public:
    CAWDN();
    ~CAWDN();
    CAWDN(std::map<string, Junction> junctions,
          std::map<string, Reservoir> reservoirs,
          std::map<string, Coordinate> coordinates,
          std::map<string, Pipe> pipes,
          std::map<string, NodeTable> nodeTable,
          std::map<string, NodeResult> nodeResults,
          std::map<string, LinkResult> linkResults,
          std::map<string, Chip*> graphicalItems,
          vector<QGraphicsLineItem *> edges,
          std::vector<string> timesteps,
          QString absolutePath);

    void updateLinkDirection();
    void init();
    void updateView();
    void writeArrivi(QString namefile, vector<double> arrivo1, vector<double> arrivo2);
    void run();
    void onestepFunction();
    static void activeCells(string );
    static NodeAC getInfoNode(string );


    vector<LinkAC> getCellularLink() const;
    void setCellularLink(const vector<LinkAC> &value);

    int getLimitTime() const;
    void setLimitTime(int value);

    double getDelta_t_sim() const;
    void setDelta_t_sim(double value);


    std::map<string, Chip *> getGraphicalItems() const;

private:

    void set_concentrazione();
    void step0_calcolaConcentrazioneOriginale();
    void step1_aggiornaMassaTubo();
    void step2_aggiornaConcentrazioneNodo();
    void step3_swiftVolumi();
    void step4_aggiornaMassaVolumeConcentrazione();
    void writeFileStat(double time, vector<double>  conc);
    void resetFileStat(vector<string> header);
    void resetFile(QString filename);
    int parseFileConfFile(std::string file_name,  double& alpha, double& tau, int& limitTime);

    std::map<string, Junction> junctions;
    std::map<string, Reservoir> reservoirs;
    std::map<string, Coordinate> coordinates;
    std::map<string, Pipe> pipes;
    std::map<string, NodeTable> nodeTable;
    std::map<string, NodeResult> nodeResults;
    std::map<string, LinkResult> linkResults;
    std::map<string, Chip*> graphicalItems;
    vector<QGraphicsLineItem*> edges;
    std::vector<string> timesteps;
    static vector<NodeAC> cellularSpaceCurrent;
    vector<NodeAC> cellularSpaceNext;
    vector<LinkAC> cellularLink;
    bool runGlobalFunction;
    double delta_t_sim;
    double tau; // delta_t
    double alpha; // coefficiente di reazione
    QString fileStat;
    bool onestep;
    int limitTime;
    vector<string> header;
    QString absolutePath;
    vector<double> arrivo1;
    vector<bool> modificatoarrivo1;
    vector<double> arrivo2;
    vector<bool> modificatoarrivo2;
};



#endif // CAWDN_H
