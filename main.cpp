/************************************
 * Student Name: Atiye Buker
 * File Name: main.cpp
 * Project 2
 * 
 * Purpose: run  the simulation
 ************************************/
#include "Customer.hpp"
#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<queue>
#include<cmath>
#include<ctime>
#include<cstdlib>


using namespace std;

//to get random intervals for arrival and service time
double getInterval(int avg) {
    double r = 0.0;
    while(r == 0.0) { //to make sure r is not 0
        r = static_cast <double> (rand()) / (RAND_MAX);
    }
    double interval = ((1.0/avg)*(-1)) * (log(r));
    return interval;
}

//factorial to get analytical values
long long factorial(int n) {
    if(n == 0 || n == 1) {
        return 1;
    }
    long long fact = 1;
    for(int i = 2; i <= n; i++) {
        fact = fact*i;
    }
    return fact;
}

//print out the analytical values for comparison
void printAnalytical(double lambda, double mu, double M) {
    double sum = 0;
    for(int i = 0; i < M; ++i) {
        sum += (1.0 / factorial(i)) * pow(lambda / mu , i);
    }

    double secondTerm = (1.0/factorial(M)) * pow(lambda/mu , M) * ((M * mu)/((M*mu) - lambda));
    double Po = 1.0 / (sum + secondTerm);

    double L = (((lambda * mu * pow(lambda/mu, M)) / (factorial(M - 1.0) * pow((M*mu) - lambda, 2))) * Po) + (lambda / mu);

    double W = L/lambda;

    double Lq = L - (lambda/mu);

    double Wq = Lq / lambda;

    double Rho = lambda/(M * mu);

    cout << fixed << setprecision(4);
    cout << "Analytical Values: " << endl;
    cout << "P0: " << Po << endl;
    cout << "L:  " << L << endl;
    cout << "W:  " << W << endl;
    cout << "Lq: " << Lq << endl;
    cout << "Wq: " << Wq << endl;
    cout << "Rho:" << Rho << endl << endl;
}


int main() {
    srand(time(NULL)); //makes random seed based on current time, making it more random

    queue<Customer> waiting; //create FIFO
    priority_queue<Customer, vector<Customer>, greater<Customer>> events; //create priority queue
    Customer currEvent(0.0);
    double lambda;
    double mu;
    double initialM;
    double M;
    int eventCount;
    string currIn;
    double timeWaited = 0.0;
    double timeService = 0.0;
    double idleTime = 0.0;

    //start with text1.txt
    ifstream iss("test1.txt");

    //check that the file is open
    if(!iss.is_open()){
        cout << "File could not be opened";
        return 1;
    }

    //read in all values
    getline(iss, currIn);
    lambda = stoi(currIn);
    getline(iss, currIn);
    mu = stoi(currIn);
    getline(iss, currIn);
    initialM = stoi(currIn);
    M = initialM;
    getline(iss, currIn);
    eventCount = stoi(currIn);

    int totalCustomer = eventCount/2; //(since every customer has two events (arrival and departure))

    iss.close();

    //begin sim. Start by creating arrival events and inserting into pq 
    double lastArrival = 0.0;
    for(int i = 0; i < 200; ++i) {
        Customer arrival(lastArrival + getInterval(lambda));
        arrival.setDepartureTime(-1.0);
        arrival.setPqTime(arrival.getArrival());
        events.push(arrival);
        lastArrival = arrival.getArrival();
    }

    int customersGenerated = 200;

    //actual event processing until queue is empty/we reach total customers
    while(!events.empty()) {
        currEvent = events.top();
        events.pop();

        if((events.size() <= M + 1) && (customersGenerated < totalCustomer)) {
            while((events.size() < 200) && (customersGenerated < totalCustomer)) { //check if we need more arrival events to be added
                Customer arrival(lastArrival + getInterval(lambda));
                arrival.setDepartureTime(-1.0);
                arrival.setPqTime(arrival.getArrival());
                events.push(arrival);
                lastArrival = arrival.getArrival();
                ++customersGenerated;
            }
        }

        if(currEvent.getDeparture() < 0.0) { //is an arrival event
            if(M > 0) { //servers available
                --M;
                currEvent.setStartService(currEvent.getArrival());
                double interval = getInterval(mu);
                currEvent.setDepartureTime(currEvent.getArrival() + interval);
                currEvent.setPqTime(currEvent.getDeparture());
                events.push(currEvent);
            } else { //not available, so needs to wait
                waiting.push(currEvent);
            }
        } else { //departure event
            ++M;
            timeWaited += (currEvent.getStartofService() - currEvent.getArrival());
            timeService += (currEvent.getDeparture() - currEvent.getStartofService());
            if(!waiting.empty()) { //people waiting
                Customer nextCustomer = waiting.front();
                waiting.pop();
                nextCustomer.setStartService(currEvent.getDeparture());
                double interval = getInterval(mu);
                nextCustomer.setDepartureTime(nextCustomer.getStartofService() + interval);
                nextCustomer.setPqTime(nextCustomer.getDeparture());
                events.push(nextCustomer);
                --M;
            } else {
                if(M == initialM){
                    if(!events.empty()) {
                        Customer nextArrival = events.top();
                        idleTime += (nextArrival.getArrival() - currEvent.getDeparture());
                    }
                }
            }
        }

    }

    //calculate simulation values
    double Po = idleTime / currEvent.getDeparture();
    double W = ((timeWaited + timeService)/customersGenerated);
    double Wq = timeWaited/customersGenerated;
    double Rho = timeService / (initialM * currEvent.getDeparture());

    cout << "Test 1:" << endl;
    printAnalytical(lambda, mu, initialM);
    cout << "Simulated Values: " << endl;
    cout << "P0: " << Po << endl;
    cout << "W:  " << W << endl;
    cout << "Wq: " << Wq << endl;
    cout << "Rho:" << Rho << endl << endl;


    // ------------------- TEST 2 ------------------- all the same stuff as test 1, just read from a different file
    //both queues should be empty, but we must clear out the values and read in the new averages

    timeWaited = 0.0;
    timeService = 0.0;
    idleTime = 0.0;

    //start with text1.txt
    ifstream iss2("test2.txt");

    //check that the file is open
    if(!iss2.is_open()){
        cout << "File could not be opened 2";
        return 1;
    }

    getline(iss2, currIn);
    lambda = stoi(currIn);
    getline(iss2, currIn);
    mu = stoi(currIn);
    getline(iss2, currIn);
    initialM = stoi(currIn);
    M = initialM;
    getline(iss2, currIn);
    eventCount = stoi(currIn);

    totalCustomer = eventCount/2;

    iss2.close();

    //begin sim. Start by creating arrival events and inserting into pq 
    lastArrival = 0.0;
    for(int i = 0; i < 200; ++i) {
        Customer arrival(lastArrival + getInterval(lambda));
        arrival.setDepartureTime(-1.0);
        arrival.setPqTime(arrival.getArrival());
        events.push(arrival);
        lastArrival = arrival.getArrival();
    }

    customersGenerated = 200;

    while(!events.empty()) {
        currEvent = events.top();
        events.pop();

        if((events.size() <= M + 1) && (customersGenerated < totalCustomer)) {
            while((events.size() < 200) && (customersGenerated < totalCustomer)) {
                Customer arrival(lastArrival + getInterval(lambda));
                arrival.setDepartureTime(-1.0);
                arrival.setPqTime(arrival.getArrival());
                events.push(arrival);
                lastArrival = arrival.getArrival();
                ++customersGenerated;
            }
        }

        if(currEvent.getDeparture() < 0.0) { //is an arrival event
            if(M > 0) {
                --M;
                currEvent.setStartService(currEvent.getArrival());
                double interval = getInterval(mu);
                currEvent.setDepartureTime(currEvent.getArrival() + interval);
                currEvent.setPqTime(currEvent.getDeparture());
                events.push(currEvent);
            } else {
                waiting.push(currEvent);
            }
        } else { //departure event
            ++M;
            timeWaited += (currEvent.getStartofService() - currEvent.getArrival());
            timeService += (currEvent.getDeparture() - currEvent.getStartofService());
            if(!waiting.empty()) {
                Customer nextCustomer = waiting.front();
                waiting.pop();
                nextCustomer.setStartService(currEvent.getDeparture());
                double interval = getInterval(mu);
                nextCustomer.setDepartureTime(nextCustomer.getStartofService() + interval);
                nextCustomer.setPqTime(nextCustomer.getDeparture());
                events.push(nextCustomer);
                --M;
            } else {
                if(M == initialM){
                    if(!events.empty()) {
                        Customer nextArrival = events.top();
                        idleTime += (nextArrival.getArrival() - currEvent.getDeparture());
                    }
                }
            }
        }

    }

    Po = idleTime / currEvent.getDeparture();
    W = ((timeWaited + timeService)/customersGenerated);
    Wq = timeWaited/customersGenerated;
    Rho = timeService / (initialM * currEvent.getDeparture());

    cout << "Test 2:" << endl;
    printAnalytical(lambda, mu, initialM);
    cout << "Simulated Values: " << endl;
    cout << "P0: " << Po << endl;
    cout << "W:  " << W << endl;
    cout << "Wq: " << Wq << endl;
    cout << "Rho:" << Rho << endl << endl;

    
    return 0;
}