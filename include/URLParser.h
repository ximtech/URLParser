#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "StringUtils.h"

#define URL_PROTOCOL_SIZE	8
#define URL_HOST_SIZE	    80
#define URL_PATH_SIZE       80
#define	URL_PARAMETERS_SIZE	80
#define URL_FRAGMENT_SIZE   26
#define URL_USERNAME_SIZE	26
#define	URL_PASSWORD_SIZE	26

typedef struct URLParser {
    const char *urlCursor;
    char protocol[URL_PROTOCOL_SIZE]; // Mandatory. Determines how data is transferred between the host and a web browser (or client). Example: HTTP, HTTPS, FTP, DNS, DHCP, IMAP, SMTP
    char host[URL_HOST_SIZE];        // Mandatory. The name or address of the webserver to be accessed. Hostname is not case-sensitive (e.g., www.somedb.com and WWW.SomeDb.com are equivalent)
    uint16_t port;                 // Optional. A number used to identify a specific webserver at the provided hostname. When omitted, a scheme specific default value is used. For http, the default is 80. For https, the default is 443.
    char path[URL_PATH_SIZE];     // Optional. The portion of the URL from a slash "/" following the origin up to the query or fragment. When omitted, the default path "/" is used.
    char parameters[URL_PARAMETERS_SIZE];  // Optional. URL parameter is a way to pass information about a click through its URL. For example, http://example.com?product=1234&utm_source=google
    char fragment[URL_FRAGMENT_SIZE];  // Optional. A fragment is an internal page reference, sometimes called a named anchor. It refers to a section within a web page.
    char username[URL_USERNAME_SIZE]; // Optional. The standard method to pass basic authentication to web servers
    char password[URL_PASSWORD_SIZE]; // Optional. The standard method to pass basic authentication to web servers
    bool isUrlValid;
} URLParser;

void parseUrlString(URLParser *url, const char *urlString);