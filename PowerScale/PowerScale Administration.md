# PowerScale
## PowerScale Nodes
PowerScale nodes are scale-out NAS cluster comprised of compute nodes forming node pools. Each node type has unique computer and storage capabilities which is designed for large volumes of unstructured data.
<u>F type</u> 
- All Flash Nodes
- Ultra Compute and High Capacity
- Latency remains predictable when cluster scales
<u>H type</u>
- Hybrid Nodes
- Highly flexible
- Maintains balance between large capacity and large storage
<u>A type</u> 
- Archive Nodes
- Highly efficient and resilient for data archiving.
- Can be combined with other types of nodes for tiered storage

| **Attribute**   | **Description**                                                                                | **Benefits**                                                                              |
| --------------- | ---------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| Scalability     | - Distributed fully symmetric clustered architecture<br>- A cluster can scale up for 252 nodes | - Increased performance per usable Terabytes<br>- Enables lower latency                   |
| Performance     | - Data reduction for storage efficiency<br>- Policy Based automated tiering                    | - Predictable performance at scale<br>- Increased storage efficiency                      |
| Data Protection | - Built-in protection design<br>- Forward Error Correction and data mirroring                  | - Eliminating single point of failure<br>- Predictable Failure handling                   |
| Sizing          | - Dedicated cache drives<br>- Converged architecture<br>- Non disruptive hardware replacement  | - Simple Growth path<br>- Customizable solution<br>- Same building block for all clusters |
## Frontend - External Network
- Clients connect to the cluster using Ethernet connections that is available on all nodes
- The complete cluster is combined with hardware, software, and networks.
![[Pasted image 20241218110144.png]]
## Backend - Internal Network
- OneFS supports a single cluster on internal network
- The networking switches provide low latency connection between devices
![[Pasted image 20241218110621.png]]

## Cluster Sizes
| Size        | Number of Devices |
| ----------- | ----------------- |
| Small       | 3-32              |
| Medium      | 32-48             |
| Large       | 48-144            |
| Extra Large | 144-252           |
# PowerScale Management Interfaces
## Serial Console Video
Support the following commands with the iDRAC (Dell Remote Access Control):
- Shutdown (power off)
- Reboot (power cycle)
- Startup (power on)
- Power Status 
This configuration Wizard automatically starts when a node is first powered on or reformatted. 

## Web Administration Interface (WebUI)
This is a graphical interface that is used to manage the cluster. This requires at lease one IP address that is configured to be on one external Ethernet port on <u>each</u> node. 
The WebUI is commonly shared through port 8080.
- The user must log in using the root account, or an admin account with  a login privilege

## Command Line Interface (CLI)
The CLI is an ssh connection to the OneFS terminal, this terminal is built on top of a Unix environment.
The OneFS commands can be used with the Unix commands.
1. The default shell in zsh
2. OneFS is built on FreeBSD
3. OneFS supports _isi_ commands.
4. This is a scriptable interface, the UNIX shell enables scripting and execution of many UNIX and OneFS commands.

The _main isi_ or _isi --help_ is an important command for a new administrator to get familiar with all CLI commands.

The _isi config_ command opens a configuration console. The console contains configured settings from the time the Wizard started.

## Platform Application Programming Interface
The OneFS API, is a secure and scriptable interface for managing the cluster. HTTPS is used in the API to encrypt communication. OneFS applies authentication and RBAC controls to API commands to ensure authorized use.

## Front Panel Display 
This is a LCD screen with five buttons that are used for basic administration tasks.

Administration commands include:
- Adding a node to the cluster
- Checking node or driver status
- Events
- Cluster details 
- Capacity 
- IP
- Mac address

# Common Cluster Operations
## Licensing
A license file contains a record of the active cluster software license and the cluster hardware. One license in contains in dell and one on the cluster and they must match.

## Device ID and Logical Node Number
There are two different numbers that identify a node. The numbers are device ID and Logical Node Number (LNN)

The _status advanced_ command from the _isi config_ sub-menu shows the LNNs and  device ID

