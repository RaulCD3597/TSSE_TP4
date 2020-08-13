#include "unity.h"
#include "HttpUtil.h"
#include <string.h>

char response[4096];
char httpMsg[256];
int32_t err;

void setUp(void)
{
    strcpy(httpMsg, "GET /login/register?name=raul&password=sx1234 HTTP/1.1");

    strcpy(response, "HTTP/1.1 200 OK\r\n");
    strcat(response, "Cache-Control: no-store, no-cache\r\n");
    strcat(response, "Keep-Alive: true\r\n");
    strcat(response, "Pragma: no-cache\r\n");
    strcat(response, "Content-Length: 106\r\n");
    strcat(response, "Content-Type: application/json; charset=utf-8\r\n");
    strcat(response, "Expires: -1\r\n");
    strcat(response, "Server: Microsoft-HTTPAPI/2.0\r\n");
    strcat(response, "Set-Cookie: Seventh.Auth=eyJuIjoia2hvbXBfYXBpIiwiYSI6bnVsbH0.kO5coOf3cX3D8bjY3bTUXDQQ; max-age=571541; path=/; httponly\r\n");
    strcat(response, "Set-Cookie: Seventh.Situator.CurrentUser=eyJpZCI6MTIyNSwiYXV0aG9yaXphdGlvbnMiOlsxMDNdLCJuYW1lIjoiS2hvbXAgQVBJIiwibG9naW4iOiJraG9tcF9hcGkiLCJyZWFkTWV0aG9kIjowLCJhbGVydER1cmF0aW9uIjowfQ%3d%3d; max-age=571541; path=/app; httponly\r\n");
    strcat(response, "Set-Cookie: Seventh.Situator.Core=eyJjdXJyZW50VmVyc2lvbiI6IjUuOC4wLjY0IiwicHJvamVjdHNWaWRlbyI6ZmFsc2UsInByb2plY3RzQWNjZXNzIjpmYWxzZX0%3d; max-age=571541; path=/app; httponly\r\n");
    strcat(response, "Date: Wed, 26 Feb 2020 12:14:18 GMT\r\n");
    strcat(response, "Connection: close\r\n\r\n");
    strcat(response, "{\"id\":1225,\"authorizations\":[103],\"name\":\"Khomp API\",\"login\":\"khomp_api\",\"readMethod\":0,\"alertDuration\":0}");

    err = 0;
}

void test_HTTPLib_getRequestMethod(void)
{
    HttpMethod method = HTTPLib_getRequestMethod(httpMsg, &err);
    TEST_ASSERT_EQUAL_INT(method, HTTP_GET);
    TEST_ASSERT_EQUAL_INT(0, err);
}

void test_HTTPLib_getRequestPath()
{
    char *pathTest= "/login/register?name=raul&password=sx1234";
    int32_t err = 0;
    
    char *path = (char * )malloc(strlen(httpMsg));

    HTTPLib_getRequestPath(httpMsg,path,&err);

    TEST_ASSERT_EQUAL_STRING(pathTest,path);
    TEST_ASSERT_EQUAL_INT(0, err);
    free(path);
}

void test_HTTPLib_getHeader()
{   
    char expected[100] = "no-store, no-cache";
    char *res;
    HTTPLib_getHeader(response, &res, "Cache-Control", &err);
    TEST_ASSERT_EQUAL_STRING(expected, res);
}

void test_HTTPLib_getCookies()
{
    char expected[3][1024];
    strcpy(expected[0],"Seventh.Auth=eyJuIjoia2hvbXBfYXBpIiwiYSI6bnVsbH0.kO5coOf3cX3D8bjY3bTUXDQQ; max-age=571541; path=/; httponly");
    strcpy(expected[1],"Seventh.Situator.CurrentUser=eyJpZCI6MTIyNSwiYXV0aG9yaXphdGlvbnMiOlsxMDNdLCJuYW1lIjoiS2hvbXAgQVBJIiwibG9naW4iOiJraG9tcF9hcGkiLCJyZWFkTWV0aG9kIjowLCJhbGVydER1cmF0aW9uIjowfQ%3d%3d; max-age=571541; path=/app; httponly");
    strcpy(expected[2],"Seventh.Situator.Core=eyJjdXJyZW50VmVyc2lvbiI6IjUuOC4wLjY0IiwicHJvamVjdHNWaWRlbyI6ZmFsc2UsInByb2plY3RzQWNjZXNzIjpmYWxzZX0%3d; max-age=571541; path=/app; httponly");

    char **cookies;
    uint8_t numOfCookies = HTTPLib_getCookies(response, &cookies, "Set-Cookie");
    TEST_ASSERT_EQUAL_INT(3, numOfCookies);
    TEST_ASSERT_EQUAL_STRING(expected[0], cookies[0]);
    TEST_ASSERT_EQUAL_STRING(expected[1], cookies[1]);
    TEST_ASSERT_EQUAL_STRING(expected[2], cookies[2]);

    free(cookies);
}

void test_HTTPLib_getPayload()
{
    char expected[256] = "{\"id\":1225,\"authorizations\":[103],\"name\":\"Khomp API\",\"login\":\"khomp_api\",\"readMethod\":0,\"alertDuration\":0}";
    char *payload = NULL;
    HTTPLib_getPayload(response, &payload);
    TEST_ASSERT_EQUAL_STRING(expected, payload);
}