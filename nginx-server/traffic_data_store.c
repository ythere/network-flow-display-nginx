#include <stdio.h>
#include <sqlite3.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "traffic_resolv.h"

#include <assert.h>

#define TRAFFIC_STORE "/vbox/bin/minvpn_flow_count.sh"
#define TRAFFIC_STORE_SEC "/vbox/bin/secvpn_flow_count.sh"

static int my_atoi(const char* str)
{
    int result = 0;
    int sign = 0;
    assert(str != NULL);
    while (*str==' ' || *str=='\t' || *str=='\n')
        ++str;
    if (*str=='-')
    {
        sign = 1;
        ++str;
    }
    else if (*str=='+')
    {
        ++str;
    }

    while (*str>='0' && *str<='9')
    {
        result = result*10 + *str - '0';
        ++str;
    }

    // return result
    if (sign==1)
       return -result;
    else
       return result;
}

//meant to use database to store before, now this function has no use
int  insert_to_database(char* Time, char* Rxtotal, char* Txtotal)
{
	int out = 0;
	sqlite3 *db = 0;
	char *pErrMsg;
	int ret = sqlite3_open("/vbox/config/tunnel_flow.db",&db);
	if( ret != SQLITE_OK )
	{
			fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
			return 1;
	}
	printf("数据库连接成功!\n");
	
	char buf[128]="";
	snprintf(buf,128,"INSERT INTO main"\
			" VALUES('%s','%s','%s');",Time, Rxtotal, Txtotal);
	out = sqlite3_exec( db, buf, 0, 0, &pErrMsg);
	if(out == SQLITE_OK)
	{
		printf("插入数据成功\n");
		sqlite3_close(db);
		return 0;
	}
	fprintf(stderr, "SQL error:%s\n", pErrMsg);
	sqlite3_close(db);
	return -1;
}

char* speed_store(char* Time, char* Rx, char* Tx, char* Counts)
{
	int normal = my_atoi(Counts);
	int a = (normal % 60);
	if (a == 0)
	{
		/*
		int end = insert_to_database(Time, Rx, Tx);
		if( end == 0)
		{
			return "success";
		}
		else
		{
			return "fail";
		}
		*/
		char cmd[256] = "";
		sprintf(cmd,"%s %s %s", TRAFFIC_STORE, Tx, Rx);
		int result = system(cmd);
		if (result == 0)
			return "success";
		else
			return "false";
	}
	else
	{
		char cmds[256] = "";
		sprintf(cmds,"%s %s %s", TRAFFIC_STORE_SEC, Tx, Rx);
		int results = system(cmds);
		if (results == 0)
			return "success";
		else
			return "false";
		/*
		FILE *fp;
		char *path = "/vbox/config/sectraffic_vpn_data.txt";
		char buf[256] = "";
		sprintf(buf,"%s,%s,%s",Tx, Rx, Time);
		fp = fopen(path, "w");
		fputs(buf,fp);
		fclose(fp);
		return "success";
		*/
	}
}
