# System

Mcube kernel is an operating system supporting dynamically switchable
kernels.
Mcube kernel can standardize OS for autonomous edge devices, in order
to reduce development cost and specialize the performance to guarantee
resource constraints.
Mcube kernel becomes a de facto standard OS for autonomous edge
devices and contributes to practical realization of autonomous
systems.

# Architectures

* SIM
* x86
* ARM Raspberry Pi3
* ARM Synquacer
* AXIS

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

* `arch/`: Architecture-dependence codes
* `docs/`: Documentations
* `drivers/`: Device driver codes
* `fs/`: File systems
* `include/`: header codes
* `ipc/`: Inter-Process Communication codes
* `kernel/`: Kernel codes
* `lib/`: Library codes
* `scripts/`: Script codes
* `user/`: User-implemented codes
* `vmdk/`: VMware-related files

# Files in Top Directory

* `Doxyfile`: Doxygen file
* `Kconfig`: Kernel configuration file
* `LICENSE`: Licnese file
* `Makefile`: Makefile
* `README.md`: Readme file
* `Rules.mak`: Makefile-related rule file
* `astylerc`: `astyle` configuration file
* `configure`: configuration file
* `pylintrc`: `pylint` configuration file
