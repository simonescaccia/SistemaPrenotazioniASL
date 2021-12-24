#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static void Report_esami_svolti_anno(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

	char header[512];
	int status;
	
	// Input for the registration routine
	char anno[5];
	int anno_int;
	
	// Get the required information
	printf("Anno: ");
	getInput(5, anno, false);
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call report_esami_anno(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}

	// Apply proper type conversions
	anno_int = atoi(anno);

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &anno_int;
	param[0].buffer_length = sizeof(anno_int);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving.");
		goto out;
	}
	
	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}


		dump_result_set(conn, prepared_stmt, header);


		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while(status == 0);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void Report_esami_svolti_mese(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

	char header[512];
	int status;
	
	// Input for the registration routine
	char mese[5];
	int mese_int;
	
	// Get the required information
	printf("Mese: ");
	getInput(5, mese, false);
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call report_esami_mese(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}

	// Apply proper type conversions
	mese_int = atoi(mese);

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &mese_int;
	param[0].buffer_length = sizeof(mese_int);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving.");
		goto out;
	}
	
	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}


		dump_result_set(conn, prepared_stmt, header);


		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while(status == 0);


    out:
	mysql_stmt_close(prepared_stmt);
}

static void Aggiorna_responsabile_ospedale(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	// Input for the registration routine
	char CFresponsabile[17];
	char codiceOspedale[5];
	int codiceOspedale_int;

	// Get the required information
	printf("\nCF responsabile: ");
	getInput(17, CFresponsabile, false);
	printf("\nCodice ospedale: ");
	getInput(5, codiceOspedale, false);

	// Apply proper type conversions
	codiceOspedale_int = atoi(codiceOspedale);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiorna_resp_ospedale(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = CFresponsabile;
	param[0].buffer_length = strlen(CFresponsabile);
	
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &codiceOspedale_int;
	param[1].buffer_length = sizeof(codiceOspedale_int);

if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while updating.");
	} else {
		printf("Update correctly completed...\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void Aggiorna_responsabile_laboratorio(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

	// Input for the registration routine
	char CFresponsabile[17];
	char codiceLaboratorio[5];
	int codiceLaboratorio_int;
	char codiceOspedale[5];
	int codiceOspedale_int;

	// Get the required information
	printf("\nCF responsabile: ");
	getInput(17, CFresponsabile, false);
	printf("\nCodice Laboratorio: ");
	getInput(5, codiceLaboratorio, false);
	printf("\nCodice Ospedale: ");
	getInput(5, codiceOspedale, false);

	// Apply proper type conversions
	codiceLaboratorio_int = atoi(codiceLaboratorio);
	codiceOspedale_int = atoi(codiceOspedale);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiorna_resp_lab(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = CFresponsabile;
	param[0].buffer_length = strlen(CFresponsabile);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &codiceOspedale_int;
	param[1].buffer_length = sizeof(codiceOspedale_int);
	
	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &codiceLaboratorio_int;
	param[2].buffer_length = sizeof(codiceLaboratorio_int);

if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "An error occurred while updating.");
	} else {
		printf("Update correctly completed...\n");
	}

	mysql_stmt_close(prepared_stmt);
}


static void Aggiungi_esame(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	// Input for the registration routine
	char descrizione[256];
	int codice = 0;

	// Get the required information
	printf("\nDescrizione: ");
	getInput(256, descrizione, false);


	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_esame(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize exam insertion statement\n", false);
	}


	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = descrizione;
	param[0].buffer_length = strlen(descrizione);
	
	param[1].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[1].buffer = &codice;
	param[1].buffer_length = sizeof(codice);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for exam insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding the exam.");
		goto out;
	}

	// Get back the ID of the newly-added esame
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[0].buffer = &codice;
	param[0].buffer_length = sizeof(codice);
	
	if(mysql_stmt_bind_result(prepared_stmt, param)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not retrieve output parameter", true);
	}
	
	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not buffer results", true);
	}

	printf("Exam correctly added with ID %d...\n", codice);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void Visualizza_reparti(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[0];
	
	char header[512];
	int status;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call visualizza_reparti()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}
if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}


		dump_result_set(conn, prepared_stmt, header);


		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while(status == 0);


    out:
	mysql_stmt_close(prepared_stmt);
}

static void Visualizza_personale(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[0];
	
	char header[512];
	int status;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call visualizza_personale()", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}
if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	
	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while retrieving.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if(conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}


		dump_result_set(conn, prepared_stmt, header);


		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	    next:
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
		
	} while(status == 0);


    out:
	mysql_stmt_close(prepared_stmt);
}

int strcompare(char s1[46], char s2[46]){
	while(*s1!='\0'){
		if(*s1 != *s2){
			return 0;		
		}
		s1++;
		s2++;
	}
	if(*s2 == '\0'){
		return 1;	
	} else {
		return 0;
	}
}

static void Crea_utente(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];
	char options[5] = {'1','2', '3'};
	char r;

	// Input for the registration routine
	char username[46];
	char password[46];
	char ruolo[46];

	// Get the required information
	printf("\nUsername: ");
	getInput(46, username, false);
	printf("Password: ");
	getInput(46, password, true);
	printf("Assign a possible role:\n");
	printf("\t1) personaleCUP\n");
	printf("\t2) personaleMedico\n");
	printf("\t3) amministratore\n");
	r = multiChoice("Select role", options, 3);

	// Convert role into enum value
	switch(r) {
		case '1':
			strcpy(ruolo, "personaleCUP");
			break;
		case '2':
			strcpy(ruolo, "personaleMedico");
			break;
		case '3':
			strcpy(ruolo, "amministratore");
			break;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
	}
	
	char str[46];
	strcpy(str,"personaleMedico");

	if(strcompare(str,ruolo)){

		//insert utente e personale
		MYSQL_STMT *prepared_stmt;
		MYSQL_BIND param[11];

		// Input for the registration routine
		char CF[17];
		char nome[51];
		char cognome[51];
		char domicilio[256];
		char tipo[256];
		char associazione[256];
		char codiceRep[5];
		int codiceRep_int;
		char codiceOsp[5];
		int codiceOsp_int;

		// Get the required information
		printf("\nCF personale: ");
		getInput(17, CF, false);
		printf("\nNome: ");
		getInput(50, nome, false);
		printf("\nCognome: ");
		getInput(50, cognome, false);
		printf("\nDomicilio: ");
		getInput(255, domicilio, false);
		printf("\nTipo di personale: ");
		getInput(255, tipo, false);
		printf("\nNome associazione: ");
		getInput(255, associazione, false);
		printf("\nCodice del reparto in cui lavora: ");
		getInput(5, codiceRep, false);
		printf("\nCodice dell'ospedale in cui lavora: ");
		getInput(5, codiceOsp, false);

		// Apply proper type conversions
		codiceRep_int = atoi(codiceRep);
		codiceOsp_int = atoi(codiceOsp);

		// Prepare stored procedure call
		if(!setup_prepared_stmt(&prepared_stmt, "call crea_utente_personale(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
		}

		// Prepare parameters
		memset(param, 0, sizeof(param));

		param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[0].buffer = username;
		param[0].buffer_length = strlen(username);

		param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[1].buffer = password;
		param[1].buffer_length = strlen(password);

		param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[2].buffer = ruolo;
		param[2].buffer_length = strlen(ruolo);

		param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[3].buffer = CF;
		param[3].buffer_length = strlen(CF);

		param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[4].buffer = nome;
		param[4].buffer_length = strlen(nome);

		param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[5].buffer = cognome;
		param[5].buffer_length = strlen(cognome);

		param[6].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[6].buffer = domicilio;
		param[6].buffer_length = strlen(domicilio);

		param[7].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[7].buffer = tipo;
		param[7].buffer_length = strlen(tipo);

		param[8].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[8].buffer = associazione;
		param[8].buffer_length = strlen(associazione);

		param[9].buffer_type = MYSQL_TYPE_LONG;
		param[9].buffer = &codiceRep_int;
		param[9].buffer_length = sizeof(codiceRep_int);

		param[10].buffer_type = MYSQL_TYPE_LONG;
		param[10].buffer = &codiceOsp_int;
		param[10].buffer_length = sizeof(codiceOsp_int);

		if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
			finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
		}

		// Run procedure
		if (mysql_stmt_execute(prepared_stmt) != 0) {
			print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
		} else {
			printf("User e personale medico correctly added...\n");
		}

		mysql_stmt_close(prepared_stmt);
	
	} else {

		// Prepare stored procedure call
		if(!setup_prepared_stmt(&prepared_stmt, "call crea_utente(?, ?, ?)", conn)) {
			finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize user insertion statement\n", false);
		}

		// Prepare parameters
		memset(param, 0, sizeof(param));

		param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[0].buffer = username;
		param[0].buffer_length = strlen(username);

		param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[1].buffer = password;
		param[1].buffer_length = strlen(password);

		param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
		param[2].buffer = ruolo;
		param[2].buffer_length = strlen(ruolo);

		if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
			finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for user insertion\n", true);
		}

		// Run procedure
		if (mysql_stmt_execute(prepared_stmt) != 0) {
			print_stmt_error (prepared_stmt, "An error occurred while adding the user.");
		} else {
			printf("User correctly added...\n");
		}

		mysql_stmt_close(prepared_stmt);
	}
}

void run_as_administrator(MYSQL *conn)
{
	char options[9] = {'1','2', '3', '4', '5', '6','7','8','9'};
	char op;
	
	printf("Switching to administrative role...\n");

	if(!parse_config("users/amministratore.json", &conf)) {
		fprintf(stderr, "Unable to load administrator configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\nError: %s [%d]\n",mysql_error(conn), mysql_errno(conn));
		exit(EXIT_FAILURE);
	}

	while(true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Aggiungi esame\n");
		printf("2) Aggiorna responsabile ospedale\n");
		printf("3) Aggiorna responsabile laboratorio\n");
		printf("4) Report esami svolti in un mese da un membro del personale\n");
		printf("5) Report esami svolti in un anno da un membro del personale\n");
		printf("6) Crea utente\n");
		printf("7) Visualizza il personale\n");
		printf("8) Visualizza i reparti\n");
		printf("9) Quit\n");

		op = multiChoice("Select an option", options, 9);

		switch(op) {
			case '1':
				Aggiungi_esame(conn);
				break;
			case '2':
				Aggiorna_responsabile_ospedale(conn);
				break;
			case '3':
				Aggiorna_responsabile_laboratorio(conn);
				break;
			case '4':
				Report_esami_svolti_mese(conn);
				break;
			case '5':
				Report_esami_svolti_anno(conn);
				break;
			case '6':
				Crea_utente(conn);
				break;
			case '7':
				Visualizza_personale(conn);
				break;
			case '8':
				Visualizza_reparti(conn);
				break;
			case '9':
				return;
				
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
