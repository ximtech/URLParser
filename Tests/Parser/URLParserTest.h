#pragma once

#include "BaseTestTemplate.h"
#include "URLParser.h"


static void assertBaseURL(URLParser *url, const char *protocol, const char *host, const char *path, const char *parameters) {
    if (protocol != NULL) assert_string_equal(url->protocol, protocol);
    if (host != NULL) assert_string_equal(url->host, host);
    if (path != NULL) assert_string_equal(url->path, path);
    if (parameters != NULL) assert_string_equal(url->parameters, parameters);
}

static void assertAdditionalURL(URLParser *url, const char *fragment, const char *username, const char *password) {
    if (fragment != NULL) assert_string_equal(url->fragment, fragment);
    if (username != NULL) assert_string_equal(url->username, username);
    if (password != NULL) assert_string_equal(url->password, password);
}

static inline void assertEmptyString(const char *value) {
    assert_int(strlen(value), ==, 0);
}

static MunitResult minimalUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assert_int16(url.port, ==, 0);
    assertEmptyString(url.path);
    assertEmptyString(url.parameters);
    assertEmptyString(url.fragment);
    assertEmptyString(url.username);
    assertEmptyString(url.password);
    return MUNIT_OK;
}

static MunitResult emptyPathUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com/");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assert_int(strlen(url.path), ==, 0);
    return MUNIT_OK;
}

static MunitResult withPathUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com/path");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", "path", NULL);
    return MUNIT_OK;
}

static MunitResult withPortUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com:80");
    assert_true(url.isUrlValid);
    assert_int16(url.port, ==, 80);
    return MUNIT_OK;
}

static MunitResult withQueryParametersUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com?query=only");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, "query=only");
    return MUNIT_OK;
}

static MunitResult withFragmentUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com#frag=f1");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assertAdditionalURL(&url, "frag=f1", NULL, NULL);
    return MUNIT_OK;
}

static MunitResult withCredentialsUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://username:password@example.com");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assertAdditionalURL(&url, NULL, "username", "password");
    return MUNIT_OK;
}

static MunitResult withPortAndPathUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com:8080/port/and/path");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", "port/and/path", NULL);
    assert_int16(url.port, ==, 8080);
    return MUNIT_OK;
}

static MunitResult withPortAndQueryUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com:8080?query=portANDquery");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, "query=portANDquery");
    assert_int16(url.port, ==, 8080);
    return MUNIT_OK;
}

static MunitResult withPortAndFragmentUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com:8080#f1");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assertAdditionalURL(&url, "f1", NULL, NULL);
    assert_int16(url.port, ==, 8080);
    return MUNIT_OK;
}

static MunitResult withPortAndCredentialsUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://username:password@example.com:8080");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assertAdditionalURL(&url, NULL, "username", "password");
    assert_int16(url.port, ==, 8080);
    return MUNIT_OK;
}

static MunitResult withPathAndQueryUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com/path/and/query?q=yes");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", "path/and/query", "q=yes");
    assert_int16(url.port, ==, 0);
    return MUNIT_OK;
}

static MunitResult withPathAndFragmentUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com/path/and#fragment");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", "path/and", NULL);
    assertAdditionalURL(&url, "fragment", NULL, NULL);
    return MUNIT_OK;
}

static MunitResult withQueryAndFragmentUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://example.com?q=yes#f1");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, "q=yes");
    assertAdditionalURL(&url, "f1", NULL, NULL);
    return MUNIT_OK;
}

static MunitResult withQueryAndCredentialsUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://username:password@example.com?q=yes");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, "q=yes");
    assertAdditionalURL(&url, NULL, "username", "password");
    return MUNIT_OK;
}

static MunitResult withEmptyCredentialsUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://:@example.com");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assertEmptyString(url.username);
    assertEmptyString(url.password);
    return MUNIT_OK;
}

static MunitResult withEmptyCredentialsAndPortUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "http://:@example.com:89");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "http", "example.com", NULL, NULL);
    assert_int16(url.port, ==, 89);
    assertEmptyString(url.username);
    assertEmptyString(url.password);
    return MUNIT_OK;
}

static MunitResult fullUrlOk(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "https://jack:password@localhost:8989/path/to/test?query=yes&q=jack#fragment1");
    assert_true(url.isUrlValid);
    assertBaseURL(&url, "https", "localhost", "path/to/test", "query=yes&q=jack");
    assertAdditionalURL(&url, "fragment1", "jack", "password");
    assert_int16(url.port, ==, 8989);
    return MUNIT_OK;
}


static MunitResult emptyUrlFail(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "");
    assert_false(url.isUrlValid);
    return MUNIT_OK;
}

static MunitResult protocolOnlyUrlFail(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "rtsp://");
    assert_false(url.isUrlValid);
    return MUNIT_OK;
}

static MunitResult hostnameOnlyUrlFail(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "hostname");
    assert_false(url.isUrlValid);
    return MUNIT_OK;
}

static MunitResult queryOnlyUrlFail(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "?query=only");
    assert_false(url.isUrlValid);
    return MUNIT_OK;
}

static MunitResult missingProtocolUrlFail(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "://");
    assert_false(url.isUrlValid);
    return MUNIT_OK;
}

static MunitResult missingHostnameUrlFail(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "rtsp://:8910/path");
    assert_false(url.isUrlValid);
    return MUNIT_OK;
}

static MunitResult missingCredentialsUrlFail(const MunitParameter params[], void *testData) {
    URLParser url;
    parseUrlString(&url, "rtsp://@hostname:8910/path");
    assert_false(url.isUrlValid);
    return MUNIT_OK;
}

static MunitTest urlParserTests[] = {
        {.name =  "Test OK parseUrlString() - Minimal URL", .test = minimalUrlOk},
        {.name =  "Test OK parseUrlString() - With empty path \"/\"", .test = emptyPathUrlOk},
        {.name =  "Test OK parseUrlString() - With path", .test = withPathUrlOk},
        {.name =  "Test OK parseUrlString() - With port", .test = withPortUrlOk},
        {.name =  "Test OK parseUrlString() - With fragment", .test = withFragmentUrlOk},
        {.name =  "Test OK parseUrlString() - With credentials", .test = withCredentialsUrlOk},
        {.name =  "Test OK parseUrlString() - With port and path", .test = withPortAndPathUrlOk},
        {.name =  "Test OK parseUrlString() - With port and query parameters", .test = withPortAndQueryUrlOk},
        {.name =  "Test OK parseUrlString() - With port and fragment", .test = withPortAndFragmentUrlOk},
        {.name =  "Test OK parseUrlString() - With port and credentials", .test = withPortAndCredentialsUrlOk},
        {.name =  "Test OK parseUrlString() - With path and query parameters", .test = withPathAndQueryUrlOk},
        {.name =  "Test OK parseUrlString() - With path and fragment", .test = withPathAndFragmentUrlOk},
        {.name =  "Test OK parseUrlString() - With query parameters and fragment", .test = withQueryAndFragmentUrlOk},
        {.name =  "Test OK parseUrlString() - With query parameters and credentials", .test = withQueryAndCredentialsUrlOk},
        {.name =  "Test OK parseUrlString() - With empty credentials", .test = withEmptyCredentialsUrlOk},
        {.name =  "Test OK parseUrlString() - With empty credentials and port", .test = withEmptyCredentialsAndPortUrlOk},
        {.name =  "Test OK parseUrlString() - Full URL", .test = fullUrlOk},

        {.name =  "Test FAIL parseUrlString() - Empty url", .test = emptyUrlFail},
        {.name =  "Test FAIL parseUrlString() - Protocol only", .test = protocolOnlyUrlFail},
        {.name =  "Test FAIL parseUrlString() - Hostname only", .test = hostnameOnlyUrlFail},
        {.name =  "Test FAIL parseUrlString() - Query only", .test = queryOnlyUrlFail},
        {.name =  "Test FAIL parseUrlString() - Missing protocol", .test = missingProtocolUrlFail},
        {.name =  "Test FAIL parseUrlString() - Missing hostname", .test = missingHostnameUrlFail},
        {.name =  "Test FAIL parseUrlString() - Missing credentials", .test = missingCredentialsUrlFail},
        END_OF_TESTS
};

static const MunitSuite urlParserTestSuite = {
        .prefix = "URLParser: ",
        .tests = urlParserTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};