//
// Created by Joshua Brard on 2019-03-25.
//

#ifndef JOSHPROJECT_ROOM_H
#define JOSHPROJECT_ROOM_H

#include <string>
#include <vector>

#include "Thing.hpp"
#include "Container.hpp"
#include "Creature.hpp"
#include "Item.hpp"
#include "Border.hpp"
#include "Trigger.hpp"

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"


class Room : public Thing {
public:
    std::string type;
    std::vector<Border * > border;
    std::vector<Container * > container;
    std::vector<Item * > item;
    std::vector<Creature * > creature;

    Room(rapidxml::xml_node<>* node, std::vector<Item *> item,
            std::vector<Container *> container, std::vector<Creature *> creature)
    {
        init_Room(node, item, container, creature);
    }

    void init_Room(rapidxml::xml_node<>* room, std::vector<Item *> item_vec,
            std::vector<Container *> container_vec, std::vector<Creature *> creature_vec) {
        // run through the nodes and store information
        for(rapidxml::xml_node <> * child = room -> first_node(); child; child = child -> next_sibling()) {
            if (std::string(child->name()) == "name") {
                name = child->value();
            }
            if (std::string(child->name()) == "status") {
                status = child->value();
            }
            if (std::string(child->name()) == "type") {
                type = child->value();
            }
            if (std::string(child->name()) == "description") {
                description = child->value();
            }
            if (std::string(child->name()) == "trigger"){
                Trigger * trig = new Trigger(child);
                trigger.push_back(trig);
            }
            if (std::string(child->name()) == "item") {
                for(auto it : item_vec)
                {
                    if(it->name == child->value())
                    {
                        item.push_back(it);
                    }
                }
            }
            if (std::string(child->name()) == "container") {
                for(auto con : container_vec)
                {
                    if(con->name == child->value())
                    {
                        container.push_back(con);
                    }
                }
            }
            if (std::string(child->name()) == "creature") {
                for(auto creat : creature_vec)
                {
                    if(creat->name == child->value())
                    {
                        creature.push_back(creat);
                    }
                }
            }
            if (std::string(child->name()) == "border") {
                Border *borderNew = new Border();
                std::string borderName;
                std::string direction;
                for (rapidxml::xml_node<> *child2 = child->first_node(); child2; child2 = child2->next_sibling()) {
                    if (std::string(child2->name()) == "name") {
                        borderName = child2->value();
                    }
                    if (std::string(child2->name()) == "direction") {
                        direction = child2->value();
                        if (direction == "north") { direction = "n"; }
                        if (direction == "south") { direction = "s"; }
                        if (direction == "west") { direction = "w"; }
                        if (direction == "east") { direction = "e"; }
                    }
                }
                borderNew->direction = direction;
                borderNew->name = borderName;
                border.push_back(borderNew);
            }
        }
    }
};

#endif //JOSHPROJECT_ROOM_H
