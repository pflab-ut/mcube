# System

Mcube kernel is an operating system supporting dynamically switchable
kernels.
Mcube kernel can standardize OS for autonomous edge devices, in order
to reduce development cost and specialize the performance to guarantee
resource constraints.
The goal of Mcube kernel is to become a de facto research OS for autonomous edge
devices and contributes to practical realization of autonomous systems.


# Architectures

* SIM (SIMulation architecture in user-level execution environment)
* x86_64
* ARM Raspberry Pi3 (AARCH64)
* AXell Instruction Set (AXIS)

# Compilers

* LLVM
* GCC

# Scheduling

* User-Defined Fixed-Priority
* Rate Monotonic
* Earliest Deadline First

# Task Queues

* *O(1)*: Each priority task queue has priority-based double circular
  linked list like O(1) scheduler in Linux.  
* *O(logN)* Binominal Heap: One priority task queue manages binominal heap.
* *O(N)* Double Circular Linked List: One priority task queue has
  priority-based double circular linked list.

# Directories

* `arch/`: Architecture-dependent codes.
** `aarch64/`: AARCH64 codes.
** `axis/`: AXIS codes.
** `sim/`: SIM codes.
** `x86_64`: x86_64 codes.
* `docs/`: Documentations and sample configuration files.
** `config/`: Configuration files.
** `fig/`: Figures.
* `drivers/`: Device driver codes.
** `misc/`: Miscellaneous codes.
**`uart/`: UART codes.
* `fs/`: File systems.
** `ext2/`: Ext2 file systems.
** `fat/`: FAT file systems.
* `include/`: Header codes.
** `aarch64/`: AARCH64 header codes.
** `axis/`: AXIS header codes.
** `debug/`: Debug header codes.
** `fs/`: File systems header codes.
** `mcube/`: Main header codes.
** `net/`: Network header codes.
** `sim/`: SIM header codes.
** `x86_64/`: x86_64 header codes.
* `ipc/`: Inter-Process Communication codes.
* `kernel/`: Kernel codes.
** `queue/:` Queue codes.
* `lib/`: Library codes.
* `scripts/`: Script codes.
** `docker/:` Docker script codes.
** `kconfig/:` Kernel configuration script codes.
** `linker/:` Linker script codes.
** `misc/:` Miscellaneous script codes.
* `user/`: User-implemented codes.
** `aarch64/:` AARCH64 user codes.
** `axis/:` AXIS user codes.
** `sim/:` SIM user codes.
** `test/:` Test user codes.
** `x86_64/:` x86_64 user codes.
* `vmdk/`: VMware-related files.

# Files in Top Directory

* `CONTRIBUTING.md`: Contributing file
* `Doxyfile`: Doxygen file
* `Kconfig`: Kernel configuration file
* `LICENSE`: Licnese file
* `Makefile`: Makefile
* `README.md`: Readme file
* `Rules.mak`: Makefile-related rule file
* `astylerc`: `astyle` configuration file
* `configure`: configuration file
* `pylintrc`: `pylint` configuration file
