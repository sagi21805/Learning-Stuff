## Data Storage

### <u>Types of Data Storage</u>
There are two types of data storage, Structured and Unstructured PowerScale specialized in storing **unstructured data**

![[Pasted image 20241215140544.png]]

### <u>Block Based VS File Based Data</u>
![[Pasted image 20241215142035.png]]
### <u>Two Types of NAS</u>
#### Scale-Up NAS
This is the traditional NAS architecture, that allows high performance and high availability single system that has a fixed capacity ceiling.
For more capacity add another disk array, and another controller may be needed.
- Allows Structured and Unstructured storage
- If more storage is needed, another independent NAS system is added
- A scale-up solution has controllers that connect to trays of disks and provide computational throughput
- Traditional NAS is great for specific types of workflows, especially those applications that require block-level access
#### Scale-Out NAS 
This is a clustered NAS solution, all the NAS boxes belong to a unified cluster with a single point of management, when more storage is needed more nodes can be added because they have there own controller which is essentially a scale up NAS
- This architecture is used in most data center environments today
- Each cluster can have it's own distributed file system
- Scale-Out nodes adds capacity and performance
- Storage options that manage files and tiering
- Flexible data protection and high availability 
- Software modules that control costs and optimize resources
## PowerScale and Big Data
### What is Big Data
Businesses generate a massive volume of digital information, which is known as big data. 
Big Data has three main characteristics 
#### <u>Volume</u>
- YouTube video uploaded every minute
When you have big data volumes of information to store, it had better be there. If an organization relies on RAID to protect against data loss or corruption, the failure of a single disk drive causes a disproportionate inconvenience. The most popular RAID implementation scheme allows the failure of only two drivers before data loss. (A sizable, big data infrastructure can have more then 1000 drivers, so odds are at least one is down) 
#### <u>Velocity</u>
- Store 17 TB of data in 1.5 seconds
Machine generated workflows produce massive volumes of data. For example, the longest stage of designing a system chip is physical verification. Where the chip design is tested in every minute  
#### <u>Variety</u>
Different types of data, for example, videos, photographs etc.

### PowerScale Scale-Out Data Lake
PowerScale is the industry's leading Scale-Out clustered solution. It provides a single volume of data storage at a massive scale that is easy to use and manage. 
- Single Volume  / File System
- High Performance
- Unmatched efficiency 
- Simple to use and manage
- Scales Linearly
## PowerScale Nodes
PowerScale has multiple servers that are called **Nodes** which combine to create a **cluster**. Each cluster behaves as a single, central storage system. 
- PowerScale Nodes are designed for large volumes of unstructured data

### PowerScale Family
#### <u>F-Series</u>
This type of nodes sit at the top of both performance and capacity with all-flash arrays for ultra compute and high capacity. All flash platform can accomplish 250-300K protocol operations per chassis and get 15 GB/s aggregate read throughput from the chassis.

#### <u>H-Series</u>
Next in term of computing power are these nodes. These are highly adaptable hybrid storage solutions that press a compromise between high performance and big capacity storage to accommodate various enterprise file workloads

#### <u>A-Series</u>
These nodes namely have lesser computer power compared to other nodes and are designed for data archival purposes. This can be combined with other nodes to create efficient tiered storage solutions.

### PowerScale Features
- <u>Cyber resiliency and security</u> - OneFS Zero Trust Model for security, 256-bit encryption, SmartLock for immutability, ransomware attack prevention, detection and recovery, air gap protection.
- <u>High performance and scalability</u> - Low latency with NVMe-based all-flash "F"nodes, scale up to up to 252 nodes, tens of petabytes and almost 1 TB/sec of bandwidth under a single unified namespace.
- <u>Software defined deployment flexibility</u> - Deploy as an appliance, on selected Intel x86-based servers
- <u>Self-Encrypting Drivers (SEDs)</u> - Self encrypting drives securely store confidential data over its lifetime. It is used in regulated verticals-Federal governments; Financial services; Healthcare (HIPPA). PowerScale implements Data at rest Encryption using SEDs.
### Node Interconnectivity
PowerScale nodes can use either an InfiniBand or an Ethernet connection for the backend. InfiniBand was designed as a high speed interconnect for high-performance computing, and Ethernet provides the flexibility and high speed that sufficiently support the PowerScale internal communication.
### Quick Scalability 
The primary purpose of a NAS approach is the **Scale-Out** part. An administrator can expend the storage and compute by adding a new node
In PowerScale, once the node is racked and cabled, adding it to the cluster takes just a few minutes because OneFS policies automatically discover the node, set up addresses for the node, incorporate the node into the cluster, and begin rebalancing capacity on all nodes. 

## PowerScale Networking Architecture
### Networking Architecture 
OneFS supports standard network communication protocols IPv4 and IPv6. PowerScale nodes include several external Ethernet connection options, providing flexibility for a wide variety of network configurations
### Type of Networks
### <u>Frontend => External Network</u>
Clients connect to the cluster using Ethernet connections that are available on all nodes. The complete cluster is combined with hardware, software and networks.
### <u>Backend => Internal Network</u>
OneFS supports a single cluster on the internal network. This backend network, which is configured with redundant switches for high availability acts as the back-plane for the cluster

![[Pasted image 20241216115043.png]]

### Interconnect
<u>Backend Network</u> - This backend network is a private PowerScale network that is used for inter-cluster communication.
<u>InfiniBand</u> - This is a high-speed unmanaged fabric. that supports Gen 5 and Gen 6 Nodes.
<u>Ethernet</u> - High speed managed fabric with limited monitoring capabilities, supports Gen 6 nodes. the minimum switch size is 24 ports
### Leaf and Spine 
- Two level hierarchy
- Cluster nodes connect to leaf switches which communicate with each other using the spine switches
- Switches are not interconnected - switches of the same type (leaf or spine) do not connect to one another
- Each leaf switch connects with all spine switches
- All leaf switches have the same number of up-links to the spine switches
## PowerScale OneFS Operating System
### OneFS - Distributed Clustered File System
- OneFS runs on all nodes. Nodes work as peers
- Grows dynamically 
- No primary node that controls the cluster
- Supports variable fault tolerance levels
- Runs on all nodes. Each node is a peer. Prevents bottle necking

### Benefits of OneFS
The OneFS architecture is designed to optimize processes an applications across the cluster
- <u>concurrency</u> - When a node is added to the cluster, it adds computing power, storage, caching, and networking resources.
- Shared infrastructure -
	- Access to resources on any node in the cluster from any other node in the cluster
	- Performance benefits of parallel processing.

- Improved utilization of resources - compute, disk, memory, networking
- Because all nodes work together, the mode nodes, the more the cluster gets more powerful.

### Multi-protocol File Access
![[Pasted image 20241216144309.png]]

OneFS supports access to the same file using different protocols and authentication methods simultaneously. SMB clients that authenticate using Active Directory (AD), and NFS clients that authenticate using LDAP, can access the same file with their appropriate permissions applied.

- OneFS translates Windows Security Identifiers (SIDs) and UNIX User Identities (UIDs) into common identity format.
- Permission activities are transparent to the client

### Authentication
- Authentication services offer a layer of security by verifying user credentials before allowing access ti the files.
- Ensure that interactions between authentication types are understood before enabling multiple methods on the cluster.
#### <u>SSH</u>
SSH multi-factor authentication supported
#### <u>AD</u> 
Active Directory can serve many functions, but the primary reason for joining the cluster to an Active Directory domain in to perform user authentication. This is Microsoft implementation of LDAP
#### <u>LDAP</u>
Networking protocol that enables you to define, query, and modify directory services and resources.
#### <u>Network Information Service</u>
This service provides authentication and identity uniformity across local area networks. OneFS includes NIS authentication.
#### <u>File Provider</u>
Enables to supply an authoritative third-party source of user information to the cluster. This is useful to sync the /etc/passwd, and /etc/group files across servers

### Policy Based Automation
- Automated policies make processes repeatable, decreasing the time spent manually managing the cluster
- Executes policies as a cohesive system
- Policies drive every process

### Management Interfaces
The OneFS management interface is used to perform various administrative and management tasks on the PowerScale cluster and nodes. Management capabilities vary based on which interface is used.

- <u>Serial Console</u> - Initial cluster configuration
- <u>Web Interface</u> - Graphical monitoring displays, and management tasks
- <u>Command Line Interface</u> - configure, monitor, manage with 'isi'
- <u>Platform Application Programming Interface</u> - operations on cluster
- <u>Front Panel Display</u> - basic administrative tasks on site.

