************************************************************************************************
LLNL collated this information from The Intel64 and IA-32 Architecture Software Developer's 
Manual Volume 4:  Model-Specific Registers from [March 2023] document number [335592-079US].  
While we took due care in its transcription, we might have made mistake or several. For the 
most recent an authoritative documentation, please consult the most recent version of
Volume 4 at https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
Corrections and amplifications are most welcome.

Also note that processor firmware may choose to disallow access to particular MSRs.  The fact
that an MSR is listed here for a particular processor model does not guarantee that it will be
accessible on your processor.
************************************************************************************************

A Makefile is provided for GenieCLI and GenieInterface. If your usecase requires direct linkage 
to the header file, simply #include "Genie.h". There are no dependencies beyond a compatible C++ 
compiler.

************************************************************************************************
GenieInterface is an interactive terminal based interface to msr-genie. Upon execution of the 
program with ./GenieInterface you will be greeted with a menu consisting of the following 
options.

(1) Debug
(2) Print MSR associated with df_dm (displayfamily_displaymodel)
(3) Print list of df_dm associated with MSR
(4) Get BitMask
(s) print list of supported df_dms
(c) clear screen
(h) help
(q) quit

Enter the correspoding number or letter for the option you wish to use and press enter to proceed.
The menu will be reprinted after each selection.

***********************************************************************************************

GenieCLI is a command line interface to msr-genie. There are currently five (5) options for the 
CLI version.

Print list of supported df_dms: ./GenieCLI supported
Print MSR list for df_dm: ./GenieCLI msr df_dm*
Print df_dm list for MSR: ./GenieCLI df_dm msr*
Print bit-mask: ./GenieCLI bitmask df_dm* msr*
Dump all DataStore values: ./GenieCLI debug

*replace with actual df_dm or msr being queried. ie. to search for msrs associated with 
df_dm = 06_4FH. we run the program with the command ./Genie msr 06_4FH. bit-mask requires both
df_dm and msr information.


## Template Maker

templatemaker.py is a small utility that can autogenerate MSR-Safe allowlist
templates from msr-genie scrapes. 

templatemaker by default expects scrapes to be in ./Intel_MSRs/blr by default
this can be updated by altering the filepath at the top of the file.

templatemaker will display a list of architectures you can select and create an
allowlist for.

Allowlists will be placed in ./templates/ by default.
