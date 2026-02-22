#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

class Customer {
    public: 
    double arrivalTime;
    double startOfServiceTime;
    double departureTime;
    double pqTime;
    Customer* nextCust; 
 
    Customer(double time) {
        arrivalTime = time;
        startOfServiceTime = 0;
        departureTime = 0;
        pqTime = arrivalTime;
    }

    void setArrival(double time) {
        arrivalTime = time;
    }

    void setStartService(double time) {
        startOfServiceTime = time;
    }

    void setDepartureTime(double time) {
        departureTime = time;
    }

    void setPqTime(double time) {
        pqTime = time;
    }

    double getArrival() {
        return arrivalTime;
    }

    double getPq() {
        return pqTime;
    }

    double getDeparture() {
        return departureTime;
    }

    double getStartofService() {
        return startOfServiceTime;
    }

    bool operator>(const Customer& other) const {
        return this->pqTime > other.pqTime;
    }
};

#endif