#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static void Report_esami_paziente(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	
	int status;
	char header[512];

	// Input for the registration routine
	char CFpaziente[17];
	
	// Get the required information
	printf("\nCF paziente: ");
	getInput(17, CFpaziente, false);
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call report_esami_paziente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = CFpaziente;
	param[0].buffer_length = strlen(CFpaziente);

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

static void Aggiungi_paziente(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[6];

	// Input for the registration routine
	char CFpaziente[17];
	char Nome[51];
	char Cognome[51];
	char Residenza[256];
	char LuogoNascita[256];
	char DataNascita[11];

	// Get the required information
	printf("\nCF paziente: ");
	getInput(17, CFpaziente, false);
	printf("\nNome: ");
	getInput(51, Nome, false);
	printf("\nCognome: ");
	getInput(51, Cognome, false);
	printf("\nResidenza: ");
	getInput(256, Residenza, false);
	printf("\nLuogo di Nascita: ");
	getInput(256, LuogoNascita, false);
	printf("\nData di  nascita: ");
	getInput(11, DataNascita, false);


	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_paziente(?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize exam insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = CFpaziente;
	param[0].buffer_length = strlen(CFpaziente);
	
	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = Nome;
	param[1].buffer_length = strlen(Nome);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = Cognome;
	param[2].buffer_length = strlen(Cognome);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = Residenza;
	param[3].buffer_length = strlen(Residenza);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = LuogoNascita;
	param[4].buffer_length = strlen(LuogoNascita);

	param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = DataNascita;
	param[5].buffer_length = strlen(DataNascita);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for exam insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding.");
		goto out;
	}else {
		printf("Insert correctly completed...\n");
	}


    out:
	mysql_stmt_close(prepared_stmt);
}

static void Aggiungi_prenotazione(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[10];

	// Input for the registration routine
	char CFpaziente[17];
	char Esame[5];
	int Esame_int;
	char CodiceLab[5];
	int CodiceLab_int;
	char CodiceOsp[5];
	int CodiceOsp_int;
	char Data[11];
	char Ora[9];
	char Urgenza[3];
	char CostoT[10];
	float CostoT_float;
	char CodPrenotazione[5];
	int CodPrenotazione_int;
	
	int codice=0; //OUT 

	// Get the required information
	printf("\nCF paziente: ");
	getInput(17, CFpaziente, false);
	printf("\nID Esame: ");
	getInput(5, Esame, false);
	printf("\nCodice laboratorio: ");
	getInput(5, CodiceLab, false);
	printf("\nCodice ospedale: ");
	getInput(5, CodiceOsp, false);
	printf("\nData esame YYYY-MM-DD: ");
	getInput(11, Data, false);
	printf("\nOra esame HH:MM:SS: ");
	getInput(9, Ora, false);
	printf("\nUrgenza: ");
	getInput(3, Urgenza, false);
	printf("\nCosto ticket: ");
	getInput(10, CostoT, false);
	printf("\nCodice Prenotazione: ");
	getInput(5, CodPrenotazione, false);


	// Apply proper type conversions
	Esame_int = atoi(Esame);
	CodiceLab_int = atoi(CodiceLab);
	CodiceOsp_int = atoi(CodiceOsp);
	CodPrenotazione_int = atoi(CodPrenotazione);
	CostoT_float = atof(CostoT);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_prenotazione(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize exam insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = CFpaziente;
	param[0].buffer_length = strlen(CFpaziente);
	
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &Esame_int;
	param[1].buffer_length = sizeof(Esame_int);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = Data;
	param[2].buffer_length = strlen(Data);

	param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = Ora;
	param[3].buffer_length = strlen(Ora);

	param[4].buffer_type = MYSQL_TYPE_STRING;
	param[4].buffer = Urgenza;
	param[4].buffer_length = strlen(Urgenza);

	param[5].buffer_type = MYSQL_TYPE_FLOAT;
	param[5].buffer = &CostoT_float;
	param[5].buffer_length = sizeof(CostoT_float);

	param[6].buffer_type = MYSQL_TYPE_LONG;
	param[6].buffer = &CodPrenotazione_int;
	param[6].buffer_length = sizeof(CodPrenotazione_int);

	param[7].buffer_type = MYSQL_TYPE_LONG;
	param[7].buffer = &CodiceLab_int;
	param[7].buffer_length = sizeof(CodiceLab_int);

	param[8].buffer_type = MYSQL_TYPE_LONG;
	param[8].buffer = &CodiceOsp_int;
	param[8].buffer_length = sizeof(CodiceOsp_int);

	param[9].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[9].buffer = &codice;
	param[9].buffer_length = sizeof(codice);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for exam insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding.");
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

	printf("Prenotazione correctly added with ID %d...\n", codice);

    out:
	mysql_stmt_close(prepared_stmt);
}

static void Rimuovi_prenotazione(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
	
	// Input for the registration routine
	char codPrenotazione[5];
	int codPrenotazione_int;

	// Get the required information
	printf("\nCodice prenotazione: ");
	getInput(5, codPrenotazione, false);

	// Apply proper type conversions
	codPrenotazione_int = atoi(codPrenotazione);
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call rimuovi_prenotazione(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codPrenotazione_int;
	param[0].buffer_length = sizeof(codPrenotazione_int);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while delete.");
		goto out;
	}else {
		printf("Delete correctly completed...\n");
	}

    out:
	mysql_stmt_close(prepared_stmt);
}

static void Report_risultati_prenotazione(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

	int status;
	char header[512];

	
	// Input for the registration routine
	char codPrenotazione[5];
	int codPrenotazione_int;

	// Get the required information
	printf("\nCodice prenotazione: ");
	getInput(5, codPrenotazione, false);

	// Apply proper type conversions
	codPrenotazione_int = atoi(codPrenotazione);
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call report_ris_prenotazione(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &codPrenotazione_int;
	param[0].buffer_length = sizeof(codPrenotazione_int);

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

static void Visualizza_pazienti(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[0];
	
	char header[512];
	int status;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call visualizza_pazienti()", conn)) {
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

static void Visualizza_prenotazioni(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[0];
	
	char header[512];
	int status;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call visualizza_prenotazioni_byCF()", conn)) {
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

static void Visualizza_esami(MYSQL *conn)
{
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[0];
	
	char header[512];
	int status;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call visualizza_esami()", conn)) {
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

static void Aggiungi_recapito(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	// Input for the registration routine
	char CFpaziente[17];
	char Descrizione[256];

	// Get the required information
	printf("\nCF paziente: ");
	getInput(17, CFpaziente, false);
	printf("\nDescrizione: ");
	getInput(256, Descrizione, false);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_recapito(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize exam insertion statement\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = Descrizione;
	param[0].buffer_length = strlen(Descrizione);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = CFpaziente;
	param[1].buffer_length = strlen(CFpaziente);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for exam insertion\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error(prepared_stmt, "An error occurred while adding.");
		goto out;
	}else {
		printf("Insert correctly completed...\n");
	}


    out:
	mysql_stmt_close(prepared_stmt);
}


void run_as_personaleCUP(MYSQL *conn)
{
	char options[10] = {'1','2','3','4','5','6','7','8','9','0'};
	char op;
	
	printf("Switching to personaleCUP role...\n");

	if(!parse_config("users/personaleCUP.json", &conf)) {
		fprintf(stderr, "Unable to load personaleCUP configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while(true) {
		printf("\033[2J\033[H");
		printf("*** What should I do for you? ***\n\n");
		printf("1) Aggiungi paziente\n");
		printf("2) Aggiungi recapito paziente\n");
		printf("3) Aggiungi prenotazione\n");
		printf("4) Report risultati prenotazione\n");
		printf("5) Report esami paziente\n");
		printf("6) Visualizza pazienti\n");
		printf("7) Visualizza esami\n");
		printf("8) Visualizza prenotazioni ordinate per CF\n");
		printf("9) Rimuovi prenotazione\n");
		printf("0) Quit\n");

		op = multiChoice("Select an option", options, 10);

		switch(op) {
			case '1':
				Aggiungi_paziente(conn);
				break;

			case '2':
				Aggiungi_recapito(conn);
				break;
				
			case '3':
				Aggiungi_prenotazione(conn);
				break;
			
			case '4':
				Report_risultati_prenotazione(conn);
				break;
			
			case '5':
				Report_esami_paziente(conn);
				break;

			case '6':
				Visualizza_pazienti(conn);
				break;

			case '7':
				Visualizza_esami(conn);
				break;
			
			case '8':
				Visualizza_prenotazioni(conn);
				break;

			case '9':
				Rimuovi_prenotazione(conn);
				break;

			case '0':
				return;
			
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
