#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "traffic_resolv.h"          

char* json_analysis(char* json, char** json_result)
{
	cJSON *jsonobj, *json_value;
	char *result = NULL;
	char *Time = NULL, *Rxtotal = NULL, *Txtotal = NULL, *Counts = NULL;

	jsonobj = cJSON_Parse(json);
	if(!jsonobj)
	{
		return "alloc json file error";
	}
	for(json_value = jsonobj->child; json_value; json_value = json_value->next)
    {
        if(json_value->type != cJSON_String)
            continue;
	if(!strcmp(json_value->string, "Time" ))
			Time = json_value->valuestring;
        else if(!strcmp(json_value->string, "Rxtotal"))
            Rxtotal = json_value->valuestring;
        else if(!strcmp(json_value->string,"Txtotal"))
            Txtotal = json_value->valuestring;
	else if(!strcmp(json_value->string,"Counts"))
            Counts = json_value->valuestring;
    }
	
	result = speed_store(Time, Rxtotal, Txtotal, Counts);	
	*json_result = result;
	return *json_result;	
}
