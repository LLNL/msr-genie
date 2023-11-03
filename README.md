msr-genie
=========

MSR-Genie is a vendor-neutral tool for fast and efficient queries about model-specific
registers (MSRs). The tool allows end users to query bi-directionally across MSR lists
as well as processor families and models, and provides them with guidance on appropriate
bitmasks.

GenieInterface is an interactive terminal-based interface to msr-genie. Upon execution
of the program with ``./GenieInterface``, you will be greeted with a menu consisting of
the following options.

```console
(1) Debug
(2) Print MSR associated with df_dm (displayfamily_displaymodel)
(3) Print list of df_dm associated with MSR
(4) Get BitMask
(s) print list of supported df_dms
(c) clear screen
(h) help
(q) quit
```

Enter the corresponding number or letter for the option you wish to use and press enter
to proceed. The menu will be reprinted after each selection.

GenieCLI is a command line interface to msr-genie. There are currently five (5) options
for the CLI version.

1. Print list of supported df_dms: ``./GenieCLI supported``
2. Print MSR list for df_dm: ``./GenieCLI [msr] [df_dm]``
3. Print df_dm list for MSR: ``./GenieCLI [df_dm] [msr]``
4. Print bitmask: ``./GenieCLI bitmask [df_dm] [msr]``
5. Dump all DataStore values: ``./GenieCLI debug``
6. Create MSR allowlist template: ``./GenieCLI allowlist``

## Building GenieCLI and GenieInterface

A makefile is provided for GenieCLI and GenieInterface. If your use case requires direct
linkage to the header file, simply ``#include "Genie.h"``. There are no dependencies
beyond a compatible C++ compiler.

## MSR-Safe Allowlist Template Generator

MSR-Safe allow list templates can be generated using the ./GenieCLI allowlist command. 
By default, it expects a directory called "safelist" within the msr-genie directory.

## License

MSR-Genie is distributed under the terms of the MIT license.

See [LICENSE](https://github.com/llnl/msr-genie/blob/main/LICENSE) and
[NOTICE](https://github.com/llnl/msr-genie/blob/main/NOTICE) for details.

SPDX-License-Identifier: MIT

LLNL-CODE-856773

## References

LLNL collated this information from The Intel64 and IA-32 Architecture Software
Developer's Manual Volume 4:  Model-Specific Registers from [March 2023] document number
[335592-079US].  While we took due care in its transcription, we might have made mistake
or several. For the most recent an authoritative documentation, please consult the most
recent version of Volume 4 at
https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
Corrections and amplifications are most welcome.

Also note that processor firmware may choose to disallow access to particular MSRs.  The
fact that an MSR is listed here for a particular processor model does not guarantee that
it will be accessible on your processor.
