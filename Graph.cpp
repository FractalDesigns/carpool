//
// Created by Achraf EL AFRIT on 6/30/15.
//
#include <iostream>
#include <fstream>
#include <cstring>
#include "Graph.h"
#include <stack>

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

int Graph::pathWeight(vector<string> & path){
    int pathweight = 0 ;
    for (auto i = path.begin(); i != path.end()-1; i++) {
        for (auto j : mgraph) {
            if(*i == j.first){
                i++;
                for (auto k : j.second){
                    if (k.vertexName==*i) {
                        pathweight += k.var;
                        break;
                    }
                }
                i--;
                break;
            }
        }
    }
    return pathweight;
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
    //if shortest path doesn't exist then no path can exist
    auto p = shortestPath(origin, destination);
    int n = 0 ;
    for (auto i : p)
        if (i != "")
            n++;
    return n>1;
}

bool Graph::appendPathOneToPathTwo(vector<string> & pathOne, vector<string> & pathTwo){
    int minmumLinkerPathWeight = INFINITY;
    string lastVertex =pathTwo[pathTwo.size()-1];
    int sizeOfNewPath = 0; //how many vertices he's got
    string appendNode = "";
    if (pathTwo[0] == pathOne[0]) {
        appendNode = pathTwo[0];
    }else{
        vector<string> traversedPath ;
        for (auto i : pathTwo){
            traversedPath.push_back(i);
            // FIXME: this need to be enhanced , to find a more optemised appendNode
            vector<string> linkerPath;
            if (pathExistsFromTo(i, pathOne[0])){
                
                linkerPath = shortestPath(i,pathOne[0]);
                if (pathWeight(linkerPath) + pathWeight(traversedPath) < minmumLinkerPathWeight) {
                    minmumLinkerPathWeight = pathWeight(linkerPath) + pathWeight(traversedPath);
                    appendNode = i;
                }
            }
        }
    }
    if(!pathExistsFromTo(pathOne[pathOne.size() -1], lastVertex))
        return false;
    if (appendNode == "" ){
        return false; // not possible to get from any vertex of path two to the first vertex of path one
    }else if (appendNode == pathTwo[pathTwo.size()-1]) { // *(pathTwo.end()) doesn't work it always return an empty string
        return false; // driver reached destination and shouldn't go back
    }else{ //start appending the path
        auto iter1 = pathOne.begin();
        bool startAppend =false;
        auto iter2 = pathTwo.begin();
        
        while (iter2!= pathTwo.end() ) {
            sizeOfNewPath++;
            iter2 = pathTwo.begin() + sizeOfNewPath - 1 ;
            if (*iter2 == appendNode) {
                //the insert method reallocate another space inthe memory for the vector each time causing the bad access code
                auto linkerPath = shortestPath(appendNode,pathOne[0] );
                //remove empty strings from linkerPath
                for (auto i = linkerPath.begin(); i != linkerPath.end(); i++) {
                    if (*i == "")
                        linkerPath.erase(i);
                }
                //insert the content of linker path into pathTwo
                auto l = linkerPath.begin() +1 ;
                while (l != linkerPath.end() -1 && *(l-1) != appendNode && l != linkerPath.end()) {
                    pathTwo.insert(++iter2, *l);
                    sizeOfNewPath++;
                    iter2 = pathTwo.begin() + sizeOfNewPath - 1;
                    l++;
                }
                if (*iter1 == appendNode){
                    iter1++;
                    //continue;
                }
                pathTwo.insert(++iter2,*iter1 );
                startAppend =true;
            }else if (startAppend){
                iter1++;
                iter2++;
                pathTwo.insert(iter2, *iter1);
            }
            if (iter1 == pathOne.end()) {
                break;
            }
            iter2 = pathTwo.begin() + sizeOfNewPath ;
            //iter2++;
        }
        //now the driver need to his initial destination
        //first we need to resize the path two to get rid of overflowwing nodes
        pathTwo.resize(sizeOfNewPath);
        if ( *(pathOne.end()) != lastVertex){
            auto pathTail = shortestPath(pathOne[pathOne.size() - 1],lastVertex);
            // remove empty strings from pathTail
            for (auto i = pathTail.begin() ; i != pathTail.end(); i++ ) {
                if (*i == "")
                    pathTail.erase(i);
            }
            if (pathTail.size()>1 ) {// if it's not then pathTail contain only one vertex witch is lastVertex
                for (auto i : pathTail)
                    if (i != pathTwo[pathTwo.size() -1]) {
                        pathTwo.push_back(i);
                    }
            }
        }
        
        return true;
    }
}
//#warning FIXME
//#error bug

//FIXME : test method
bool pathContainCriticalVertices(vector<string> & path , vector<string> & criticalVertices){
    
    bool newPath = true;
    auto pathIter = path.begin();
    for (auto i :criticalVertices) {
        
        if (newPath) { // then we will try to locate the critical vertex i which is the beginning of some demand . if we find it then proceed to next phase else return false
            while (pathIter != path.end()) {
                if (i != *pathIter)
                    pathIter++;
                else
                    break;
            }
           
            if (i!= *pathIter) // pathIter reached the end of path witout finding a match
                return false;
        }else{//then we will try to locate the critical vertex i which is the end of some demand from the last pathIter position otherwise it doesn't make sense
            while (pathIter != path.end()) {
                if (i != *pathIter)
                    pathIter++;
                else
                    break;
            }
           if (i!= *pathIter)
               return false;
            else
                pathIter = path.begin(); // reinit pathIter for the next demand lookup
        }
        newPath = !newPath;
    }
    return true; // found all critical vertices
}
void Graph::tryToSatisfyAllDemands(const char* outputfilename ){
    //FIXME : implement method
    auto demandcopy = demands ;
    int deletionIndex = -1 ;
    auto offerCopy = offers ;
    bool offerSatisfied;
    outputfile.open(outputfilename ,fstream::app); //FIXME: when done testing add fstream::app flag to open file i append mode
    for (auto i = offerCopy.begin() ; i != offerCopy.end(); i++) {
        for(auto j = i->second.begin() ;j != i->second.end() ;j++){ // adjacency list vertex-Offer
            offerSatisfied = false;
            vector<string> offerPath;
           // offerPath.resize(0);
            offerPath=shortestPath(i->first , j->vertexName );
            if (offerPath.size() < 2 ) { //there is no path
                cout<< "impossible path"<<endl; //FIXME: problem with size calculation here, body of the if statement never executed witch makes it a dead code
                outputfile<< endl<<"impossible path";
                continue;
            }else{
                //then we need to check the demands
                vector<string> criticalVertices; 
                for (auto k = demandcopy.begin() ; k != demandcopy.end(); k++ ) {
                    deletionIndex = -1 ;
                    for ( auto l = k->second.begin() ; l != k->second.end() ; l++) { //adjacency list vertex-demand
                        criticalVertices.push_back(k->first); // eventual duplications are needed here because function pathContainCriticalVertices need a pair set of vertices in in the vector criticalVertices. each pair represent the beginning and the end of demand path
                        criticalVertices.push_back(*(l));

                        vector<string> demandPath ;
                        vector<string> offerPathCopy = offerPath;
                        demandPath.resize(0);
                        demandPath = shortestPath(k->first, *(l));
                        deletionIndex++;
                        //FIXME: we need to check the critical paths demandPath[0] and demandPath[demandPath.size()-1] for each demand
                        //FIXME: add a function to check if critical vertices are in offerpathcopy
                        if (j->var >0 && appendPathOneToPathTwo(demandPath, offerPathCopy) && pathContainCriticalVertices(offerPathCopy, criticalVertices)) {
                            cout << "the offer : "<<i->first << " ==> "<< j->vertexName << " picked up the demand : "<< k->first <<" --> "<< *l <<endl;
                            outputfile<<"the offer : "<<i->first << " ==> "<< j->vertexName << " picked up the demand : "<< k->first <<" --> "<< *l <<endl;
                            cout << "old offer path is :";
                            outputfile<< "old offer path is :";
                            printVector(offerPath); // fixme : write vector to file;
                            cout << "new offer path is : ";
                            outputfile<<"new offer path is : ";
                            printVector(offerPathCopy);// fixme : write vector to file;
                            offerPath = offerPathCopy ;
                            k->second.erase(k->second.begin() + deletionIndex);
                            l--;
                            j->var--;
                        }else if (j->var == 0) { // then offer is satisfied
                            break;
                        }else if (pathContainCriticalVertices(offerPathCopy, criticalVertices) == false){ // the current demand stashed the previous satisfied demands so what we need to do is leave it until maybe next offer can take care of it
                            
                            
                            
                        }else{ // then the problem is with the append
                            
                        }
                    }
                    if (j->var == 0) { // then offer is satisfied
                        cout<< "offer : "<< i->first << " ==> "<< j->vertexName <<" satisfied the folloawing demands : "<<endl;
                        outputfile<<"offer : "<< i->first << " ==> "<< j->vertexName <<" satisfied the folloawing demands : "<<endl;
                        // display satisfied demands that we can find in criticalVertices
                        bool newDemand = true;
                        for (auto i : criticalVertices) {
                            if(newDemand){
                                cout<< i;
                            outputfile<<i;
                            }else{
                                cout<< " --> "<< i <<endl;
                                outputfile<<" --> "<< i <<endl;
                            }
                            newDemand = !newDemand;
                        }
                        cout<< "original path : ";
                        outputfile<<"original path : ";
                        auto originalPath = shortestPath(i->first,j->vertexName );
                        printVector(originalPath); // fixme : write vector to file
                        cout << "new path : ";
                        outputfile<<"new path : ";
                        printVector(offerPath); // offerPath which contain latest path with no stashed demands
                        // fixme : write vector to file
                        i->second.erase(j);
                        j--;
                        break;
                    }
                }
            }
            
        }
        
        
        
        
        if (i->second.size() == 0){ //
            offerCopy.erase(i);
            i--;
        }
        
    }
    // report all unsatisfied demands:
    if (demandcopy.size()>0) { // then print unsatisfied demands
        cout << " ****************"<<endl;
        cout << "unsatisfied demands are : "<<endl;
        outputfile<<" ****************"<<endl;
        outputfile<<"unsatisfied demands are : "<<endl;
        for (auto i : demandcopy) {
            for (auto j : i.second ) {
                cout << i.first <<" --> "<<j <<endl;
                outputfile<<i.first <<" --> "<<j <<endl;
            }
        }
    }else{
        cout << "All demand successfully satisfied "<<endl;
        outputfile<<"All demand successfully satisfied "<<endl;
    }
    
    outputfile.close();
}
void Graph::tryToSatisfyDemandsKeepingTheShortestPath(const char* outputfilename ){

    outputfile.open(outputfilename,fstream::app);//FIXME: when done testing add fstream::app flag to open file i append mode
    auto offercopy = offers ; // must make a copy because data might be changed
    bool demandSatisfied ;
    for (  auto i : demands) {
        //the demand list is similar to adjacency list of the graph
        //iterate over the destinations
        for (auto h : i.second) {
            demandSatisfied =false;
            //FIXME : need to implement a move asign to reinitialize the size back to zero otherwise the subsequent check of the demandPath 's size doesn't make sense
            vector<string> demandPath;
            demandPath.resize(0);
            demandPath= shortestPath(i.first , h );
            if (demandPath.size() < 2 ) { //there is no path
                cout<< "impossible path"<<endl; //FIXME: problem with size calculation here
                outputfile<< endl<<"impossible path";
                continue;
            }else{
                //then we need to check the offers
                
                for (auto j : offercopy) {
                    //iterating over the adjacency list of offers
                    for (auto k = j.second.begin() ; k != j.second.end() ; k++ ) {
                        vector<string> offerPath ;
                        offerPath.resize(0);
                        offerPath= shortestPath(j.first, k->vertexName);
                        bool  pathOnacontainpathtow=pathOneContainPathTow(offerPath,demandPath );
                        if (k->var >0 && pathOnacontainpathtow) {
                            // demand was satisfied
                            --k->var; //decrement the number of availeble seats for the offer
                            demandSatisfied = true ;
                            cout<< endl<<"demand satisfied"<<endl;
                            cout<< "demand : " <<i.first <<"->"<< h<<endl;
                            cout<< "satisfied by offer : "<< j.first << "->"<< k->vertexName << k->var<<endl;
                            //write result to a file
                            outputfile<<endl<< "demand satisfied"<<endl;
                            outputfile<<"demand : " <<i.first <<"->"<< h<<endl;
                            outputfile<<"satisfied by offer : "<< j.first << "->"<< k->vertexName << k->var<<endl;
                            break;
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
                outputfile<<"demand can't be satisfied "<< i.first << " -> " << h<<endl;
               
            }
            cout<<endl<<"*************"<<endl;
             outputfile<<endl<<"*************"<<endl;
        }
    }
    outputfile.close();
}
bool compareString (const string &a ,const string &b){
    return a < b;
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
