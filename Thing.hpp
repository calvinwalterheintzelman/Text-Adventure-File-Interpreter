//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_THING_H
#define JOSHPROJECT_THING_H

#include "Trigger.hpp"
class Thing {
public:
    std::string name;
    std::string status;
    std::string description;
    std::vector<Trigger *> trigger;
    Thing(){}
};


#endif //JOSHPROJECT_THING_H
