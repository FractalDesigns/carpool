//
//  main.cpp
//  projet d'été
//
//  Created by Achraf EL AFRIT on 6/28/15.
//  Copyright (c) 2015 Achraf EL AFRIT. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include "Graph.h"
using namespace std;


void printVector(const vector<string> &a){
    cout<<endl;
    for (auto i : a) {
        cout << i <<" ";
    }
    cout<<endl;
    
}


int main(int argc, const char * argv[])
{
    Graph carPoolingGraph("test.txt");
    //carPoolingGraph.showGraphContent();
    //carPoolingGraph.printVerticesSet();

    carPoolingGraph.tryToSatisfyOffersKeepingTheShortestPath();
    return EXIT_SUCCESS;
}
