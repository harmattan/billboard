
/**
 * Billboard - Low Power Mode Standby Screen for the N9
 * Webpage: http://thp.io/2012/billboard/
 * Copyright (C) 2012, 2013, 2014 Thomas Perl <thp.io/about>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "network.h"

struct network_interface *
get_interfaces()
{
    struct network_interface *result = NULL;
    struct network_interface *tmp_intf;
    struct dirent *ent;
    char tmp[1024];
    DIR *d;
    FILE *fp;

    d = opendir("/sys/class/net");
    if (d == NULL) {
        return NULL;
    }

    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;
        strcpy(tmp, "/sys/class/net/");
        strcat(tmp, ent->d_name);
        strcat(tmp, "/operstate");
        fp = fopen(tmp, "r");
        if (fp == NULL) continue;
        if (fgets(tmp, sizeof(tmp), fp) == NULL) {
            fclose(fp);
            continue;
        }
        if (tmp[strlen(tmp)-1] == '\n') {
            tmp[strlen(tmp)-1] = '\0';
        }
        if (strcmp(tmp, "up") == 0 ||
                strcmp(ent->d_name, "gprs0") == 0) {
            tmp_intf = (struct network_interface*)malloc(
                    sizeof(struct network_interface));
            tmp_intf->name = strdup(ent->d_name);
            tmp_intf->next = result;
            result = tmp_intf;
        }
        fclose(fp);
    }

    closedir(d);

    return result;
}

char *
get_ip(const char *intf)
{
    int sfd;
    struct ifreq ifr;
    struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
    char *result;

    memset(&ifr, 0, sizeof(ifr));
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        return NULL;
    }

    strcpy(ifr.ifr_name, intf);
    sin->sin_family = AF_INET;

    if (ioctl(sfd, SIOCGIFADDR, &ifr) != 0) {
        close(sfd);
        return NULL;
    }

    result = strdup(inet_ntoa(sin->sin_addr));
    close(sfd);

    return result;
}

const char *
get_ip_addresses()
{
    struct network_interface *interfaces;
    struct network_interface *tmp;

    char *ip;
    static char result[2048];
    memset(result, 0, sizeof(result));

    interfaces = get_interfaces();
    while (interfaces) {
        ip = get_ip(interfaces->name);
        if (ip == NULL) goto next;
        if (strlen(result) != 0) {
            strcat(result, ", ");
        }
        strcat(result, ip);
        free(ip);

next:
        free(interfaces->name);
        tmp = interfaces;
        interfaces = interfaces->next;
        free(tmp);
    }

    return result;
}

