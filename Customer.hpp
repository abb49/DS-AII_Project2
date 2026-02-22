/************************************
 * Student Name: Atiye Buker
 * File Name: Customers.hpp
 * Project 2
 * 
 * Purpose: declare and define the class customer which will be the objects in the queues
 ************************************/
#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

class Customer {
    public: 
    double arrivalTime;
    double startOfServiceTime;
    double departureTime;
    double pqTime;
    Customer* nextCust; 
 
    Customer(double time) { //initial constructor
        arrivalTime = time;
        startOfServiceTime = 0;
        departureTime = 0;
        pqTime = arrivalTime;
    }

    void setArrival(double time) { //to set the arrival time
        arrivalTime = time;
    }

    void setStartService(double time) { //to set the start of service time
        startOfServiceTime = time;
    }

    void setDepartureTime(double time) { //to set the departure time
        departureTime = time;
    }

    void setPqTime(double time) { //to set the pq time (either arrival or departure)
        pqTime = time;
    }

    double getArrival() { //get arrival time
        return arrivalTime;
    }

    double getPq() { //get pq time
        return pqTime;
    }

    double getDeparture() { //get departure time
        return departureTime;
    }

    double getStartofService() { //get start of service time
        return startOfServiceTime;
    }

    bool operator>(const Customer& other) const { //operator to be able to compare pq times for priority queue
        return this->pqTime > other.pqTime;
    }
};

#endif