// Copyright 2023 Lawrence Livermore National Security, LLC and other
// msr-genie Project Developers. See the top-level LICENSE file for details.
//
// SPDX-License-Identifier: MIT

#include "Genie.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <array>
#include <cstring>

using namespace std;

unordered_set<string> df_dm_list
{
    "06_2AH",
    "06_2DH",
    "06_3AH",
    "06_3CH",
    "06_3DH",
    "06_3EH",
    "06_3FH",
    "06_45H",
    "06_46H",
    "06_47H",
    "06_4EH",
    "06_4FH",
    "06_55H",
    "06_56H",
    "06_5EH",
    "06_66H",
    "06_6AH",
    "06_6CH",
    "06_7DH",
    "06_7EH",
    "06_8CH",
    "06_8DH",
    "06_8EH",
    "06_8FH",
    "06_97H",
    "06_9AH",
    "06_9EH",
    "06_A5H",
    "06_A6H",
    "06_BFH"
};

unordered_set<string> amd_arch_list
{
	"zen3",	
};

void print_help()
{
    cout << "********************************************Notice**********************************************\n";
    cout << "LLNL collated this information from The Intel64 and IA-32 Architecture Software Developer's\n"
         << "Manual Volume 4:  Model-Specific Registers from [March 2023] document number [335592-079US].\n"
         << "While we took due care in its transcription, we might have made mistake or several.\n"
         << "For the most recent an authoritative documentation, please consult the most recent version of\n"
         << "Volume 4 at https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html\n"
         << "Corrections and amplifications are most welcome.\n\n"
         << "Also note that processor firmware may choose to disallow access to particular MSRs. The fact\n"
         << "that an MSR is listed here for a particular processor model does not guarantee that it will be\n"
         << "accessible on your processor.\n";

    cout << "************************************************************************************************\n";
    cout << "Incorrect usage! use one of the following options\n";
    cout << "Print list of supported df_dms: ./GenieCLI supported\n";
    cout << "Print MSR list for df_dm: ./GenieCLI msr df_dm*\n";
    cout << "Print df_dm list for MSR: ./GenieCLI df_dm msr*\n";
    cout << "Print bit-mask: ./GenieCLI bitmask df_dm* msr*\n";
    cout << "Dump all DataStore values: ./GenieCLI debug\n";
	cout << "Print AMD MSRs associated with an architecture (Only zen3 is currently supported): ./GenieCLI amd_msr *architecture*\n";
	cout << "Scan the current system for MSRs based on documentation as well as a range of possible address values for-\nundocumented MSRs (Only INTEL/AMD is supported on Linux systems with rdmsr and sudo access): ./GenieCLI msrscan\n";
    cout << "Create allowlist for MSR-Safe: ./GenieCLI allowlist\n\n";
    cout << "*replace with actual value of df_dm or msr being queried, note that bit-mask requires both df_dm\n";
    cout << "as well as the msr. If we with to search for MSRs associated with the df_dm \"06_4FH\" we use the\n";
    cout << "following command, ./GenieCLI msr 06_4FH\n";
    cout << "************************************************************************************************\n";
}

void print_msr(vector<array<string, 5> > &df_dm_ret, string df_dm)
{
    for (const auto &row : df_dm_ret)
    {
        std::cout << "Hex address: " << setw(12) << row[0] << "\tMSR name: " << row[1]
                  << "\tDomain: " << row[2] << "\tDescription: " << row[3] << "\tTable: " <<
                  row[4] << "\n";
    }
    std::cout << "number of entries for " << df_dm << ": " << df_dm_ret.size() <<
              "\n";
    std::cout <<
              "------------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

void print_df_dm(vector<string> &df_dms, string msr)
{
    std::cout << "number of dfdms associated with MSR " << msr << ": " <<
              std::to_string(df_dms.size()) << "\n";
    for (auto &dfdm : df_dms)
    {
        std::cout << dfdm << " ";
    }
    cout << "\n";
}

void print_bitmask(array<string, 3> &bitmask_ret)
{
    cout << "MSR address: " << setw(10) << bitmask_ret[0] << setw(
             18) << "\tMASK: " << bitmask_ret[1] << "\t Table: " << bitmask_ret[2] << "\n";
}

void print_amd_msrs(vector<array<string, 4> >&msrs)
{
	for (const auto &msr : msrs)
	{
		cout << msr[0] << " -- "  << msr[1];
		if (msr[2] != "")
		{
			cout << "\n" << msr[2];
		}
		if (msr[3] != "")
		{
			cout << "\n" << msr[3];
		}

		cout << "\n****************************************************\n\n";
	}
}

int main(int argc, char *argv[])
{
    GenieDataManager manager;
    if (argc < 2 or argc > 4)
    {
        print_help();
        return -1;
    }
    else if (strcmp(argv[1], "msr") == 0)
    {
        if (argc != 3)
        {
            print_help();
            return -1;
        }

        if (df_dm_list.find(argv[2]) != df_dm_list.end())
        {
            auto df_dm_ret = manager.getMSRsForDFDM(argv[2]);
            print_msr(df_dm_ret, argv[2]);
        }
        else
        {
            cout << "the df_dm you entered is not supported! Please make sure the df_dm is correct and in capital letters\n";
            return -1;
        }
    }

	else if (strcmp(argv[1], "amd_msr") == 0)
	{
		if (argc != 3)
		{
			print_help();
			return -1;
		}
		
		if (amd_arch_list.find(argv[2]) != amd_arch_list.end())
		{
			auto msr_ret = manager.getMSRsForAMD(argv[2]);
			print_amd_msrs(msr_ret);
		}
		else 	
		{
			cout << "the amd architecture you entered is not supported!\n";
			return -1;
		}
	}

    else if (strcmp(argv[1], "df_dm") == 0)
    {
        if (argc != 3)
        {
            print_help();
            return -1;
        }

        auto ret = manager.getDFDMsForMSR(argv[2]);
        print_df_dm(ret, argv[2]);
    }
    else if (strcmp(argv[1], "bitmask") == 0)
    {
        if (argc != 4)
        {
            print_help();
            return -1;
        }

        auto ret = manager.getMask(argv[2], argv[3]);
        print_bitmask(ret);
    }
    else if (strcmp(argv[1], "supported") == 0)
    {
        for (auto &df_dm : df_dm_list)
        {
            cout << df_dm << "\n";
        }
    }
    else if (strcmp(argv[1], "debug") == 0)
    {
        manager.debug();
    }
    else if (strcmp(argv[1], "allowlist") == 0)
    {
		system("mkdir safelist");
        manager.createIntelAllowlist();
		manager.createAMDAllowList();
    }
	else if (strcmp(argv[1], "msrscan") == 0)
	{
		manager.scanMSRs();
	}
    else
    {
        print_help();
        return -1;
    }

    return 0;
}
