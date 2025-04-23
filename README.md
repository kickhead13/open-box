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
$ ./build
```
in the root directory of the project. And running the project means running one of the executables built into the ./target/ folder. For example, running the server is as easy as running the following command (after building the project):
```sh
$ ./target/server
```

