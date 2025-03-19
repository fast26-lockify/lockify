# Lockify

Lockify is a novel distributed lock manager designed to reduce lock acquisition overheads in shared-storage environments under Linux. This implementation enhances performance on shared-disk file systems such as GFS2 and OCFS2 by mitigating unnecessary remote communication, even in low-contention scenarios.

## Overview

- **Title:** Understanding Linux Distributed Lock Management Overheads in Shared Storage  

- **Key Idea:**  
  - The existing Linux kernel DLM suffers from increased lock acquisition latency due to excessive remote communication as the number of clients grows.
  - Lockify minimizes this overhead by introducing self-owner notifications and asynchronous ownership management, allowing the node creating a new file or directory to immediately declare ownership.

- **Performance Improvements:**  
  - Achieves up to approximately 6.4× throughput improvement in low-contention scenarios compared to the traditional kernel DLM.

## Main Features

- **Self-Owner Notification:**  
  - During file or directory creation, the creating node immediately notifies the directory node of self-ownership, bypassing the typical owner lookup process.

- **Extended Lock Acquisition Interface:**  
  - Introduces a "NOTIFY" flag in the `dlm_lock(...)` interface, enabling the self-owner notification for new lock objects.

- **Asynchronous Ownership Management:**  
  - Manages confirmation responses from the directory node asynchronously, using a timer-based retransmission mechanism to ensure consistency.

## Evaluation Summary

- **Test Environment:**  
  - Evaluated on GFS2 and OCFS2 file systems within an NVMe-over-TCP testbed.
  - A testbed consisting of 5 servers.
 
- **Experimental Results:**  
  - In a low-contention setup (e.g., with 5 clients), Lockify improves OCFS2 throughput by approximately 2.9× and GFS2 throughput by about 6.4×.
  - It significantly reduces the proportion of lock acquisition latency, thus enhancing overall system throughput.
 
## Installation and Build Instructions

Lockify is implemented as an extension to the Linux kernel's DLM module. Below is a basic guideline for building and testing the patch:

1. **Clone the Repository:**  
   ```bash
   git clone https://github.com/fast26-lockify/lockify.git
   cd lockify
   ```

2. **Apply Kernel Patch and Build:**  
   - Compile the kernel with the Lockify extension.
   - Ensure that your kernel's .config file has CONFIG_NVME_TARGET_TCP, CONFIG_NVME_TCP options enabled.

3. **Set Up Test Environment:**  
   - Configure your system to use either the GFS2 or OCFS2 file system and set up an NVMe-over-TCP environment.
   - Use benchmarking tools such as mdtest, Postmark, and Filebench to evaluate performance.
  
## Github Repository

For more information and the latest updates, please visit the GitHub repository:
https://github.com/fast26-lockify/lockify
