/** 
 * @Author : Lu7fer
 * @Date: 2020-06-11 22:18:37
 * @LastEditTime: 2020-06-14 23:31:29
 * @FilePath: /C-language-study-homework/test1.c
 * @Stu_ID: 2019X....X229_Lu7fer
 * @Github: https://github.com/Lu7fer/C-language-study-homework
 * @Copyright
-------------------------------------------
Copyright (C) 2020 - Lu7fer
C-language-study-homework is free software:
you can redistribute it and/or modify it under the terms of 
the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
 
You should have received a copy of the GNU General Public License 
along with C-language-study-homework. 
If not, see <http: //www.gnu.org/licenses/>.
-------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ras.h>

RASENTRYA params, *params_ptr;
int RvenRtn;
int rtn;
DWORD rasEntrySize = sizeof(params);
RASDIALPARAMSA rasDialParams;
HRASCONN dialHandle;
RASCONNSTATUSA rasConnStus;
#ifdef __cplusplus
extern "C"
{
#endif

  void Rasdialfunc(UINT arg1, RASCONNSTATE arg2, DWORD arg3)
  {
    printf("arg1 %d, Connect state: %d, arg3 %d\n", arg1, arg2, arg3);
  }

  int main(int argc, char const *argv[])
  {
    char username[129];
    char passwd[129];
    FILE *fprop = fopen(".\\dial.conf", "r");
    fscanf(fprop, "username=%s password=%s", username, passwd);
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
    /*
如果函数成功，则返回值为ERROR_SUCCESS。
如果函数失败，则返回值为以下错误代码之一，或者为“路由和远程访问错误代码”或Winerror.h中的值。
返回值
价值	意义
ERROR_INVALID_PARAMETER
使用无效参数调用了该函数。
ERROR_INVALID_SIZE
lpRasEntry 的dwSize成员的值太小。
ERROR_BUFFER_INVALID
lpRasEntry 指定的地址或缓冲区无效。
ERROR_BUFFER_TOO_SMALL
lpdwEntryInfoSize中 指示的缓冲区大小太小。
ERROR_CANNOT_FIND_PHONEBOOK_ENTRY
电话簿条目不存在，或者电话簿文件已损坏和/或缺少组件。*/

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
    int rtn = RasSetEntryPropertiesA(NULL, "Dr.COM", params_ptr, sizeof(params), NULL, 0);
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
      /*
 如果函数失败，则返回值为以下错误代码之一，或者为“路由和远程访问错误代码”或Winerror.h中的值。
返回值
价值	意义
ERROR_BUFFER_TOO_SMALL
该lprasentryname缓冲区不够大。所述LPCB参数小于所述的dwSize构件在lprasentryname应之前调用该函数设定参数。该函数在lpcb指向的变量中返回所需的缓冲区大小。
Windows Vista或更高版本：  该lprasentryname缓冲区可以设置为NULL和变量指向LPCB可设置为零。该函数将在lpcb指向的变量中返回所需的缓冲区大小。

ERROR_INVALID_SIZE
lprasentryname指向的 RASENTRYNAME结构中 的dwSize值指定了当前平台不支持的结构版本。例如，在Windows 95上， 如果dwSize指示 RASENTRYNAME包括dwFlags和szPhonebookPath成员，则RasEnumEntries返回此错误，因为Windows 95不支持这些成员（只有Windows 2000和更高版本才支持这些成员）。
ERROR_NOT_ENOUGH_MEMORY
该函数无法分配足够的内存来完成操作。
    */
      printf("RasEnumEntries失败\n");
      goto end;
    }

    // strcpy(&params.szEntryName, "Dr.COM");
    // params.szUserName[0] = 0x0D;
    // params.szUserName[1] = 0x0A;
    // strcy((&params.szUserName) + 2, "t2019407229");
    // strcpy(&params.szPassword, "045012");
    // RasDialW(NULL, NULL, &params, );
    // RasSetEntryProperties();
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
    printf("Normal\n");
    system("pause");
    return 0;
  end:
    printf("error\n");
    // Sleep(5000);
    return -1;
  }
#ifdef __cplusplus
}
#endif

// API, RasValidateEntryName, RasGetEntryProperties,
// RasSetEntryProperties, RasEnumEntries，RasSetEntryDialParams,
// RasDial, RasHangUp，RasDialFunc
