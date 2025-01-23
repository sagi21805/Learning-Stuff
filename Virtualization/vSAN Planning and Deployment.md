# What is vSAN
VMware vSAN is a distributed layer of software that run natively as a part of the ESXi hypervisor. vSAN aggregates local or direct-attached capacity devices of a host cluster and creates a single storage pool shared across all hosts in the vSAN cluster. vSAN eliminates the need for external the need for external shared storage, and simplifies storage configuration and virtual machine provisioning.
## vSAN  Concepts
VMware vSAN uses a software-defined approach that creates a shared storage for virtual machines. It virtualizes the local physical storage of ESXi hosts and turns them into a pools of storage that can be divided as assigned to virtual machines and applications according to their quality-of-service requirements. vSAN is implemented directly on the ESXi host.

vSAN can be configured to to work either as hybrid or all-flash cluster.
<u>Hybrid cluster</u> - flash devices are used for cache layer and and magnetic disks are used for the storage capacity layer.
<u>All-Flash</u> - in all-flash devices are used for both cache and capacity.

vSAN can be activated on existing host cluster, or when you create a new cluster. vSAN aggregates all local local capacity devices into a single datastore shared by all hosts in the vSAN cluster. Data store can be expended by adding capacity devices or hosts with capacity devices to the cluster. vSAN works best when all ESXi hosts in the cluster has similar or identical configuration balances. The consistent configuration makes it so virtual machines balances across multiple hosts and devices on the cluster.

In vSAN <u>Original Storage Architecture (OSA)</u>, each host that contributes storage devices to the vSAN datastore must provide at least on device for flash cache and at lease one device for capacity. 
In vSAN <u>Express Storage Architecture (ESA)</u>, all storage devices claimed by the vSAN contribute to the capacity and performance. Each host's storage devices claimed by the vSAN form a storage pool. The pool represents the amount of caching and capacity provided by to the vSAN datastore.

## Characteristics of vSAN

| Supported Features                               | Description                                                                                                                                                                                                                        |
| ------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Shared Storage support                           | vSAN supports VMware features that require shared storage such as HA, vMotion, and DRS. If host is overloaded, Dynamic Resource Scheduler can migrate machines to other hosts.                                                     |
| On-disk format                                   | vSAN on-disk virtual file format provides highly scalable snapshot and clone management support per vSAN cluster.                                                                                                                  |
| All-flash and hybrid configuration               | vSAN can be configured for all-flash or hybrid cluster                                                                                                                                                                             |
| Fault domains                                    | vSAN supports configuring fault domains to protect hosts from rack or chassis failures when the vSAN clusters spans across multiple racks                                                                                          |
| File Service                                     | vSAN file service enables to create shares in a vSAN datastore that client workstations or VMs can access                                                                                                                          |
| iSCSI target service                             | vSAN iSCSI target service enables hosts and physical workloads reside outside vSAN cluster to access the vSAN datastore                                                                                                            |
| vSAN stretched cluster and Two node vSAN cluster | vSAN supports stretched clusters that span across two geographic locations                                                                                                                                                         |
| vSAN health service                              | vSAN health service includes pre-configured health check tests to monitor, troubleshoot, diagnose the cause of cluster component problems and identify the risk.                                                                   |
| vSAN performance service                         | vSAN performance                                                                                                                                                                                                                   |
| Integration with vSphere storage fetures         | vSAN integrates with vSphere data management features traditionally used with VMFS and NFS storage. these features includes snapshots, linked clones, vSphere replication                                                          |
| Virtual Machine Storage Policies                 | vSAN works with the VM storage policies to support a VM-centric approach to storage management. if you do not assign a storage policy to the VM during deployment, the default policy is automatically assigned.                   |
| Rapid Provisioning                               | vSAN enables rapid provisioning of storage in the vCenter Server during virtual machine creation and deployment operations.                                                                                                        |
| Deduplication and Compression                    | vSAN performs block-level deduplication and compression on a vSAN all-flash cluster, all redundant data withing each disk group is reduced.                                                                                        |
| Data at Rest Encryption                          | Data is encrypted after all other processing, such as deduplication is performed, This method encrypts the data while stored, so if a device is removed data is protected                                                          |
| Data in Transit Encryption                       | vSAN can encrypt data in transit across hosts in the cluster, When this is enabled, vSAN encrypts all data and metadata traffic between hosts.                                                                                     |
| SDK support                                      | The VMware vSAN SDK is an extension on the VMware vSphere Management SDK. It includes documentation, libraries and code examples that help developers automate installation configuration, monitoring, and troubleshooting of vSAN |
## vSAN Terms and Definition
### Disk Group (vSAN Original Storage Architecture)
- This is a unit of physical storage capacity and performance on a host and a group of physical devices that provide performance and capacity to the vSAN cluster. 
- On each ESXi host, that contributes its local devices to a vSAN cluster, devices organized into <u>disk groups</u>
- Each <u>disk group</u> must have one flash cache device and one or multiple capacity devices
- Caching devices cannot be shared across disk groups.
### Storage Pool (vSAN Express Storage Architecture)
A storage pool is a representation of all storage devices on a host that are claimed by vSAN. Each host contains one storage pool. Each device in the storage pool contributes both capacity and performance
### Object Based Storage
vSAN stores and manages data in the form of a flexible data containers called objects.
An object is a logical volume that has its data and metadata distributed across the cluster. For example every _.vmdk_ file is an object as is every snapshot. When you provision a virtual machine on a vSAN datastore, vSAN creates a set of objects comprised of multiple components for each virtual disk.  
It also creates the VM home namespace, which is a container object that stores all metadata files of your virtual machine. When vSAN creates an object for a virtual disk and determines how to distribute the object in a cluster, it considers the following factors:
- vSAN verifies that the correct cluster resources are used at the time of provisioning. Based on the protection policy, vSAN determines how many replicas to create, the amount of cache, and RAID configuration
- vSAN verifies that the virtual disk requirements are applied according to the specified VM storage policy
- vSAN continually monitors and reports the policy compliance status of the virtual disk
- vSAN verifies that the required protection components, such as mirrors and witnesses, are placed on separate hosts or fault domains.
### vSAN Datastore
After vSAN is enabled on the cluster, a single datastore is created. It appears as another type of datastore in the list of datastores that might be available, including Virtual Volume, VMFS, and NFS. A single vSAN datastore can provide different service levels for each virtual machine or each virtual disk. In a vCenter Server, storage characteristics of the vSAN datastore appear as a set of capabilities. 
vSAN datastore has specific characteristics to consider.
- vSAN provides a single vSAN datastore accessible to all hosts in the cluster, whether or not they contribute storage to the cluster. Each host can also mount any other datastores including Virtual Volumes, VMFS, or NFS
# Building a vSAN Cluster
## Choosing Architecure 
<u>Original Storage Architecture</u> - is used for a wide range of storage devices, including flash, solid stated drives, and magnetic drives (HDD).
<u>Express Storage Architecture</u> - is used for high performance NVMe based TLC flash devices and high performance networks 
## vSAN Deployment Options
### Single Site vSAN Cluster
This type of site consists of a minimum of three hosts that are connected on the same Layer 2 network. 
### Stretched vSAN Cluster
This type provides resiliency against the loss of an entire site. The hosts in the vSAN are distributed evenly across two sites. The two sites must have latency that is lower then 5ms.

# Requirements for Enabling vSAN
## Storage Device 
Must have at least 10% of the storage as cache 
Must have at least 4 NVMe TLC devices
## Host Memory
vSAN Express Storage Architecture requires 512 GB host memory. The memory for the entire environment depends on the number of devices in the host's storage pool.
## Cluster Requirements for vSAN
Verity that a host cluster meets the requirements for enabling vSAN
- All capacity devices, drives, and firmware versions in your vSAN configuration must be certified and listed in the vSAN section of _VMware Compatibility Guide_
- A standard vSAN cluster must contain a minimum of three hosts that contribute capacity to the cluster. A two host vSAN cluster consists of two data host and an external witness host.
## Software Requirements for vSAN
Verify that the vSphere components in your environment meet the software version for vSAN. To use the full set of vSAN capabilities, the ESXi hosts that participate in vSAN clusters must be version 8.0 or later.
## Networking Requirements
Verify that the network infrastructure and the networking configuration on the ESXi hosts meet the minimum networking requirements for vSAN

| Networking Component     | Requirement                                                                                                                                                                            |
| ------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Host Bandwidth           | Each host must have minimum bandwidth dedicated to vSAN                                                                                                                                |
| Connection Between Hosts | - vSAN OSA: Dedicated 1 Gbps for hybrid configuration, dedicated or shared 10 Gbps for all-flash<br>-  vSAN ESA: Dedicated or shared 25 Gbps                                           |
| Host Network             | Each host in the cluster, regardless of whether it contributes capacity, must have a VMkernel adapter for vSAN trafiic                                                                 |
| IPv4 and IPv6 Support    | The vSAN network support both IPv4 and IPv6                                                                                                                                            |
| Network Latency          | - Maximum of 1 ms RTT for single site cluster between all hosts<br>- Maximum of 5 ms RTT between the two main sites<br>- Maximum of 200 ms RTT from main site to the vSAN witness host |
# Designing and Sizing vSAN Storage
Plan capacity and cache based on the expected data storage consumption. 
- <u>Plan Capacity in vSAN</u> - You can calculate the capacity of a vSAN datastore to accommodate the virtual machines (VMs) files in the cluster, and to handle failures and maintenance operations.
- <u>Design Considerations for Flash Caching Devices in vSAN</u> - Plan the configuration of flash devices for vSAN cache and all-flash capacity to provide high-performance and required storage space and accommodate future growth.
- <u>Design Consideration for Flash and Capacity Devices in vSAN</u> - Plan the configuration of flash capacity devices for vSAN all-flash configurations to provide high performance and required storage space, and to accommodate future growth.
- <u>Design Considerations for Magnetic Disks in vSAN</u> - Plan the size and number of magnetic disks for capacity in hybrid configuration by following the requirements for storage space and performance.
- <u>Design Consideration for Storage Controllers in vSAN</u> - Use storage controllers on the hosts of a vSAN cluster that best satisfy your requirements for performance and availability.
## Planing Capacity in vSAN
You can calculate the capacity of a vSAN datastore to accommodate the virtual machines (VMs) files in the cluster, and to handle failures and maintenance operations.
## Raw Capacity 
Use this formula to determine the raw capacity of a vSAN datastore. 
$(NumberOfDiskGroups * AvarageSizeOfGroups) - vSANOverhead$
## Failure to Tolerate
When you plan the capacity of the vSAN datastore, not including the number of virtual machines and the size of their $VMDK$ Files 

