#include "cawdn.h"
#include "utilis.h"

vector<NodeAC> CAWDN::cellularSpaceCurrent;

void getDataFromString(std::string& strOneLine, std::string& output)
{
    std::string delimiter = " ";
    size_t pos = strOneLine.find(delimiter);
    strOneLine.erase(0, pos + delimiter.length());
    pos = strOneLine.find(delimiter);
    output = strOneLine.substr(0, pos);
}

int CAWDN::parseFileConfFile(std::string file_name, double& alpha, double& tau, int& limitTime)
{
    ifstream inFile(file_name.c_str());

    if (!inFile)
    {
        cerr << "unable to open input file: "
             << file_name << " \n";
        return -1;
    }
    string strOneLine;
    while (inFile)
    {
        getline(inFile, strOneLine);
        if (strOneLine.find("Alpha") != std::string::npos)
        {
            string output;
            getDataFromString(strOneLine, output);
            alpha =-(std::atof(output.c_str())/86400);
            if(alpha == 0)
                alpha = 0;
        }
        if (strOneLine.find("Tau") != std::string::npos)
        {
            string output;
            getDataFromString(strOneLine, output);
            tau = std::atof(output.c_str());

        }

        if (strOneLine.find("Tempo_della_simualzione") != std::string::npos)
        {
            string output;
            getDataFromString(strOneLine, output);
            limitTime = std::atoi(output.c_str());
        }
    }
    inFile.close();
    return 0;
}

CAWDN::CAWDN()
{

}

CAWDN::CAWDN(std::map<string, Junction> junctions,
             std::map<string, Reservoir> reservoirs,
             std::map<string, Coordinate> coordinates,
             std::map<string, Pipe> pipes,
             std::map<string, NodeTable> nodeTable,
             std::map<string, NodeResult> nodeResults,
             std::map<string, LinkResult> linkResults,
             std::map<string, Chip *> graphicalItems,
             vector<QGraphicsLineItem *> edges,
             std::vector<string> timesteps,
             QString absolutePath)
{

    this->junctions = junctions;
    this->reservoirs = reservoirs;
    this->coordinates = coordinates;
    this->pipes = pipes;
    this->nodeTable = nodeTable;
    this->nodeResults = nodeResults;
    this->linkResults = linkResults;
    this->graphicalItems = graphicalItems;
    this->edges = edges;
    this->timesteps = timesteps;
    this->alpha = 0;
    this->delta_t_sim = 0;
    this->fileStat = absolutePath+"/ris.out";
    onestep = false;
    limitTime = 3600;
    this->absolutePath = absolutePath;

    QString confFile =  absolutePath + "/conf.txt" ;
    cout << "Leggo configurazione dal file:" << endl;
    cout << confFile.toStdString() << endl;
    parseFileConfFile(confFile.toStdString(), this->alpha, this->tau, limitTime);
    cout << "Delta T simulation: " << limitTime << endl;
    cout << "Saving output :  " << this->fileStat.toStdString() << endl;
    cout << "Alpha :  " << this->alpha << endl;
    cout << "Tau :  " << this->tau << endl;
    cout << "Finished Initialization" << endl;



}
CAWDN::~CAWDN()
{
    runGlobalFunction = false;
}



void CAWDN::updateLinkDirection()
{
    for (auto it =linkResults.cbegin(); it != linkResults.cend(); it++ )
    {
        for (auto i = 0; i < cellularLink.size(); i++)
        {
            string t = cellularLink.at(i).id+timesteps.at(timesteps.size()-1);
            if(t == it->first && cellularLink.at(i).id == it->second.id)
            {
                NodeAC* ni;
                NodeAC* nj;
                for (auto i = 0; i < cellularSpaceCurrent.size(); i++)
                {
                    if(cellularSpaceCurrent.at(i).id == nodeTable.at(it->second.id).startNode)
                        ni = &cellularSpaceCurrent.at(i);
                    if(cellularSpaceCurrent.at(i).id == nodeTable.at(it->second.id).endNode)
                        nj = &cellularSpaceCurrent.at(i);
                }

                if(it->second.flow < 0)
                {
                    cellularLink.at(i).nodes.push_back(nj);
                    cellularLink.at(i).nodes.push_back(ni);
                }else
                {
                    cellularLink.at(i).nodes.push_back(ni);
                    cellularLink.at(i).nodes.push_back(nj);
                }
            }

        }
    }

}


void CAWDN::init()
{
    runGlobalFunction = false;
    cellularSpaceCurrent.clear();

    for (auto it = junctions.cbegin(); it != junctions.cend(); it++ )
    {
        double concentrazione = 0;
        bool serbatoio = false;

        NodeAC n(it->first,0,serbatoio, concentrazione);
        cellularSpaceCurrent.push_back(n);
        cellularSpaceNext.push_back(n);
        //        }
    }

    for (auto it = reservoirs.cbegin(); it != reservoirs.cend(); it++ )
    {
        double concentrazione = 100;

        NodeAC n(it->first,0,true,concentrazione);
        cellularSpaceCurrent.push_back(n);
        cellularSpaceNext.push_back(n);
    }


    cellularLink.clear();
    for (auto it = nodeTable.cbegin(); it != nodeTable.cend(); it++ )
    {
        LinkAC l (it->second.id);
        l.length = it->second.length;
        l.diameter = convertmminM(it->second.diameter);
        double r = l.diameter/2;
        double areacerchio = (M_PI*r*r);
        l.volume = convertmcuboinL(areacerchio*l.length); // in Litri
        cellularLink.push_back(l);
    }

    for (auto i = 0; i < cellularLink.size(); i++)
    {
        for (auto it =linkResults.cbegin(); it != linkResults.cend(); it++ )
        {
            string t = cellularLink.at(i).id+timesteps.at(timesteps.size()-1);
            if(t == it->first)
            {
                cellularLink.at(i).flow = it->second.flow;
            }
        }
    }

    double min = cellularLink[0].volume/abs(cellularLink[0].flow);
    for (auto i = 1; i < cellularLink.size(); i++)
    {
        double voltmp = cellularLink[i].volume/abs(cellularLink[i].flow);
        if(voltmp < min)
        {
            min= voltmp;
        }
    }

    if(tau == -1)
        tau = min;

    for (auto i = 0; i < cellularLink.size(); i++)
    {
        if(cellularLink[i].volume == 0)
             cellularLink[i].ni = 1;
        else
           cellularLink[i].ni = floor(cellularLink[i].volume/(abs(cellularLink[i].flow)*tau));

        if(cellularLink[i].ni <= 0 || isinf(cellularLink[i].ni))
            cellularLink[i].ni = 1;
        cellularLink[i].v = cellularLink[i].volume/cellularLink[i].ni;
        cellularLink[i].massa.resize(cellularLink[i].ni);
        cellularLink[i].concentrazione.resize(cellularLink[i].ni);
    }

    updateLinkDirection();

    for (auto l = 0; l < cellularLink.size(); l++)
    {
        for (auto i = 0; i < cellularSpaceCurrent.size(); i++)
        {
            if(cellularSpaceCurrent.at(i).id.compare(cellularLink[l].nodes[START]->id)==0)
            {
                cellularSpaceCurrent.at(i).link_uscenti.push_back(&cellularLink[l]);
            }
            if(cellularSpaceCurrent.at(i).id.compare(cellularLink[l].nodes[END]->id)==0)
            {
                cellularSpaceCurrent.at(i).link_entranti.push_back(&cellularLink[l]);
            }
        }
    }


    cellularSpaceNext = cellularSpaceCurrent;
    set_concentrazione();
    cellularSpaceNext = cellularSpaceCurrent;
}


void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void CAWDN::set_concentrazione()
{
    for (auto n = 0; n < cellularSpaceCurrent.size(); n++)
    {

        for (auto l = 0; l < cellularSpaceCurrent[n].link_uscenti.size(); l++)
        {
            double concentrazione = cellularSpaceCurrent[n].concentrazione;

            for (auto c = 0; c < cellularSpaceNext[n].link_uscenti[l]->concentrazione.size(); c++)
            {
                cellularSpaceNext[n].link_uscenti[l]->concentrazione[c]=concentrazione;
            }

        }
    }

}

//Link
void CAWDN::step0_calcolaConcentrazioneOriginale()
{
    for (auto n = 0; n < cellularSpaceCurrent.size(); n++)
    {
        if(cellularSpaceCurrent[n].reservoir)
        {

            for (auto l = 0; l < cellularSpaceCurrent[n].link_uscenti.size(); l++)
            {
                cellularSpaceNext[n].link_uscenti[l]->massa[START] = cellularSpaceNext[n].link_uscenti[l]->concentrazione[START]*abs(cellularSpaceCurrent[n].link_uscenti[l]->flow)*tau;
            }

        }
    }

}
//Link
void CAWDN::step1_aggiornaMassaTubo()
{

    for (auto n = 0; n < cellularSpaceCurrent.size(); n++)
    {
            for (auto l = 0; l < cellularSpaceCurrent[n].link_uscenti.size(); l++)
            {
                for (auto m = 0; m < cellularSpaceCurrent[n].link_uscenti[l]->massa.size(); m++)
                {
                     cellularSpaceCurrent[n].link_uscenti[l]->massa[m] =  cellularSpaceCurrent[n].link_uscenti[l]->massa[m]* exp(alpha*tau);
                }
            }
    }
}

//Link
void CAWDN::step2_aggiornaConcentrazioneNodo()
{
    for (auto n = 0; n < cellularSpaceCurrent.size(); n++)
    {
        if(!cellularSpaceCurrent[n].reservoir)
        {
            double sommaMassaUltimiVolumi=0;
            double sommaPortate=0;
            for (auto l = 0; l < cellularSpaceCurrent[n].link_entranti.size(); l++)
            {
                int sizeVolumi = cellularSpaceCurrent[n].link_entranti[l]->massa.size();
                sommaMassaUltimiVolumi+=cellularSpaceCurrent[n].link_entranti[l]->massa[sizeVolumi-1];
                sommaPortate+=abs(cellularSpaceCurrent[n].link_entranti[l]->flow)*tau;
            }
            if(sommaPortate != 0)
                cellularSpaceNext[n].concentrazione=sommaMassaUltimiVolumi/sommaPortate;
            else
                cellularSpaceNext[n].concentrazione=0;

        }
    }
    set_concentrazione();
}
//Link
void CAWDN::step3_swiftVolumi()
{
    //Step 2
    for (auto n = 0; n < cellularSpaceCurrent.size(); n++)
    {
            for (auto l = 0; l < cellularSpaceCurrent[n].link_uscenti.size(); l++)
            {
                for (auto m = cellularSpaceCurrent[n].link_uscenti[l]->massa.size()-1; m > 0; m--)
                {
                    cellularSpaceCurrent[n].link_uscenti[l]->massa[m] =  cellularSpaceCurrent[n].link_uscenti[l]->massa[m-1];
                    cellularSpaceCurrent[n].link_uscenti[l]->concentrazione[m] =  cellularSpaceCurrent[n].link_uscenti[l]->concentrazione[m-1];
                }
                cellularSpaceCurrent[n].link_uscenti[l]->massa[0] =0.;
            }
    }

}

//Node
void CAWDN::step4_aggiornaMassaVolumeConcentrazione()
{
    //Step 3
    for (auto n = 0; n< cellularSpaceCurrent.size(); n++)
    {
        if(!cellularSpaceCurrent[n].reservoir)
        {
            for (auto l = 0; l < cellularSpaceCurrent[n].link_uscenti.size(); l++)
            {
                double portata = abs(cellularSpaceCurrent[n].link_uscenti[l]->flow);
                double v = portata*tau;
                cellularSpaceNext[n].link_uscenti[l]->massa[START] = cellularSpaceCurrent[n].link_uscenti[l]->concentrazione[START]*v;
            }
        }
    }
}



#include <fstream>
void CAWDN::writeFileStat(double time, vector<double> concentrations)
{
    //
    ofstream myfile (fileStat.toStdString(),ios_base::app);
    if (myfile.is_open())
    {
        myfile << time/60 << " ";
        for (int i = 0; i< concentrations.size();i++) {
            myfile << concentrations[i] << " ";
        }
        myfile << " \n";
        myfile.close();
    }
    else cout << "Unable to open file";
}

void CAWDN::writeArrivi(QString namefile, vector<double> arrivo1, vector<double> arrivo2)
{
    ofstream myfile (namefile.toStdString(),ios_base::app);
    if (myfile.is_open())
    {
    for (auto h = 0; h < header.size(); h++)
    {
        for (int n = 0; n < cellularSpaceCurrent.size(); n++)
        {
            if(header[h].compare(cellularSpaceCurrent[n].id)==0)
            {
                int i = stoi(cellularSpaceCurrent[n].id);

                myfile << cellularSpaceCurrent[n].id << " " << arrivo1[i] << " " << arrivo2[i] << " " << arrivo1[i]/60 << " " << arrivo2[i]/60 << " \n";
            }
        }
    //

    }
    myfile << " \n";
    myfile.close();
    }
    else cout << "Unable to open file";
}

void CAWDN::resetFileStat(vector<string> header)
{

    ofstream myfile (fileStat.toStdString(),ios_base::out);
    if (myfile.is_open())
    {
        myfile << "delta_t";
        for (int i = 0; i< header.size();i++) {
            myfile << " " << header[i];
        }
        myfile << " \n";
        myfile.close();
    }
    else cout << "Unable to open file";
}
void CAWDN::resetFile(QString filename)
{

    ofstream myfile (filename.toStdString(),ios_base::out);
    if (myfile.is_open())
    {

        myfile.close();
    }
    else cout << "Unable to open file";
}

std::map<string, Chip *> CAWDN::getGraphicalItems() const
{
    return graphicalItems;
}

double CAWDN::getDelta_t_sim() const
{
    return delta_t_sim;
}

void CAWDN::setDelta_t_sim(double value)
{
    delta_t_sim = value;
}

int CAWDN::getLimitTime() const
{
    return limitTime;
}

void CAWDN::setLimitTime(int value)
{
    limitTime = value;
}

bool myfunction (string i,string j) { return (stoi(i)<stoi(j)); }


void CAWDN::onestepFunction()
{

    if(delta_t_sim == 0)
    {
        for (auto n = 0; n < cellularSpaceCurrent.size(); n++)
        {
            header.push_back(cellularSpaceCurrent[n].id);
        }
        std::sort (header.begin(), header.end(), myfunction);
        resetFileStat(header);
        resetFile(absolutePath+"/arrivi.out");
        resetFile((fileStat+"_Massa"));
        resetFile((fileStat+"_Flow"));
        init();
        updateView();
        runGlobalFunction = true;

        arrivo1.resize(cellularSpaceCurrent.size()+1);
        arrivo2.resize(cellularSpaceCurrent.size()+1);
        modificatoarrivo1.resize(cellularSpaceCurrent.size()+1);
        modificatoarrivo2.resize(cellularSpaceCurrent.size()+1);
        std::fill(arrivo1.begin(), arrivo1.end(), -1);
        std::fill(arrivo2.begin(), arrivo2.end(), -1);
        std::fill(modificatoarrivo1.begin(), modificatoarrivo1.end(), false);
        std::fill(modificatoarrivo2.begin(), modificatoarrivo2.end(), false);
    }


    vector<double> concentrations;
    for (auto h = 0; h < header.size(); h++)
    {
        for (int n = 0; n < cellularSpaceCurrent.size(); n++)
        {

            if(header[h].compare(cellularSpaceCurrent[n].id)==0)
            {
                int index = stoi(cellularSpaceCurrent[n].id);
                    concentrations.push_back(cellularSpaceCurrent[n].concentrazione);
            }
        }
    }

    step0_calcolaConcentrazioneOriginale();
    writeFileStat(delta_t_sim,concentrations);
    concentrations.clear();
    cellularSpaceCurrent = cellularSpaceNext;
    step1_aggiornaMassaTubo();
    cellularSpaceCurrent = cellularSpaceNext;
    step2_aggiornaConcentrazioneNodo();
    cellularSpaceCurrent = cellularSpaceNext;
    step3_swiftVolumi();
    cellularSpaceCurrent = cellularSpaceNext;
    step4_aggiornaMassaVolumeConcentrazione();
    cellularSpaceCurrent = cellularSpaceNext;

    delta_t_sim+=tau;

}
void CAWDN::run()
{
    vector<string> header;
    for (auto n = 0; n < cellularSpaceCurrent.size(); n++)
    {

        header.push_back(cellularSpaceCurrent[n].id);
    }
    std::sort (header.begin(), header.end(), myfunction);
    resetFileStat(header);
    updateView();
    runGlobalFunction = true;

    while(delta_t_sim <= limitTime)
    {

        vector<double> concentrations;
        for (auto h = 0; h < header.size(); h++)
        {
            for (int n = 0; n < cellularSpaceCurrent.size(); n++)
            {

                if(header[h].compare(cellularSpaceCurrent[n].id)==0)
                {

                    concentrations.push_back(cellularSpaceCurrent[n].concentrazione);
                }
            }
        }
        writeFileStat(delta_t_sim,concentrations);
        step0_calcolaConcentrazioneOriginale();
        step1_aggiornaMassaTubo();
        step2_aggiornaConcentrazioneNodo();
        step3_swiftVolumi();
        step4_aggiornaMassaVolumeConcentrazione();

        cellularSpaceCurrent = cellularSpaceNext;
        updateView();

        delta_t_sim+=tau;
    }


}

NodeAC CAWDN::getInfoNode(string i)
{
    for (auto j = 0; j < cellularSpaceCurrent.size();j++)
    {
        if(cellularSpaceCurrent.at(j).id == i)
        {
            return cellularSpaceCurrent.at(j);
        }
    }
}

void CAWDN::activeCells(string i)
{
    for (auto j = 0; j < cellularSpaceCurrent.size();j++)
    {
        if(cellularSpaceCurrent.at(j).id == i)
        {
            cellularSpaceCurrent.at(j).state=1;
        }
    }
}

vector<LinkAC> CAWDN::getCellularLink() const
{
    return cellularLink;
}

void CAWDN::setCellularLink(const vector<LinkAC> &value)
{
    cellularLink = value;
}



void CAWDN::updateView()
{
    for (auto i = 0; i < cellularSpaceCurrent.size(); i++ )
    {
        if(cellularSpaceCurrent.at(i).state == 0)
        {
            graphicalItems.at(cellularSpaceCurrent.at(i).id)->changeColor(QColor(0,255,0));
            graphicalItems.at(cellularSpaceCurrent.at(i).id)->update();
        }
        else
            if(cellularSpaceCurrent.at(i).state == 1)
            {
                graphicalItems.at(cellularSpaceCurrent.at(i).id)->changeColor(QColor(0,0,255));
                graphicalItems.at(cellularSpaceCurrent.at(i).id)->update();
            }
    }



}
