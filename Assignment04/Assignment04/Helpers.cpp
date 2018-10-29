#include "stdafx.h"
#include "Helpers.h"
#include <stdio.h>
#include <time.h>

bool restart_log_()
{
	time_t now;
	char* date;
	FILE* file = fopen(LOG_FILE, "w"); //write

	if (!file)
	{
		fprintf(stderr, "ERROR: Could not open LOG_FILE %s for writing\n", LOG_FILE);
		return false;
	}

	now = time(NULL);
	date = ctime(&now);
	fprintf(file, "%s\n", date);
	fclose(file);
	return true;
}

bool log_(const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen(LOG_FILE, "a"); //append
	if (!file)
	{
		fprintf(stderr, "ERROR: Could not open LOG_FILE %s for appending\n", LOG_FILE);
		return false;
	}

	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

bool err_log_(const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen(LOG_FILE, "a"); //append
	if (!file)
	{
		fprintf(stderr, "ERROR: Could not open LOG_FILE %s for appending\n", LOG_FILE);
		return false;
	}

	fprintf(file, "[ERROR] ");
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}
