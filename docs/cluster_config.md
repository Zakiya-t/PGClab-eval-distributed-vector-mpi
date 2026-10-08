# Cluster Configuration

## Nodes

| Node | MPI Role | Hostname | IP | vCPU | RAM |
|---|---|---|---|---:|---:|
| Master | Rank 0 | master | 192.168.217.128 | 4 | 4.8 GiB |
| Worker 1 | Rank 1 | worker1 | 192.168.217.130 | 4 | 4.8 GiB |
| Worker 2 | Rank 2 | worker2 | 192.168.217.131 | 4 | 4.8 GiB |

## Network

The VMs communicate through the VMware `192.168.217.0/24` network. The `172.17.0.1` interface is the Docker bridge and is not used for MPI communication.

Open MPI is forced onto the VMware network with:

```bash
--mca btl_tcp_if_include 192.168.217.0/24
--mca oob_tcp_if_include 192.168.217.0/24
```

## SSH Verification

From Master:

```bash
ssh -o BatchMode=yes worker1 hostname
ssh -o BatchMode=yes worker2 hostname
```

Expected:

```text
worker1
worker2
```

## Hostfile

The repository hostfile uses hostnames so the cluster can be recreated after hostname resolution is configured:

```text
master slots=1
worker1 slots=1
worker2 slots=1
```
