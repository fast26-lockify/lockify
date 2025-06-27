📌 **Note:**  
This artifact has been moved to the final repository:  
👉 https://github.com/skku-syslab/lockify  

Please refer to the above repository for the latest version and updates.  
**This document is no longer maintained here.**


# Understanding Linux Distributed Lock Management Overheads in Shared Storage (FAST '26)

Lockify is a novel distributed lock manager (DLM) for shared-disk file systems that reduces lock acquisition latency in the Linux kernel. It avoids unnecessary remote communication through self-owner notifications and asynchronous ownership management. Implemented in the Linux kernel and evaluated on GFS2 and OCFS2, Lockify achieves up to 6.4× higher throughput than the default DLM, even under low-contention workloads.

## Installation, Build, and Benchmark Setup

Lockify is implemented as an extension to the Linux kernel 6.6.23's DLM module. Follow the steps below to build the kernel and set up the evaluation environment:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/fast26-lockify/lockify.git
   cd lockify
   ```

2. **Apply Kernel Patch and Build:**
   - Apply the Lockify patch to your Linux kernel source.
   - Ensure the following kernel config options are enabled:
     - `CONFIG_NVME_TARGET_TCP`
     - `CONFIG_NVME_TCP`
   - Compile and install the patched kernel.

3. **Set Up Test Environment:**
   - Prepare a shared-disk cluster using either the GFS2 or OCFS2 file system.
   - Set up an NVMe-over-TCP environment between multiple nodes.

4. **Install Benchmarking Tools:**

The following benchmarking tools are required for evaluation:

- **mdtest (via IOR)**  
  https://github.com/hpc/ior

- **Postmark**  
  Installable via APT:
    sudo apt install postmark

- **Filebench**  
  https://github.com/filebench/filebench

- **xfstests**  
  https://github.com/kdave/xfstests

For all tools above, clone (or install) them from the provided URLs and follow the installation and build instructions in each repository’s README.
After installation, refer to the `scripts/` directory for example test scripts and automated evaluation routines.

### Notes

- Each script contains placeholder paths such as `/path/to/...`. Be sure to modify these paths according to your environment before execution.
- The mdtest script must be executed as a non-root user.
- For mdtest contention scenarios, make sure to edit the `hosts` file used by `mpirun` to match your cluster configuration.
- The scripts for mdtest, and xfstests are designed to be run from within each tool's source directory.
- xfstests cases — generic/430 ~ 434 — are observed to hang indefinitely or fail to complete within a reasonable time. We recommend excluding these cases when running xfstests during artifact evaluation.
- The File I/O experiment presented in **Figure 2** can be reproduced using the IOR benchmark included in the IOR project.

## Authors

- Taeyoung Park (Sungkyunkwan University) <pty3595@g.skku.edu>  
- Yunjae Jo (Sungkyunkwan University) <jack3319@g.skku.edu>  
- Daegyu Han (Sungkyunkwan University) <hdg9400@skku.edu>  
- Beomseok Nam (Sungkyunkwan University) <bnam@skku.edu>  
- Jaehyun Hwang (Sungkyunkwan University) <jh.hwang@skku.edu>

## Github Repository

For more information and the latest updates, please visit the GitHub repository:
https://github.com/fast26-lockify/lockify
