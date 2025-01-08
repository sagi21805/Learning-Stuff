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



