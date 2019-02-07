#include "Factory.h"

int init_mutex(pthread_mutex_t* m) {
    pthread_mutexattr_t a;
	pthread_mutexattr_init(&a);
	pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK_NP);
    return pthread_mutex_init(m, &a);
}


Factory::Factory(){
    this->isOpen = 1;
    this->isReturnOpen = 1;
    this->count_thieves = 0;
    init_mutex(&(this->factoryLock));
    pthread_cond_init(&(this->factoryCond), NULL);
}

Factory::~Factory(){
    this->isOpen = 0;
    this->isReturnOpen = 0;
    this->count_thieves = 0;
    pthread_mutex_destroy(&(this->factoryLock));
    pthread_cond_destroy(&(this->factoryCond));
    this->availableProducts.clear();
    this->stolenProducts.clear();
}


typedef struct productionParams {
    int num_products;
    Product* products;
    Factory* factory;
} productionParams;

void* Factory::threadStartProductionFunc(void* args) {
    productionParams* params = (productionParams*) args;
    params->factory->produce(params->num_products, params->products);
    delete params;
    return NULL;
}

void Factory::startProduction(int num_products, Product* products,unsigned int id){
    pthread_t threadId;
    productionParams* params = new productionParams();
    params->num_products = num_products;
    params->products = products;
    params->factory = this;
    pthread_create(&threadId, NULL, Factory::threadStartProductionFunc, (void*)params);
    this->mapingIds.insert(std::pair<unsigned int, pthread_t>(id, threadId));
}

void Factory::produce(int num_products, Product* products){
    pthread_mutex_lock(&(this->factoryLock));

    for (int i=0; i<num_products; i++) {
        this->availableProducts.push_back(products[i]);
    }
    pthread_cond_broadcast(&(this->factoryCond));
    pthread_mutex_unlock(&(this->factoryLock));
}

void Factory::finishProduction(unsigned int id){
    pthread_join((this->mapingIds.find(id)->second), NULL);
    this->mapingIds.erase(id);
}




void* Factory::threadTryBuyOne(void* args) {
    Factory* factory = (Factory*) args;
    int* res = new int (factory->tryBuyOne());
    return (void*)res;
}

void Factory::startSimpleBuyer(unsigned int id){
    pthread_t threadId;
    pthread_create(&threadId, NULL, Factory::threadTryBuyOne, this);
    this->mapingIds.insert(std::pair<unsigned int, pthread_t>(id, threadId));
}

int Factory::tryBuyOne(){
    if (pthread_mutex_trylock(&(this->factoryLock)) == 0) {
        if (isOpen) {
            if (availableProducts.size() == 0) {
                pthread_mutex_unlock(&(this->factoryLock));
                return -1;
            }
            int product_id = (availableProducts.front()).getId();
            availableProducts.pop_front();
            pthread_mutex_unlock(&(this->factoryLock));
            return product_id;
        }
        pthread_mutex_unlock(&(this->factoryLock));
    }
    return -1;
}

int Factory::finishSimpleBuyer(unsigned int id){
    pthread_t threadId = this->mapingIds.find(id)->second;
    this->mapingIds.erase(id);
    void* res;
    pthread_join(threadId, &res);
    int final_res = *((int*)res);
    delete (int*)res;
    return final_res;
}





typedef struct companyParams {
    int num_products;
    int min_value;
    Factory* factory;
    unsigned int id;
} companyParams;

void* Factory::threadStartCompanyBuyer(void* args) {
    companyParams* params = (companyParams*) args;
    std::list<Product> products = params->factory->buyProducts(params->num_products);

    std::list<Product> returnProductsList;
    for (std::list<Product>::iterator it = products.begin(); it != products.end(); ++it){
        if ((*it).getValue() < params->min_value) {
            returnProductsList.push_back(*it);
        }
    }
    products.clear();
    int* count = new int (returnProductsList.size());
    params->factory->returnProducts(returnProductsList, params->id);
    delete params;
    return (void*)count;
}

void Factory::startCompanyBuyer(int num_products, int min_value,unsigned int id){
    pthread_t threadId;
    companyParams* params = new companyParams();
    params->num_products = num_products;
    params->min_value = min_value;
    params->factory = this;
    pthread_create(&threadId, NULL, Factory::threadStartCompanyBuyer, params);
    this->mapingIds.insert(std::pair<unsigned int, pthread_t>(id, threadId));
}

std::list<Product> Factory::buyProducts(int num_products){
    pthread_mutex_lock(&(this->factoryLock));

    while (!isOpen || this->availableProducts.size() < num_products || count_thieves) {
        pthread_cond_wait(&(this->factoryCond), &(this->factoryLock));
    }

    std::list<Product> produtsList;
    for (int i=0; i<num_products; i++) {
        produtsList.push_back(this->availableProducts.front());
        this->availableProducts.pop_front();
    }

    pthread_mutex_unlock(&(this->factoryLock));
    return produtsList;
}

void Factory::returnProducts(std::list<Product> products,unsigned int id){
    if (products.size() == 0) {
        return;
    }
    
    pthread_mutex_lock(&(this->factoryLock));

    while (!isOpen || !isReturnOpen || count_thieves) {
        pthread_cond_wait(&(this->factoryCond), &(this->factoryLock));
    }

    for (std::list<Product>::iterator it = products.begin(); it != products.end(); ++it){
        this->availableProducts.push_back(*it);
    } 

    pthread_cond_broadcast(&(this->factoryCond));
    pthread_mutex_unlock(&(this->factoryLock));
}

int Factory::finishCompanyBuyer(unsigned int id){
    return finishSimpleBuyer(id);
}



typedef struct thiefParams {
    int num_products;
    unsigned int fake_id;
    Factory* factory;
} thiefParams;

void* Factory::threadStartThief(void* args) {
    thiefParams* params = (thiefParams*) args;
    int* res = new int (params->factory->stealProducts(params->num_products, params->fake_id));
    delete params;
    return (void*)res;
}

void Factory::startThief(int num_products,unsigned int fake_id){
    pthread_mutex_lock(&(this->factoryLock));
    this->count_thieves++;
    pthread_mutex_unlock(&(this->factoryLock));
    pthread_t threadId;
    thiefParams* params = new thiefParams();
    params->num_products = num_products;
    params->fake_id = fake_id;
    params->factory = this;
    pthread_create(&threadId, NULL, Factory::threadStartThief, params);
    this->mapingIds.insert(std::pair<unsigned int, pthread_t>(fake_id, threadId));
}

int Factory::stealProducts(int num_products,unsigned int fake_id){
    pthread_mutex_lock(&(this->factoryLock));
    while (!isOpen) {
        pthread_cond_wait(&(this->factoryCond), &(this->factoryLock));
    }

    int count = 0;
    for (int i=0; i<num_products; i++) {
        if (this->availableProducts.empty()) {
            break;
        }
        this->stolenProducts.push_back(std::pair<Product, unsigned int>(this->availableProducts.front(), fake_id));
        count++;
        this->availableProducts.pop_front();
    }
    this->count_thieves--;
    if (this->count_thieves == 0) {
        pthread_cond_broadcast(&(this->factoryCond));
    }
    pthread_mutex_unlock(&(this->factoryLock));
    return count;
}

int Factory::finishThief(unsigned int fake_id){
    return finishSimpleBuyer(fake_id);
}


void Factory::closeFactory(){
    pthread_mutex_lock(&(this->factoryLock));
    this->isOpen = 0;
    pthread_mutex_unlock(&(this->factoryLock));
}

void Factory::openFactory(){
    pthread_mutex_lock(&(this->factoryLock));
    this->isOpen = 1;
    pthread_cond_broadcast(&(this->factoryCond));
    pthread_mutex_unlock(&(this->factoryLock));
}

void Factory::closeReturningService(){
    pthread_mutex_lock(&(this->factoryLock));
    this->isReturnOpen = 0;
    pthread_mutex_unlock(&(this->factoryLock));
}

void Factory::openReturningService(){
    pthread_mutex_lock(&(this->factoryLock));
    this->isReturnOpen = 1;
    pthread_cond_broadcast(&(this->factoryCond));
    pthread_mutex_unlock(&(this->factoryLock));
}

std::list<std::pair<Product, int>> Factory::listStolenProducts(){
    pthread_mutex_lock(&(this->factoryLock));
    std::list<std::pair<Product, int>> res = this->stolenProducts;
    pthread_mutex_unlock(&(this->factoryLock));
    return res;
}

std::list<Product> Factory::listAvailableProducts(){
    pthread_mutex_lock(&(this->factoryLock));
    std::list<Product> res = this->availableProducts;
    pthread_mutex_unlock(&(this->factoryLock));
    return res;
}
