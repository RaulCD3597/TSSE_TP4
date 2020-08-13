#include "HttpUtil.h"
#include <string.h>

#define HTTP_BODY_SEPARATOR "\r\n\r\n"

static uint8_t FirstWordCpy(char *Msg, char *buffer, uint8_t maxSize)
{
    uint8_t size = 0;
    while (Msg[size] != ' ')
    {
        if (size == maxSize - 1)
        {
            break;
        }
        size++;
    }
    strncpy(buffer, Msg, size);
    buffer[size + 1] = '\0';
    return size;
}

HttpMethod HTTPLib_getRequestMethod(char *httpMsg, int32_t *err)
{
    char method[10];
    uint8_t size = FirstWordCpy(httpMsg, method, 10);
    if (!strncmp(method, "GET", size))
    {
        *err = 0;
        return HTTP_GET;
    }
    else if (!strncmp(method, "POST", size))
    {
        *err = 0;
        return HTTP_POST;
    }
    else if (!strncmp(method, "PUT", size))
    {
        *err = 0;
        return HTTP_PUT;
    }
    else if (!strncmp(method, "DELETE", size))
    {
        *err = 0;
        return HTTP_DELETE;
    }
    else if (!strncmp(method, "OPTIONS", size))
    {
        *err = 0;
        return HTTP_OPTIONS;
    }
    else if (!strncmp(method, "PATCH", size))
    {
        *err = 0;
        return HTTP_PATCH;
    }
    else if (!strncmp(method, "HEAD", size))
    {
        *err = 0;
        return HTTP_HEAD;
    }
    else
    {
        *err = -1;
        return HTTP_UNKNOWN;
    }
}

void HTTPLib_getRequestPath(char *httpMsg, char *res, int32_t *err)
{
    httpMsg = strstr(httpMsg, "/");
    if (NULL == httpMsg)
    {
        *err = -1;
        return;
    }
    uint16_t index = 0;

    while (' ' != (*httpMsg))
    {
        res[index] = (*httpMsg);
        index++;
        httpMsg++;
    }
    
    *err = 0;
    res[index] = 0;
}

void HTTPLib_getHeader(char *httpMsg, char **res, char *search, int32_t *err)
{
    (*res) = strstr(httpMsg, search);
    if ((*res) != NULL)
    {
        (*res) = strstr((*res), " ");
        (*res)++;
        (*res) = strtok((*res), "\r");
        *err = 0;
    }
    else 
    {
        *err = -1;
    }
}

uint8_t HTTPLib_getCookies(char* httpMsg, char ***cookies, char* search)
{
    int numOfCookies = 0;
    char *msg = strstr(httpMsg, search);
    char buffer[10][300];
    while (msg != NULL)
    {
        msg = strstr(msg, " ");
        msg++;
        int charCount = 0;
        while (msg[charCount] != '\r')
        {
            charCount++;
        }
        strncpy(buffer[numOfCookies], msg, charCount);
        buffer[numOfCookies][charCount] = '\0';
        numOfCookies++;
        msg = strstr(msg, search);
    }
    int index;
    (*cookies) = malloc(numOfCookies * sizeof(char*));
    for (index = 0; index < numOfCookies; index++)
    {   
        (*cookies)[index] = buffer[index];
    }
    return numOfCookies;
}

int HTTPLib_getPayload(char* httpMsg, char **payload){
	(*payload) = strstr(httpMsg, HTTP_BODY_SEPARATOR);
	if (*payload){
		(*payload) += 4;
		return 1;
	}else{
		return 0;
	}
}