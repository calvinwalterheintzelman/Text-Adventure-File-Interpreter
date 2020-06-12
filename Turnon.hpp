//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_TURNON_HPP
#define JOSHPROJECT_TURNON_HPP
#include <string>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

class Turnon {
public:
    std::vector<std::string> print;
    std::vector<std::string> action;
    //bool isTurnOn;
    explicit Turnon(rapidxml::xml_node<>* node){init_Turnon(node);}
    void init_Turnon(rapidxml::xml_node<>* turnon)
    {
        //isTurnOn = false;
        for(rapidxml::xml_node <> * child = turnon -> first_node(); child; child = child -> next_sibling()) {
            if(std::string(child->name()) == "print") {
                print.push_back(child->value());
            }
            if(std::string(child->name()) == "action") {
                action.push_back(child->value());
            }

        }
    }

};
#endif //JOSHPROJECT_TURNON_HPP
