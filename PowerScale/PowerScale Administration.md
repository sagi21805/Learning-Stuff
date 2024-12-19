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
