#pragma once

#pragma warning(disable:4996)

constexpr auto LOG_FILE = "output.log";

bool restart_log_();
bool log_(const char* message, ...);
bool err_log_(const char* message, ...);
