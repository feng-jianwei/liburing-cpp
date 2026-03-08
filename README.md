# liburing-cpp
```shell
git clone https://github.com/feng-jianwei/liburing-cpp.git && cd liburing-cpp
conan install . --build=missing --profile:all=profiles/clang-release --deployer=full_deploy --deployer-folder=third_party
cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -S . -B build/Release -G Ninja
cmake --build build/Release --parallel 16 --target all
```

```
sudo bpftrace -e '
tracepoint:syscalls:sys_enter_mmap
/strncmp(comm, "unitest_", 8) == 0/ {

    // 打印mmap
    printf("\n[%s] PID %d TID %d comm \"%s\" mmap enter:\n",
           strftime("%H:%M:%S", nsecs), pid, tid, comm);
    printf("  addr    = 0x%lx\n", args->addr);
    printf("  length  = %lu (%lu MB)\n", args->len, args->len / 1024 / 1024);
    printf("  prot    = 0x%x (%s%s%s%s)\n", args->prot,
           (args->prot & 0x1) ? "PROT_READ " : "",
           (args->prot & 0x2) ? "PROT_WRITE " : "",
           (args->prot & 0x4) ? "PROT_EXEC " : "",
           (args->prot == 0) ? "PROT_NONE" : "");
    printf("  flags   = 0x%x (%s%s%s%s%s)\n", args->flags,
           (args->flags & 0x01) ? "MAP_SHARED " : "MAP_PRIVATE ",
           (args->flags & 0x10) ? "MAP_FIXED " : "",
           (args->flags & 0x20) ? "MAP_ANONYMOUS " : "",
           (args->flags & 0x80) ? "MAP_NORESERVE " : "",
           (args->flags & 0x100000) ? "MAP_POPULATE " : "");
    printf("  fd      = %d\n", args->fd);
}
'
```