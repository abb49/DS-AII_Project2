#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

class Customer {
    double arrivalTime;
    double startOfServiceTime;
    double departureTime;
    double pqTime;
    Customer* nextCust; 
};

#endif