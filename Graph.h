//
// Created by Achraf EL AFRIT on 6/30/15.
//

#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <set>
#include "terminal_colorized_output.h"

using namespace std;

#ifndef PROJET_D_ÉTÉ_GRAPH_H
#define PROJET_D_ÉTÉ_GRAPH_H

#ifndef _VERTEX_
#define _VERTEX_
typedef struct vertex{
    string vertexName;
    int var;
//var can be either a distance or the number of vacant places in the car
}vertex;
#endif

#define MAX_CHARS_PER_LINE  20
#define MAX_TOKENS_PER_LINE  3
#define INFINITY 10000

class Graph {
private:

    map<string, vector<vertex> > mgraph;
    map<string, vector<vertex> > offers;
    map<string, vector<string> > demands;
    // create a file-reading object
    ifstream inputfile;
    ofstream outputfile;
    set<string> vertices;
    
    //private methods:
    int EdgeWeight(string,string);
    void printVector(vector<vertex> & );
    void printVector(vector<string> & );
    void fprintVector(vector<string> & );
    bool pathExistsFromTo(string, string);
    vector<string> searchForpreviousVerticesOf(string);
    
public:
    bool insertPathOneIntoPathTwo(vector<string> & pathOne, vector<string> & pathTwo);
    vector<string>  shortestPath(string , string );
    bool pathOneContainPathTow(vector<string> &, vector<string> &);
    void tryToSatisfyDemandsKeepingTheShortestPath(const char* outputfilename = "result.txt");
    void tryToSatisfyAllDemands(const char* outputfilename = "result.txt");
    void printShortestPathFromto(string  , string );
    
    Graph(const char* inputfilename);
    int pathWeight(vector<string> &);
    void showGraphContent();
    void printVerticesSet();
    ~Graph();

};




#endif //PROJET_D_ÉTÉ_GRAPH_H
