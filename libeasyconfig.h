int _config_create();
extern int Config_init(const char *conf_name);
extern int Config_write_param(const char *par_name, const char *par_value);
char *Config_read_param(const char *par_name);
extern int Config_change_param(const char *par_name, const char *par_value);
extern int Config_remove(const char *conf_name);
extern int Config_close();

