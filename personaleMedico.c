#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static void insert_esameSostenuto(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	// Input for the registration routine
	char prenotazione[5];
	int prenotazione_int;
	char diagnosi[3000];

	// Get the required information
	printf("\nPrenotazione ID: ");
	getInput(5, prenotazione, false);
	printf("Diangnosi: ");
	getInput(3000, diagnosi, false);

	// Apply proper type conversions
	prenotazione_int = atoi(prenotazione);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_esamesostenuto(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &prenotazione_int;
	param[0].buffer_length = sizeof(prenotazione_int);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = diagnosi;
	param[1].buffer_length = strlen(diagnosi);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while registering the exam.");
	} else {
		printf("Registration correctly completed...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void insert_presenza(MYSQL *conn, char username[50]) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	// Input for the registration routine
	char prenotazione[5];
	int prenotazione_int;
	
	// Get the required information
	printf("\nPrenotazione ID: ");
	getInput(5, prenotazione, false);

	// Apply proper type conversions
	prenotazione_int = atoi(prenotazione);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_presenza(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &prenotazione_int;
	param[1].buffer_length = sizeof(prenotazione_int);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while registering.");
	} else {
		printf("Registration correctly completed...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void aggiungi_parametro(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

	// Input for the registration routine
	char esame[5];
	int esame_int;
	char nome[256];
	char valore[10];
	float valore_float;
	
	// Get the required information
	printf("\nID prenotazione: ");
	getInput(5, esame, false);
	printf("\nNome parametro: ");
	getInput(256, nome, false);
	printf("\nValore: ");
	getInput(10, valore, false);
	

	// Apply proper type conversions
	esame_int = atoi(esame);
	valore_float = atoi(valore);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_parametro(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = nome;
	param[0].buffer_length = strlen(nome);

	param[1].buffer_type = MYSQL_TYPE_FLOAT;
	param[1].buffer = &valore_float;
	param[1].buffer_length = sizeof(valore_float);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &esame_int;
	param[2].buffer_length = sizeof(esame_int);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while registering.");
	} else {
		printf("Registration correctly completed...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void Verifica_prenotazione(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	
	char header[512];
	int status;
	char prenotazione[5];
	int prenotazione_int;

	// Get the required information
	printf("\nPrenotazione ID: ");
	getInput(5, prenotazione, false);

	// Apply proper type conversions
	prenotazione_int = atoi(prenotazione);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call verifica_prenotazione(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &prenotazione_int;
	param[0].buffer_length = sizeof(prenotazione_int);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}
	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving.");
		goto out;
	}

	dump_result_set(conn, prepared_stmt, header);

    out:
	mysql_stmt_close(prepared_stmt);
}

void run_as_personaleMedico(MYSQL *conn, char username[50])
{
	char options[5] = {'1','2','3','4','5'};
	char op;
	
	printf("Switching to personaleMedico role...\n");

	if(!parse_config("users/personaleMedico.json", &conf)) {
		fprintf(stderr, "Unable to load personaleMedico configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while(true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Inserisci un esame sostenuto\n");
		printf("2) Inserisci una presenza\n");
		printf("3) Visualizza prenotazione dell'esame\n");
		printf("4) Inserisci risultati esame\n");
		printf("5) Quit\n");

		op = multiChoice("Select an option", options, 5);

		switch(op) {
			case '1':
				insert_esameSostenuto(conn);
				break;
			
			case '2':
				insert_presenza(conn, username);
				break;
		
			case '3':
				Verifica_prenotazione(conn);
				break;

			case '4':
				aggiungi_parametro(conn);
				break;
	
			case '5':
				return;
				
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
