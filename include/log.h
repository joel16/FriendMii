#ifndef FRIENDMII_LOG_H
#define FRIENDMII_LOG_H

#define DEBUG(...) log_func(__VA_ARGS__)

void log_func(const char *s, ...);

#endif
