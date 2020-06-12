//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_TRIGGER_HPP
#define JOSHPROJECT_TRIGGER_HPP

#include <vector>
#include <string>
#include "Condition.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
class Trigger {
public:
    std::string type;
    std::string command;
    std::vector<Condition *> condition;
    std::vector<std::string> print;
    std::vector<std::string> action;

    Trigger(rapidxml::xml_node<>* node){init_Trigger(node);}
    void init_Trigger(rapidxml::xml_node <> * trig)
    {
        type = "single";
        for(rapidxml::xml_node <> * child =  trig -> first_node(); child; child = child -> next_sibling()){
            if(std::string(child->name()) == "type") {
                type = child->value();
            }
            if(std::string(child->name()) == "command"){
                command = child->value();
            }
            if(std::string(child->name()) == "print"){
                print.push_back(child->value());
            }
            if(std::string(child->name()) == "action"){
                action.push_back(child->value());
            }
            if(std::string(child->name()) == "condition"){
                Condition * cond = new Condition(child);
                condition.push_back(cond);

            }
        }
    }
};
#endif //JOSHPROJECT_TRIGGER_HPP
