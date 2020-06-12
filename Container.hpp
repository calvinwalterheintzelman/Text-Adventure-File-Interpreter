//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_CONTAINER_H
#define JOSHPROJECT_CONTAINER_H

#include "Thing.hpp"
#include "Item.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

using namespace rapidxml;
class Container : public Thing {
public:
    std::vector<Item * > item;
    std::vector<std::string> accept;
    bool isOpen;
    bool locked;
    explicit Container(xml_node<>* node, std::vector<Item *> item){init_container(node, item);}
    void init_container(xml_node<>* container, std::vector<Item *> item_vec)
    {
        isOpen = false;
        for(rapidxml::xml_node <> * child = container -> first_node(); child; child = child -> next_sibling()){
            if(std::string(child->name()) == "name") {
                name = child->value();
            }
            if(std::string(child->name()) == "status") {
                status = child->value();
            }
            if(std::string(child->name()) == "description") {
                description = child->value();
            }
            if(std::string(child->name()) == "item") {

                for(auto it : item_vec) {
                    if(it->name == child->value()) {
                        item.push_back(it);
                    }
                }

            }
            if(std::string(child->name()) == "accept") {
                accept.push_back(child->value());
            }
            if (std::string(child->name()) == "trigger"){
                Trigger * trig = new Trigger(child);
                trigger.push_back(trig);
            }
        }
        if(accept.empty()) {
            locked = false;
        }
        else {
            locked = true;
        }
    }
};
#endif //JOSHPROJECT_CONTAINER_H
