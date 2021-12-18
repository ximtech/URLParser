#include "URLParser.h"

#define LINE_END '\0'

static void parseUrlProtocol(URLParser *url);

static void parseUrlUsernamePasswordIfPresent(URLParser *url);
static bool isUserPassSpecified(const char *usernamePasswordStart);
static inline bool isUsernameEndDelimiter(char character);
static inline bool isPasswordEndDelimiter(char character);

static void parseUrlHost(URLParser *url);
static inline bool isEndOfIPv6Host(char character);
static inline bool isEndOfHost(char character);

static void parseUrlPortIfPresent(URLParser *url);
static inline bool isPortNumberEnd(char character);

static void parseUrlPathIfPresent(URLParser *url);
static inline bool isUrlPathEnd(char character);

static void parseUrlParametersIfPresent(URLParser *url);
static inline bool isEndOfParameters(char character);

static void parseUrlFragmentIfPresent(URLParser *url);


void parseUrlString(URLParser *url, const char *urlString) {    // See RFC 1738, 3986
    memset(url->protocol, 0, URL_PROTOCOL_SIZE);
    memset(url->host, 0, URL_HOST_SIZE);
    memset(url->path, 0, URL_PATH_SIZE);
    memset(url->parameters, 0, URL_PARAMETERS_SIZE);
    memset(url->fragment, 0, URL_FRAGMENT_SIZE);
    memset(url->username, 0, URL_USERNAME_SIZE);
    memset(url->password, 0, URL_PASSWORD_SIZE);

    url->urlCursor = urlString;
    url->isUrlValid = true;
    url->port = 0;

    if (isStringBlank(urlString)) {
        url->isUrlValid = false;
        return;
    }
    parseUrlProtocol(url);
    parseUrlUsernamePasswordIfPresent(url);
    parseUrlHost(url);
    parseUrlPortIfPresent(url);

    bool isUrlFinished = (!url->isUrlValid || *url->urlCursor == LINE_END);
    if (isUrlFinished) return;  // End of the string

    parseUrlPathIfPresent(url);
    parseUrlParametersIfPresent(url);
    parseUrlFragmentIfPresent(url);
}

static void parseUrlProtocol(URLParser *url) {
    if (!url->isUrlValid) return;
    const char *protocolEndPointer = strchr(url->urlCursor, ':'); // Read protocol
    bool isProtocolFound = (protocolEndPointer != NULL);
    if (!isProtocolFound) {
        url->isUrlValid = false;
        return;
    }

    uint8_t protocolLength = protocolEndPointer - url->urlCursor; // Get the protocol length
    for (uint8_t i = 0; i < protocolLength; i++) {  // Check restrictions
        bool isNotValidProtocolChar = (isalpha(url->urlCursor[i]) == 0);
        if (isNotValidProtocolChar) {
            url->isUrlValid = false;
            return;
        }
    }
    strncpy(url->protocol, url->urlCursor, protocolLength);   // Copy the protocol to the storage
    toLowerCaseString(url->protocol);  // Make the character to lower if it is upper case

    protocolEndPointer++;   // Skip ':'
    if (!containsString(protocolEndPointer, "//")) {// Check that after protocol next "//"
        url->isUrlValid = false;
        return;
    }
    protocolEndPointer += 2;  // and skip "//"
    url->urlCursor = protocolEndPointer;
}


static void parseUrlUsernamePasswordIfPresent(URLParser *url) {
    if (!url->isUrlValid) return;
    const char *usernamePasswordPointer = url->urlCursor;

    if (isUserPassSpecified(usernamePasswordPointer)) { // User and password specification
        while (!isUsernameEndDelimiter(*usernamePasswordPointer)) { // Read username
            usernamePasswordPointer++;
        }

        uint16_t usernameLength = (usernamePasswordPointer - url->urlCursor);
        if (usernameLength > URL_USERNAME_SIZE) {
            url->isUrlValid = false;
            return;
        }
        strncpy(url->username, url->urlCursor, usernameLength);

        if (*usernamePasswordPointer == ':') {  // Proceed current pointer
            usernamePasswordPointer++;  // Skip ':'

            uint16_t passwordLength = 0;
            while (!isPasswordEndDelimiter(*usernamePasswordPointer)) {
                usernamePasswordPointer++;
                passwordLength++;
            }

            if(passwordLength > URL_PASSWORD_SIZE) {
                url->isUrlValid = false;
                return;
            }
            const char *passwordStartPointer = (usernamePasswordPointer - passwordLength);
            strncpy(url->password, passwordStartPointer, passwordLength);
        }

        bool isUsernamePassCorrectlyFormatted = (*usernamePasswordPointer == '@');
        if (!isUsernamePassCorrectlyFormatted) {
            url->isUrlValid = false;
            return;
        }
        usernamePasswordPointer++;  // Skip '@'
    }
    url->urlCursor = usernamePasswordPointer;
}

static bool isUserPassSpecified(const char *usernamePasswordStart) {
    const char *usernamePasswordPointer = usernamePasswordStart;
    while (*usernamePasswordPointer != LINE_END) {  // Check if the user (and password) are specified
        if (*usernamePasswordPointer == '@') {
            return true; // Username and password are specified
        } else if (*usernamePasswordPointer == '/') {
            return false;    // End of <host>:<port> specification
        }
        usernamePasswordPointer++;
    }
    return false;
}

static inline bool isUsernameEndDelimiter(char character) {
    return character == LINE_END || character == ':';
}

static inline bool isPasswordEndDelimiter(char character) {
    return character == LINE_END || character == '@';
}


static void parseUrlPortIfPresent(URLParser *url) {
    if (!url->isUrlValid) return;
    const char *portNumberPointer = url->urlCursor;

    bool isPortNumberSpecified = (*url->urlCursor == ':');
    if (isPortNumberSpecified) {
        portNumberPointer++;    // Skip ':'

        uint8_t portNumberLength = 0;
        while (!isPortNumberEnd(*portNumberPointer)) {   // Read port number
            portNumberPointer++;
            portNumberLength++;
        }

        const char *portNumberStartPointer = (portNumberPointer - portNumberLength);
        char portString[portNumberLength + 1];
        memset(portString, 0, portNumberLength + 1);
        strncpy(portString, portNumberStartPointer, portNumberLength);
        url->port = strtol(portString, NULL, 10);
    }
    url->urlCursor = portNumberPointer;
}

static inline bool isPortNumberEnd(char character) {
    return character == LINE_END || character == '/' || character == '?' || character == '#';
}


static void parseUrlHost(URLParser *url) {
    if (!url->isUrlValid) return;
    const char *hostPointer = url->urlCursor;

    uint32_t hostLength = 0;
    bool isHostHaveIPv6StartBracket = (*hostPointer == '[');
    while (*hostPointer != LINE_END && *hostPointer != '?' && *hostPointer != '#') { // Proceed on by delimiters with reading host
        if (isHostHaveIPv6StartBracket && isEndOfIPv6Host(*hostPointer)) {
            hostPointer++;  // End of IPv6 address.
            break;
        } else if (!isHostHaveIPv6StartBracket && isEndOfHost(*hostPointer)) {
            break;  // Port number is specified.
        }
        hostPointer++;
        hostLength++;
    }

    if (hostLength == 0 || hostLength > URL_HOST_SIZE) {
        url->isUrlValid = false;
        return;
    }
    const char *hostStartPointer = hostPointer - hostLength;
    strncpy(url->host, hostStartPointer, hostLength);
    url->urlCursor = hostPointer;
}

static inline bool isEndOfIPv6Host(char character) {
    return character == ']';
}

static inline bool isEndOfHost(char character) {
    return character == ':' || character == '/';
}


static void parseUrlPathIfPresent(URLParser *url) {
    if (!url->isUrlValid) return;
    const char *pathPointer = url->urlCursor;

    bool isUrlFinished = (*pathPointer == LINE_END || *pathPointer == '?' || *pathPointer == '#'); // line end or query start
    if (isUrlFinished) {  // End of the string
        return;
    }

    bool isPathStartValid = (*pathPointer == '/');
    if (!isPathStartValid) {
        url->isUrlValid = false;
        return;
    }
    pathPointer++;  // Skip '/'

    uint16_t pathLength = 0;
    while (!isUrlPathEnd(*pathPointer)) {    // Parse path
        pathPointer++;
        pathLength++;
    }

    if (pathLength > URL_PATH_SIZE) {
        url->isUrlValid = false;
        return;
    }
    strncpy(url->path, pathPointer - pathLength, pathLength);
    url->urlCursor = pathPointer;
}

static inline bool isUrlPathEnd(char character) {
    return character == LINE_END || character == '#' || character == '?';
}


static void parseUrlParametersIfPresent(URLParser *url) {
    if (!url->isUrlValid) return;
    const char *parametersPointer = url->urlCursor;
    bool isParametersSpecified = *parametersPointer == '?';
    if (isParametersSpecified) {
        parametersPointer++;    // Skip '?'

        uint16_t parametersLength = 0;
        while (!isEndOfParameters(*parametersPointer)) {   // Read parameters
            parametersPointer++;
            parametersLength++;
        }

        if (parametersLength > URL_PARAMETERS_SIZE) {
            url->isUrlValid = false;
            return;
        }
        const char *parametersStartPointer = parametersPointer - parametersLength;
        strncpy(url->parameters, parametersStartPointer, parametersLength);
    }
    url->urlCursor = parametersPointer;
}

static inline bool isEndOfParameters(char character) {
    return character == LINE_END || character == '#';
}

static void parseUrlFragmentIfPresent(URLParser *url) {
    if (!url->isUrlValid) return;
    const char *fragmentPointer = url->urlCursor;
    bool isFragmentSpecified = (*fragmentPointer == '#');

    if (isFragmentSpecified) {
        fragmentPointer++;  // Skip '#'

        uint16_t fragmentLength = 0;
        while (*fragmentPointer != LINE_END) {   // Read fragment
            fragmentPointer++;
            fragmentLength++;
        }

        if(fragmentLength > URL_FRAGMENT_SIZE) {
            url->isUrlValid = false;
            return;
        }
        const char *fragmentStartPointer = fragmentPointer - fragmentLength;
        strncpy(url->fragment, fragmentStartPointer, fragmentLength);
    }
    url->urlCursor = fragmentPointer;
}