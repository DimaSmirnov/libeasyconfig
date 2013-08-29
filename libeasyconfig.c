#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libeasyconfig.h"

#define LINE_LENGTH 200 // Длина строки конфига - максимум 200 символов

FILE *pFile;
char config_path[100];
char param_value[50];
char* homedir;
int lines_qty=0;

struct  conf{ // -- Список параметров
	char config_line[LINE_LENGTH];
	char param_name[40]; // параметр - название
	char param_value[40]; // параметр - значение
}; conf  *config_content;

//////////////////////////////////////////// Private
int _config_create() {
	pFile=fopen(config_path,"w");
	if (pFile == NULL) { fprintf(stderr, "Can't create config file !\n"); return 1; }
return 0;
}
//////////////////////////////////////////// Public
int Config_remove(const char *c_name) {
	sprintf (config_path,"%s/.config/%s.conf",homedir, c_name);
	if( remove(config_path) != 0 ) { perror( "Can`t remove config file!" ); return -1; }
return 0;
}
/////////////////////////////////////////// Public
int Config_init(const char *conf_name) {
	int a=0, ret=0, counter=0, counter1=1;
	char temp[LINE_LENGTH], *result;
	char* comment;

	homedir = getenv ("HOME");
	sprintf (config_path,"%s/.config/%s.conf", homedir, conf_name);
	if (!(pFile=fopen(config_path,"r"))) a=_config_create(); // Файл конфига не существует, создаем его
	if (a) return 1; // при невозможности создания конфига - выходим с ошибкой
	while (fgets(temp,LINE_LENGTH,pFile)) {
		lines_qty++;
	}
	config_content = new conf[lines_qty+50]; // 50 это резерв, за 1 цикл работы можно добавить максимум 50 значений в конфиг
	for (int i=0;i<lines_qty+50;i++) {
		strcpy(config_content[i].config_line, "");
		strcpy(config_content[i].param_name, "");
		strcpy(config_content[i].param_value, "");
	}
	//printf ("DEBUG: Config readed, %d lines\n",lines_qty);
	rewind(pFile);
	while (fgets(temp,LINE_LENGTH,pFile)) {
		if (strlen(temp)>LINE_LENGTH) { printf("Config error. Very long line: %d\n", counter1); ret=1; break; }  // Слишком длинная строка конфига
		if (strlen(temp)<4) { // Пустая строка
			counter1++;
			continue;
		}

		if (comment = strstr(temp,";")) { // найден комментарий
			if (strstr(temp,"=")) {  // Комментарий после параметра - недопустимо
				strcpy(config_content[counter].config_line,temp);
				strcpy(temp, strncpy (temp,temp,comment-temp)); // Удаляем комментарий в конце строки
			}
			else {
				strcpy(config_content[counter].config_line,comment); // Записываем строку с комментарием
				counter++; counter1++;
				continue;
			}
		}

		strcpy(config_content[counter].config_line,temp);
		result=strtok(temp,"="); // [0] - название параметра
		if (!result) { printf("Config error. Line: %d\n", counter1); ret=1; break; }
        strcpy(config_content[counter].param_name,result);
		result=strtok(NULL,"\n"); // [1] - значение параметра
		if (!result) { printf("Config error. Line: %d\n", counter1); ret=1; break; }
		strcpy(config_content[counter].param_value,result);
		//printf ("DEBUG: Param %d: %s, Value: %s\n",counter, config_content[counter].param_name, config_content[counter].param_value);
		counter++; counter1++;
	}
	fclose(pFile);
return ret;
}
/////////////////////////////////// Public
char *Config_read_param(const char *par_name) {
	for (int i=0;i<lines_qty;i++) {
		//printf ("DEBUG: Read Param %s , %s\n",par_name, config_content[i].param_name);
		if (!strcmp(config_content[i].param_name,par_name)) {
			strcpy(param_value,config_content[i].param_value);
			return param_value;
		}
	}
return NULL;
}
/////////////////////////////////// Public
 int Config_write_param(const char *par_name, const char *value) {
	int i;
	char line[LINE_LENGTH];

	for (i=0;i<lines_qty;i++) { // Ищем параметр и если находим то изменяем его (если вызван Config_write_param вместо Config_change_param)
		if (!strcmp(config_content[i].param_name,par_name)) {
			strcpy(config_content[i].param_value, value);
			sprintf(config_content[i].config_line,"%s=%s\n",par_name,value);
			return 0;
		}
	}
	sprintf(config_content[i].config_line,"%s=%s\n",par_name,value);
	strcpy(config_content[i].param_name, par_name);
	strcpy(config_content[i].param_value, value);
	//printf ("DEBUG: Config_write_param - %s", config_content[i].config_line);
	lines_qty++;

return 0;
}
/////////////////////////////////// Public
 int Config_change_param(const char *par_name, const char *value) {
	 char line[LINE_LENGTH];

	for (int i=0;i<lines_qty;i++) {
		if (!strcmp(config_content[i].param_name,par_name)) {
			strcpy(config_content[i].param_value, value);
			sprintf(config_content[i].config_line,"%s=%s\n",par_name,value);
			return 0;
		}
	}
	return 1; // параметр не найден в конфиге
}
/////////////////////////////////// Public
 int Config_close() {
	int i=0;
	pFile=fopen(config_path,"w");
	//printf ("DEBUG: Lines qty= %d\n", lines_qty);
	while (i<lines_qty) {
		fputs (config_content[i].config_line,pFile);
		//printf ("DEBUG: New config line %d: %s", i, config_content[i].config_line);
		i++;
	}
	fclose(pFile);
	delete[] config_content;
	return 0;
}
