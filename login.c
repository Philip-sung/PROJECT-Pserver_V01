#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int exploit();
int main (int argc, char **argv) {
  char command[256];
  int commandCheck;
  int pass1 = 0;
  char buf[24];
  char pass2[8] = "A";
  char buffer[8];
  MYSQL_ROW row;
  MYSQL *conn = mysql_init(NULL);
  char *ret;
  
  //connect to database
  if(mysql_real_connect(conn, "localhost", "root", "1111", "Wiki", 0, NULL, 0) == NULL) {
    printf("Connection Failed\n");
	return 0;
  }
  // construct query
  commandCheck = snprintf(command, sizeof command, "SELECT * FROM Users WHERE BINARY id = \'%s\' AND BINARY password = \'%s\';", argv[1], argv[2]);
  
  if(mysql_real_query(conn, command, 250) != 0) { //check information
    printf("query failed\n");
    pass1 = 35;
    strcpy(pass2, "C");
    sprintf(buf, argv[1]); // format string vulnerability
    printf(buf);
  }
  else {
  MYSQL_RES *result = mysql_store_result(conn);
  if(mysql_num_rows(result) != 0) {
    row = mysql_fetch_row(result);
    if(strcmp(row[3], "1") == 0){
		pass1 = 33;
		strcpy(pass2, "A");
	}
    else{
		pass1 = 34;
		strcpy(pass2, "B");
}
  }
  else {
    pass1 = 35;
    strcpy(pass2, "C");
  }
}
  mysql_close(conn);
  strcpy(buffer, argv[1]); //buffer overflow vulnerability

  if ((pass1 == 33) || (strcmp(pass2,"A") == 0)){
	  printf("superUser");
	  return 10;
  }
  else if ((pass1 == 34) || (strcmp(pass2, "B") == 0)){
	  printf("normalUser");
	  return 11;
  }
  else {
	  printf("unauthorized");
	  return 12;
  }
}

int exploit() {
printf("[Team Ate] Dummy Function for PoC\n");
return 0;
}
