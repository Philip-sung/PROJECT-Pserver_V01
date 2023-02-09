#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

#define MAX_LEN 100

int exploit(void);
char* concat(const char* s1, const char* s2);

int main(int argc, char** argv) {
	FILE* fp;
	char command[256];
	int commandCheck;
	char* wikiKeyword;
	char* wikiDescription;
	MYSQL_ROW row;
	MYSQL* conn = mysql_init(NULL);
	int isNewKeyword = 0;

	if (argc != 3){
		printf("Arguments number must be two.\n");
		return 0;
	}
	else if(argc == 3){
		wikiKeyword= argv[1];
		wikiDescription = argv[2];
		printf("\nTO FIND wiki Keyword : %s\n", wikiKeyword);
		printf("TO ADD wiki Description : %s\n", wikiDescription);
	}

	//connect to database
	if (mysql_real_connect(conn, "localhost", "root", "1111", "Wiki", 0, NULL, 0) == NULL) {
		printf("Connection Failed");
		return 0;
	}

	//create tempfile with keyword and concat descriptions in tempfile	
	commandCheck = snprintf(command, sizeof command, "SELECT * FROM Information WHERE word=\'%s\';",wikiKeyword);
	if (mysql_real_query(conn, command, 250) != 0) {
		printf("query failed\n");
	}
	else {
		MYSQL_RES* result = mysql_store_result(conn);
		int resnum = mysql_num_rows(result);
		fp = fopen(wikiKeyword, "w");
		if(resnum == 1){
			printf("**Existing Keyword**\n");
			while(row = mysql_fetch_row(result)){
				fprintf(fp, "%s\n", row[1]);
			}
			isNewKeyword = 0;
		}
		else if(resnum == 0) {
			printf("**New Keyword**\n");
			isNewKeyword = 1;
		}
		fprintf(fp, "%s", argv[2]);
		fclose(fp);
		fprintf(stdout, "**File Wrote**\n");
	}

	//insert record into database with tempfile
	fp = fopen(wikiKeyword, "r");
	char descriptionStr[MAX_LEN];
	char* content = "";
	while(fgets(descriptionStr,MAX_LEN,fp)){
		content = concat(content, descriptionStr);
	}

	printf(content);
	if(isNewKeyword == 1){
		commandCheck = snprintf(command, sizeof command, "INSERT INTO Information VALUES (\'%s\', \'%s\');", wikiKeyword, content);
		mysql_real_query(conn, command, 250);
	}
	else if(isNewKeyword == 0){
		commandCheck = snprintf(command, sizeof command, "UPDATE Information SET description=\'%s\' WHERE word=\'%s\';", content, wikiKeyword);
		mysql_real_query(conn, command, 250);
	}
	fprintf(stdout, "\n**Database Wrote**\n");
	fclose(fp);

	//delete tempfile

	return 0;
}

int exploit() {
	printf("[Team Ate] Dummy Function for PoC\n");
	return 0;
}

char* concat(const char* s1,const char* s2){
	char* res = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(res, s1);
	strcat(res, s2);
	return res;
}