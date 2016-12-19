#ifndef TRAFFIC_RESOLV_H
#define TRAFFIC_RESOLV_H
extern  char* json_analysis(char* json, char** json_result);
extern int insert_to_database(char* Time, char* Rxtotal, char* Txtotal);
extern char* speed_store(char* Time, char* Rx, char* Tx, char* Counts);
#endif // TRAFFIC_RESOLV_H            
