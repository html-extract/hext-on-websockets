# Hext on Websockets

[Hext](https://hext.thomastrapp.com/) is a domain-specific language for extracting structured data from HTML documents.

This is the websocket server behind the "[Try Hext in your Browser!](https://hext.thomastrapp.com)" section.


## Dependencies

* CMake
* OpenSSL
* Boost 1.70.0
* RapidJSON
* [Hext](https://github.com/html-extract/hext)
* Testing: websocat, jq


## Build

```
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
ls -lh ./hextws
```


## Usage

```
./hextws <address> <port> <num_threads> \
    <path-to-cert> <path-to-key> <path-to-dhparam>
```


## License

All source code of this project is released under the Apache License v2.0.

