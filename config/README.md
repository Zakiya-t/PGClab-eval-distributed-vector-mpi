# Configuration

`hosts` lists the three MPI execution nodes. Hostnames are used instead of hard-coded IP addresses so the repository remains readable and the cluster can be recreated after configuring `/etc/hosts`.

```text
master slots=1
worker1 slots=1
worker2 slots=1
```
