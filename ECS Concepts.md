# Introduction to ECS
## Data Transformation 
Lately significant disruptions like the pandemic have quickly led organizations to reshape their operational strategies.

The adoption of remote collaboration tools has become essential. triggering evaluation of IT infrastructure for energy efficient, hybrid work models, and multi-cloud strategies.

## Todays Challenges
- <u>Unstructured Data</u>
	 - Ensuring data containment and protection
	 - Extracting meaning from data through discovery and analysis for insights and effective management.
	 - Converting insights for actionable business
- <u>Inadequate Foundation</u>
	- Data security and compliance drive the preference for on-premise object storage solutions.
- Data Security and Compliance
- <u>Inflexible Stack</u>
	- Efficient solutions for data management
	- Agile practices and Technologies
	- Future application trends
- <u>Cloud Repatriation</u>
	- Unanticipated costs
	- Security compliance concerns

## ECS Value Proposition
ECS key values cover four main aspects
### Multi-Purpose Platform 
- Supports billions of files and major object protocols
- Natively supports Hadoop, S3, Swift and CAS
- Enables concurrent data access through industry protocols like HDFS and NFS
### Smart Object Storage
- Provides intelligent storage with instant metadata search.
- Integrated HDFS and background erasure coding are included.
- Enables unified global file access and disaster recovery.
- Native support for NFS v3. 
- Integrated metadata management 

### Enterprise Class
- Provides multi-tenant capabilities for flexible storage services
- It caters to diverse clients while ensuring data integrity 

## ECS Features

- ECS scales linearly as customer requirements grow, both compute and capacity is added when a new server is added to the cluster.
- Multi-tenancy is supported natively on ECS. ECS Provides both physical and logical separations.
- ECS is built in as an object storage, with API supporting HTTPS and HTTP for easy web integration.
- ECS is designed to scale up from a small size cluster into a larger one covering multiple sites.
- ECS supports files and objects of any size, and it can pack them efficiently in the storage.
- ECS has a built in metadata, and custom metadata capabilities to meet the needs for the organization. 

## Use Cases
### <u>Storage For IoT</u>
- Challenges
	- Unstructured data growth from IoT sensors.
	- 'No Public Cloud' policy: On premises required.
	- Streamline Hadoop analytics workflow
- Benefits
	- Low cost per GB for protected replicated data
	- High availability with On-Premise security
	- Compatibility with S3 and OpenStack
	- HDFS compatible 
	- Keeps up with increase capacities

### <u>Data Protection for Hadoop</u>
- Challenges
	- Exponential data growth
	- Wide range of data types
	- High speeds of data read and write
- ECS Benefits
	- Integrated metadata search
	- Native multi-protocol support and geo capabilities
	- Ability to analyze faster and protect big data by distributing data across multiple centers

### <u>Data Lake</u>
- Challenges 
	- Manage cost, volume, and velocity of data
	- Hadoop native replication requires a large footprint
	- Reduce manual data preparation
	- Analyze data quickly 
- Benefits 
	- Cost-effective storage for PBs of large or small files
	- Brings analytics to geo-distributed data and archives
	- Data Encryption inside the ECS

# ECS Storage Structure
## Storage Layers
![[Pasted image 20250108101714.png]]
1. <u>Virtual Data Center</u> - Both physical and logical construct. VDCs are typically connected racks that are logically defined as a cluster. VDCs are the top level resources that represents the collection of ECS infrastructure to manage as a unit, also called and ECS _cluster_, multiple VDCs can be located within a single physical location or site.
2. <u>Storage Pool</u> - Provides mechanism for creating physical separation, by logically grouping the storage on different sets of nodes. The grouping logically separates that storage from the storage on other sets of nodes.
3. <u>Replication Groups</u> - Logical construct that defines the storage pool protection relationship. This is the relationship between two storage pools, or locally inside a storage pool. Storage pools are placed into a replication group to provide flexibility
   Replication Groups can be local or global.
   <u>Local replication</u> protects objects within the same VDC against disk or note failures. (Replicate on the same site)
   <u>Global Replication</u> protect objects against disk, node, and site failures. (Replicates to multiple sites) 
4. <u>Namespace</u> - This is a logical construct that provides the equivalent to a dynamic logical volume. Each namespace creates a logically defined storage separation withing a replication group.
5. <u>Buckets</u> - These are object containers that are created on top of a namespace. These can be used to give access control to objects and set properties that define attributes for all contained objects.

## ECS Hardware Building Blocks
ECS systems are built using fundamental building blocks to form a cluster in a distributed storage architecture. Nodes and network switches are combined to form rack. Racks are added to expand the cluster.

### Nodes
- The fundamental building block of a cluster.
- Node consists of compute resources, or server components, and storage capacity in the form of a storage enclosure with drives.
### Switches
Top of rack switches are part of the integrated solution and provide the communications infrastructure from the nodes to the customer network, and between nodes.

![[Pasted image 20250108112914.png]]

## Physical Storage
- Physical storage is grouped into _racks_ to create a VDC, VDCs that contains multiple racks are physically networked together, and virtually defined in the ECS software.
- Connecting the racks within a VDC requires the use of backed switches. This network connection is known an 'Nile Area Network'

### Storage Pools 
All the storage in a cluster is divided into pools. Storage pools can be inside the rack, within multiple racks, and separate 
- All storage within a given node can belong to only one storage pool

## Logical Storage
This is a software defined grouping or separation of storage that is built on the physical storage.
### Federation
The establishment of a federation enables the connection of VDCs.
This enables replication and shared access between the VDCs which enhances data availability and durability. 
![[Pasted image 20250108132938.png]]

### Replication Groups 
A logical construct that defines where a storage pool content is protected. Storage pools are placed into a replication group to provide flexibility wherein data can be accessed.
![[Pasted image 20250109134428.png]]
<u>Local Replication Group</u> - Can be used to replicate information across the cluster, for example RAID 1 or a Backup.
<u>Global Replication Group</u> - Can be used to replicate information across multiple clusters, for easier 'cached' use inside the cluster.
### Namespaces
Provides capabilities to separate ECS users using logical boundaries.
### Buckets
- Objects are stored by writing into a bucket. Objects are retrieved by reading from a bucket. 
- Writing and reading occur when the object application connects to the IP address of one of the nodes withing the namespace containing the desired buckets.
- S3 or Swift buckets can be configured to support file system access for NFS and HDFS
## Chunks
![[Pasted image 20250109141428.png]]

1. ECS creates separate chunks to store actual data objects and the object's associated metadata. Whether the data is file data or object data, ECS stores the data using the same method and mixes data withing the same chunk
2. As data is added to the ECS system, the objects are added to the next available chunk regardless of the size of the object. Small objects may be combined to the same chunks, and larger objects are divided into multiple chunks.
3. Chunks are seen as <u>open</u> when less than 128 MB of data is added to the container or a set of time has not been reached. New data and modified data is written as an append-only to the next available 
4. Existing data blocks are never modified in place, which prevents the need to lock a file for modification. ECS continues to add data to an open chunk 

# ECS Architectural View
## Architecture Layers
ECS provides software-defined cloud storage platform that van be deployed on a set of qualified industry-standard hardware or turnkey storage appliance. 
### Data Services Layer 
The data services layer provides the services, tools and APIs to support Object and file access to the system. 
Clients can access to data stored in the ECS through Object, HDFS, and file protocols. The data services are used to handle requests, Extracting required information, and passing the request to the storage engine for further processing (like read or write). This layer acts as interpreter from user requests and storage engine
![[Pasted image 20250109153919.png]]

### Portal and Provisioning Services
Portal services provides a web-based portal that allows self-service access, management automation, reporting, and ECS node configuration and management. The services also handles licensing, authentication, multitenancy, and provisioning services.
ECS Provides web based user interface to manage, license, and provision ECS nodes. The portal has comprehensive reporting capabilities that include: 
- Capacity utilization per site, storage pools, node and disk
- Performance monitoring on latency, throughput, and replication
- Diagnostic information, such as node and disk recovery
The ECS dashboard offers system health and performance insight, enhancing overall visibility. Users receive alerts for critical events like capacity, quota limits, disk, node failures or software failures.
- Detailed performance can be seen under the advance monitoring folder, presented in a Grafana dashboard.
ECS also provides command-line tools to install, upgrade, and monitor the ECS system. SSH access to the nodes is used for CLI management.
ECS may also be managed through a RESTful APIs. The management API allows an administrator to manage the ECS through custom tools, scripts
The web interface and CLI use the RESTful API under the hood.
### Storage Engine
Responsible for storing and retrieving data, managing transactions, data protection and data replication. This layer also has some services: 
1. Resource Service - Stores information like user, namespace, bucket
2. Transaction Service - Parses object requests, manages I/O
3. Index Service - Maps files and chunks of storage
4. Chunk Management Service - Manages chunk info, like location, I/O
5. Storage Server Management Service - Monitors storage server and disks, and re-protects data upon hardware failures.
6. Partition Record Service - Records and manages the owners of a partition
7. Storage Server Chunk I/O Service - Manages direct I/O operations 
### Fabric
Provides clustering, health, software and configuration management, upgrade capabilities, and alerting
This is a distributed set of software that run on every node. This is a lightweight java agent. This agent manages Docker-related issues, networking, disks and system processes. The agent provides an API and semantics to deploy and manage applications dynamically so it can react to changes.
- This layer is responsible for software upgrade and for monitoring each node of ECS
### Infrastructure 
This is the core node operating system, and networking components. ECS uses ==SUSE Linux enterprise server 12== 
Docket is installed on the infrastructure to deploy the encapsulated ECS layers. The ECS software is written in java, and the Java Virtual Machine (JVM) is installed as part of the infrastructure.
### Hardware 
Provides the physical compute and storage components, or nodes. 
ECS foundation begins with the hardware that the ECS software runs on. The hardware layer specifically sees the node hardware that is combined with the rack designations in the VDC

![[Pasted image 20250109145404.png]]

## Networking
ECS has backend switches for inner cluster communications between nodes, and frontend switches for communication with clients. 

- Frontend switches often are referred as Hare and Rabbit 
- Backend switches often are referred as Hound and fox

# ECS Features  and Functionality
## ECS Portal 
The ECS portal is a graphical user interface for configuring, managing and monitoring an ECS VDC. 
- The dashboard provides a system status and management overview
- The monitor section provides access to detailed monitoring info
## Multi-tenancy
ECS Provides native multitenancy capabilities.
- Namespaces provide logical separation for administration and tenant data
- Buckets provide an additional logical separation for subtenant data 
- The enterprise-level features and simple management design provide storage as a service.
## Namespace Administration
Each namespace has it's own administrator. The administrator cam create buckets, assign policies, and quotas on the namespace or for specific bucket, he can change user permissions, and access namespace-specific monitoring.

## User Authentication and Authorization
ECS contains authentication and authorization at multiple levels. ECS supports local users, domain users, and domain groups through Active Directory, LDAP, and OpenStack
- Authorization permissions are set using client at the object level, using an authentication provider, or using the management REST API
- Roles can be assigned to Active Directory and Groups. 

## Multi-Protocol Support
ECS supports multi-protocol access to the same data stored on the ECS. 
Access to data is independent of the protocol that is used to write, modify, or read data. Client protocol access to the storage engine layer is managed through the data service layer.
![[Pasted image 20250109165351.png]]
- Hadoop uses configurable access to the ECS to simulate direct attached storage. Hadoop on ECS enables analytics on live data that is stored on the ECS. ECS stored Hadoop file system data, which allows organizations to create big data repositories on ECS that Hadoop can consume and process
- File access is provided using NFSv3 exports and, CIFS shares
- ECS supports <u>Rich ACLs</u> which allow more complex permissions.
- <u>Global namespaces</u> provides the ability to access the file from any node any site using a load balancer

## S3 Feature Support 
### Metadata
Metadata search which is enabled during bucket creation, allows searching large S3 datasets using system and custom metadata attributes such as search keys, creation time, owner or custom attributes. up to 30 search keys can be in a single query.
## Object Lock
Prevents deletion during defined retention. Immutable S3 objects use Write Once Read Many (WORM) and retention settings. 
## Object Tagging
Allows the categorization of objects by assigning tags to the individual objects. A single object can have multiple tags that are associated with it, which enables multidimensional categorization.
- Tagging in IAM (S3 Only)
- Tagging with bucket policies
- Tagging during PSO/TSO
- Tagging during object life time cycle management
## Local Data Protection
Data is locally protected on each ECS cluster. A combination of protection mechanisms is applied to assure data availability and integrity
Data is chunked into 128 MB chunks and is sealed once it is filled. The protection is done with Reed-Solomon Erasure coding once the chunk is sealed.
## Mirroring and Erasure Coding
Metadata and object data are stored in the local ECS using a combination of mirroring and Reed-Solomon erasure coding.
Unlike most storage systems ECS don't apply RAID or protection at individual file level. The use of chunk level protection enables protection efficiencies and realized using chunks.
![[Pasted image 20250109174851.png]]
## Failure Handling and Data Recovery
The most common failures in ECS storage are disk and node hardware. ECS protects against data loss and self-heals against disk and node loss failures.
When ECS is recognizing that a disk has failed, it proactively protects the data and stores copies on disks that hasn't failed.
## Multi-VDC, Multi-Site Data Replication
ECS offers geo replication options, including active-active replication with full replication copies, active-active storage and active-passive replication with space efficient storage.
- ECS supports natively supports multi-VDC and multi-site data protection
- Multiple VDCs can be deployed in a single location and added to the same federation.
- Geo replication capabilities can protect from an entire VDC loss.
- Geo replication makes a full duplicate between sites.
- Replications to all site allows fast storage reads, and low latency.
- XOR optimization only sends copy to every other site instead of all the sites which still benefits from a VDC loss at a lower cost.
## Temporary Site Outage (TSO)
- During a TSO ECS can detect and automatically handle and temporary site failures. ECS provides options that affect how objects can be accessed during TSO
- By default, the ECS retains a strong consistency by allowing access to data owned by sites are accessible and prevents access to data owned by an inaccessible site.
### With ADO 
Access During Outage (ADO) occurs during connectivity outage inder the following circumstances
- ADO allows read and potentially write access to geo-replicated data including one that is owned by the site that is down.
## Multi-site Geo Active with XOR Storage
Enables low storage overhead that improves with an increased number of sites in a replication group.
- If one VDC fails, ECS can reconstruct all data within the failed VDC using available chunks in the other VDC
- Write and Update operations in ECS's multi-site data deployment are more complex compared to a two-site mirrored data setup, where read requests remain unaffected.
## Permanent Site Outage (PSO)
A situation where an entire site becomes unrecoverable due to a disatster
- If the failure is permanent, the System Administrator must permanently fail over the site from the federation to initiate failover processing.
- This initiates re-synchronization and re-protection of the objects that are stored on the failed site. The recovery tasks run as background process.

## Data Encryption
- D@OR encrypts the data that is stored on the ECS nodes. The data is stored using software encryption, Which prevents sensitive data from being read from disks that are removed from the system.
- While data is not processed and there is 'free' CPU time, the encryption is happening on the data.
## Quotas
Enabling quotas provides simple mechanisms to allocate and limit tenants and subsystems with storage space. Each tenants is getting a limit through it's quota.
## Identity and Access Management
Enables to control secure access to ECS S3 Resources. This functionality ensures that each access request to the ECS resource is identified, authenticated and authorized.
## ECS Monitoring and Reporting 
ECS provides monitoring which provides increased visibility and determines capacity across various metrics.