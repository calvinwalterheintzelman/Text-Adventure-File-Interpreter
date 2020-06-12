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

using namespace rapidxml;
bool fileExists(const char *fileName) {
	std::ifstream infile(fileName);
	return infile.good();
}
int main(int argc, char * argv []) {
	std::cout << fileExists(argv[1]) << std::endl;

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
	std::vector<Item * > item_vec = CreateItemVec(item_node);
	std::vector<Container * > container_vec = CreateContainerVec(container_node, item_vec);
	std::vector<Creature * > creature_vec = CreateCreatureVec(creature_node);
	std::vector<Room * > room_vec = CreateRoomVec(room_node, item_vec, container_vec, creature_vec);

	while (true) { //break; command is reached when the game is finished (perhaps use system("pause"); too)

		//Takes the users input
		std::string command;
		getline(std::cin, command);

		// checks to see if it's a valid command and acts accordingly
		if (command == "n" || command == "s" || command == "e" || command == "w") {

			if (command == "n") {

			}
			else if (command == "s") {

			}
			else if (command == "e") {
				
			}
			else if (command == "w") {

			}
		}
		else if (command == "i" || command == "inventory") {

		}
		else if (command.substr(0, 4) == "take") {
			if (command.substr(0, std::string::npos) == "take" || command.substr(0, std::string::npos) == "take ") {
				std::cout << "Take what?" << std::endl;
			}
			else if (command[4] == ' ') {
				std::string item_s = command.substr(5, std::string::npos);

				//PUT CODE HERE
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

				//PUT CODE HERE
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

				//PUT CODE HERE
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

				//PUT CODE HERE
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

					//PUT CODE HERE
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

				//PUT CODE HERE
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

					//PUT CODE HERE
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

		}
		else if (command.substr(0, 7) == "examine") {
			if (command.substr(0, std::string::npos) == "examine" || command.substr(0, std::string::npos) == "examine ") {
				std::cout << "Examine what?" << std::endl;
			}
			else if (command[7] == ' ') {
				std::string thing_s = command.substr(8, std::string::npos);

				//PUT CODE HERE
			}
			else {
				std::cout << "Error" << std::endl;
			}
		}
		else {
			std::cout << "Error" << std::endl;
		}
	}
}
