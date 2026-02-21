#include "Customer.hpp"
#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<queue>
#include<cmath>

using namespace std;

double getInterval(int avg) {
    double r = 0.0;
    while(r == 0.0) { //to make sure r is not 0
        r = static_cast <double> (rand()) / (RAND_MAX);
    }
    double interval = ((1.0/avg)*(-1)) * (log(r));
    return interval;
}



int main() {
    queue<Customer> waiting;
    priority_queue<Customer, vector<Customer>, greater<double>> events;
    int lambda;
    int mu;
    int M;
    int eventCount;
    string currIn;

    //start with text1.txt
    ifstream iss("test1.txt");

    //check that the file is open
    if(!iss.is_open()){
        cout << "File could not be opened";
        return 1;
    }

    getline(iss, currIn);
    lambda = stoi(currIn);
    getline(iss, currIn);
    mu = stoi(currIn);
    getline(iss, currIn);
    M = stoi(currIn);
    getline(iss, currIn);
    eventCount = stoi(currIn);

    iss.close();

    //begin sim. Start by creating arrival events and inserting into pq 
    Customer currCustomer(getInterval(lambda));
    currCustomer.setDepartureTime(-1.0);
    events.push(currCustomer);
    while(!events.empty()) {
        if(currCustomer.getDeparture() < 0.0) { //is an arrival event
            if(M > 0) {
                --M;
                currCustomer.setStartService(currCustomer.getArrival());
                double interval = getInterval(mu);
                currCustomer.setDepartureTime(currCustomer.getArrival() + interval);
                currCustomer.setPqTime(currCustomer.getDeparture());
                events.push(currCustomer);
            } else {
                waiting.push(currCustomer);
            }
        } else { //departure event
            ++M;
            processStats();
            if(!waiting.empty()) {
                Customer nextCustomer = waiting.front();
                waiting.pop();
                nextCustomer.setStartService(currCustomer.getDeparture());
                double interval = getInterval(mu);
                nextCustomer.setDepartureTime(nextCustomer.getStartofService() + interval);
                events.push(nextCustomer);
                --M;
            } 
        }

    }


    
    return 0;
}