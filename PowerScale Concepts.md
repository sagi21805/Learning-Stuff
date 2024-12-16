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
### PowerScale Nodes
PowerScale has multiple servers that are called **Nodes** which combine to create a **cluster**. Each cluster behaves as a single, central storage system. 
- PowerScale Nodes are designed for large volumes of unstructured data

### PowerScale Family
#### <u>F-Series</u>
This type of nodes sit at the top of both performance and capacity with all-flash arrays for ultra compute and high capacity. All flash platform can accomplish 250-300K protocol operations per chassis and get 15 GB/s aggregate read throughput from the chassis.

#### <u>H-Series</u>
Next in term of computing power are these nodes. These are highly adaptable hybrid storage solutions that press a compromise between high performance and big capacity storage to accommodate various enterprise file workloads

#### <u>A-Series</u>
These nodes namely have lesser computer power compared to other nodes and are designed for data archival purposes. This can be combined with other nodes to create efficient tiered storage solutions.

#### PowerScale Features
- <u>Cyber resiliency and security</u> - OneFS Zero Trust Model for security, 256-bit encryption, SmartLock for immutability, ransomware attack prevention, detection and recovery, air gap protection.
- <u>High performance and scalability</u> - Low latency with NVMe-based all-flash "F"nodes, scale up to up to 252 nodes, tens of petabytes and almost 1 TB/sec of bandwidth under a single unified namespace.
- Changes
