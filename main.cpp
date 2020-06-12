#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <list>
#include "Room.hpp"
#include "Utilities.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

using namespace rapidxml;  // only namespace used

bool fileExists(const char *fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

bool DoGameCommand(std::string command, std::vector<Item *> &del_items, std::vector<Container *> &del_containers,
                   std::vector<Creature *> &del_creatures, std::vector<Room *> &del_rooms,
                   std::vector<Item *> &item_vec, std::vector<Container *> &container_vec,
                   std::vector<Creature *> &creature_vec, std::vector<Room *> &room_vec, Room  * &currRoom,
                   std::vector<Item *> &inventory, bool &gameOver) {
    if (command == "n" || command == "s" || command == "e" || command == "w") {
        // run though the borders of current room
        //bool direction_found = false;
        bool border_found = false;
        for(auto a_border : currRoom->border) {
            if(border_found) {break;}
            if(a_border->direction == command) {
                border_found = true;
                bool room_found = false;
                for(auto some_room : room_vec) {
                    if(room_found) {break;}
                    if(some_room->name == a_border->name) {
                        currRoom = some_room;
                        std::cout << currRoom->description << std::endl;
                        room_found = true;
                    }
                }
                if(!room_found) {
                    std::cout << "Can't go that way." << std::endl;
                }
            }
        }
        if(!border_found) {
            std::cout << "Can't go that way." << std::endl;
        }
    }
    else if (command == "i" || command == "inventory") {
        if (inventory.empty()) {
            std::cout << "inventory is empty" << std::endl;
        }
        else {
            int i = 0;
            // prints the inventory as a comma seperated list
            std::cout << "inventory: ";
            for(auto inv_item : inventory) {
                if (i == inventory.size() - 1) {
                    std::cout << inv_item->name << std::endl;
                }
                else {
                    std::cout << inv_item->name << ", ";
                }
                i++;
            }
        }
    }
    else if (command.substr(0, 4) == "take") {
        if (command.substr(0, std::string::npos) == "take" || command.substr(0, std::string::npos) == "take ") {
            std::cout << "Take what?" << std::endl;
        }
        else if (command[4] == ' ') {
            std::string item_s = command.substr(5, std::string::npos);
            bool found = false;
            // run through all the items in the current room
            int item_index = 0;
            for(auto item_temp : currRoom->item) {
                if (item_temp->name == item_s) {
                    break;
                }
                item_index++;
            }
            if (item_index != currRoom->item.size()) {
                found = true;
                inventory.push_back(currRoom->item[item_index]);  // item from current room to inventory
                currRoom->item.erase(currRoom->item.begin() + item_index);  // remove item from room
                std::cout << item_s << " placed in inventory" << std::endl;
            }

            if (!found) {
                int con_idx = 0;
                for(auto cont : currRoom->container) {
                    item_index = IndexItemInInventory(cont->item, item_s);
                    // found a container that has the item you want
                    if(item_index != -1) {
                        break;
                    }
                    con_idx++;
                }
                //
                if (con_idx != currRoom->container.size()) {
                    if(currRoom->container[con_idx]->isOpen) {
                        found = true;
                        inventory.push_back(currRoom->container[con_idx]->item[item_index]);
                        currRoom->container[con_idx]->item.erase(currRoom->container[con_idx]->item.begin() + item_index);
                        std::cout << item_s << " placed in inventory" << std::endl;
                    }
                    else {
                        std::cout << currRoom->container[con_idx]->name << " is not open" << std::endl;
                    }
                }
            }
            if (!found) {
                std::cout << "you can't find a " << item_s << " in this room..." << std::endl;
            }

        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    else if (command.substr(0, 4) == "open") {
        if (command.substr(0, std::string::npos) == "open" || command.substr(0, std::string::npos) == "open ") {
            std::cout << "Open what?" << std::endl;
        }
        else if (command[4] == ' ') {
            std::string container_s = command.substr(5, std::string::npos);
            if(container_s == "exit") {
                if(currRoom->type == "exit") {
                    gameOver = true;
                    std:: cout << "Game Over" << std::endl;
                    return gameOver;  // just abort here :D
                }
                else {
                    std:: cout << "Current room is not an exit" << std::endl;
                }

            }
            else {
                int con_idx = 0;
                for(auto con_room : currRoom->container) {
                    if (con_room->name == container_s) {
                        break;
                    }
                    con_idx++;
                }
                if (con_idx == currRoom->container.size()) {
                    std::cout << "The creature " << container_s << " is not in current room" << std::endl;
                }
                else {
                    if (currRoom->container[con_idx]->isOpen) {
                        //std::cout << container_s << " is open already!" << std::endl;
                        if (!currRoom->container[con_idx]->item.size()) {
                            std::cout << container_s << " is empty" << std::endl;
                        }
                        else {
                            std::cout << container_s << " contains: ";  // print name
                            int i = 0;
                            // run through all the items in the selected container
                            for (auto con_item : currRoom->container[con_idx]->item) {
                                if(i == currRoom->container[con_idx]->item.size() - 1) {
                                    std::cout << con_item->name;
                                }
                                else {
                                    std::cout << con_item->name << ", ";
                                }
                                i++;
                            }
                            std::cout << std::endl;
                        }
                    }
                    else {
                        // container is locked
                        if (currRoom->container[con_idx]->locked) {
                            std::cout << container_s << " is locked" << std::endl;
                        }
                            // container is not locked
                        else {
                            std::cout << container_s << " contains: ";  // print name
                            int i = 0;
                            // run through all the items in the selected container
                            if (currRoom->container[con_idx]->item.size() == 0) {
                                std::cout << container_s << " contains no items";
                            }
                            else {
                                for (auto con_item : currRoom->container[con_idx]->item) {
                                    if(i == currRoom->container[con_idx]->item.size() - 1) {
                                        std::cout << con_item->name;
                                    }
                                    else {
                                        std::cout << con_item->name << ", ";
                                    }
                                    i++;
                                }
                            }
                            std::cout << std::endl;
                            currRoom->container[con_idx]->isOpen = true;
                        }
                    }
                }
            }
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    else if (command.substr(0, 4) == "read") {
        if (command.substr(0, std::string::npos) == "read" || command.substr(0, std::string::npos) == "read ") {
            std::cout << "Read what?" << std::endl;
        }
        else if (command[4] == ' ') {
            std::string item_s = command.substr(5, std::string::npos);

            if(inventory.empty()) {
                std::cout << "inventory is empty" << std::endl;
            }
            else {
                int index_item = IndexItemInInventory(inventory, item_s);
                // returns -1 if item is not in inventory
                if(index_item == -1) {
                    // if item is not in inventory tells you
                    std::cout << item_s << " not in inventory" << std::endl;
                }
                else {
                    // checks if the item has a description and writing
                    bool hasDescWriting = false;
                    if(!inventory[index_item]->description.empty()) {
                        std:: cout << inventory[index_item]->description << std::endl;
                        hasDescWriting = true;
                    }
                    if(!inventory[index_item]->writing.empty()) {
                        std:: cout << inventory[index_item]->writing << std::endl;
                        hasDescWriting = true;
                    }
                    if (!hasDescWriting) {
                        std:: cout << inventory[index_item]->name << " has no description or writing" << std::endl;
                    }
                }
            }
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    else if (command.substr(0, 4) == "drop") {
        if (command.substr(0, std::string::npos) == "drop" || command.substr(0, std::string::npos) == "drop ") {
            std::cout << "Drop what?" << std::endl;
        }
        else if (command[4] == ' ') {
            std::string item_s = command.substr(5, std::string::npos);

            //PUT CODE HERE  // check if item_s is in inventory
            if(inventory.empty()) {
                std::cout << "inventory is empty" << std::endl;
            }
            else {
                int index_item = IndexItemInInventory(inventory, item_s);
                // returns -1 if item is not in inventory
                if(index_item == -1) {
                    // if item is not in inventory tells you
                    std::cout << item_s << " not in inventory" << std::endl;
                }
                else {
                    // remove item from inventory and place in current room
                    currRoom->item.push_back(inventory.at(index_item));  // add item to currRoom
                    inventory.erase(inventory.begin() + index_item);  // delete item from inventory
                    std::cout << item_s << " dropped" << std::endl;
                }

            }
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    else if (command.substr(0, 3) == "put") {
        if (command.substr(0, std::string::npos) == "put" || command.substr(0, std::string::npos) == "put ") {
            std::cout << "Put what in what?" << std::endl;
        }
        else if (command[3] == ' ') {
            int space_i = (command.substr(4, std::string::npos)).find(" ");
            space_i += 4;
            if (space_i != 3 && command.length() > space_i + 4 && command.substr(space_i, 4) == " in ") {
                std::string item_s = command.substr(4, space_i - 4);
                std::string container_s = command.substr(space_i + 4, std::string::npos);

                // PUT CODE HERE  // check if item_s is in inventory and container_s is in currRoom
                int item_index = IndexItemInInventory(inventory, item_s);

                if(item_index == -1) {
                    std::cout << item_s << " not in inventory" << std::endl;
                }
                else {
                    int con_idx = 0;
                    for(auto con_room : currRoom->container) {
                        if(con_room->name == container_s) {
                            break;  // found index of container inputted
                        }
                        con_idx++;
                    }
                    if (con_idx == currRoom->container.size()) {
                        std::cout << container_s << " not in current room" << std::endl;
                    }
                    else {
                        if(currRoom->container[con_idx]->locked) {
                            bool unlock = false;
                            for (auto acc : currRoom->container[con_idx]->accept){
                                if (item_s == acc) {
                                    unlock = true;
                                }
                            }
                            if (!unlock) {
                                std::cout << container_s << "is locked" << std::endl;
                            }
                            else {
                                std::cout << item_s << " placed in " << container_s << std::endl;
                                currRoom->container[con_idx]->item.push_back((inventory[item_index]));
                                inventory.erase(inventory.begin() + item_index);
                                currRoom->container[con_idx]->locked = false;
                            }
                        }
                        else {
                            if(!currRoom->container[con_idx]->isOpen) {
                                std::cout << container_s << "is not open" << std::endl;
                            }
                            else {
                                std::cout << item_s << " placed in " << container_s << std::endl;
                                currRoom->container[con_idx]->item.push_back(inventory[item_index]);
                                inventory.erase(inventory.begin() + item_index);
                            }
                        }
                    }
                }
            }
            else {
                std::string error_s;
                if (space_i == 3) {
                    error_s = command.substr(4, std::string::npos);
                }
                else {
                    error_s = command.substr(4, space_i - 4);
                }
                std::cout << "Put " << error_s << " in what?" << std::endl;
            }
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    else if (command.substr(0, 7) == "turn on") {
        if (command.substr(0, std::string::npos) == "turn on" || command.substr(0, std::string::npos) == "turn on ") {
            std::cout << "Turn on what?" << std::endl;
        }
        else if (command[7] == ' ') {
            std::string item_s = command.substr(8, std::string::npos);

            if(inventory.empty()) {
                std::cout << "inventory is empty" << std::endl;
            }
            else {
                int index_item = IndexItemInInventory(inventory, item_s);
                // returns -1 if item is not in inventory
                if(index_item == -1) {
                    // if item is not in inventory tells you
                    std::cout << item_s << " not in inventory" << std::endl;
                }
                else {
                    // TODO add checking for turnon
                    if (inventory[index_item] -> turnon == nullptr) {
                        std::cout << "You cannot activate the " << inventory[index_item]->name << std::endl;
                    }
                    else {
                        std::cout << "You activated the " << inventory[index_item]->name << std::endl;
                        for (auto print_this : inventory[index_item]->turnon->print) {
                            std::cout << print_this << std::endl;
                        }
                        for (auto act : inventory[index_item]->turnon->action) {
                            DoAction(inventory, item_vec, container_vec, creature_vec, room_vec,
                                     act, del_items, del_containers,
                                     del_creatures, del_rooms, gameOver, currRoom);
                        }
                    }
                }
            }
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    else if (command.substr(0, 6) == "attack") {
        if (command.substr(0, std::string::npos) == "attack" || command.substr(0, std::string::npos) == "attack ") {
            std::cout << "Attack what with what?" << std::endl;
        }
        else if (command[6] == ' ') {
            int space_i = (command.substr(7, std::string::npos)).find(" ");
            space_i += 7;
            if (space_i != 6 && command.length() > space_i + 6 && command.substr(space_i, 6) == " with ") {
                std::string creature_s = command.substr(7, space_i - 7);
                std::string item_s = command.substr(space_i + 6, std::string::npos);

                //PUT CODE HERE // big boi

                int creat_idx = 0;
                for (auto creat : currRoom->creature) {
                    if (creat->name == creature_s) {
                        break;
                    }
                    creat_idx++;
                }
                if (creat_idx == currRoom->creature.size()) {
                    std::cout << creature_s << " is not in the current room" << std::endl;
                }
                else {
                    int inv_idx = IndexItemInInventory(inventory, item_s);
                    // could not find the item
                    if (inv_idx == -1) {
                        std::cout << item_s << " is not in inventory!" << std::endl;
                    }
                    else {
                        // run through creature vulnerabilities
                        bool itemMatchVul = false;
                        for (auto const vul : currRoom->creature[creat_idx]->vulnerability) {
                            if (item_s == vul) {
                                itemMatchVul = true;
                                break;
                            }
                        }
                        // item matches creature vulnerability
                        if (itemMatchVul) {
                            // item matches object
                            if (currRoom->creature[creat_idx]->attack != nullptr) {
                                // no conditions
                                if(currRoom->creature[creat_idx]->attack->condition.empty()) {
                                    std::cout << "you attacked the " << creature_s << " with the " << item_s << std::endl;
                                    for(auto print_this : currRoom->creature[creat_idx]->attack->print) {
                                        std::cout << print_this << std::endl;
                                    }
                                    for (auto act : currRoom->creature[creat_idx]->attack->action) {
                                        DoAction(inventory, item_vec, container_vec, creature_vec, room_vec,
                                                 act, del_items, del_containers,
                                                 del_creatures, del_rooms, gameOver, currRoom);
                                    }
                                }
                                else {
                                    for (auto cond : currRoom->creature[creat_idx]->attack->condition) {
                                        bool will_trigger = ConditionMet(cond, inventory, item_vec,
                                                                         container_vec, creature_vec,
                                                                         room_vec, command);
                                        // match at least one condition
                                        if (will_trigger) {
                                            std::cout << "you attacked " << creature_s << " with the " << item_s << std::endl;
                                            for(auto print_this : currRoom->creature[creat_idx]->attack->print) {
                                                std::cout << print_this << std::endl;
                                            }
                                            for (auto act : currRoom->creature[creat_idx]->attack->action) {
                                                DoAction(inventory, item_vec, container_vec, creature_vec, room_vec,
                                                         act, del_items, del_containers,
                                                         del_creatures, del_rooms, gameOver, currRoom);
                                            }
                                            break;
                                        }
                                        else {
                                            std::cout << "Your attack was ineffective" << std::endl;
                                        }
                                    }
                                }
                            }
                            else {
                                std::cout << "You attacked the " << creature_s << " with the " << item_s << std::endl;
                            }
                        }
                        else {
                            std::cout << item_s << " does not match creature's vulnerability" << std::endl;
                        }
                    }
                }
            }
            else {
                std::string error_s;
                if (space_i == 6) {
                    error_s = command.substr(7, std::string::npos);
                }
                else {
                    error_s = command.substr(7, space_i - 7);
                }
                std::cout << "Attack " << error_s << " with what?" << std::endl;
            }
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    else if (command == "look") {
        // prints everything in current room
        std::cout << "Name: " << currRoom->name << std::endl;
        std::cout << "Description: " << currRoom->description << std::endl;

        if (currRoom->item.size()) {
            std::cout << "Item(s): ";
        }
        int idx = 0;
        for (auto it : currRoom->item) {
            if (idx == currRoom->item.size() - 1) {
                std::cout << it->name << std::endl;
            }
            else {
                std::cout << it->name << ", ";
            }
            idx++;
        }
        if (currRoom->container.size()) {
            std::cout << "Container(s): ";
        }
        idx = 0;
        for(auto con : currRoom->container) {
            if (idx == currRoom->container.size() - 1) {
                std::cout << con->name << std::endl;
            }
            else {
                std::cout << con->name << ", ";
            }
            idx++;

        }
        idx = 0;
        if (currRoom->creature.size()) {
            std::cout << "Creature(s): ";
        }
        for(auto cre : currRoom->creature) {
            if (idx == currRoom->creature.size() - 1) {
                std::cout << cre->name << std::endl;
            }
            else {
                std::cout << cre->name << ", ";
            }
            idx++;
        }

    }
    else if (command.substr(0, 7) == "Update ") {
        //std::cout << "You updated: " << command.substr(7, std::string::npos) << std::endl;
        SecretUpdate(command.substr(7, std::string::npos), item_vec, container_vec, creature_vec, room_vec);
    }
    else if (command.substr(0, 4) == "Add ") {
        //std::cout << "You added: " << command.substr(4, std::string::npos) << std::endl;
        SecretAdd(command.substr(4, std::string::npos), del_items, del_containers, del_creatures, del_rooms,
                  item_vec, container_vec, creature_vec, room_vec);
    }
    else if (command.substr(0, 7) == "Delete ") {
        //std::cout << "You deleted: " << command.substr(7, std::string::npos) << std::endl;
        SecretDelete(command.substr(7, std::string::npos), del_items, del_containers, del_creatures, del_rooms,
                     item_vec, container_vec, creature_vec, room_vec, inventory);
    }
    else if (command == "Game Over") {
        std::cout << "Victory!" << std::endl;
        gameOver = true;
    }
    else if (command == "easter egg") {
        std::cout << R"(
        _____/\\\\\\\\\________/\\\\\\\\\\\__________/\\\\\\\\\__/\\\\\\\\\\\__/\\\\\\\\\\\_
         ___/\\\\\\\\\\\\\____/\\\/////////\\\_____/\\\////////__\/////\\\///__\/////\\\///__
          __/\\\/////////\\\__\//\\\______\///____/\\\/_______________\/\\\_________\/\\\_____
           _\/\\\_______\/\\\___\////\\\__________/\\\_________________\/\\\_________\/\\\_____
            _\/\\\\\\\\\\\\\\\______\////\\\______\/\\\_________________\/\\\_________\/\\\_____
             _\/\\\/////////\\\_________\////\\\___\//\\\________________\/\\\_________\/\\\_____
              _\/\\\_______\/\\\__/\\\______\//\\\___\///\\\______________\/\\\_________\/\\\_____
               _\/\\\_______\/\\\_\///\\\\\\\\\\\/______\////\\\\\\\\\__/\\\\\\\\\\\__/\\\\\\\\\\\_
                _\///________\///____\///////////___________\/////////__\///////////__\///////////__ )" << std::endl;
    }
    else {
        std::cout << "Error" << std::endl;
    }
    return gameOver;
}

int main(int argc, char * argv []) {

    //std::cout << fileExists(argv[1]) << std::endl;
    assert(fileExists(argv[1]));
    file <> xmlFile(argv[1]);
    xml_document<> doc;    // character type defaults to char
    doc.parse<0>(xmlFile.data());    // 0 means default parse flags
    xml_node<> * node = doc.first_node(); //map node (root)

    //Setup xml lists
    std::vector <xml_node <> * > room_node;
    std::vector <xml_node <> * > container_node;
    std::vector <xml_node <> * > item_node;
    std::vector <xml_node <> * > creature_node;

    for (xml_node<> * top = node -> first_node(); top; top = top -> next_sibling()) {
        if ( std::string(top->name()) == std::string("room") ) {
            room_node.push_back(top);
        }
        if ( std::string(top->name()) == std::string("container") ) {
            container_node.push_back(top);
        }
        if ( std::string(top->name()) == std::string("item") ) {
            item_node.push_back(top);
        }
        if ( std::string(top->name()) == std::string("creature") ) {
            creature_node.push_back(top);
        }
    }
    std::vector<Item * > item_vec = CreateItemVec(item_node);  // vector of Items from XML
    std::vector<Container * > container_vec = CreateContainerVec(container_node, item_vec);  // vector of containers
    std::vector<Creature * > creature_vec = CreateCreatureVec(creature_node);  // vector of creatures
    std::vector<Room * > room_vec = CreateRoomVec(room_node, item_vec, container_vec, creature_vec);  // vector of rooms

    std::vector<Item * > del_items;
    std::vector<Container * > del_containers;
    std::vector<Creature * > del_creatures;
    std::vector<Room * > del_rooms;

    Room * currRoom;                // current room of game
    std::vector<Item *> inventory;  // inventory
    for (auto room : room_vec) {
        if (room->name == "Entrance") {
            currRoom = room;
        }
    }

    std::cout << currRoom->description << std::endl;
    //std::cout << currRoom->item.size() << std::endl;
    bool gameOver = false;
    while (!gameOver) { //break; command is reached when the game is finished (perhaps use system("pause"); too)
        //Takes the users input
        std::string command;
        getline(std::cin, command);  // actual input
        if(command.substr(0, 4) == "Add " or command.substr(0, 7) == "Update " or
           command.substr(0, 7) == "Delete " or command == "Game Over") {
            std::cout << "Hey you shouldn't be using this!" << std::endl;
            continue;
        }

        bool triggered = CheckTriggers(currRoom, inventory, item_vec, container_vec, creature_vec, room_vec, command,
                                       del_items, del_containers, del_creatures, del_rooms, gameOver);

        if(!triggered) {
            gameOver = DoGameCommand(command, del_items, del_containers, del_creatures, del_rooms,
                                     item_vec, container_vec, creature_vec, room_vec, currRoom,
                                     inventory, gameOver);
        }
        triggered = true;
        while(triggered) {
            triggered = CheckTriggers(currRoom, inventory, item_vec, container_vec, creature_vec, room_vec, "",
                                      del_items, del_containers, del_creatures, del_rooms, gameOver);
        }  // loop this
        // need to call this until no triggers are set off
        // checks to see if it's a valid command and acts accordingly
    }

    return 0;
}
