void write_to_history_file(void) {
	FILE *pFile;
	time_t timer;
	char buffer[50];
	char tmp[60];
	struct tm* tm_info;
	timer = time(NULL);
	tm_info = localtime(&timer);
	
	pFile=fopen("doorbell_history.txt", "a");

	if(pFile==NULL) {
		printf("Error opening file.");
	}
	else {
			strftime(buffer, sizeof(buffer), "%d.%m.%Y um %H:%M:%S Uhr", tm_info);
			snprintf(tmp, sizeof(tmp), "Klingel gedr\201ckt am %s\r\n", buffer);
			fprintf(pFile, "%s", tmp);
		}
	fclose(pFile);
}