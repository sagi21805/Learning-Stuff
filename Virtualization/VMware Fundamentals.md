# Why Virtualization
- Higher server ability
- Cheaper operating costs
- Eliminate server complexity 
- Increased application performance
- Deploy workload quicker
### Physical VS Virtual Architecture
Virtualization technology abstracts physical components into software components and provides solutions for many IT problems.
![[Pasted image 20250112111024.png]]
# Virtual Architecture
Core Components -> 
- <u>ESXi</u> is the hypervisor software that extracts the computer's hardware and presents it as virtual hardware to VMs
- <u>vCenter</u> which is a server that manages the ESXi hosts that are connected on a network
- ### Types of Hypervisors
| Bare-Metal Hypervisor (Type 1)              | Hosted Hypervisor (Type2)                                |
| ------------------------------------------- | -------------------------------------------------------- |
| Installed <u>directly</u> on the hardware   | Installed on an OS (windows, linux)                      |
| Easier to maintain than a hosted hypervisor | Treat the hypervisor as an application running on the OS |
| Example: VMware ESXi                        | Example: VMware Workstation                              |
## ESXi Host
<u>Using the Hypervisor</u>
Before you can create and run VMs, you must install the hypervisor on a host or physical machine.
1. Use a media device that contains the software installer and mount it to the physical host.
2. Connect a display and a keyboard to the host to access the ESXi console
3. Manage the ESXi installation process from the console
The console is mainly used for initial setup, and limited troubleshooting

## vCenter Server
We use the vSphere client to log in to vCenter Server and manage the vSphere environment.
- Move VMs from one host to another
- Move VMs from one datastore to another
- View all the ESXi hosts that are part of a cluster
- Configure hosts, vCenter, VM, Networks etc.

## vCenter Features
The most common features are 
- vMotion
- High Availability
- vSphere Scheduler 
- Power management
- Fault Tolerance
- Storage vMotion
- Virtual Switch
- vCenter High Avilability

# Virtual Machine on ESXi
Any application in any supported OS can run in a VM (guest) and consume CPU, memory, disk, and network from host-based resoureces.
![[Pasted image 20250112161750.png]]
## Virtual Machine Files
A VM includes a set of relative files

| File Type                   | File Name                            |
| --------------------------- | ------------------------------------ |
| Configuration file          | <VM_name>.vmx                        |
| Swap files                  | <VM_name>.vswp<br><vmx-VM_name>.vswp |
| BIOS file                   | <VM_name>.nvram                      |
| Log files                   | vmware.log                           |
| Template configuration file | <VM_name>.vmtx                       |
| Disk descriptor file        | <VM_name>.vmdk                       |
| Disk data file              | <VM_name-flat>.vmdk                  |
| Suspend state file          | <VM_name>-*.vmss                     |
## CPU and Memory
You can add, change, or configure CPU and memory resources to improve VM performance. The maximum number of virtual CPUs (vCPUs) that can assign to a VM depends on the following factors: 
- The number of logical CPUs on the host
- The type of installed guest operating system
A VM running on an ESXi 7.0 host can have up to 256 vCPUs
The maximum memory size of a VM with ESXi 7 is 6TB
## Virtual Storage 
Virtual disks are connected to virtual storage adapters.
The ESXi hosts offers VMs several choices in storage adapters.
## Thick-Provisioned and Thin-Provisioned Disks
| Categories                                 | Thick Provision Lazy-Zeroed                                     | Thick-Provisioned Eager-Zeroed                                  | Thin Provisioned                                                   |
| ------------------------------------------ | --------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------------------ |
| <u>Creation Time</u>                       | Fast.                                                           | Slow and proportional to disk size.                             | Fastest.                                                           |
| <u>Block Allocation</u>                    | Fully preallocated                                              | Fully preallocated                                              | Allocated and zeroed out on demand at first write to block         |
| <u>Virtual Disk Layout</u>                 | Higher chance of contiguous file blocks                         | Higher change of contiguous file blocks                         | Layout varies according to the dynamic state of the volume at time |
| <u>Zeroing out of allocated file block</u> | File blocks are zeroed out when each block is first written to. | File blocks are allocated and zeroed out when disks are created | File blocks are zeroed out when blocks are allocated               |
## Virtual Network
VMs and physical machines communicate through a virtual network.
When you configure networking for a VM, you select or change the following setting: 
- Network adapter type
- Port group to connect to
- Network connection state
- Whether to connect to the network when the VM power on

## Network Adapter Types
<u>E1000-E1000E</u> - Emulated version of an Intel Gigabit Ethernet NIC
<u>VMXNET3</u> - Available only with VMware Tools 
<u>Flexible</u> - Can function as either a Vlance or VMXNET adapter
<u>SR-IOV pass-through</u> - Allows VM and physical adapter to exchange data without using the VMKernel as in intermediary
<u>vSphere DirectPath I/O</u> - Allows VM access to physical PCI network functions on platforms with an I/O memory management unit
<u>PVRDMA</u> - Para-virtualized device that provides improved virtual device performance. It provides an RDMA-like interface for vSphere guests
## Other Virtual Devices
A VM must have a vCPU and virtual memory. The addition of other virtual device makes the VM more useful.
- <u>CD/DVD drive:</u> For connecting to a CD, DVD, or ISO image
- <u>USB 3.0 and 3.1:</u> Supported with host-connected and client-connected devices
- <u>Floppy drive:</u> For connecting a VM to a floppy drive or floppy image
- <u>Generic SCSI devices:</u> A VM can be connected to additional SCSI adapters
- <u>vGPUs:</u> A VM can use GPUs on the physical host for high-computation activities
# Networking
## Manage Virtual Network
- When network is configured properly, virtual machines can communicate with other virtual/physical machines
- Virtual switches connect to physical network
- Provide connectivity between VMs on the same ESXi host or different one
- Support VMkernel services like vMotion, vSphere, iSCSI, NFS
## Types of Virtual Switches
| Standard Switch                                     | Distributed Switch                                                          |
| --------------------------------------------------- | --------------------------------------------------------------------------- |
| Virtual switch that is configured for a single host | Virtual switch that is configured for the entire data center                |
|                                                     | Up to 2000 hosts can be attached to the same switch                         |
|                                                     | The configuration is consistent across all hosts                            |
|                                                     | Hosts must either have an Enterprise Plus license or belong to vSAN cluster |
|                                                     |                                                                             |
## Type of Virtual Switch Connections
A virtual switch has specific connection types
- VM group ports
- VMkernel port: For IP storage, vSphere, vSphere motion, vSphere Fault Tolerance, vSAN, vSphere Replication, and ESXi management network.
- Uplink ports
# Storage
## Datastore
This is a logical storage unit that can use disk space on one physical device or span on several physical devices. Datastores are used to hold 
- VM Files
- VM Templates
- ISO Images
- VMFS (Virtual Machine File System)
- vSAN
- vSphere Virtual Volumes
## Storage Overview
ESXi hosts should be configured with shared access to datastores
![[Pasted image 20250114095332.png]]
## Storage Protocol Overview
Each datastore uses a protocol with varying support features

| Datastore Type | Storage Protocol         | SAN | vMotion | HA  | DRS |
| -------------- | ------------------------ | --- | ------- | --- | --- |
| VMFS           | Fiber Channel            | yes | yes     | yes | yes |
| VMFS           | FCoE                     | yes | yes     | yes | yes |
| VMFS           | iSCSI                    | yes | yes     | yes | yes |
| VMFS           | iSER/NVME-of (RDMA)      | no  | yes     | yes | yes |
| VMFS           | DAS (SAS, SATA, NVMe)    | N/A | yes     | no  | no  |
| NFS            | NFS                      | no  | yes     | yes | yes |
| Virtual Volume | FC/Ethernet (iSCSI, NFS) | no  | yes     | yes | yes |
| vSAN Datastore | vSAN                     | no  | yes     | yes | yes |
## Physical Storage Considerations
Before implementing your vSphere environment, the following things must be considered
- LUN sizes
- I/O bandwidth required by applications
- I/O requests per second
- Disk cache parameters
- Zoning and masking
- Multipathing setting for your storage arrays (active-active (When writing information on two line, use both for the write) or active-passive (When writing information on two lines use only one, and use the other one as a backup))
- Export properties for NFS datastore
# Virtual Machine Management
- Virtual machines are the foundation on virtual infrastructure
- Managing VMs effectively requires skills in creating templates and clones, modifying VMs, taking snapshots, and protecting the VMs through replications and backups.
## Modifying Virtual Machine Settings
- Add virtual hardware (Some hardware can be added while the VM is powered on)
- Remove virtual hardware (Some hardware can be removed only when the VM is off)
- Set VM options
- Control the VM CPU and memory resources.
## Removing VMs 



