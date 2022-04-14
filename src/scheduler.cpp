#include "scheduler.h"

Scheduler::Scheduler(){
    for(uint8_t i = 0; i < 7; i++) {
        for(uint8_t j = 0; j < 20; j++) {
            scheduler[i][j] = nullptr;
        }
    }
}

// Scheduler::~Scheduler(){
//     for(uint8_t i = 0; i < 7; i++) {
//         for(uint8_t j = 0; j < 20; j++) {
//             delete scheduler[i][j];
//         }
//     }
// }

void Scheduler::push(Program* program){
    for(uint8_t j = 0; j < 20; j++) {
        if(!scheduler[0][j]){
            scheduler[0][j] = program;
            return;
        }
    }
    Serial.println("ERROR: L0 of the scheduler is full");   
}

void Scheduler::push(Program* program, const uint8_t & level){
    for(uint8_t j = 0; j < 20; j++) {
        if(!scheduler[level][j]){
            scheduler[level][j] = program;
            return;
        }
    }
    Serial.print("ERROR: L");
    Serial.print(level);
    Serial.println(" of the scheduler is full");
}

Program* Scheduler::getNext(const uint8_t & level) {
    Program* found = nullptr;
    uint8_t firstReadyIndex = 0;
    do {
        found = scheduler[level][firstReadyIndex];
        firstReadyIndex++;
    } while(firstReadyIndex < levelLen && found && !found->isReady());
    if(firstReadyIndex == levelLen && !found->isReady()) {
        Serial.println("ERROR: Scheduler level is filled with waiting processes");
        return nullptr;
    }
    if(!found)
        return nullptr;
    for(uint8_t i = firstReadyIndex + 1; i < 20; i++){
        scheduler[level][i-1] = scheduler[level][i];
    }
    scheduler[level][19] = nullptr;
    return found;
}

Program* Scheduler::find(const uint16_t & id) const{
    for(uint8_t i = 0; i < 7; i++) {
        for(uint8_t j = 0; j < 20; j++) {
            if(scheduler[i][j]->getId() == id)
                return scheduler[i][j];
        }
    }
    Serial.println("ERROR: cannot find program in scheduler.find()");
    return nullptr;
}