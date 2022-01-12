#include "centralizedmsg-api.h"
#include "centralizedmsg-api-constants.h"

#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int validRegex(char *buf, char *reg)
{
    int reti;
    regex_t regex;
    reti = regcomp(&regex, reg, REG_EXTENDED);
    if (reti)
    { // If the regex didn't compile
        fprintf(stderr, "[-] Internal error on parsing regex. Please try again later and/or contact the developers.\n");
        return 0;
    }
    if (regexec(&regex, buf, (size_t)0, NULL, 0))
    { // If the buffer doesn't match with the pattern
        regfree(&regex);
        return 0;
    }
    regfree(&regex); // Free allocated memory
    return 1;
}

int validAddress(char *address)
{ // First pattern is for hostname and second one is for IPv4 addresses/IP addresses
    return (validRegex(address, "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9]).)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9-]*[A-Za-z0-9])$") ||
            validRegex(address, "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]).){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$"));
}

int validPort(char *port)
{ // Ports range from 0 to 65535
    return validRegex(port, "^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$");
}

int parseClientCommand(char *command)
{
    if (!strcmp(command, "reg"))
        return REGISTER;
    else if (!strcmp(command, "unregister") || !strcmp(command, "unr"))
        return UNREGISTER;
    else if (!strcmp(command, "login"))
        return LOGIN;
    else if (!strcmp(command, "logout"))
        return LOGOUT;
    else if (!strcmp(command, "showuid") || !strcmp(command, "su"))
        return SHOWUID;
    else if (!strcmp(command, "exit"))
        return EXIT;
    else if (!strcmp(command, "groups") || !strcmp(command, "gl"))
        return GROUPS;
    else if (!strcmp(command, "subscribe") || !strcmp(command, "s"))
        return SUBSCRIBE;
    else if (!strcmp(command, "unsubscribe") || !strcmp(command, "u"))
        return UNSUBSCRIBE;
    else if (!strcmp(command, "my_groups") || !strcmp(command, "mgl"))
        return MY_GROUPS;
    else if (!strcmp(command, "select") || !strcmp(command, "sag"))
        return SELECT;
    else if (!strcmp(command, "showgid") || !strcmp(command, "sg"))
        return SHOWGID;
    else if (!strcmp(command, "ulist") || !strcmp(command, "ul"))
        return ULIST;
    else if (!strcmp(command, "post"))
        return POST;
    else if (!strcmp(command, "retrieve") || !strcmp(command, "r"))
        return RETRIEVE;
    else
    { // No valid command was received
        fprintf(stderr, "[-] Invalid user command code. Please try again.\n");
        return INVALID_COMMAND;
    }
}

int validUID(char *UID)
{
    return validRegex(UID, "^[0-9]{5}$");
}

int validPW(char *PW)
{
    return validRegex(PW, "^[a-zA-Z0-9]{8}$");
}

int isNumber(char *num)
{
    size_t len = strlen(num);
    for (int i = 0; i < len; ++i)
    {
        if (num[i] < '0' || num[i] > '9')
            return 0;
    }
    return 1;
}

int validGID(char *GID)
{
    return validRegex(GID, "^([0][1-9]|[1-9][0-9])$");
}

int validGName(char *GName)
{
    return validRegex(GName, "^[a-zA-Z0-9_-]{1,24}$");
}

int isMID(char *MID)
{
    return validRegex(MID, "^[0-9]{4}$");
}

int sendTCP(int fd, char *message)
{
    int bytesSent = 0;
    ssize_t nSent;
    size_t messageLen = strlen(message);
    while (bytesSent < messageLen)
    { // Send initial message
        nSent = write(fd, message + bytesSent, messageLen - bytesSent);
        if (nSent == -1)
        {
            perror("[-] Failed to write on TCP");
            return nSent;
        }
        bytesSent += nSent;
    }
    return bytesSent;
}

int readTCP(int fd, char *message, int maxSize)
{
    int bytesRead = 0;
    ssize_t n;
    while (bytesRead < maxSize)
    {
        n = read(fd, message + bytesRead, maxSize - bytesRead);
        if (n == 0)
        {
            break; // Peer has performed an orderly shutdown -> POSSIBLE message complete
        }
        if (n == -1)
        {
            perror("[-] Failed to receive from server on TCP");
            return n;
        }
        bytesRead += n;
    }
    return bytesRead;
}