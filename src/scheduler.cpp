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
    for(uint8_t j = 0; j < levelLen; j++) {
        if(!scheduler[0][j]){
            scheduler[0][j] = program;
            Serial.println("Pushed new function to top of scheduler");
            return;
        }
    }
    Serial.println("ERROR: L0 of the scheduler is full");   
}

void Scheduler::push(Program* program, const uint8_t & level){
    for(uint8_t j = 0; j < levelLen; j++) {
        if(!scheduler[level][j]){
            scheduler[level][j] = program;
            return;
        }
    }
    Serial.print("ERROR: L");
    Serial.print(level);
    Serial.println(" of the scheduler is full");
}

void Scheduler::remove(Program* program) {
    for(uint8_t i = 0; i < schedulerLevels; i++) {
        for(uint8_t j = 0; j < levelLen; j++) {
            if(scheduler[i][j] == program) {
                scheduler[i][j] = nullptr;
                delete program;
                program = nullptr;
            }
        }
    }
}

Program* Scheduler::getNext(const uint8_t & level) {
    Program* found = nullptr;
    uint8_t firstReadyIndex = 0;
    do {
        found = scheduler[level][firstReadyIndex];
        firstReadyIndex++;
    } while(firstReadyIndex < levelLen && found && !(found->isReady()));
    //firstReadyIndex is one greater than its actual index after this point
    if(firstReadyIndex == levelLen && !(found->isReady())) {
        Serial.println("ERROR: Scheduler level is filled with waiting processes");
        return nullptr;
    }
    if(!found) {
        return nullptr;
    }
    for(uint8_t i = firstReadyIndex; i < levelLen; i++){
        scheduler[level][i-1] = scheduler[level][i];
    }
    scheduler[level][19] = nullptr;
    return found;
}

Program* Scheduler::find(const uint8_t & id) const{
    for(uint8_t i = 0; i < schedulerLevels; i++) {
        for(uint8_t j = 0; j < levelLen; j++) {
            if(scheduler[i][j] && scheduler[i][j]->getId() == id)
                return scheduler[i][j];
        }
    }
    Serial.println("ERROR: cannot find program in scheduler.find()");
    return nullptr;
}