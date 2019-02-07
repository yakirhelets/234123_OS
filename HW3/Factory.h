#ifndef FACTORY_H_
#define FACTORY_H_

#include <pthread.h>
#include <list>
#include "Product.h"
#include <iostream>
#include <map>
#include <stdlib.h>
#include <unistd.h>

class Factory{
public:
    Factory();
    ~Factory();
    
    void startProduction(int num_products, Product* products, unsigned int id);
    void produce(int num_products, Product* products);
    static void* threadStartProductionFunc (void*); //HW3
    void finishProduction(unsigned int id);
    
    void startSimpleBuyer(unsigned int id);
    int tryBuyOne();
    static void* threadTryBuyOne(void*); //HW3
    int finishSimpleBuyer(unsigned int id);

    void startCompanyBuyer(int num_products, int min_value,unsigned int id);
    std::list<Product> buyProducts(int num_products);
    void returnProducts(std::list<Product> products,unsigned int id);
    static void* threadStartCompanyBuyer(void*); //HW3 
    int finishCompanyBuyer(unsigned int id);

    void startThief(int num_products,unsigned int fake_id);
    int stealProducts(int num_products,unsigned int fake_id);
    static void* threadStartThief(void*); //HW3
    int finishThief(unsigned int fake_id);

    void closeFactory();
    void openFactory();
    
    void closeReturningService();
    void openReturningService();
    
    std::list<std::pair<Product, int>> listStolenProducts();
    std::list<Product> listAvailableProducts();

protected:
//HW3
    std::map<unsigned int, pthread_t> mapingIds;
    pthread_mutex_t factoryLock;
    pthread_cond_t factoryCond;
    int isOpen;
    int isReturnOpen;
    int count_thieves;
    std::list<Product> availableProducts;
    std::list<std::pair<Product, int>> stolenProducts;


};
#endif // FACTORY_H_
