//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_CREATURE_H
#define JOSHPROJECT_CREATURE_H
#include "Thing.hpp"
#include "Attack.hpp"
#include "Trigger.hpp"
#include <vector>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

class Creature : public Thing {
public:
    std::vector<std::string> vulnerability;
    Attack * attack;

    explicit Creature(rapidxml::xml_node<>* node){init_Creature(node);}
    void init_Creature(rapidxml::xml_node<>* creature)
    {
        for(rapidxml::xml_node <> * child =  creature -> first_node(); child; child = child -> next_sibling()) {
            if(std::string(child->name()) == "name"){
                name = child->value();
            }
            if(std::string(child->name()) == "status"){
                status = child->value();
            }
            if(std::string(child->name()) == "description"){
                description = child->value();
            }
            if(std::string(child->name()) == "vulnerability"){
                vulnerability.push_back(child->value());
            }
            if(std::string(child->name()) == "attack"){
                attack = new Attack(child);
            }
            if(std::string(child->name()) == "trigger")
            {
                Trigger * trig = new Trigger(child);
                trigger.push_back(trig);
            }
        }
    }

};
#endif //JOSHPROJECT_CREATURE_H
