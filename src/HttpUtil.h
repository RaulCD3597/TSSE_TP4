#include <stdlib.h>
#include <stdint.h>

typedef enum {
    HTTP_UNKNOWN = -1,
    HTTP_GET = 0,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_HEAD,
    HTTP_OPTIONS
} HttpMethod;

HttpMethod HTTPLib_getRequestMethod(char *httpMsg, int32_t *err);

void HTTPLib_getRequestPath(char *httpMsg, char *res, int32_t *err);

void HTTPLib_getHeader(char *httpMsg, char **res, char *search, int32_t *err);

uint8_t HTTPLib_getCookies(char* httpMsg, char ***cookies, char* search);

int HTTPLib_getPayload(char* httpMsg, char **payload);