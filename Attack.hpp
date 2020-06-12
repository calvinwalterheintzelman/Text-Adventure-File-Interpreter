//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_ATTACK_H
#define JOSHPROJECT_ATTACK_H

#include "Condition.hpp"
#include <vector>
#include <string>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
class Attack {
public:
    std::vector<Condition *> condition;
    std::vector<std::string> print;
    std::vector<std::string> action;

    Attack(rapidxml::xml_node<> * node){init_Attack(node);};
    void init_Attack(rapidxml::xml_node<> * att)
    {
        for(rapidxml::xml_node <> * child = att -> first_node(); child; child = child -> next_sibling()) {
            if(std::string(child->name()) == "print"){
                print.push_back(child->value());
            }
            if(std::string(child->name()) == "action"){
                action.push_back(child->value());
            }
            if(std::string(child->name()) == "condition"){
                Condition * con_temp = new Condition(child);
                condition.push_back(con_temp);
            }
        }
    }
};
#endif //JOSHPROJECT_ATTACK_H
