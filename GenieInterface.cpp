#include "Genie.h"
#include <vector>
#include <iostream>


using namespace std;

void print_menu(){
	cout << "please enter one of the following options: \n";
	cout << "(1) Debug\n";
	cout << "(2) Print MSR associated with df_dm (displayfamily_displaymodel)\n";
	cout << "(3) Print list of df_dm associated with MSR\n";
	cout << "(4) Get BitMask\n";
	cout << "(s) print list of supported df_dms\n";
	cout << "(c) clear screen\n";
	cout << "(h) help\n";
	cout << "(q) quit\n";
}

void print_msr(GenieDataManager &manager)	{
	string in = "";
	cout << "To print associated MSRs, please enter the df_dm of the processor (e.g. 06_2AH)\n";
	cin >> in;
	manager.printMSRs(in);
}

void get_df_dm(GenieDataManager &manager)	{
	string in = "";
	cout << "To print associated df_dms for a MSR, please enter the MSR (e.g. 611H)\n";
	cin >> in;
	auto vec = manager.getDFDMsForMSR(in);
	std::cout << "number of dfdms associated with MSR " <<in << ": " << std::to_string(vec.size()) << "\n";
	for(auto & dfdm : vec) {
		std::cout << dfdm << "\n";
	}
}


void get_bitmask(GenieDataManager &manager)	{
	string df_dm = "";
	string msr = "";
	cout << "Please enter the df_dm for the bitmask (e.g. 06_2AH)\n";
	cin >> df_dm;
	cout << "Please enter the MSR for the bitmask (e.g. 17AH)\n";
	cin >> msr;
	auto ret = manager.getMask(df_dm, msr);
	cout << "MSR address: " << setw(18) << ret[0] << setw(18) << "\tMASK: " << ret[1] << "\t Table: " << ret[2] << "\n";
}

void help() {

	cout << "********************************************Notice**********************************************\n";
	cout << "LLNL collated this information from The Intel64 and IA-32 Architecture Software Developer's\n"
		 << "Manual Volume 4:  Model-Specific Registers from [March 2023] document number [335592-079US].\n"
		 <<	"While we took due care in its transcription, we might have made mistake or several.\n"
		 << "For the most recent an authoritative documentation, please consult the most recent version of\n"
		 <<	"Volume 4 at https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html\n"
		 <<	"Corrections and amplifications are most welcome.\n\n"
		 <<	"Also note that processor firmware may choose to disallow access to particular MSRs. The fact\n"
		 <<	"that an MSR is listed here for a particular processor model does not guarantee that it will be\n"
		 << "accessible on your processor.\n"
		 << "************************************************************************************************\n";

	cout << "Debug options dumps all MSR data currently stored in memory. First block is a list containing \n"
		 << "table names, number of MSRs associated with the table and the table address in memory\n"
		 << "Second block is a mapping of df_dm to memory addresses of associated tables (this is for program\n" 
		 << "debug purposes, not useful to the user) Third block contains all of the MSRs including the hex\n"
		 << "-address, name, the table it is sourced from and associated df_dms\n\n"
		 << "Print MSR option takes in a df_dm and outputs all associated MSRs\n\n"
		 << "Print list of df_dm option takes in a MSR hex address and outputs the associated df_dms\n\n"
		 << "Get bitmask takes in a df_dm and a MSR hex address and outputs the bitmask (Currently only works\n"
		 << "on MSRs without variable length bitfields)\n\n"
		 << "To check your machine's df_dm (Intel), run \"lscpu\" in ther terminal and convert the values\n" 
		 << "under family and model to hex then append an H to the end\n(e.g. family: 6, model: 79 --> 06_4FH)\n"    
		 << "*************************************************************************************************\n";
}

int main() {

	GenieDataManager manager;
	string in = "";

	while(1) {
		print_menu();
		cin >> in;
		if(in == "1"){
			manager.debug();
		}
		else if(in == "2"){
			print_msr(manager);
		}
		else if(in == "3"){
			get_df_dm(manager);
		}
		else if(in == "4"){
			get_bitmask(manager);
		}
		else if(in == "s"){
			manager.print_supported_df_dms();
		}
		else if(in == "h" or in == "help"){
			help();
		}
		else if(in == "c" or in == "clear"){
			system("clear");
			continue;
		}
		else if(in == "q" or in == "quit"){
			break;
		} else {
			continue;
		}
	
	}


}
