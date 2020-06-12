//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_ITEM_H
#define JOSHPROJECT_ITEM_H

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"


#include <string>
#include "Turnon.hpp"
class Item : public Thing {
public:
    std::string writing;
    Turnon * turnon;  // has print and action

    explicit Item(rapidxml::xml_node<>* node) {init_Item(node);}

    void init_Item(rapidxml::xml_node<>* item)
    {
        for(rapidxml::xml_node <> * child = item -> first_node(); child; child = child -> next_sibling()) {
            if(std::string(child->name()) == "name"){
                name = child->value();
            }
            if(std::string(child->name()) == "status"){
                status = child->value();
            }
            if(std::string(child->name()) == "description"){
                description = child->value();
            }
            if(std::string(child->name()) == "writing"){
                writing = child->value();
            }
            if(std::string(child->name()) == "turnon"){
                turnon = new Turnon(child);
            }
            if(std::string(child->name()) == "trigger"){
                Trigger * trig = new Trigger(child);
                trigger.push_back(trig);

            }
        }
    }

};
#endif //JOSHPROJECT_ITEM_H
