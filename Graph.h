//
// Created by Achraf EL AFRIT on 6/30/15.
//

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <stack>
#include <map>
#include <set>

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
    set<string> vertices;
    
    //private methods:
    int EdgeWeight(string,string);
    void printVector(vector<vertex> & );
    void printVector(vector<string> & );
    bool pathExistsFromTo(string, string);
    
    
    vector<string> searchForpreviousVerticesOf(string);
    
    
        
public:
    vector<string>  shortestPath(string , string );
    bool pathOneContainPathTow(vector<string> &, vector<string> &);
    void tryToSatisfyOffersKeepingTheShortestPath();
    void printShortestPathFromto(string  , string );
    Graph(const char* inputfilename);
    bool inputfileisgood();
    void showGraphContent();
    void writeSolutionToFile();
    void generateoutputfile();
    void readFileAndWriteTokensToAnotherFile();
    void printVerticesSet();
    ~Graph();

};




#endif //PROJET_D_ÉTÉ_GRAPH_H
