# Kubernetes
## Overview
Kubernetes is a portable, extensible, open source platform for managing containerized workloads and services, that facilitate both declarative configuration and automation. It has large, rapidly growing ecosystem. 
Kubernetes was created because container orchestration was a hard task. With kubernetes containers could deploy multiple containers on multiple machines. 
## Multi-Tenancy
Sharing clusters saves costs and simplifies administration. However sharing a cluster presents security problems.
### Multiple Teams
This is in the case that a lot of teams in an organization which may operate different workloads that can communicate with each other. In this case teams may have direct access to kubernetes resources via tools like `kubectl`, or indirect access through GitOps controllers or other types of release automation tools. There is RBAC access control
### Multiple Customers
This case involves multi-tenancy of Software As A Service (SaaS) vendor running multiple instances of a workload for customers. In this scenario customers don't have access to the cluster; and kubernetes is invisible from their perspective and is only used by the vendor to manage workloads 

![[Pasted image 20250218191339.png]]
## Isolation
There are several ways to design and build multi-tenant solutions with kubernetes, Each of these methods comes with its own set of trade-offs that impact the isolation level, implementation effort, Operational Complexity and cost of service.
Kubernetes has a control plane and data plane, the control plane has the kubernetes software running on it, while the data plane consists of worker nodes where tenant workloads are executed as pods. Isolation can be applied to both planes.
<u>Hard Isolation</u> -> Tenants don't trust each other, often from security and resource sharing perspective
<u>Soft Isolation</u> -> Tenants kind of trust each other
In more extreme cases, tenants can be on separate clusters, or even running on different VMs or even different physical hardware.
### Control Plane Isolation
#### Namespaces
In kubernetes a <u>Namespace</u> provides mechanism for isolation groups of API resources within a single cluster. This isolation has two key dimensions:
1. Object names within a namespace can overlap names in other namespaces
2. Many kubernetes security policies are scoped to namespaces, like RBAC Control, Network Policies
In multi-tenant environment, a Namespace helps to segment a tenant's workload into a logical and distinct management unit.
It is common to segment each workload in a different namespace even if they all tied to the same customer.
#### Access Control
The most important type of isolation is control plane authorization, If other teams could access and change other teams API resources, there is practically no isolation. 
Role Based Access Control (RBAC) is commonly used to enforce kubernetes control plane, for both users and workloads (service accounts) <u>Roles and RoleBindings</u> are kubernetes objects that are used at the namespace level to enforce access control. 
In multi-tenant environment, RBAC must be used to restrict tenants' access to the appropriate namespaces, and ensure that cluster-wide resources can only be accessed or modified by privileged users such as cluster administrators.
#### Resource Quotas 
Kubernetes workloads consume node resources, like CPU and memory. 
In a multi-tenant environment, you can use Resource Quotas to manage resource usage of tenant workloads. This can be used to to limit the number of API requests, for example, number of pods or number of ConfigMaps etc...
Resource quotas are namespaced objects. By mapping tenants to namespaces, cluster admins can use resource quotas to make sure tenants don't monopolize a cluster's resources or overwhelm control plane. 
Quotas prevent single tenant form consuming greater than their allocated share of resources hence minimizing the "Noisy Neighbor" issue where one neighbor negatively impacts the performance of other tenants' workloads.
When a quota is applied to a namespace, kubernetes requires also to specify upper bound to resources requests and limits for each container.
Quotas cannot protect against all kinds of resource sharing, such as network traffic. Node isolation (described below) may be a better solution for this problem.
## Data Plain Isolation
Data isolation ensures that pods and workloads for different tenants are sufficiently isolated
### Network Isolation
By default, all pods in kubernetes cluster are allowed to communicate with each other, and all traffic in unencrypted. This can lead into a security vulnerability, where traffic in sent maliciously to an unintended destination.
Pod to Pod communication can be controlled using <u>Network Policy</u> which restrict communication between pods using namespace labels or IP address range. 
### Storage Isolation
Kubernetes offers types of volumes that can be used as persistent storage. For security and data isolation, dynamic volume provisioning is recommended and volume types that use node resources should be avoided. 
<u>Storage Classes</u> allow you to describe custom "classes" of storage, offered by the cluster. which can provide Quality of Service levels, Backup policies, or custom policies. 
Pods can request storage using a Persistent Volume Claim, which is a namespaced resource, which enables isolating portions of storage systems and dedicating it to tenants withing the shared Kubernetes 
### Sandboxing Containers 
`This is an OpenSource project which is not the core of kubernetes`
Kubernetes pods are composed of one or more containers that execute on worker nodes. Containers utilize OS-level virtualization and hence offer a weaker isolation boundary then a virtual machine.
In a shared environment, unpatched vulnerabilities in the application and system layers can be exploited by attackers for container breakouts and remote code execution. 
<u>Sandboxing</u> provides a way to isolate workloads running in a shared cluster. It typically involves running each pod in a separate execution environment such as a virtual machine or a user-space kernel. Sandboxing is often recommended, when running untrusted code where workloads are assumed to be malicious. Part of the reason this type of isolation in necessary is because containers processes running on a shared kernel mount file-systems in `/sys` and `/proc` from the underlying host. 
### Node Isolation
Node isolation is another technique that you can use to isolate tenant workloads from each other. With node isolation, a set of nodes, a set of nodes is dedicated to running pods from a particular tenant and co-mingling of tenant pods is prohibited. This configuration reduces the noisy tenant phenomenon. This also secures from container escapes, because of a hacker escapes from a container of other tenants he is on a different node so he can't access your resources.
Although the nodes are different, the control plane of the cluster is the same, so services like kubelet and the API are still shared (unless using virtual control plane) so a skilled attacker can you the permissions assigned to the kubelet or other pods to move his container to the victim node and then escape from it to gain access to the node. 
# What is Desired State
Desired state is the state that we want an object (Container, ReplicaSet, Pod, etc..) to be in, for example, we want a deployment with two pods that run nginx containers, this will be our desired state, and kubernetes will know how to calculate the path from the current state to the desired one.
# Uses of Kubernetes
- <u>Horizontal Scaling</u> -> Scale applications up and down with simple command, or U, or automatically based on cpu usage.
- <u>Self Healing</u> -> Restarts containers that fail, replaces and reschedules containers when nodes die, kills containers that don't respond to user-defined health checks. 
- <u>Batch Execution</u> -> Manage batch execution and CI workloads, replacing containers that fail if desired. 
# Cluster Components 
## Control Plane Componenets
### API Server 
_The core component server that exposes the Kubernetes HTTP API_
The API server exposes an HTTP API that let end users, parts of the cluster and external components to communicate with each other. 
The Kubernetes API lets you query and manipulate the state of kubernetes' control plane.
The Kubernetes API lets you to query and manipulate the state of API objects in Kubernetes (for example: Pods, Namespaces, ConfigMaps, Events, etc..)
### Etcd
_Consistent and highly-available key value store for all API server data_
This is a Key-Value store that is highly available, that stores data about all the cluster. 
The tools that are used to control it are -> 
- `etcdctl` -> This is the primary command line tool that is used to interact with etcd over the network, and it used daily for managing keys and values, as well as managing the cluster, checking health and more.
- `etcdutl` -> This is and administration utility designed to operate etcd directly, for example, migrating between data between versions, deframenting a database, restoring snapshots, and validating data consistency.
### Scheduler
In kubernetes, Scheduling refers to making sure that <u>Pods</u> are matched to <u>Nodes</u> so <u>Kubelet</u> can run them. 
The scheduler watches for newly created Pods that have no Node assigned. For every Pod that the scheduler discovers, it is responsible to find the best matching node for this pod. 
### Kube-Scheduler
_Looks for Pods not yet bound to a node, and assigns each Pod to a suitable node._
This is the default scheduler for kubernetes, and it is designed to support custom scheduling functions that the client provides. 
Kube-Scheduler selects an optimal node to run newly created or not yet scheduled pods. 
There are two main actions to schedule a Node ->
- `Filtering` -> This step is required to find a set of `feasible` nodes. Since containers in pods and pods themselves can have different requirements, the scheduler filters out any nodes that don't meet a Pod's specific criteria. If a node is found it is called `feasible` of None are found the pods will not be scheduled.
- `Scoring` -> After a set of nodes is obtained, they are scored to find which is the must suitable for the pods, there are certain checks that can be performed, for example how much resources are available on the node. The node with the most resources will be picked.
## Node Components 
### Kubernetes Node
Kubernetes runs your workloads on Pods that are sitting on <u>Nodes</u> which are the physical or virtual machines that actually run the workloads. Each node is managed by the control plane and contains services necessary to run Pods. Typically there are multiple nodes inside a cluster.
### Controller Manager
_Runs controllers to Kubernetes API behavior._
The controller manager is a daemon that embeds the core control of loops shipped with Kubernetes. A control loop is a non-terminating loop that regulates that state of a system. This controller is a control loop that that ships with kubernetes and watches the shared state of the cluster through the apiserver and makes changes attempting to move the current state towards the desired state. Some controllers that ship with kubernetes are ->
- Endpoint controller 
- Namespace controller
- Service Account Controller
### KubeProxy 
_Maintains network rules on nodes to implement Services_
The kubernetes network proxy runs on each node. This reflects services as defined in Kubernetes API on each node and can do simple TCP, UDP,  SCTP (Stream Control Transmission Protocol) forwarding or round robin TCP, UDP and SCTP forwarding. Service cluster IPs and ports are currently found through Docker-links-compatible environment variables specifying ports opened by the service proxy. 
### Kubelet
_Ensures that Pods are running, including their containers._
The kubelet is the primary agent that runs on each node. It can register the node with the apiserver using one of ->
- Hostname
- A flag to override the hostname
- Specific logic for a cloud provider
The kubelet works in terms of `PodSpec` Which is a <u>YAML</u> or <u>JSON</u> object that describes a pod. The kubelet takes a set of PodSpecs that are provided through various mechanisms (Primarily the apiserver) and ensures that the containers described in those PodSpecs are running and healthy. The kubelet doesn't manage containers which were not created with kubernetes.   
### Container Runtime
This is the software that run the containers, like docker!
# Kubernetes Nodes
Kubernetes runs the workloads on pods that are placed on _Nodes_. A _Node_ is a physical or a virtual machine that runs the workloads pods that run containers inside them. The components inside a node contains [[#Kubelet]], [[#KubeProxy]] and [[#Container Runtime]]
## Node Management
There are two main ways to have node added to the API Server:
1. The kubelet on a self-registers to the control plane.
2. You (Or another human) manually add a node object.
After a node is created, or the kubelet on the node self registers, the control plane checks whether the new Node object is valid. For example, if you try to create a node from the following JSON manifest ->
```json
{
"kind": "Node",
"apiVersion": "v1",
"metadata": {
		"name": "10.240.79.157",
		"labels": {
			"name": "my-first-k8s-node"
		}
	}
}
```
- Kubernetes creates the Node object internally (The representation).
- Kubernetes checks that the kubelet has registered to the API server that matches `metadata.name`
- If the node is healthy (i.e all necessary services are running), then it is eligible to run a Pod. 
- Otherwise the node is ignored for any cluster activity until it becomes healthy.
## Name Uniqueness
The name of the node is the identifier for that node, so that means <u>every node must have a different name</u> because kubernetes assumes resources with the same name are the same resources. 
In a case of a node it assumes implicitly that this is the same object and configures the same state to it (e.g. network settings, root disk content, and attributes like node lables)
If a node needs to be replaced or modified it is recommended to be removed from the server and then re-added
## Self Registration of Nodes
When the flag `--register-node` is true (the default), the kubelet will attempt to register itself to the apiserver. This preferred pattern is used by most distros. For self registration, the kubelet is started with the following options ->
- `--kubeconfig`-> Path to credentials to authenticate itself to the apiserver
- `--cloud-provider` -> How to talk to a cloud provider, to read metadata about itself
- `--register-node` -> Automatically register the node with the apiserver (true by default)
- `--register-with-taints` -> Register the node with list of taints (comma separated `<key>=<value>:<effect>`) No-Op if --node-register is false.
- `--node-ip` -> Optional list of comma separated ip addresses for the node. if none is provided the default ip address of the node is used.
- `--node-lables` -> <u>Lables</u> to add when registering the node to the cluster.
In this method, basically the credentials are given and the node registers itself to the apiserver.
## Manual Register of Nodes
Node can be created manually and modified using the `kubectl` cli utility.
When you want to create Node objects manually, set the kubelet flag `--register-node=false` 
You can modify nodes regardless of the setting of `--register-node`. For example, you can set labels on an existing node or mark it as unschedulable.
Marking node as unschedulable prevent the scheduler from placing new pods onto that node but does not affect existing pods on that node. This is a useful step before a rest to a node for reboot or maintenance.
To mark node as unschedulable run `kubectl cordon $NODENAME`
## Node Status 
To see Node status you can run the following command ->
```bash
kubectl describe node <node_name>
```
This will contain the following information ->
- <u>Addresses</u> -> This will contain the hostname as reported by the nodes kernel, The external IP of the node, The internal IP of the node
- <u>Conditions</u> -> Describes the status of all the `Running` nodes, for example, ready, DiskPressure, MemoryPressure, PIDPresure, NetworkUnavilable  
- <u>Capacity & Allocatable</u> -> Describes the resources available on the node: CPU, memory, and the maximum number of pods that it can schedule
- <u>Info</u> -> General information about the node, like kernel versions, container runtime details, node os.
## Node Heartbeat
Heartbeats sent by kubernetes nodes, help your cluster to determine the availability of each node, and to take actions when failures are detected.
for nodes there are two types of heartbeats ->
- Updates to the `.status` of a node
- Lease objects within the `kube-node-lease` namespace Each node has an associated Lease object
## Node Controller
The node controller is kubernetes control plane component that manages various aspects of nodes.
The node controller has multiple roles in a node's life. 
- The first is to CIDR block to the node when registered (if CIDR assignment is on)
- The second is keeping the nodes controller's internal list of nodes up to date with the cloud provider's list of available machines. The controller asks the cloud provider if the VM for that node is still available. If not, the node controller deletes the node from the list. 
- The third is monitoring health and it is responsible for ->
	- Update the ready condition of the node, and set it to the updated one.
	- Trigger eviction if a node is not available for more then 5 minutes.
# Plugin Architecture
## Container Network Interface (CNI)
Kubernetes lets you use CNI plugins for cluster networking. You must use a CNI plugin that is compatible with your cluster and that suites your needs. 
A CNI plugin required to implement the Kubernetes Network Model
## Container Runtime Interface (CRI)
This plugin interface which enables the kubelet to use a wide variety of container runtimes, without having a need to recompile cluster components
You need a working container runtime on each node in the cluster, so that the kubelet can launch Pods and their containers.
This is the main protocol of communication between the runtime and the kubelet
## Container Storage Interface (CSI)
This is a container storage interface, prior to CSI kubernetes provided a powerful volume plugin system, it was challenging to add support for new volume plugins to kubernetes: Volume plugins were "in-tree" meaning their code was part of the core kubernetes code and shipped with the core binaries. CSI was developed as a standard for exposing arbitrary block and file storage storage systems.
