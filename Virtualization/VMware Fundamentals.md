# Why Virtualization
- Higher server ability
- Cheaper operating costs
- Eliminate server complexity 
- Increased application performance
- Deploy workload quicker
### Physical VS Virtual Architecture
Virtualization technology abstracts physical components into software components and provides solutions for many IT problems.
![[Pasted image 20250112111024.png]]
## Virtual Architecture
Core Components -> 
- <u>ESXi</u> is the hypervisor software that extracts the computer's hardware and presents it as virtual hardware to VMs
- <u>vCenter</u> which is a server that manages the ESXi hosts that are connected on a network
- ### Types of Hypervisors
| Bare-Metal Hypervisor (Type 1)              | Hosted Hypervisor (Type2)                                |
| ------------------------------------------- | -------------------------------------------------------- |
| Installed <u>directly</u> on the hardware   | Installed on an OS (windows, linux)                      |
| Easier to maintain than a hosted hypervisor | Treat the hypervisor as an application running on the OS |
| Example: VMware ESXi                        | Example: VMware Workstation                              |
### ESXi Host
<u>Using the Hypervisor</u>
Before you can create and run VMs, you must install the hypervisor on a host or physical machine.
1. Use a media device that contains the software installer and mount it to the physical host.
2. Connect a display and a keyboard to the host to access the ESXi console
3. Manage the ESXi installation process from the console
The console is mainly used for initial setup, and limited troubleshooting

