## Open Box 

### Description

Open Box is a file encryption, decryption and hashing ecosystem that provides server, user and administration tools. It allows setting up a server handling client file encryption/hashing requests. It has support for modern and classic encryption/hashing algorithms such as:
  - AES
  - RSA
  - TWOFISH
  - CHACHA
  - SHA
  - many more

### Building
The project uses the no build philosophy, and as such it uses tsoding's [nob.h](https://github.com/tsoding/nob.h) implementation. To build the project you have to run:
```sh
$ gcc -o debug nob.c && gcc -o install nob.c -DINSTALL_FOLDER="\"/usr/bin/\""
$ ./debug   # compile debug build in ./target/
$ ./install # compile system wide version (at /usr/bin/openbox)
```
in the root directory of the project. This will compile two executables "debug" and "install". If you want to toy around with openbox you can run the "debug" executable to compile the project in the "./target/" directory. To actually install openbox system-wide, you have to run the "install" executable, which will install openbox in "/usr/bin/". And running the project means running one of the executables built into the ./target/ folder. For example, running the server is as easy as running the following command (after building the project):
```sh
$ openbox server
```

### Dependencies
Openbox has the following dependencies:
 - libgmp (for RSA implementation) [ubuntu](https://installati.one/install-libgmp-dev-ubuntu-22-04/), [gentoo](https://packages.gentoo.org/packages/dev-libs/gmp), [arch](https://archlinux.org/packages/core/x86_64/gmp/), [build from source](https://gmplib.org/manual/Installing-GMP)
 - libnettle (for other encryption algorithms) [ubuntu](https://installati.one/install-libnettle7-ubuntu-20-04/), [gentoo](https://packages.gentoo.org/packages/dev-libs/nettle), [arch](https://archlinux.org/packages/core/x86_64/nettle/)
