// #define WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "configparse.h"
#include "auth.h"
#include <ras.h>
// #include "drdial.c"
#ifdef linux
#include <limits.h>
#include "daemon.h"
#include "eapol.h"
#include "libs/common.h"
#endif

#define VERSION "1.6.2"

int argcs = 5;
char *args[] = {NULL, "-m", "pppoe", "-c", ".\\dogcom.conf"};
void print_help(int exval);
int try_smart_eaplogin(void);
int state;
static const char default_bind_ip[20] = "0.0.0.0";

int dogcom_main(int argc, char const *argv[])
{

    if (argc == 1)
    {
        print_help(1);
    }

    char *file_path;

    while (1)
    {
        static const struct option long_options[] = {
            {"mode", required_argument, 0, 'm'},
            {"conf", required_argument, 0, 'c'},
            {"bindip", required_argument, 0, 'b'},
            {"log", required_argument, 0, 'l'},
#ifdef linux
            {"daemon", no_argument, 0, 'd'},
            {"802.1x", no_argument, 0, 'x'},
#endif
            {"eternal", no_argument, 0, 'e'},
            {"verbose", no_argument, 0, 'v'},
            {"help", no_argument, 0, 'h'},
            {0, 0, 0, 0}};

        int c;
        int option_index = 0;
#ifdef linux
        c = getopt_long(argc, argv, "m:c:b:l:dxevh", long_options, &option_index);
#else
        c = getopt_long(argc, argv, "m:c:b:l:evh", long_options, &option_index);
#endif

        if (c == -1)
        {
            break;
        }
        switch (c)
        {
        case 'm':
            if (strcmp(optarg, "dhcp") == 0)
            {
                strcpy(mode, optarg);
            }
            else if (strcmp(optarg, "pppoe") == 0)
            {
                strcpy(mode, optarg);
            }
            else
            {
                printf("unknown mode\n");
                exit(1);
            }
            break;
        case 'c':
#ifndef __APPLE__
            if (mode != NULL)
            {
#endif
#ifdef linux
                char path_c[PATH_MAX];
                realpath(optarg, path_c);
                file_path = strdup(path_c);
#else
            file_path = optarg;
#endif
#ifndef __APPLE__
            }
#endif
            break;
        case 'b':
            strcpy(bind_ip, optarg);
            break;
        case 'l':
#ifndef __APPLE__
            if (mode != NULL)
            {
#endif
#ifdef linux
                char path_l[PATH_MAX];
                realpath(optarg, path_l);
                log_path = strdup(path_l);
#else
            log_path = optarg;
#endif
                logging_flag = 1;
#ifndef __APPLE__
            }
#endif
            break;
#ifdef linux
        case 'd':
            daemon_flag = 1;
            break;
        case 'x':
            eapol_flag = 1;
            break;
#endif
        case 'e':
            eternal_flag = 1;
            break;
        case 'v':
            verbose_flag = 1;
            break;
        case 'h':
            print_help(0);
            break;
        case '?':
            print_help(1);
            break;
        default:
            break;
        }
    }

#ifndef __APPLE__
    if (mode != NULL && file_path != NULL)
    {
#endif
#ifdef linux
        if (daemon_flag)
        {
            daemonise();
        }
#endif

#ifdef WIN32 // dirty fix with win32
        char tmp[10] = {0};
        strcpy(tmp, mode);
#endif
        if (!config_parse(file_path))
        {
#ifdef WIN32 // dirty fix with win32
            strcpy(mode, tmp);
#endif

#ifdef linux
            if (eapol_flag)
            { // eable 802.1x authorization
                if (0 != try_smart_eaplogin())
                {
                    printf("Can't finish 802.1x authorization!\n");
                    return 1;
                }
            }
#endif
            if (strlen(bind_ip) == 0)
            {
                memcpy(bind_ip, default_bind_ip, sizeof(default_bind_ip));
            }
            dogcom(4);
        }
        else
        {
            return 1;
        }
#ifndef __APPLE__
    }
    else
    {
        printf("Need more options!\n\n");
        return 1;
    }
#endif
    return 0;
}

void Rasdialfunc(UINT arg1, RASCONNSTATE arg2, DWORD arg3)
{
    printf("arg1 %d, Connect state: %d, arg3 %d\n", arg1, arg2, arg3);
    state = arg2;
}

void print_help(int exval)
{
    printf("\nDrcom-generic implementation in C.\n");
    printf("Version: %s\n\n", VERSION);

    printf("Usage:\n");
    printf("\tdogcom -m <dhcp/pppoe> -c <FILEPATH> [options <argument>]...\n\n");

    printf("Options:\n");
    printf("\t--mode <dhcp/pppoe>, -m <dhcp/pppoe>  set your dogcom mode \n");
    printf("\t--conf <FILEPATH>, -c <FILEPATH>      import configuration file\n");
    printf("\t--bindip <IPADDR>, -b <IPADDR>        bind your ip address(default is 0.0.0.0)\n");
    printf("\t--log <LOGPATH>, -l <LOGPATH>         specify log file\n");
#ifdef linux
    printf("\t--daemon, -d                          set daemon flag\n");
    printf("\t--802.1x, -x                          enable 802.1x\n");
#endif
    printf("\t--eternal, -e                         set eternal flag\n");
    printf("\t--verbose, -v                         set verbose flag\n");
    printf("\t--help, -h                            display this help\n\n");
    exit(exval);
}

RASENTRYA params, *params_ptr;
DWORD rasEntrySize = sizeof(params);
RASDIALPARAMSA rasDialParams;
HRASCONN dialHandle;
RASCONNSTATUSA rasConnStus;
int main(int argc, char const *argv[])
{
    args[0] = argv[0];
    int RvenRtn;
    int rtn, counter = 0;

    puts("If you wanna use this program, you need login dr.com once first.");
    char username[129];
    char passwd[129];
    FILE *fprop = fopen(".\\dial.conf", "r");
    fscanf(fprop, "username=%s password=%s", username, passwd);
    fclose(fprop);
    if (fprop == NULL)
    {
        puts("请输入账号:");
        scanf("%s", username);
        puts("请输入密码:");
        scanf("%s", passwd);
    }
    // 如果函数成功，则返回值为ERROR_SUCCESS。
    //     如果函数失败，则返回值为以下错误代码之一，或者为“路由和远程访问错误代码”或Winerror.h中的值。
    //     返回值
    //         价值 意义
    //             ERROR_ALREADY_EXISTS 条目名称已存在于指定的电话簿中。
    //  ERROR_CANNOT_FIND_PHONEBOOK 指定的电话簿不存在。
    //   ERROR_INVALID_NAME  指定的条目名称的格式无效。
    RvenRtn = RasValidateEntryNameA(NULL, "Dr.COM");
    if (RvenRtn == ERROR_SUCCESS)
    {
        printf("Valid Name\n");
        goto setprop;
    }
    else if (RvenRtn == ERROR_ALREADY_EXISTS)
    {
        printf("ALREADY_EXISTS\n");
        goto setprop;
    }
    else if (RvenRtn == ERROR_CANNOT_FIND_WND_CLASS)
    {
        printf("指定的电话簿不存在\n");
        goto setprop;
    }
    else if (RvenRtn == ERROR_INVALID_NAME)
    {
        printf("INVALID_NAME\n");
        goto end;
    }
setprop:
    params.dwSize = sizeof(params);
    params_ptr = &params;
    params_ptr->dwfOptions = 0x3d2d0218;

    if ((rtn = RasGetEntryPropertiesA(NULL, "Dr.COM", params_ptr, &rasEntrySize, NULL, NULL)) == ERROR_SUCCESS)
    {
        puts("GetEntryProperties SUCCESS");
    }
    else if (rtn == ERROR_INVALID_PARAMETER)
    {
        puts("ERROR_INVALID_PARAMETER 使用无效参数调用了该函数。");
        goto end;
    }
    else if (rtn == ERROR_INVALID_FORM_SIZE)
    {
        puts("ERROR_INVALID_SIZE");
        goto end;
    }
    else
    {
        printf("error + %d", rtn);
        goto end;
    }

    // 如果未为这些成员提供值，则 RasSetEntryProperties失败，并显示 ERROR_INVALID_PARAMETER。
    strcpy(params_ptr->szLocalPhoneNumber, "");
    rtn = RasSetEntryPropertiesA(NULL, "Dr.COM", params_ptr, sizeof(params), NULL, 0);
    if (rtn == ERROR_INVALID_PARAMETER)
    {
        puts("RasSetEntryProperties失败");
        goto end;
    }
    RASENTRYNAMEA entrys[12];
    entrys[0].dwSize = sizeof(RASENTRYNAMEA);
    DWORD size = sizeof(RASENTRYNAMEA) * 12;
    DWORD nums;
    if (rtn = RasEnumEntriesA(NULL, NULL, entrys, &size, &nums) != ERROR_SUCCESS)
    {
        printf("RasEnumEntries失败\n");
        goto end;
    }
    rasDialParams.dwSize = sizeof(RASDIALPARAMSA);
    strcpy(rasDialParams.szEntryName, entrys[0].szEntryName);
    strcpy(rasDialParams.szPhoneNumber, params.szLocalPhoneNumber);
    rasDialParams.szUserName[0] = '\r';
    rasDialParams.szUserName[1] = '\n';
    strcpy(rasDialParams.szUserName + 2, username);
    strcpy(rasDialParams.szPassword, passwd);
    rtn = RasDialA(NULL, NULL, &rasDialParams, 0, Rasdialfunc, &dialHandle);
    printf("rasDial %d\n", rtn);
    rasConnStus.dwSize = sizeof(RASCONNSTATUSA);
    RasGetConnectStatusA(dialHandle, &rasConnStus);

    while (state != 8192 && counter < 20)
    {
        counter++;
        Sleep(500);
    }
    // printf("Normal\n");
    dogcom_main(argcs, args);
end:
    printf("error\n");
    return 0;
}

#ifdef linux
int try_smart_eaplogin(void)
{
#define IFS_MAX (64)
    int ifcnt = IFS_MAX;
    iflist_t ifs[IFS_MAX];
    if (0 > getall_ifs(ifs, &ifcnt))
        return -1;

    for (int i = 0; i < ifcnt; ++i)
    {
        setifname(ifs[i].name);
        if (0 == eaplogin(drcom_config.username, drcom_config.password))
            return 0;
    }
    return -1;
}

#endif