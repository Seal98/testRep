
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "User.h"
#include "Client.h"
sqlite3 *db;
sqlite3_stmt *res;
char *err_msg = 0;
struct User currentUser;

int autorizationCallback(void *data, int argc, char **argv, char **azColName) {
	if (strcmp(argv[0], "")) {
		strcpy(currentUser.firstname, argv[0]);
		strcpy(currentUser.lastname, argv[1]);
		strcpy(currentUser.login, argv[2]);
		strcpy(currentUser.type, argv[3]);
		currentUser.ID = atoi(argv[4]);
		return 0;
	}
	return -1;
}

int authorization(sqlite3* db, char* login, char* password) {
	//MAX_QUERY_LENGTH
	char sql[150];
	char *zErrMsg = 0;
	sprintf(sql, "SELECT first_name, last_name, login, user_type, id from BANK_USERS where login=\"%s\" AND password=\"%s\"", login, password);
	printf(sql);
	return sqlite3_exec(db, sql, autorizationCallback, 0, &zErrMsg);
}

void clientMenu(sqlite3* db) {
	int option;
	printf("clientMenu");
	do {
		printf("1 - show balance\n");
		printf("2 - top up your balance\n");
		printf("3 - withdraw money\n");
		printf("4 - exit\n");
		printf("===========================\n");
		scanf("%d",&option);
		double c;
		double sum;
		int acc;
		switch (option)
		{
		case 1:
			showClientAccounts(db, currentUser.login);
			break;
		/*case 2:
			acc = chooseAccount(db, currentUser.login);
			printf("Enter sum\n");
			scanf("%lf", &sum);
			printf("%lf", sum);
			if ((c = comm(sum)) != -1) {
				credit(db, acc, sum - c);
			}
			break;
		case '3':

			acc = chooseAccount(db, currentUser.login);
			printf("Enter sum\n");
			scanf("%lf", &sum);
			printf("%lf %d ", sum, acc);
			if ((c = comm(sum)) != -1) {
				if (debit(db, acc, sum + c, OVER) == -1)
					printf("Failed\n");
			}
			break;
			*/
		}
		printf("===========================\n");
	} while (option != 4);
}

void mainMenu(sqlite3* db) {
	// FIELD_LENGTH надо где-то определить
	char login[15];
	char password[15];
	printf("Enter login\n");
	scanf("%s", login);
	printf("Enter password\n");
	scanf("%s", password);
	authorization(db, login, password);
	//printf("============%s===========\n", currentUser.type);
	printf("===========================\n");
	printf(currentUser.type);
	if (!strcmp(currentUser.type, "CLIENT"))
	{
		printf("hi");
		clientMenu(db);
	}
	/*if (!strcmp(currentUser.type, "OPERATOR"))
		operatorMenu(db);
	if (!strcmp(currentUser.type, "ADMINISTRATOR"))
		administratorMenu(db);*/
}

int main()
{
	int choice = 0;
	int rc = sqlite3_open("BANK.db", &db);
	if (rc != SQLITE_OK) {

		fprintf(stderr, "Cannot open database: %s\n",
			sqlite3_errmsg(db));
		sqlite3_close(db);

		return 1;
	}
	mainMenu(db);
	return 0;
}