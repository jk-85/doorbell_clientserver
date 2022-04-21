int logfile(char* message) {
	char buffer[4096];
	
	setlogmask(LOG_UPTO (LOG_NOTICE));
	openlog("bell_server", LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	//syslog (LOG_NOTICE, "Program started by User %d", getuid ());
	snprintf((char *)buffer, sizeof(buffer), "[%s]", message);
	syslog(LOG_NOTICE, buffer);
	closelog();
}
