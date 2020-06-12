//
// Created by Joshua Brard on 2019-03-28.
//

#ifndef JOSHPROJECT_UTILITIES_HPP
#define JOSHPROJECT_UTILITIES_HPP
#include "Room.hpp"
#include "Condition.hpp"
#include "Creature.hpp"
#include <vector>

void DoAction(std::vector<Item *> &inventory, std::vector<Item *> &item_vec,
              std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
              std::vector<Room *> &room_vec, std::string command, std::vector<Item *> &del_items,
              std::vector<Container *> &del_containers,
              std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, bool &gameOver,
              Room* &currRoom);
bool DoGameCommand(std::string command, std::vector<Item *> &del_items, std::vector<Container *> &del_containers,
                   std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms,
                   std::vector<Item *> &item_vec, std::vector<Container *> &container_vec,
                   std::vector<Creature *> &creature_vec, std::vector<Room *> &room_vec, Room  * &currRoom,
                   std::vector<Item *> &inventory, bool &gameOver);
bool CheckTriggers(Room * &currRoom, std::vector<Item *> &inventory, std::vector<Item *> &item_vec,
                   std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
                   std::vector<Room *> &room_vec, std::string command, std::vector<Item *> &del_items,
                   std::vector<Container *> &del_containers,
                   std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, bool &gameOver);

int IndexItemInInventory(const std::vector<Item *>  &inventory, const std::string item_s)
{
    // this function returns the index of an item that is "supposed" to be in the inventory
    // if the item is not in the inventory the index returned is greater than the list size
    int i = 0;
    for (auto item : inventory) {
        if(item->name == item_s) {
            return i;
        }
        i++;
    }
    i = -1;
    return i;
}

std::vector <Item*> CreateItemVec(std::vector <xml_node <> * > item_node)
{
    std::vector<Item * > item_vec;
    for(auto item_one : item_node) {
        Item * item = new Item(item_one);
        item_vec.push_back(item);
    }
    return item_vec;
}

std::vector<Container * > CreateContainerVec(std::vector <xml_node <> * > container_node, std::vector<Item *> item_vec)
{
    std::vector<Container * > container_vec;
    for(auto container_one : container_node) {
        Container * container = new Container(container_one, item_vec);
        container_vec.push_back(container);
    }
    return container_vec;
}


std::vector<Creature * > CreateCreatureVec(std::vector<xml_node <> * > creature_node)
{
    std::vector<Creature * > creature_vec;
    for(auto creature_one : creature_node) {
        Creature * creature = new Creature(creature_one);
        creature_vec.push_back(creature);
    }
    return creature_vec;
}

std::vector<Room * > CreateRoomVec(std::vector<xml_node <> *> room_node, std::vector<Item *> item_vec,
                                   std::vector<Container *> container_vec, std::vector<Creature *> creature_vec)
{
    std::vector<Room * > room_vec;  // vector of room objects
    for(auto room_one : room_node)
    {
        Room * room = new Room(room_one, item_vec, container_vec, creature_vec);  // create a room
        room_vec.push_back(room);  // add the room to a vector
    }
    return room_vec;
}

void SecretAdd(std::string command, std::vector<Item *> &del_items, std::vector<Container *> &del_containers,
               std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, std::vector<Item *> &item_vec,
               std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
               std::vector<Room *> &room_vec) {
    int space_i = command.substr(0, std::string::npos).find(" ");
    std::string add_this = command.substr(0, space_i);
    std::string to_this = command.substr(space_i + 4, std::string::npos);
    //Deleted stuff; if it's deleted, un-delete it
    for(auto stuff : del_items) {
        if(add_this == stuff->name) {
            item_vec.push_back(stuff);
        }
    }
    for(auto stuff : del_containers) {
        if(add_this == stuff->name) {
            container_vec.push_back(stuff);
        }
    }
    for(auto stuff : del_creatures) {
        if(add_this == stuff->name) {
            creature_vec.push_back(stuff);
        }
    }
    for(auto stuff : del_rooms) {
        if(add_this == stuff->name) {
            room_vec.push_back(stuff);
        }
    }

    bool to_this_found = false;
    bool add_found = false;
    for(auto stuff : item_vec) {
        if(add_found) {break;}
        if(add_this == stuff->name) {
            add_found = true;
            for(auto c : container_vec) {
                if(to_this_found) {break;}
                if(c->name == to_this) {
                    to_this_found = true;
                    c->item.push_back(stuff);
                }
            }
            for(auto r : room_vec) {
                if(to_this_found) {break;}
                if(r->name == to_this) {
                    to_this_found = true;
                    r->item.push_back(stuff);
                }
            }
        }
    }
    for(auto stuff : container_vec) {
        if(add_found) {break;}
        if(add_this == stuff->name) {
            add_found = true;
            for (auto r : room_vec) {
                if (to_this_found) { break; }
                if (r->name == to_this) {
                    to_this_found = true;
                    r->container.push_back(stuff);
                }
            }
        }
    }
    for(auto stuff : creature_vec) {
        if(add_found) {break;}
        if(add_this == stuff->name) {
            add_found = true;
            for (auto r : room_vec) {
                if (to_this_found) { break; }
                if (r->name == to_this) {
                    to_this_found = true;
                    r->creature.push_back(stuff);
                }
            }
        }
    }
}

void SecretDelete(std::string command, std::vector<Item *> &del_items, std::vector<Container *> &del_containers,
                  std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, std::vector<Item *> &item_vec,
                  std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
                  std::vector<Room *> &room_vec, std::vector<Item *> &inventory) {
    //command is what you want to delete
    bool delete_found = false;
    int i;
    int j;
    int k;

    i = 0;
    for(auto stuff : item_vec) {
        if(stuff->name == command) {
            if(!delete_found) {
                delete_found = true;
                del_items.push_back(stuff);
            }
            item_vec.erase(item_vec.begin() + i);
        }
        i += 1;
    }

    i = 0;
    for(auto stuff : container_vec) {
        j = 0;
        for(auto items : stuff->item) {
            if(items->name == command) {
                if(!delete_found) {
                    delete_found = true;
                    del_items.push_back(items);
                }
                stuff->item.erase(stuff->item.begin() + j);
            }
            j += 1;
        }
        if(stuff->name == command) {
            if(!delete_found) {
                delete_found = true;
                del_containers.push_back(stuff);
            }
            container_vec.erase(container_vec.begin() + i);
        }
        i += 1;
    }

    i = 0;
    for(auto stuff : creature_vec) {
        if(stuff->name == command) {
            if(!delete_found) {
                delete_found = true;
                del_creatures.push_back(stuff);
            }
            creature_vec.erase(creature_vec.begin() + i);
        }
        i += 1;
    }

    i = 0;
    for(auto stuff : room_vec) {
        j = 0;
        for(auto items : stuff->item) {
            if(items->name == command) {
                if(!delete_found) {
                    delete_found = true;
                    del_items.push_back(items);
                }
                stuff->item.erase(stuff->item.begin() + j);
            }
            j += 1;
        }
        j = 0;
        for(auto containers : stuff->container) {
            k = 0;
            for(auto cont_items : containers->item) {
                if(cont_items->name == command) {
                    if(!delete_found) {
                        delete_found = true;
                        del_items.push_back(cont_items);
                    }
                    containers->item.erase(containers->item.begin() + k);
                }
                k += 1;
            }
            if(containers->name == command) {
                if(!delete_found) {
                    delete_found = true;
                    del_containers.push_back(containers);
                }
                stuff->container.erase(stuff->container.begin() + j);
            }
            j += 1;
        }
        j = 0;
        for(auto creatures : stuff->creature) {
            if(creatures->name == command) {
                if(!delete_found) {
                    delete_found = true;
                    del_creatures.push_back(creatures);
                }
                stuff->creature.erase(stuff->creature.begin() + j);
            }
            j += 1;
        }
        if(stuff->name == command) {
            if(!delete_found) {
                delete_found = true;
                del_rooms.push_back(stuff);
            }
            room_vec.erase(room_vec.begin() + i);
        }
        i += 1;
    }

    i = 0;
    for(auto stuff : inventory) {
        if(stuff->name == command) {
            inventory.erase(inventory.begin() + i);
        }
        i += 1;
    }
}

void SecretUpdate(std::string command, std::vector<Item *> &item_vec,
                  std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
                  std::vector<Room *> &room_vec) {
    int space_i = (command.substr(0, std::string::npos)).find(" ");
    std::string update_this = command.substr(0, space_i);
    std::string to_this = command.substr(space_i + 4, std::string::npos);

    bool obj_found = false;
    for(auto stuff : item_vec) {
        if(obj_found) {break;}
        if(stuff->name == update_this) {
            obj_found = true;
            stuff->status = to_this;
        }
    }
    for(auto stuff : container_vec) {
        if(obj_found) {break;}
        if(stuff->name == update_this) {
            obj_found = true;
            stuff->status = to_this;
        }
    }
    for(auto stuff : creature_vec) {
        if(obj_found) {break;}
        if(stuff->name == update_this) {
            obj_found = true;
            stuff->status = to_this;
        }
    }
    for(auto stuff : room_vec) {
        if(obj_found) {break;}
        if(stuff->name == update_this) {
            obj_found = true;
            stuff->status = to_this;
        }
    }
}

bool ConditionMet(Condition* cond, std::vector<Item *> inventory, std::vector<Item *> item_vec,
                  std::vector<Container *> container_vec,  std::vector<Creature *> creature_vec,
                  std::vector<Room *> room_vec, const std::string command) {
    bool will_trigger = false;
    if (cond->has.empty()) { // status of an object
        bool obj_found = false;
        for (auto stuff : item_vec) {
            if (obj_found) { break; }
            if (stuff->name == cond->object) {
                obj_found = true;
                if (stuff->status == cond->status) {
                    will_trigger = true;
                }
            }
        }
        for (auto stuff : creature_vec) {
            if (obj_found) { break; }
            if (stuff->name == cond->object) {
                obj_found = true;
                if (stuff->status == cond->status) {
                    will_trigger = true;
                }
            }
        }
        for (auto stuff : container_vec) {
            if (obj_found) { break; }
            if (stuff->name == cond->object) {
                obj_found = true;
                if (stuff->status == cond->status) {
                    will_trigger = true;
                }
            }
        }
        for (auto stuff : room_vec) {
            if (obj_found) { break; }
            if (stuff->name == cond->object) {
                obj_found = true;
                if (stuff->status == cond->status) {
                    will_trigger = true;
                }
            }
        }
    } else { // check if/if not x has y
        bool h = (cond->has == "yes");
        bool obj_found = false;

        //okay so I don't need to find the object; just the owner
        //find the owner and its type, and search all its relevant
        //fields for something that has the same name as object

        // find the owner

        if (cond->owner == "inventory") {
            for (auto stuff : inventory) {
                if (stuff->name == cond->object) {
                    obj_found = true;
                    break;
                }
            }
        } else {
            bool owner_found = false;
            //check containers
            for (auto con : container_vec) {
                if (owner_found) { break; }
                if (con->name == cond->owner) {
                    owner_found = true;
                    for (auto stuff : con->item) {
                        if (stuff->name == cond->object) {
                            obj_found = true;
                            break;
                        }
                    }
                }
            }
            //check rooms
            for (auto ro : room_vec) {
                if (owner_found) { break; }
                if (ro->name == cond->owner) {
                    owner_found = true;
                    for (auto stuff : ro->item) {
                        if (obj_found) { break; }
                        if (stuff->name == cond->object) {
                            obj_found = true;
                        }
                    }
                    for (auto stuff : ro->container) {
                        for (auto cont_items : stuff->item) {
                            if (obj_found) { break; }
                            if (cont_items->name == cond->object) {
                                obj_found = true;
                            }
                        }
                        if (obj_found) { break; }
                        if (stuff->name == cond->object) {
                            obj_found = true;
                        }
                    }
                    for (auto stuff : ro->creature) {
                        if (obj_found) { break; }
                        if (stuff->name == cond->object) {
                            obj_found = true;
                        }
                    }
                }
            }
        }

        if (h == obj_found) {
            will_trigger = true;
        }
    }
    return will_trigger;
}

bool DoTriggers(Trigger* trig, std::vector<Item *> &inventory, std::vector<Item *> &item_vec,
                std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
                std::vector<Room *> &room_vec, std::string command, std::vector<Item *> &del_items,
                std::vector<Container *> &del_containers,
                std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, bool &gameOver,
                Room* &currRoom) {
    if(trig->command != "" and trig->command != command){
        return false;
    }
    bool will_trigger;
    for(auto cond : trig->condition) {
        will_trigger = false;
        if(cond->has.empty()) { // status of an object
            bool obj_found = false;
            for(auto stuff : item_vec) {
                if(obj_found) {break;}
                if(stuff->name == cond->object) {
                    obj_found = true;
                    if(stuff->status == cond->status) {
                        will_trigger = true;
                    }
                }
            }
            for(auto stuff : creature_vec) {
                if(obj_found) {break;}
                if(stuff->name == cond->object) {
                    obj_found = true;
                    if(stuff->status == cond->status) {
                        will_trigger = true;
                    }
                }
            }
            for(auto stuff : container_vec) {
                if(obj_found) {break;}
                if(stuff->name == cond->object) {
                    obj_found = true;
                    if(stuff->status == cond->status) {
                        will_trigger = true;
                    }
                }
            }
            for(auto stuff : room_vec) {
                if(obj_found) {break;}
                if(stuff->name == cond->object) {
                    obj_found = true;
                    if(stuff->status == cond->status) {
                        will_trigger = true;
                    }
                }
            }
        } else { // check if/if not x has y
            bool h = (cond->has == "yes");
            bool obj_found = false;

            //okay so I don't need to find the object; just the owner
            //find the owner and its type, and search all its relevant
            //fields for something that has the same name as object

            // find the owner

            if (cond->owner == "inventory") {
                for (auto stuff : inventory) {
                    if (stuff->name == cond->object) {
                        obj_found = true;
                        break;
                    }
                }
            } else {
                bool owner_found = false;
                //check containers
                for (auto con : container_vec) {
                    if (owner_found) { break; }
                    if (con->name == cond->owner) {
                        owner_found = true;
                        for (auto stuff : con->item) {
                            if (stuff->name == cond->object) {
                                obj_found = true;
                                break;
                            }
                        }
                    }
                }
                //check rooms
                for (auto ro : room_vec) {
                    if (owner_found) { break; }
                    if (ro->name == cond->owner) {
                        owner_found = true;
                        for (auto stuff : ro->item) {
                            if (obj_found) { break; }
                            if (stuff->name == cond->object) {
                                obj_found = true;
                            }
                        }
                        for (auto stuff : ro->container) {
                            for (auto cont_items : stuff->item) {
                                if (obj_found) {break;}
                                if (cont_items->name == cond->object) {
                                    obj_found = true;
                                }
                            }
                            if (obj_found) { break; }
                            if (stuff->name == cond->object) {
                                obj_found = true;
                            }
                        }
                        for (auto stuff : ro->creature) {
                            if (obj_found) { break; }
                            if (stuff->name == cond->object) {
                                obj_found = true;
                            }
                        }
                    }
                }
            }

            if (h == obj_found) {
                will_trigger = true;
            }
            if(!will_trigger) {break;}
        }
    }

    if(will_trigger) {
        for(auto print_statement : trig->print) {
            std::cout << print_statement << std::endl;
        }
        for(auto act : trig->action) {
            DoAction(inventory, item_vec, container_vec, creature_vec, room_vec, act, del_items, del_containers,
                     del_creatures, del_rooms, gameOver, currRoom);
        }
    }
    return will_trigger;
}

void DoAction(std::vector<Item *> &inventory, std::vector<Item *> &item_vec,
              std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
              std::vector<Room *> &room_vec, std::string command, std::vector<Item *> &del_items,
              std::vector<Container *> &del_containers,
              std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, bool &gameOver,
              Room* &currRoom) {
    gameOver = DoGameCommand(command, del_items, del_containers, del_creatures, del_rooms,
                                 item_vec, container_vec, creature_vec, room_vec, currRoom,
                                 inventory, gameOver);
}

// CheckTriggers(currRoom, &inventory, &item_vec, &container_vec, &creature_vec, &room_vec);
bool CheckTriggers(Room * &currRoom, std::vector<Item *> &inventory, std::vector<Item *> &item_vec,
                   std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
                   std::vector<Room *> &room_vec, std::string command, std::vector<Item *> &del_items,
                   std::vector<Container *> &del_containers,
                   std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, bool &gameOver)
{
    int i = 0;
    int j = 0;
    bool will_trigger = false;
    //checks for triggers for room, need to check for triggers in
    //inventory items, items in current room, containers in current room,
    //items in those containers in current room, and creatures in
    //current room

    //room
    for(auto trig : currRoom->trigger) {
        if(will_trigger) {break;}
        if (!(command.empty()) and command != trig->command) {continue;}
        will_trigger = DoTriggers(trig, inventory, item_vec,
                container_vec, creature_vec, room_vec, command, del_items,
                del_containers, del_creatures, del_rooms, gameOver, currRoom);
        //only deletes if occurs
        if (trig->type == "single" and will_trigger) {
            (currRoom->trigger).erase((currRoom->trigger).begin() + i);
        }
        i += 1;
    }
    //inventory
    i = 0;
    for(auto inv_items : inventory) {
        if(will_trigger) {break;}
        for(auto trig : inv_items->trigger) {
            if(will_trigger) {break;}
            if (!(command.empty()) and command != trig->command) {continue;}
            will_trigger = DoTriggers(trig, inventory, item_vec,
                                      container_vec, creature_vec, room_vec, command, del_items,
                                      del_containers, del_creatures, del_rooms, gameOver, currRoom);
            //only deletes if occurs
            if (trig->type == "single" and will_trigger) {
                (inv_items->trigger).erase((inv_items->trigger).begin() + i);
            }
            i += 1;
        }
    }
    //room items
    i = 0;
    for(auto curr_room_items : currRoom->item) {
        if(will_trigger) {break;}
        for(auto trig : curr_room_items->trigger) {
            if(will_trigger) {break;}
            if (!(command.empty()) and command != trig->command) {continue;}
            will_trigger = DoTriggers(trig, inventory, item_vec,
                                      container_vec, creature_vec, room_vec, command, del_items,
                                      del_containers, del_creatures, del_rooms, gameOver, currRoom);
            //only deletes if occurs
            if (trig->type == "single" and will_trigger) {
                (curr_room_items->trigger).erase((curr_room_items->trigger).begin() + i);
            }
            i += 1;
        }
    }
    //room containers
    i = 0;
    for(auto curr_room_containers : currRoom->container) {
        if(will_trigger) {break;}
        for(auto trig : curr_room_containers->trigger) {
            if(will_trigger) {break;}
            //if (trig->command != "" and command != "" and command != trig->command) {continue;}
            will_trigger = DoTriggers(trig, inventory, item_vec,
                                      container_vec, creature_vec, room_vec, command, del_items,
                                      del_containers, del_creatures, del_rooms, gameOver, currRoom);
            //only deletes if occurs
            if (trig->type == "single" and will_trigger) {
                (curr_room_containers->trigger).erase((curr_room_containers->trigger).begin() + i);
            }
            i += 1;
        }
        //items in containers
        j = 0;
        for(auto container_items : curr_room_containers->item) {
            if(will_trigger) {break;}
            for(auto trig : container_items->trigger) {
                if(will_trigger) {break;}
                if (!(command.empty()) and command != trig->command) {continue;}
                will_trigger = DoTriggers(trig, inventory, item_vec,
                                          container_vec, creature_vec, room_vec, command, del_items,
                                          del_containers, del_creatures, del_rooms, gameOver, currRoom);
                //only deletes if occurs
                if (trig->type == "single" and will_trigger) {
                    (container_items->trigger).erase((container_items->trigger).begin() + j);
                }
                j += 1;
            }
        }
    }
    //room creatures
    i = 0;
    for(auto curr_room_creatures : currRoom->creature) {
        if(will_trigger) {break;}
        for(auto trig : curr_room_creatures->trigger) {
            if(will_trigger) {break;}
            if (!(command.empty()) and command != trig->command) {continue;}
            will_trigger = DoTriggers(trig, inventory, item_vec,
                                      container_vec, creature_vec, room_vec, command, del_items,
                                      del_containers, del_creatures, del_rooms, gameOver, currRoom);
            //only deletes if occurs
            if (trig->type == "single" and will_trigger) {
                (curr_room_creatures->trigger).erase((curr_room_creatures->trigger).begin() + i);
            }
            i += 1;
        }
    }

    return will_trigger;
}

void DoAttack(Room * &currRoom, std::vector<Item *> &inventory, std::vector<Item *> &item_vec, Attack* &attack,
              std::vector<Container *> &container_vec,  std::vector<Creature *> &creature_vec,
              std::vector<Room *> &room_vec, std::vector<Item *> &del_items,
              std::vector<Container *> &del_containers,
              std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms, bool &gameOver) {

}

#endif //JOSHPROJECT_UTILITIES_HPP
