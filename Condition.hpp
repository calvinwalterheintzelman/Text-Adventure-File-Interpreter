//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_CONDITION_HPP
#define JOSHPROJECT_CONDITION_HPP

#include <string>
#include <vector>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
class Condition {
public:
    std::string has;
    std::string object;
    std::string owner;
    std::string status;

    Condition(rapidxml::xml_node<>* node){init_Condition(node);}
    void init_Condition(rapidxml::xml_node<>* cond){

        for(rapidxml::xml_node <> * child = cond -> first_node(); child; child = child -> next_sibling()){
            if(std::string(child->name()) == "has"){
                has = child->value();
            }
            if(std::string(child->name()) == "object"){
                object = child->value();
            }
            if(std::string(child->name()) == "owner"){
                owner = child->value();
            }
            if(std::string(child->name()) == "status"){
                status = child->value();
            }
        }

    }
};
#endif //JOSHPROJECT_CONDITION_HPP
