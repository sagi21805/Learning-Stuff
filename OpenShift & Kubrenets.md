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
https://kubernetes.io/docs/concepts/security/multi-tenancy/