#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>

namespace utils {

		std::vector<std::string> df_dm = {"06_2AH    2-20 2-21 2-22",
				"06_2DH	    2-20 2-23 2-24",
				"06_3AH	    2-20 2-21 2-22 2-25",
				"06_3CH	    2-20 2-21 2-22 2-29 2-30",
				"06_3DH	    2-20 2-21 2-22 2-25 2-29 2-30 2-34 2-35",
				"06_3EH	    2-20 2-24 2-26 2-27 2-28",
				"06_3FH	    2-20 2-29 2-32 2-33",
				"06_45H	    2-20 2-21 2-22 2-29 2-30 2-31 2-38",
				"06_46H	    2-20 2-21 2-22 2-29 2-30",
				"06_47H	    2-20 2-21 2-22 2-25 2-29 2-30 2-34 2-35",
				"06_4EH	    2-20 2-21 2-25 2-29 2-35 2-39 2-40",
				"06_4FH	    2-20 2-21 2-29 2-34 2-36 2-37 2-38",
				"06_55H	    2-20 2-21 2-25 2-29 2-35 2-39 2-50",
				"06_56H	    2-20 2-29 2-34 2-36 2-37",
				"06_5EH	    2-20 2-21 2-25 2-29 2-35 2-39 2-40",
				"06_66H	    2-20 2-21 2-25 2-29 2-35 2-39 2-40 2-42 2-43",
				"06_6AH	    2-20 2-21 2-25 2-29 2-35 2-39 2-51",
				"06_6CH	    2-20 2-21 2-25 2-29 2-35 2-39",
				"06_7DH	    2-20 2-21 2-25 2-29 2-35 2-39 2-44",
				"06_7EH	    2-20 2-21 2-25 2-29 2-35 2-39 2-44",
				"06_8CH	    2-20 2-21 2-25 2-29 2-35 2-39 2-45",
				"06_8DH	    2-20 2-21 2-25 2-29 2-35 2-39 2-45",
				"06_8EH	    2-20 2-21 2-25 2-29 2-35 2-39 2-40",
				"06_8FH	    2-52",
				"06_97H	    2-20 2-21 2-25 2-29 2-35 2-39 2-46 2-44 2-45 2-47 2-48 2-49",
				"06_9AH	    2-20 2-21 2-25 2-29 2-35 2-39 2-46 2-44 2-45 2-47 2-48 2-49",
				"06_9EH	    2-20 2-21 2-25 2-29 2-35 2-39 2-40 2-41",
				"06_A5H	    2-20 2-21 2-25 2-29 2-35 2-39",
				"06_A6H	    2-20 2-21 2-25 2-29 2-35 2-39",
				"06_BFH	    2-20 2-21 2-25 2-29 2-35 2-39 2-46 2-44 2-45 2-47 2-48 2-49"};

		std::vector<std::string> tables = {
				"2-20	06_2AH 06_2DH 06_3AH 06_3CH 06_3DH 06_3EH 06_3FH 06_45H 06_46H 06_47H 06_4EH 06_4FH 06_55H 06_56H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH",
				"2-21	06_2AH 06_3AH 06_3CH 06_3DH 06_45H 06_46H 06_47H 06_4EH 06_4FH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH",
				"2-22	06_2AH 06_3AH 06_3CH 06_3DH 06_45H 06_46H 06_47H",
				"2-23	06_2DH",
				"2-24	06_2DH 06_3EH",
				"2-25	06_3AH 06_3DH 06_47H 06_4EH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH",
				"2-26	06_3EH",
				"2-27	06_3EH",
				"2-28	06_3EH",
				"2-29	06_3CH 06_3DH 06_3FH 06_45H 06_46H 06_47H 06_4EH 06_4FH 06_55H 06_56H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH",
				"2-30	06_3CH 06_3DH 06_45H 06_46H 06_47H",
				"2-31	06_45H",
				"2-32	06_3FH",
				"2-33	06_3FH",
				"2-34	06_3DH 06_47H 06_4FH 06_56H",
				"2-35	06_3DH 06_47H 06_4EH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_7EH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH",
				"2-36	06_4FH 06_56H",
				"2-37	06_4FH 06_56H",
				"2-38	06_45H 06_4FH",
				"2-39	06_4EH 06_55H 06_5EH 06_66H 06_6AH 06_6CH 06_7DH 06_8CH 06_8DH 06_8EH 06_97H 06_9AH 06_9EH 06_A5H 06_A6H 06_BFH",
				"2-40	06_4EH 06_5EH 06_66H 06_8EH 06_9EH",
				"2-41	06_9EH",
				"2-42	06_66H",
				"2-43	06_66H",
				"2-44	06_7DH 06_7EH 06_97H 06_9AH 06_BFH",
				"2-45	06_8CH 06_8DH 06_97H 06_9AH 06_BFH",
				"2-46	06_97H 06_9AH 06_BFH",
				"2-47	06_97H 06_9AH 06_BFH",
				"2-48	06_97H 06_9AH 06_BFH",
				"2-49	06_97H 06_9AH 06_BFH",
				"2-50	06_55H",
				"2-51	06_6AH",
				"2-52	06_8FH"};


		typedef std::unordered_map<std::string, std::vector<std::string> > mappingContainer;

		mappingContainer getMapping(std::vector<std::string> &input) {
			utils::mappingContainer map;
			for (auto & entry : input) {
				std::vector<std::string> temp;
				std::istringstream ss(entry);
				std::string token;
				while(ss >> token){
					temp.push_back(token);
				}
				for(size_t i = 1; i < temp.size(); ++i) {
					map[temp[0]].push_back(temp[i]);
				}
			}
			return map;

		}

		mappingContainer dfdm_to_table() {
			return getMapping(df_dm);
		}
		
		mappingContainer tables_to_dfdm() {
			return getMapping(tables);
		} 

		//mask returns 0 for non numerical range
		uint64_t MASK(std::string range_string) {
			std::vector<int> range;
			std::istringstream ss(range_string);
			std::string token;
			while(std::getline(ss, token, ':')){
				for(auto &character : token) {
					if(!isdigit(character)) {
						return 0;
					}
				}
				range.push_back(std::stoi(token));
			}
			uint64_t larger = range[0]>range[1]?range[0]:range[1];
			uint64_t smaller = range[0]<range[1]?range[0]:range[1];
			if(range[0] == 63 and range[1] == 0) return 0xFFFFFFFFFFFFFFFF;
			return( ( ( (uint64_t)1 << (larger - smaller + 1) ) - 1) << range[1] ); 
		}



}

