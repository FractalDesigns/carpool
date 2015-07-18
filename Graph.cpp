//
// Created by Achraf EL AFRIT on 6/30/15.
//
#include <iostream>
#include <fstream>
#include <cstring>
#include <stack>
#include "Graph.h"
using namespace std;

bool Graph::inputfileisgood(){
    return inputfile.is_open();
}

int convertToInt(const char number[]){
    int numb = -3;
    try {
        numb = stoi(number);
        return numb;
    } catch (invalid_argument) {
        return numb;
    }
    return 0;
}

Graph::Graph(const char * inputfilename) {
    //the text file contains all data from witch we construct the adjacency list
    //sort(adjacencyList.begin(),adjacencyList.end());
    inputfile.open(inputfilename);
    vertex a2;
    string key="";
    char buf[MAX_CHARS_PER_LINE];
    const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
    bool processingTheAdjacencyList = true ;
    bool processingOffers = false ;
    bool processingDemands = false ;
    const char* const DELIMITER = " ->:@#";
    //int dist=0;
    vector<vertex> nextvertices;
    while (!inputfile.eof())
    {   // read an entire line into memory
        inputfile.getline(buf, MAX_CHARS_PER_LINE);
        ////// parse the line //////
        int n = 0; // a for-loop index and the number of tokens in the line
        // parse the line
        token[0] = strtok(buf, DELIMITER); // first token
        if (token[0]) // zero if line is blank
        {
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(nullptr, DELIMITER); // subsequent tokens
                if (!token[n]) break; // no more tokens
            }
        }
        // process the tokens on the line
        if (processingTheAdjacencyList){
            for (int i = 0; i < n/* n = #of tokens*/; i++) {
                /*i had to use exeption handling because the stoi() function throw an exeption of type invalid_argument when token[i] cannot be converted into integer */
                if (key == "end"){
                    processingTheAdjacencyList = false ;
                    processingDemands = true ;
                    break;
                }
                if (strcmp(token[i],"Graph")!=0 && strcmp(token[i],"AdjacencyList")!=0 && strcmp(token[i],"Demands")!=0 &&strcmp(token[i],"Offers")!=0) {
                    /*that means if token[i] is not equal to Graph, AdjacencyList,Demands and Offers*/
                    if (convertToInt(token[i])<0) {
                        if (i==0){
                            key = token[i];
                            //add the vertex name to the vertices set
                            if (vertices.count(key)==0 && key != "end") {
                                vertices.insert(key);
                            }
                        }else if (i==1){
                            a2.vertexName = token[1];
                            //add the vertex name to the vertices set
                            if (vertices.count(token[1]) == 0) {
                                vertices.insert(token[1]);
                            }
                        }
                    }else{
                        a2.var = convertToInt(token[i]); // var = distance
                        mgraph[key].push_back(a2);
                    }
                }
            }
        }else if (processingDemands){
            key = "" ;
            for (int i = 0; i < n/* n = #of tokens*/; i++) {
                if (strcmp(token[i],"Graph")!=0 && strcmp(token[i],"AdjacencyList")!=0 && strcmp(token[i],"Demands")!=0 &&strcmp(token[i],"Offers")!=0) {
                    /*that means if token[i] is not equal to Graph, AdjacencyList,Demands and Offers*/
                    if (i==0){
                        key = token[i];
                        if (key == "end"){
                            processingDemands = false;
                            processingOffers = true ;
                            break;
                        }
                        
                    }else if (i==1){
                        demands[key].push_back(token[i]);
                    }
                }
            }
        }else if (processingOffers){
            key = "";
            for (int i = 0; i < n/* n = #of tokens*/; i++) {
                if (key == "end"){
                    processingOffers = false ;
                    break;
                }
                if (strcmp(token[i],"Graph")!=0 && strcmp(token[i],"AdjacencyList")!=0 && strcmp(token[i],"Demands")!=0 &&strcmp(token[i],"Offers")!=0) {
                    /*that means if token[i] is not equal to Graph, AdjacencyList,Demands and Offers*/
                    if (convertToInt(token[i])<0) {
                        if (i==0){
                            key = token[i];
                        }else if (i==1){
                            a2.vertexName = token[i];
                        }
                    }else{
                        a2.var = convertToInt(token[i]); // var = #vacant places
                        offers[key].push_back(a2);
                    }
                }
            }
        }
    }
    inputfile.close();
}

void Graph::printVector(vector<vertex> & vect){
    cout<<"[";
    for (auto i : vect) {
        cout<<i.vertexName<<", "<<i.var<<" "; // var = distance
    }
    cout<<"]"<<endl;
}
void Graph::printVector(vector<string> & vect){
    cout<<"[";
    for (auto i : vect) {
        cout<<i<<", "; // var = distance
    }
    cout<<"]"<<endl;
}

void Graph::showGraphContent(){
    cout<< "adjacencyList"<<endl;
    for ( auto i : mgraph) {
        cout << i.first<<" : ";
        Graph::printVector(i.second);
    }
    cout<< "Offers"<<endl;
    
    for (auto i : offers) {
        cout << i.first<<" : ";
        Graph::printVector(i.second);
    }
    cout <<"demand"<<endl;
    for (auto i :demands) {
        cout<< i.first <<" : ";
        Graph::printVector(i.second);
        cout<<endl;
    }
}
vector<string> Graph::searchForpreviousVerticesOf(string vertexName){
    vector<string> previousvertices;
    for (auto i : vertices) {
        for(auto j : mgraph[i]){
            if(j.vertexName == vertexName){
                previousvertices.push_back(i);
            }
        }
    }
    return previousvertices;
}
bool Graph::pathExistsFromTo(string origin , string destination){
    
    return false;
}

void Graph::tryToSatisfyOffersKeepingTheShortestPath(const char* outputfilename ){

    outputfile.open(outputfilename);
    auto offercopy = offers ; // must make a copy because data might be changed
    bool demandSatisfied ;
    for (  auto i : demands) {
        //the demand list is similar to adjacency list of the graph
        //iterate over the destinations
        for (auto h : i.second) {
            demandSatisfied =false;
            //FIXME : need to implement a move asign to reinitialize the size back to zero otherwise the subsequent check of the demandPath 's size doesn't make sense
            auto demandPath = shortestPath(i.first , h );
            if (demandPath.size() < 2 ) { //there is no path
                cout<< "impossible path"<<endl;
                continue;
            }else{
                //then we need to check the offers
                
                for (auto j : offercopy) {
                    //iterating over the adjacency list of offers
                    for (auto k = j.second.begin() ; k != j.second.end() ; k++ ) {
                        auto offerPath = shortestPath(j.first, k->vertexName);
                        if (k->var >0 && pathOneContainPathTow(offerPath,demandPath )) {
                            // demand was satisfied
                            --k->var; //decrement the number of availeble seats for the offer
                            demandSatisfied = true ;
                            cout<< "demand satisfied"<<endl;
                            cout<< "demand : " <<i.first <<"->"<< h<<endl;
                            cout<< "offer : "<< j.first << "->"<< k->vertexName << k->var<<endl;
                            //write result to a file
                            break;
                        }else if (pathOneContainPathTow(offerPath,demandPath ) == false ){
                            //BEWARE : path is not satisfied in this iteration
                        }
                    }
                    if (demandSatisfied) {
                        break; // because demand is satisfied no need to continue looping over other offers
                    }
                }
            }
            if (!demandSatisfied) {
                //write to file that demand can't be satisfied
                cout<< "demand can't be satisfied "<< i.first << " -> " << h<<endl;
                
            }
            cout<<endl<<"*************"<<endl;
        }
    }
}
bool compareString (string &a ,string &b){
    return a > b;
};

bool Graph::pathOneContainPathTow(vector<string> & Offer, vector<string> & demand){
    // normally we shoold carpool with a single person to avoid the risk of time overlap betwwen drivers if we want to satisfy a single demand with multiple offers
    // that's why we need to check if eventual path for the demand is included in the path of the offer and not the other way around
    return includes(Offer.begin(), Offer.end(), demand.begin(), demand.end(),compareString);
//Returns true if the sorted range [Offer.begin(),Offer.end()) contains all the elements in the sorted range [demand.begin(),demand.end())
    
    
}
vector<string>  Graph::shortestPath(string origin, string destination){
    string source = origin;
    map<string ,  string> parent;
    map<string , int> pathweight;
    map<string ,int> previousPathWeight;
    //Bellman-Ford init
    set<string>::iterator vertexIterator = vertices.begin();
 
    pathweight[source] = 0;
    previousPathWeight[source] = 0 ;
    for (; vertexIterator != vertices.end() ; vertexIterator ++) {
        if (*vertexIterator == source )
            continue;
        parent[*vertexIterator] = "";
        previousPathWeight[*vertexIterator] = INFINITY;
        
    }
    //starts Bellman-Ford' s iterations
    vertexIterator = vertices.begin();
    vector<string> predecessors ;
    vector<string>::iterator predecessorIterator ;
    for (int i = 0 ; i < vertices.size() - 2 ; i++){
        for ( ;vertexIterator != vertices.end()  ; vertexIterator++) {
            if (*vertexIterator == source) {
                continue;
            }
            predecessors = searchForpreviousVerticesOf(*vertexIterator);
            predecessorIterator = predecessors.begin();
            for(; predecessorIterator != predecessors.end(); predecessorIterator++){
                if (previousPathWeight[*vertexIterator] > previousPathWeight[*predecessorIterator] + EdgeWeight(*predecessorIterator,*vertexIterator) && EdgeWeight(*predecessorIterator,*vertexIterator) > 0) {
                    
               //relax the node
                        pathweight[*vertexIterator] =  previousPathWeight[*predecessorIterator] + EdgeWeight(*predecessorIterator,*vertexIterator);
                        parent[*vertexIterator] = *predecessorIterator;
                }
//                else {
//                    pathweight[*vertexIterator]= previousPathWeight[*vertexIterator];
//                }
            }
        }
    
        vertexIterator = vertices.begin();
        //export the pathweight elements to the previousPathweight
        for ( ;vertexIterator != vertices.end()  ; vertexIterator++) {
            if (pathweight[*vertexIterator]>0) // to check if the pathweight is set because otherwise it's going to asign 0 when the key *vertexIterator is not defined in the pathWeight map
            previousPathWeight[*vertexIterator] = pathweight[*vertexIterator];
        }
        vertexIterator = vertices.begin();
        
    }
    // no need to implement the negative cycle detection given that all weights are positive for this project
    
    // constructing the shortest path
    vector<string> shortestPath;
    stack<string> s;
    s.push(destination);
    string dest = destination;
    while (dest != origin){
        s.push( dest = parent[dest]);
        if (s.size() >= vertices.size()) {
            break;
        }
    }
    while (s.size()!= 0) {
        shortestPath.push_back(s.top());
        s.pop();
    }
    return shortestPath;
}
int Graph::EdgeWeight(string head, string tail){
auto iter = mgraph[head].begin();
auto end = mgraph[head].end();
for(; iter != end ; iter++ )
{
    if((*iter).vertexName == tail ){
    return (*iter).var ;
    } 
}
return -1;
}
void Graph::printShortestPathFromto(string start , string finish){
    vector<string> path = Graph::shortestPath(start, finish);
    cout<< endl<<"the path from "<< start<< " to "<< finish<<" : ";
    for (auto i : path) {
        cout<< i ;
    }
}
void Graph::printVerticesSet(){
    cout<<"there is "<<vertices.size()<<" vertices"<<endl;
    for (auto i : vertices) {
        cout<<i<<" ";
    }
}
Graph::~Graph(){
}
void Graph::writeSolutionToFile(){
    ofstream outputfile("result.txt");
}
