# Copyright 2011-2023 Lawrence Livermore National Security, LLC and other
# msr-safe Project Developers. See the top-level COPYRIGHT file for
# details.
#
# SPDX-License-Identifier: GPL-2.0-only

import os
import collections
import re

scrap_dir = 'Intel_MSRs/blr'

docname = ("volume 4 of the Intel 64 and IA-32 Architectures\n"
           "## Software Development Manual (335592-079US March 2023)")

class MSRMap:
    def __init__(self, directory=scrap_dir):
        self.msrs = {}
        self.file_indices = {}
        self.msr_names = {}
        self.read_all_files(directory)
        self.sorted = False

    def add_msr(self, msr, fname, name):
        self.sorted = False
        # Treats (msr, arch) as unique (Ik Ik...)
        if msr not in self.msrs:
            self.msrs[msr] = []
        self.msrs[msr].append(fname)

        if msr not in self.msr_names:
            self.msr_names[msr] = {}
        self.msr_names[msr][fname] = name

        # Inverted index
        if fname not in self.file_indices:
            self.file_indices[fname] = []
        self.file_indices[fname].append(msr)


    def get_table(self, msr):
        if not self.sorted:
            self.sort()
        return self.msrs.get(msr, [])
    

    def get_msrs(self, fname):
        if not self.sorted:
            self.sort()
        return self.file_indices.get(fname, [])


    def read_all_files(self, directory):
        for filename in os.listdir(directory):
            if filename.endswith(".txt"):
                with open(os.path.join(directory, filename), 'r') as f:
                    for line in f:
                        stripped_line = line.lstrip()  # lstrip() removes leading whitespaces
                        if stripped_line and re.match(r'^[0-9A-Fa-f]+H', stripped_line):  # checks if the line starts with a hexadecimal number followed by 'H'
                            parts = stripped_line.split('\t')
                            msr = parts[0].rstrip('H')
                            if(len(parts) > 2):
                                name = parts[2].strip() 
                            else:
                                name = parts[1].strip() if len(parts) > 1 else ''
                            try:
                                msr_int = int(msr, 16)
                                file_name_without_ext = os.path.splitext(filename)[0]
                                self.add_msr(msr_int, file_name_without_ext, name)
                            except ValueError:
                                print('Invalid MSR: ' + msr)
        self.sort()


    def sort(self):
        for key in self.msrs:
            self.msrs[key].sort()
        self.msrs = collections.OrderedDict(sorted(self.msrs.items()))
        for key in self.file_indices:
            self.file_indices[key].sort()
        self.file_indices = collections.OrderedDict(sorted(self.file_indices.items()))
        self.sorted = True

    def generate_sorted_unique_msrs_for_file_list(self, table_list):
        msrs = []
        for table in table_list:
            file_msrs = self.get_msrs(table)
            #print(file_msrs)
            for msr in file_msrs:
                #msr_cat = self.get_categories(msr)
                msr_name = self.msr_names.get(msr, {}).get(table, "")
                #print(msr_name)
                msrs.append((msr, msr_name))  # Append a tuple containing the MSR name & table

        # Remove duplicates and sort
        msrs = list(set(msrs))
        
        msrs.sort()
        return msrs

    def msr_arch_filter(self, key, table_list):
        key, value = pair
        if key in table_list:
            return true
        return False

    def check_duplicate_name(self, msr, table_list):
        names = {}
        for table in table_list:
            a_name = self.msr_names.get(msr, {}).get(table, "") 
            if a_name != '':
                names[table] = a_name 
        namess = set([name for name in names.values()])
        if len(namess) != 1: 
            return names

        return None

df_dm = {
    "06_2A": ["2-20", "2-21", "2-22"],
    "06_2D": ["2-20", "2-23", "2-24"],
    "06_3A": ["2-20", "2-21", "2-22", "2-25"],
    "06_3C": ["2-20", "2-21", "2-22", "2-29", "2-30"],
    "06_3D": ["2-20", "2-21", "2-22", "2-25", "2-29", "2-30", "2-34", "2-35"],
    "06_3E": ["2-20", "2-24", "2-26", "2-27", "2-28"],
    "06_3F": ["2-20", "2-29", "2-32", "2-33"],
    "06_45": ["2-20", "2-21", "2-22", "2-29", "2-30", "2-31", "2-38"],
    "06_46": ["2-20", "2-21", "2-22", "2-29", "2-30"],
    "06_47": ["2-20", "2-21", "2-22", "2-25", "2-29", "2-30", "2-34", "2-35"],
    "06_4E": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-40"],
    "06_4F": ["2-20", "2-21", "2-29", "2-34", "2-36", "2-37", "2-38"],
    "06_55": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-50"],
    "06_56": ["2-20", "2-29", "2-34", "2-36", "2-37"],
    "06_5E": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-40"],
    "06_66": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-40", "2-42", "2-43"],
    "06_6A": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-51"],
    "06_6C": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39"],
    "06_7D": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-44"],
    "06_7E": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-44"],
    "06_8C": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-45"],
    "06_8D": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-45"],
    "06_8E": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-40"],
    "06_8F": ["2-52"],
    "06_97": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-46", "2-44", "2-45", "2-47", "2-48", "2-49"],
    "06_9A": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-46", "2-44", "2-45", "2-47", "2-48", "2-49"],
    "06_9E": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-40", "2-41"],
    "06_A5": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39"],
    "06_A6": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39"],
    "06_BF": ["2-20", "2-21", "2-25", "2-29", "2-35", "2-39", "2-46", "2-44", "2-45", "2-47", "2-48", "2-49"]
}


def find_largest_table(nums):
    def parse_num(num_str):
        a, b = map(int, num_str.split('-'))
        return a, b
    
    if len(nums) > 1:
       return max(nums, key=parse_num)
    return nums[0]


msr_map = MSRMap()

def write_msrs_to_file(msrs, filename, architecture, directory='templates'):
    errata_list = []

    with open(os.path.join(directory, "al_"+filename), 'w') as f:
        f.write(f"## This file contains the model-specific registers available in {filename} processors\n"
                f"## based on a close reading of {docname}.\n"  
                "## Uncommenting allows reading a particular MSR.\n"  
                "## Modifying the write mask allows writing to those particular bits.\n"
                "## Be sure to cat the modified list into /dev/cpu/msr_allowlist.\n"  
                "## See the README file for more details.\n##\n")
        f.write("# MSR        # Write Mask       # Comment\n")

        for msr, name in msrs:
            if msr not in errata_list:
                try: 
                    err = msr_map.check_duplicate_name(msr, df_dm[architecture])
                    #print(err)
                    if err is not None:
                        #print(err)
                        for key in err:
                            table = key
                        name = err[table]
                        errata_list.append(msr)
                    else:
                        table = msr_map.get_table(msr);
                        #print(table)
                        table = list(set(table).intersection(df_dm[architecture]))
                        table = find_largest_table(table)
                    #print(msr, table)
                    
                    #print(table)
                    f.write('# 0x{0:08X} 0x0000000000000000 # "{1} (Table: {2})"\n'.format(msr, name, table))
                #f.write(f'# 0x{0:08X} 0x0000000000000000 #'.format(msr))
                #for dup in errata:
                #    if msr == dup[0]:
                #        f.write(f"ERRATA duplicate MSR {dup[0]:#X} address with different labels:")
                #        f.write(f"Name: {dup[1][1]} Table: {dup[1][0]} and Name: {dup[2][1]} table:{dup[2][0]}\n")
                #        errata.remove(dup)
                #        break

                #f.write(f'{1} (Table: {2})"\n'.format(name, cat))
                except ValueError:
                    print('Invalid MSR: ' + msr)
            else:
                print(f'ignored msr {msr:#X} as it has been determined to be errata, it\'s most recent definition will be used')

    print('Template written to templates/' + filename)

def main():
    print('Welcome to the MSR allow-list template generator') 

    while True:
        print('Available architectures:')
        for architecture in df_dm.keys():
            print(architecture)
        print('Enter architecture name to generate template for (A for all):')
        architecture = input()
        if architecture == 'A':
            architectures = df_dm.keys()
            for architecture in architectures:
                msrs = msr_map.generate_sorted_unique_msrs_for_file_list(df_dm[architecture])
                filename = architecture
                write_msrs_to_file(msrs, filename, architecture)
            break

        elif architecture not in df_dm:
            print('Architecture not found')
            continue
        else:
            msrs = msr_map.generate_sorted_unique_msrs_for_file_list(df_dm[architecture])
            print('Enter output filename:')
            filename = input()
            with open(os.path.join('templates', filename), 'w'):
                write_msrs_to_file(msrs, filename, architecture)
            print('Template written to templates/' + filename)

if __name__ == '__main__':
    main()