# Why Containers
Containers solve 2 main problems in computer software and deployment
## Building Software is Annoying
most software comes with install scripts and make configuration. but most of the time, especially after some time without touching the project or switching to different OS. 
```bash
$ git clone <some repo>
$ cd <some repo>
$ ./configure
$ make all
$ ERROR: you have version 2.1.1 and version 2.1.4 is needed
```
Containers package all the dependencies in the container itself. 
Containers have their own file system, and even if the the Linux distribution don't match the container can still run because the image has all what it needs in it.
## Deploying Software is Annoying
Programs don't work because of versioning problems, like different python versions, deprecated functions etc. 
The container, again, has all the dependencies that it needs inside it, so all the dependencies are with the right version.
# The Big Idea: Include EVERY dependency 
- Containers package EVERY dependency together.
- A container is a tarball of a file system, for example typical rails app container includes: 
	- Web app code
	- libc + other system libraries 
	- Ruby interpreter 
	- Ruby gems 
	- Ubuntu base OS
- How images are built 
	- start with a base OS
	- install program + dependencies
	- configure it how you want
	- make a tarball of the WHOLE FILESYSTEM with tools like 'docker build'
- How to run an image
	- Download the tarball
	- Unpack it into a directory
	- run the program and pretend the directory is a whole filesystem
Images let you install programs really easily, and set up a PostgreSQL test DB in 5 seconds
# Containers Aren't Magic 
This bash code, using Linux kernel built in commands, creates a container running fish shell
```bash
wget bit.ly/fish-container -O fish.tar
# 1. Download the image

mkdir container-root; cd container-root
tar -xf ../fish.tar                          
# 2. Unpack image into a directory

cgroup_id="cgroup_$(shuf -i 1000-2000 -n 1)"
 # 3. Generate random cgroup name

cgcreate -g "cpu,cpuacct,memory:$cgroup_id"
cgset -r cpu.shares=512 "$cgroup_id"
cgset -r memory.limit_in_bytes=1000000000 "$cgroup_id"
# 4. Make a cgroup and set CPU/Memory limits 

cgexec -g "cpu,cpuacct,memory:$cgroup_id"
# 5. use the cgroup

unshare -fmupin --mount-proc 
chroot "$pwdt/"

```

# Container = Process
- A Container is a group of Linux processes, on other operating systems containers run inside a virtual machine.
- A `top` command that is ran inside a container will look like this:

| Outside The Container                                                                                                | Inside The Container                                                        |
| -------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| ps aux \| grep top<br><br>USER PID        START COMMAND<br>root    23450   20:55   top<br>bork   23456   20:57   top | ps aux \| grep top<br><br>USER PID START COMMAND<br>root    25  20:55   top |
The top process are the same process!
- A Container process can do anything a normal process can
- Some Container restrictions are 
	- Different PID namespace
	- Different Root directory
	- cgroup memory limit
	- limited capabilities
	- not allowed to run some system calls
- The restrictions are enforced by the Linux Kernel

# Container Kernel Features
Containers doesn't have a clear definition, but docker containers are using the following Linux kernel features: 
- <u>Pivot Root</u> - Set a process's root directory with the contents of the container image
- <u>cgroup</u> - Limit memory/CPU/Network/Storage and more
- <u>namespaces</u> - allow processes to have their own network, PIDs, hostname, mounts, users and more.
- <u>capabilities</u> - cuts roots permissions into specific capabilities, a process can demand capabilities, so if a process is not given all the capabilities he is restricted from certain actions.
- <u>seccomp-bpf</u> - allows to have a configuration to enable only certain system calls to a process.
- <u>overlay filesystems</u> - this what makes layers work! sharing layers saves disk space & helps containers start faster
## Pivot Root
- A container image is a tarball of a file system
<u>chroot</u> -> change a process's <u>root</u> directory
If you chroot to /fake/root, when it opens the file /usr/bin/redis it'll get /fake/root/usr/bin/redis instead you can "run" a container just by using chroot like this:
```bash
mkdir redis; cd redis
tar -xzf redis.tar
chroot $PWD /usr/bin/redis
```
The problem is that programs can escape from chroot, pivot root unmounts the old file system unlike chroot so it is not possible to access files from the outside filesystem
<u>To have a container, more is needed</u> ->
- set CPU/Memory limits
- Hide other running processes
- Use the same port as another processes
- restrict dangerous systemcalls
## Layers 
 - Different Images have similar files 
 - Reusing layers saves disk space (Multiple containers can run on the same base image)
 - Each layer is a directory
 - Every layer has an ID, which is usually a sha256 of the layers content
 - If a file is in 2 layers, you'll see the version from the top layer, for example if there is a file /code/file.py in layer one and the same file also in layer 2
 - By default, writes go to a temporary layer. To save the changes, write to a directory that is mounted from outside of the container
## Overlay Filesystem
How layers work:
	```mount -t overlay```
This command has 4 parameters
- <u>lowerdir</u> - list of read-only directories
- <u>upperdir</u> - directory where writes should go
- <u>workdir</u> - empty directory for internal use
- <u>target</u> - the merged result
for example:
```bash
$ mount -t overlay overlay -o \
	lowerdir=/lower,upperdir=/upper,workdir=/work /merged

$ ls /upper
# cat.txt dog.txt
$ ls /lower
# dog.txt bird.txt
$ ls /merged
# cat.txt dog.txt bird.txt
## The merged version of dog is the one from the upper version
```

## Namespaces
Inside a container things, like processes, files etc can look different. 
for example, `ps aux` can show only 4 processes which is weird, this can happen if the command is ran inside a PID namespace.
EVERY process has 7 Namespaces:
- ID: 4026531835 cgroup
- ID: 4026531836 pid
- ID: 4026531837 user
- ID: 4026531838 uts
- ID: 4026531839 ipc
- ID: 4026531840 mnt
- ID: 4026532009 net
### How to make Namespaces
- Processes use their parent's name by default 
- A namespace can be switched at any time. 
Command line tools:
```bash
$ unshare --net COMMAND
# run COMMAND in a new network namesapce.
$ sudo lsns
# lists all namespaces
$ nsenter -t PID --all COMMAND 
# run COMMAND in the same namespaces as PID
```
Namespaces SystemCalls:
- <u>clone</u> - make a new process and also to place a child process in a different namespace
- <u>unshare</u> - make + use a namespace
- <u>setns</u> - use an existing namespace
### Process ID Namespaces
The Same process has different PIDs in different namespaces. 

| PID in host | PID in container |
| ----------- | ---------------- |
| 23512       | 1                |
| 23513       | 4                |
| 23514       | 12               |
You can see the process in a child PID namespaces 
PID 1 is the init process, killing it will kill all other processes
<u>Rules for signaling PID1</u>
- <u>From the same Container</u> -
	Only works if the process has set a signal handler
- <u>From the host</u> - 
	Only SIGKILL and SIGSTOP are ok, or if there is a signal handler.

### User Namespaces
User namespaces are a security feature, for example for the root of the container to be unprivileged. but not all containers use them :(
Root actually doesn't always have admin access, and he mostly have access to files he owns.
But in a username space UIDs are mapped to host UIDs for example if a unmapped users show as nobody, and mapped users are showed to their corresponding map on the host system

### Network Namespace
Namespaces usually have 2 interfaces or more:
- The loopback interface (127.0.0.1/8 for connections inside the namespace).
- Another interface with it's own address for connections from outside.
Every server listens on a port and a network interface(s) 0.0.0.0:8080 means 
"Port 8080 on every network interface in my namespace"
The physical network card on the machine is connected to the host network namespace. 
Other namespaces are connected to the host namespace with a <u>bridge</u>. 
#### Container IP
Container often get their own IP address and they mostly user private addresses (192.168.x.x 10.x.x.x 172.32.x.x as per RFC 1918) even if the machine hosting them has it's own public IP This is because they are not directly are on the network, but are hosted inside a computer.
For packets to get forwarded correctly there are routs, each computer has inside it it's own routing table. 

### Capabilities
We always think of root as being all-powerful, but actually to do "root" a process needs to have all the <u>capabilities</u>, there are many capabilities, but some important ones are
- <u>CAP_SYS_ADMIN</u> - Lets you do a LOT of things (avoid giving if can)
- <u>CAP_NET_ADMIN</u> - Allow changing network settings
By default containers have limited capabilities 
```bash
$ getcaps PID 
# Print the capabilities that process PID has
$ getcap / setcap
# are system calls to get and set capabilities
```
### seccomp-bpf
All programs use system calls, rarely-used system calls can help an attacker, <u>seccomp-bpf</u> lets you run a function before every system call that a process makes and the function can decide if the syscall is allowed. Docker <u>blocks</u> dozens of syscalls by default.
There are mostly 2 ways to block system calls:
- Limit container capabilities
- set a seccop-bpf whitelist
**Both should be configured**
# Configuration Options
Six most important things that can be configured on a container.
## Map a port to the host
## Mount directories from the host
## Set capabilities
## Add seccomp-bpf
## Set Memory and CPU limit
## Use the host network namespace

# Virtual Machines VS Containers

| .              | Container                        | Virtual Machine               |
| -------------- | -------------------------------- | ----------------------------- |
| OS             | Shares the host operating system | has it's own operating system |
| Portability    | more portable                    | less portable                 |
| Speed          | Faster to start up and shutdown  | Slower                        |
| Resource usage | Uses less resources              | Uses more resources           |
VMs are mostly used to test software with different operating systems, Test software and malware in a isolated environment. 
Containers are used to deploy software on the computer while sharing the same kernel and os
# The Docker File and Building an Image
<u>Docker Image OR Docker File</u> - This is a template with commands of how to configure and initialize the container to a state the developer wants.
- Images are immutable, Once an image is created it can't be modified, Only a new image can be created.
- Container images are composed of layers, each layer represents a set of file system changes that add, remove or modify files. 
## Image Layer 
Each layer on an image contains a set of filesystem changes - additions, deletions, or modifications 
for example: 
1. The first layer adds basic commands and a package manager such as APT 
2. The second layer installs a python runtime, and pip for dependency management
3. The third layer copies a requirements.txt file 
4. The forth layer install the dependencies
5. The fifth layer copies the actual source code of the app
This is useful because layers can be reused in multiple containers!
For example if two layers run a python application they can share the same python layer which makes builds faster and requires less storage.
## Stacking The Layers
This is made possible due to content-addressable storage and union file systems
<u>content-addressable storage</u> - calculate the location of the storage based on it's hash so the same storage (or layer in this case) will be saved on the same location
<u>union file systems</u> - like a union data structure in C, this FS allows to files and directories from other file systems to be overlapped, so only the top layer matters.
1. After each layer is downloaded, it is extracted into it's own directory on the host FS
2. When a container is ran from the image, a Union FS is created where layers are stacked on top of each other creating a new unified view.
3. When the container starts, its root directory is set to the location of the unified directory using chroot (or pivot_root
## Building an Image
Images can be built with several tools beside docker. 
### Podman
This is a daemonless container engine that can build, run and manage containers and it is fully docker compatible 
- No need for background service
- Rootless containers for better security 
- Fully docker compatible
- Can run kubernetes pods natively  
### Kaniko
Tool for building container images inside kubernetes clusters
- Runs without requiring privileged access
- Designed for environments that docker is not available like CI/CD piplines
### Buildah
This is a tool specifically designed to build Open Container Initiative (OCI) images without a container runtime.
- Can build images without container engine
- More flexible then docker, lets you to create images from scratch and modify existing 
- Can be used inside kubernetes
### The From Instruction
This instruction initializes a new build and sets the base image for the subsequent instructions, every docker file must start with this instruction

## Multi-Stage Build
Multi-Stage build uses multiple from statement in the image, for example, if the application is compilable, firstly, the source code and dependencies can be installed and then compile the code, then, start a FROM scratch, instruction after the compilation, copy the compiled application and libraries, and then you can just delete all dependencies and source code for a really small container.

### CMD vs ENTRYPOINT
The default values in CMD can be changed when running the docker file, including the running executable from the docker run cli. While in ENTRYPOINT, the arguments are given from the CMD or default ones, but the executable is given from the ENERYPOINT, the arguments can be overridden.
```dockerfile
FROM ubuntu
ENTERYPOINT ["echo"]
CMD ["Hello World"]
```
The argument "Hello World" can be overridden to print something else but the command <u>can't be overidden</u> 
```dockerfile
FROM ubuntu
CMD ["echo", "Hello World"]
```
In this example the command can be overridden to run something else, which is not recommended.
The combination of CMD and ENTRYPOINT makes CMD to add arguments to the entry point.

### Basic Docker Commands

<u>build</u> -> Create a docker image from the docker file
```bash
docker build -t [IMAGE_TAG]:[TAG] [PATH_TO_DOCKERFILE]
docker build -t mycustomimage:v1 .
```
<u>run</u> -> This runs a docker image in a container
<u>Ps</u> -> List of running containers and their state, similar to ps command on linux
<u>Images</u> -> List all the installed images on the local machine. 
<u>Exec</u> -> This command is used to execute some command inside the container. 
```bash
docker exec -it [CONTAINER_ID] [COMMAND]
docker exec -it aaaaaaa /bin/bash
# This command is used to execute a shell inside the container
# The -it flag is interactive mode
```
<u>Logs</u> -> get logs from a certain container, the --follow will print in live the STDOUT and STDERR of the container to the local terminal
<u>Stop</u> -> This command will stop the running container
<u>Start</u> -> Start images that have been stopped or existed, unlike run which is only for new images
<u>Inspect</u> -> Get low level and detailed information about a container
<u>Info</u> -> This command displays information about the docker installation itself and the system running it.
<u>Saved</u -> Save and image as a tar archive so it can be transported efficiently, then can be loaded with docker load
<u>Load</u> -> load a tar archive of a docker image, saved with docker save.

## Container Layers
Docker build itself on top of layers, each layer has cache and an 'intermediate image', the intermediate image takes more space, but can be cached for faster builds. the cache works by hashing the content so the same content will be reused in next builds. Once a layer is created it can't be modified and a new layer needs to be created.
#### Scratch Image
This is the most minimal image that can be created, and this is the base ancestor for all other images. This is actually an empty image, this can be used for a statically compiled executable which is self contained so it doesn't need anything else to run. This is efficient for a multi-stage build the in the first stage it builds the code and links it statically, and then runs the static executable in a scratch image.

### Docker Images Best Practices
#### Use Multi-Stage Builds 
This can help reducing the final size of the image, for example by using scratch image 
#### Create Reusable Stages
If multiple Images has a lot in common, creating reusable layers that can be cached can drastically improve build time, and storage usage.
#### Choose The Right Base Image
When choosing an image, make sure that it contains only what is needed for the application to run, and nothing more, and also that it is from a trusted host 
#### Rebuild Images Often
Docker images are immutable, and building an image is basically taking a snapshot of that image at the moment, so it is recommended to build often and with --no-cache, so the image have the latest and most correct dependencies.
#### Use .dockerignore
This is common to copy files to the image with ```COPY . .``` This can copy garbage files to the image and make it bigger, the .dockerignore file makes it so files that are listed there will not be copied.
#### Create Ephemeral Container
This is containers that they can be stopped and destroyed at any given moment, and can be rebuilt and replaced with very minimal configuration.
#### Decouple Applications
Each container should have only one concern, this makes the application much more scalable, because containers can be reused. For instance, web application should have one container for the application, one for DB and one for in-memory or cache. 
#### Sort Multi-Line arguments
```dockerfile
RUN apt-get update && apt-get install -y --no-install-recommends \
  bzr \
  cvs \
  git \
  mercurial \
  subversion \
  && rm -rf /var/lib/apt/lists/*
```
This helps in maintenance of the file, and for other using, and changing the file.
#### Leverage Build Cache
Every instruction in the docker image can be reused from other layer, if you know multiple images have similar dependencies, cache can be reused and thus save storage and memory
#### Pin Base Image Versions 
If the same base image is important for execution pin the version
```dockerfile
FROM alpine:3.19
```
This may be problematic if the exact same base image is needed, because the developer can change what is inside that tag, or different patches like 3.19.3 or 3.19.4 can be used, to make sure you get the exact same image the digest hash can be used.
```dockerfile
FROM alpine:3.19@sha256:13b7e62e8df80264dbb747995705a986aa....
```
This makes sure that the same image is used.
#### Build and Test with CI
Building and testing the image in CI makes sure that the production branch has a working version that is buildable, and can be used by customers. 
## Open Container Initiative
This is an open image format that can be used to build container images, this format can be used to build docker images, and create images in other platforms such as buildha, kaniko, podman etc..
<u>Podman</u> -> Fully docker and OCI compatible container runtime and management (build, run, network, volumes, etc). This container engine is daemonless and support rootless containers.   
<u>Buildah</u> -> This is a container image building tool that is daemonless. Supports rootless image building. This is fully OCI and Docker compatible.
<u>Docker</u> -> Full container management (run, build, network, volumes, etc), docker requires the docker daemon which runs as root. docker is compliant with OCI but it tied to the docker echosystem.
<u>Docker CE</u> -> (Community Edition) is a free version of docker, that is less mature and contains updates from the community. Docker EE (Enterprise Edition) is a subscription based docker engine that is more mature and contains more Enterprise oriented features like 
# Container Networking
Container networking allows container to communicate with other containers, or with the internet. There are several popular container network models ->
### <u>None</u>
The simplest mode of networking is a loop back interface in which the container does not communicate with an external network.
This container network type ensures that the container gets a network stack, The container don't have external network interface but receives a loop back.   
### <u>Bridge</u>
An internal host network that enables communication between containers on the same host
A Linux bridge provides the internal host communication between containers on the same host. Bridge networking employs IP tables for NAT and port mapping to provide single-host networking and its the default docker networking type, and has its interface 'docker0'
basically a switch that 
### <u>Host</u>
Allows a container to share its network namespace with the host, enabling high speed networking.
In this configuration, the container and the host are on the same networking interface, so the communication is shared, this can lead to port conflicts, The container can't configure the networking stack of the host unless in privilege mode
### <u>Overlays</u>
Overlay network allows containers on multiple physical hosts to communicate with each other and the see each other on the same LAN, this is like a VPN for containers, containers that are not on the network don't have access to the packets that are sent via this network.
## Container Registries 
Sharing container images is useful. because it has the code with all of it's dependencies, so it is very easy to run. 
- A registry is a server that serves images, each image has it's own unique ID which is a hash of the content, and a tag like 18.4 or latest. Some of the registries are private and some or public. 
### Image Tagging 
Full image name has the following components:
- `[HOST[:PORT_NUMBER]/]PATH`
Example of image tag -> ```docker tag 0e5574283393 fedora/httpd:version1.0```
If a version is not specified, latest is set.
### Docker Login
This command is used to login into a registry that requires authentication. 
The credentials are from $HOME/.docker/config.json
The default store is docker hub.
### Docker Push
This command is used to push image to a registry so it can be shared, images are shared in a compressed form.
### Docker Pull
Try to pull an image from the registry, if a tag is not specified, the latest tag will be added
### Local VS Remote Regsitries
Local registries are inside information of an organization, they are more secured because only people from the organization can add images to it, This can also work in an air-gapped environment, most of the time this requires lower latency. 
Remote registries are access