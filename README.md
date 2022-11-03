# URL Parser

[![tests](https://github.com/ximtech/URLParser/actions/workflows/cmake-ci.yml/badge.svg)](https://github.com/ximtech/URLParser/actions/workflows/cmake-ci.yml)
[![codecov](https://codecov.io/gh/ximtech/URLParser/branch/main/graph/badge.svg?token=GFIQA6J04M)](https://codecov.io/gh/ximtech/URLParser)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/9cefce8be92847d5b2b6dbd06e5a7312)](https://www.codacy.com/gh/ximtech/URLParser/dashboard)

URLParser parser allows splitting the URL into individual components.

### Features

Library parses URL for such components:

- ***Protocol***. Determines how data is transferred between the host and a web browser (or client).
  Example: `HTTP, HTTPS, FTP, DNS, DHCP, IMAP, SMTP`
- ***Host***. The name or address of the webserver to be accessed. Hostname is not case-sensitive (
  e.g., `www.somedb.com`
  and `WWW.SomeDb.com` are equivalent)
- ***Port***. A number used to identify a specific webserver at the provided hostname. When omitted, a scheme specific
  default value is used. For http, the default is `80`. For https, the default is `443`.
- ***Path***. The portion of the URL from a slash `/` following the origin up to the query or fragment. When
  omitted, the default path `/` is used.
- ***Parameters***. URL parameter is a way to pass information about a click through its URL. For
  example, `http://example.com?product=1234&utm_source=google`
- ***Fragment***. A fragment is an internal page reference, sometimes called a named anchor. It refers to a section
  within a web page.
- ***Username***. The standard method to pass basic authentication to web servers
- ***Password***. The standard method to pass basic authentication to web servers

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME URLParser
        GITHUB_REPOSITORY ximtech/URLParser
        GIT_TAG origin/main)

target_link_libraries(${PROJECT_NAME} URLParser)
```

```cmake
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
# For Clion STM32 plugin generated Cmake use 
target_link_libraries(${PROJECT_NAME}.elf URLParser)
```

### Usage

```c
URLParser parser;
// common usage
parseUrlString(&parser, "http://example.com/api/test");
if (parser.isUrlValid) {
  printf("Protocol: %s\n", parser.protocol);
  printf("Host: %s\n", parser.host);
  printf("Path: %s\n", parser.path);
}

// all in one
parseUrlString(&parser, "https://jack:password@localhost:8989/path/to/test?query=yes&q=jack#fragment1");
if (parser.isUrlValid) {
  printf("Protocol: %s\n", parser.protocol);
  printf("Host: %s\n", parser.host);
  printf("Port: %d\n", parser.port);
  printf("Path: %s\n", parser.path);
  printf("Params: %s\n", parser.parameters);
  printf("Fragment: %s\n", parser.fragment);
  printf("Username: %s\n", parser.username);
  printf("Password: %s\n", parser.password);
}
```
