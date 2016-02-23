/*************************************************************************
    > File Name: shttp_log.h
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月13日 21时25分25秒 CST
 ************************************************************************/

#ifndef _SHTTP_LOG_H_
#define _SHTTP_LOG_H_
    void format_time(char *time_buf);
	void log_output_client(int client,char *method,char *url);
    void log_error(int client,char *msg);
    void log_success(int client,char *msg);
    void log_start(int listenfd);
#endif
