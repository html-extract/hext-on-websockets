# Hext on Websockets

[Hext](https://hext.thomastrapp.com/) is a domain-specific language for extracting structured data from HTML documents.

This is the websocket server behind the "[Try Hext in your Browser!](https://hext.thomastrapp.com)" section.


## Dependencies

* CMake
* OpenSSL
* Boost
* RapidJSON
* [Hext](https://github.com/html-extract/hext)
* Optional: libseccomp
* Testing: websocat, jq


## Build

```
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8
ls -lh ./hextws
```

Build with seccomp enabled (see the list of allowed system calls in [SetupSeccomp.cpp](ws/SetupSeccomp.cpp), YMMV):

```
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DHEXTWS_SECCOMP=On .. && make -j8
ls -lh ./hextws
```


## Usage

```
./hextws <address> <port> <num-threads> <max-searches> \
    <path-to-cert> <path-to-key> <path-to-dhparam>
```


## License

All source code of this project is released under the Apache License v2.0.

