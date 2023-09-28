#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <iomanip>
#include "utils/mapping.h"

//Interface
//Create GenieDataManager object, constructor will initialize datastructures and load MSR source files (may be changed in the future to accomodate multiple sources and user selection)
//------Debug function will print all data stored in GenieDataStore (Table name, number of associated MSRs, table address), addresses of tables associated with each DF_DM, and MSRs contained 
//within all tables and their associated DF_DMs and bitfields. 
//------PrintMSRs function takes 3 arguments (string df_dm, bool all, string manufacturer) argument 2 is true by default which prints all data associated with the MSR, if false, then only the
//hex address will be printed. manufacturer is "INTEL" by default. argument 2 and 3 are both optional.
//------getMSRsForDFDM function takes 2 arguments (string df_dm, string manufacturer) argument 2 is optional, default is INTEL. Return vector of array(string, 4) (hex address, MSR name, domain, description)
//------getDFDMsForMSR function takes 2 arguments (string MSR hex address, manufacturer) argument 2 is optional, default is INTEL. Return vector of df_dms
//------getMask function takes 3 arguments (df_dm, msr address, manufacturer) argument 3 is optional, default is INTEL. return vector of pairs, pair.first is the mask value, pari.second is 
//a string array of size 3 for the bit range, function and description of the bitfield

class MSR
{
	private:

		struct MSRBitFields
		{
			std::string values;
			std::string function;
			std::string description;
			MSRBitFields(std::vector<std::string > &input): values(input[0]),
				function(input[1]), description(input[2]) {}

			void updateValues(std::vector<std::string > &input)
			{
				values = input[0];
				function = input[1];
				description = input[2];
			}
		};

	std::string name;
	std::string domain;
	std::string description;
	std::vector<std::string > associated_df_dm;
	std::vector<MSRBitFields> bit_fields;

	void printBitfields()
	{
		for (auto &bitfield: bit_fields)
		{
			std::cout << "\tBit range: " << std::setw(6) << bitfield.values << "\tFunction: " << std::setw(10) << bitfield.function << "\tDescription: " << bitfield.description << "\n";
		}
	}

	void printDFDM(std::string hex_address, std::string table)
	{
		std::cout << "Table name: " << table << "\tHex address: " << std::setw(12) << hex_address << "\tMSR name: " << std::setw(30) << name << "\tDomain: " << std::setw(10) << domain << "\tDescription: " << description << "\nAssociated DFDMs: ";
		for (auto &df_dm: associated_df_dm)
		{
			std::cout << df_dm << " ";
		}

		std::cout << "\n";
		printBitfields();
		std::cout << "\n";
	}

	public:

		MSR(std::vector<std::string > &input, std::vector<std::string > &df_dm_list): name(input[2]), domain(input[3]), description(input[4])
		{
			associated_df_dm = df_dm_list;
		};

	void insertBitfield(std::vector<std::string > &input)
	{
		bit_fields.emplace_back(MSRBitFields(input));
	};

	void print(std::string hex_address, std::string table = "N/A")
	{
		printDFDM(hex_address, table);
	}

	void fill(std::array<std::string, 5> &data)
	{
		data[1] = name;
		data[2] = domain;
		data[3] = description;
	}

	std::vector<std::string > getDFDMsForMSR()
	{
		return associated_df_dm;
	}

	std::vector<std::array<std::string, 3>> getBitfields()
	{
		std::vector<std::array<std::string, 3>> ret;
		for (auto &bitfield: bit_fields)
		{
			std::array<std::string, 3> temp;
			temp[0] = bitfield.values;
			temp[1] = bitfield.function;
			temp[2] = bitfield.description;
			ret.push_back(temp);
		}

		return ret;
	}
};

class GenieDataStore
{
	private:

	typedef std::unordered_map<std::string, MSR*> msr_table_hash;

	//manufacturer --> Table name --> MSR Hex address --> Pointer to MSR
	std::unordered_map<std::string, std::unordered_map<std::string, msr_table_hash>> MSR_info;

	//manufacturer --> df_dm --> vector of pointers to table 
	std::unordered_map<std::string, std::unordered_map<std::string, std::vector< msr_table_hash*> >> df_dm_info;

	//table memory address to table name lookup
	std::unordered_map<msr_table_hash*, std::string > table_lookup;

	//debug	
	void printTableNames()
	{
		for (const auto &manu: MSR_info)
		{
			for (const auto &table: manu.second)
			{
				std::cout << "Manufacturer: " << manu.first << "\tTable name: " << table.first << "\tNumber of MSRs: " << std::to_string(table.second.size()) << "\tTable address:\t" << &table.second << "\n";
			}
		}
	}

	//debug
	void printTablePointerAddresses()
	{
		for (const auto &manu: df_dm_info)
		{
			std::cout << manu.first << "\n";
			for (const auto &df_dm: manu.second)
			{
				std::cout << df_dm.first << " ";
				for (const auto &table_address: df_dm.second)
				{
					std::cout << table_address << " ";
				}

				std::cout << "\n";
			}
		}
	}

	//debug
	void printMSRandBitfields()
	{
		for (const auto &manu: MSR_info)
		{
			std::cout << "Manufacturer: " << manu.first << "\t";
			for (const auto &table: manu.second)
			{
				std::cout << "Table: " << table.first << "\n";
				for (const auto &msr: table.second)
				{
					msr.second->print(msr.first, table.first);
				}
			}
		}
	}

	public:

		GenieDataStore() {}

		~GenieDataStore()
		{
			for (auto &manufacturer: MSR_info)
			{
				for (auto &table: manufacturer.second)
				{
					for (auto &msr: table.second)
					{
						delete msr.second;
					}
				}
			}
		}

	void insertMSR(std::vector<std::string > &msr_info, std::vector<std::string > &df_dm_list)
	{
		if (msr_info[0] == "INTEL")
		{
			//access hash of MSRs by manufacturer and table and hex address	
			MSR_info[msr_info[0]][msr_info[5]][msr_info[1]] = new MSR(msr_info, df_dm_list);
		}
	}

	void insertBitField(std::vector<std::string > &bit_field_info)
	{
		std::vector<std::string > temp(3);
		temp[0] = bit_field_info[2];
		temp[1] = bit_field_info[3];
		temp[2] = bit_field_info[4];

		//manufacturer/table-name/Hex address --> pointer to MSR: insert bitfield into MSR
		MSR_info[bit_field_info[0]][bit_field_info[5]][bit_field_info[1]]->insertBitfield(temp);
	}

	//for each df_dm associated with the table being processed, insert a pointer to the 
	void insertTablePointer(std::string tablename, std::string manufacturer, std::vector<std::string > &df_dm_vector)
	{
		for (auto &df_dm: df_dm_vector)
		{
			df_dm_info[manufacturer][df_dm].push_back(&MSR_info[manufacturer][tablename]);
		}

		table_lookup[ &MSR_info[manufacturer][tablename]] = tablename;
	}

	//debug	
	void debug()
	{
		printTableNames();
		printTablePointerAddresses();
		printMSRandBitfields();
	}

	//print list of supported df_dms
	void supported_df_dms()
	{
		std::cout << "Supported df_dms for each manufacturer as follows:\n";
		for (const auto &manufacturer: df_dm_info)
		{
			std::cout << manufacturer.first << "\n";
			for (const auto &df_dm: manufacturer.second)
			{
				std::cout << "\t" << df_dm.first << "\n";
			}
		}
	}

	//print MSRs associated with a df_dm
	void printMSRs(std::string df_dm, std::string manufacturer)
	{
		if (df_dm_info[manufacturer].find(df_dm) == df_dm_info[manufacturer].end())
		{
			std::cout << "Invalid df_dm or no data available\n";
			return;
		}

		std::cout << df_dm << " : ";
		for (auto &table_address: df_dm_info[manufacturer][df_dm])
		{
			//print table addresses associated with df_dm
			std::cout << table_address << " ";
		}

		std::cout << "\n";
		//int count = 1;
		for (auto &table_address: df_dm_info[manufacturer][df_dm])
		{
			//print all MSR hex addresses associated with df_dm
			for (auto &p: *table_address)
			{
				p.second->print(p.first, table_lookup[table_address]);
			}
		}
	}

	std::vector<std::array<std::string, 5>> getMSRsForDFDM(std::string dfdm, std::string manufacturer)
	{
		//get list of MSRs from highest numbered table to lowest number
		//skip MSRs which have been seen in a higher numbered table by 
		//keeping track of MSR hex addresses in hash set
		std::unordered_set<std::string > seen;
		std::vector<std::array<std::string, 5>> ret;
		auto table_addresses = df_dm_info[manufacturer][dfdm];
		std::reverse(table_addresses.begin(), table_addresses.end());
		for (auto &table_address: table_addresses)
		{
			for (auto &p: *table_address)
			{
				if (seen.find(p.first) != seen.end()) continue;
				seen.insert(p.first);
				std::array<std::string, 5> temp;
				temp[0] = p.first;
				temp[4] = table_lookup[table_address];
				p.second->fill(temp);
				ret.push_back(temp);
			}
		}
		sort(ret.begin(), ret.end());
		return ret;
	}

	//return all MSRs associated with MSR hex address
	std::vector<std::string > getDFDMsForMSR(std::string MSR_hex, std::string manufacturer)
	{
		//use set to remove duplicates
		std::unordered_set<std::string > ret;

		std::unordered_map<std::string, msr_table_hash> &tables = MSR_info[manufacturer];

		for (const auto &table: tables)
		{
			if (table.second.find(MSR_hex) != table.second.end())
			{
				for (const auto &dfdm: table.second.at(MSR_hex)->getDFDMsForMSR())
				{
					if (ret.find(dfdm) == ret.end())
					{
						ret.insert(dfdm);
					}
				}
			}
		}

		std::vector<std::string > retvec(ret.begin(), ret.end());
		return retvec;
	}

	//get mask given df_dm and MSR hex
	std::array<std::string, 3> getMask(std::string df_dm, std::string msr_hex, std::string manufacturer = "INTEL")
	{
		//returns the mask in array (hex address of MSR, hex value of mask, table msr belongs to)
		std::array<std::string, 3> ret;
		//reverse the table addresses to lookup the highest numbered table first, if we find the MSR in the highest table referenced then we use it and break. (table addresses are originally from 2-20 to 2-52)
		auto address_vec = df_dm_info[manufacturer][df_dm];
		std::reverse(address_vec.begin(), address_vec.end());
		for (auto &table_address: address_vec)
		{
			auto table = *table_address;
			auto tablename = table_lookup[table_address];
			if (table_address->find(msr_hex) != table_address->end())
			{
				auto msr = table[msr_hex];
				auto bitfield_vector = msr->getBitfields();	// return a vector of array of strings of size 3 (range, function, description);

				uint64_t total = 0xFFFFFFFFFFFFFFFF;

				//format hex address
				std::string removeH = msr_hex;
				removeH.pop_back();
				std::stringstream hex_to_decimal;
				std::stringstream decimal_to_hex;
				int msr_address_decimal;
				hex_to_decimal << std::hex << removeH;
				hex_to_decimal >> msr_address_decimal;
				decimal_to_hex << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << msr_address_decimal;

				ret[0] = decimal_to_hex.str();
				ret[2] = tablename;
				
				for (auto &entry: bitfield_vector)
				{
					std::string entry_des_substr = entry[2].substr(0,8);

					if (entry_des_substr == "Reserved") 
					{
						if (entry[0].size() > 2)
						{
							uint64_t range_mask = utils::MASK(entry[0]);
							total -= range_mask;
						} else {
							total -= 1 << stoi(entry[0]);
						}

					} else {

						continue;
					} 
				}
				
				std::ostringstream converter;
				converter << "0x" << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << total;
				ret[1] = converter.str();

				break;
			}
		}

		return ret;
	}
};

class FileLoader
{
	public:

		FileLoader() {}

	void Initialize(GenieDataStore & data)
	{
		IntelTableLoader(data);
	}

	private:

	utils::mappingContainer df_dm_to_tables = utils::dfdm_to_table();
	utils::mappingContainer tables_to_dfdm = utils::tables_to_dfdm();

	void IntelTableLoader(GenieDataStore & data)
	{
		//load tables 20 through 52 and parse each line
		for (int i = 20; i <= 52; ++i)
		{
			std::string tablefile = "Intel_MSR_Tables/2-" + std::to_string(i) + ".txt";
			std::string tablename = "2-" + std::to_string(i);
			std::ifstream table(tablefile);
			std::string msr_data;

			//keep track of previous hex address for bit_field rows(bit field rows have no address information)
			std::string previous = "";

			while (std::getline(table, msr_data))
			{
				std::istringstream linestream(msr_data);
				std::string token;
				std::vector<std::string > tokens;

				while (std::getline(linestream, token, '\t'))
				{
					tokens.push_back(token);
				}
				
				//if documentation does not provide a description. Append description to output
				if (tokens.size() < 5) 
				{
					tokens.push_back("None provided in documentation");
				}

				//bit_field entry detected, insert entry into previous MSR vector
				if (tokens[0] == ""
					and tokens[1] == "")
				{
					std::vector<std::string > temp(6);
					temp[0] = "INTEL";
					temp[1] = previous;
					temp[2] = tokens[2];
					temp[3] = tokens[3];
					temp[4] = tokens[4];
					temp[5] = tablename;
					data.insertBitField(temp);
				}
				else
				{
				 	//new MSR entry detected, add new MSR to table
					std::vector<std::string > temp(6);
					temp[0] = "INTEL";
					temp[1] = tokens[0];	//hex address
					temp[2] = tokens[2];	//MSR name
					temp[3] = tokens[3];	//function
					temp[4] = tokens[4];	//description
					temp[5] = tablename;	//current table being processed
					data.insertMSR(temp, tables_to_dfdm[tablename]);
					previous = tokens[0];	//update hex address to current;
				}
			}

			data.insertTablePointer(tablename, "INTEL", tables_to_dfdm[tablename]);
		}
	}
};

class GenieDataManager
{
	private:
		GenieDataStore data;
		FileLoader loader;

	public:
		GenieDataManager()
		{
			loader.Initialize(data);
		}

	//debug, dump all data in datastore
	void debug()
	{
		data.debug();
	}

	//output msr data for a given df_dm in the terminal (includes bitfield info, function below does not contain bitfield info)
	void printMSRs(std::string df_dm, std::string manufacturer = "INTEL")
	{
		data.printMSRs(df_dm, manufacturer);
	}

	//returns vector of df_dm
	std::vector<std::string > getDFDMsForMSR(std::string MSR_hex, std::string manufacturer = "INTEL")
	{
		return data.getDFDMsForMSR(MSR_hex, manufacturer);
	}

	//return vector of array of size 5, hex address, name, domain, description, table name
	std::vector<std::array<std::string, 5>> getMSRsForDFDM(std::string dfdm, std::string manufacturer = "INTEL")
	{
		return data.getMSRsForDFDM(dfdm, manufacturer);
	}

	//return array of strings (MSR hex address, Mask hex value, table name)	
	std::array<std::string, 3> getMask(std::string df_dm, std::string msr_hex, std::string manufacturer = "INTEL")
	{
		return data.getMask(df_dm, msr_hex, manufacturer);
	}

	void print_supported_df_dms()
	{
		data.supported_df_dms();
	}
};

