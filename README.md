LLNL collated this information from The Intel64 and IA-32 Architecture Software Developer's Manual Volume 4: Model-Specific Registers from [March 2023] document number [335592-079US].
While we took due care in its transcription, we might have made mistake or several. For the most recent an authoritative documentation, please consult the most recent version of Volume 4 at https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html Corrections and amplifications are most welcome.

Also note that processor firmware may choose to disallow access to particular MSRs. The fact that an MSR is listed here for a particular processor model does not guarantee that it will be accessible on your processor.

Makefile is provided for GenieCLI and GenieInterface. If your usecase requires direct linkage to the header file, simply #include "Genie.h". There are no dependencies beyond a compatible C++ compiler.
