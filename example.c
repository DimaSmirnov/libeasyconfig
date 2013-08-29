#include <string.h>
#include <stdio.h>

#include "libeasyconfig.h"

int main () {
	int a;
	const char *par_val;

	a = Config_init("test_config");
	if (a) { printf ("Can`t initialize config library! \n"); return -1; }
	else printf ("Config library initialized! \n");

	a= Config_write_param("config_param1", "value1");
	if (a) { printf ("Can`t write config param! \n"); return -1; }
	else printf ("Config parameter wrote! \n");

	a= Config_write_param("config_param2", "value2");
	if (a) { printf ("Can`t write config param! \n"); return -1; }
	else printf ("Config parameter wrote! \n");

	par_val = Config_read_param("config_param1");
	if (!par_val) { printf ("Can`t read config param! \n"); return -1; }
	else printf ("Config param: %s\n", par_val);

	a= Config_change_param("config_param1", "new_value1");
	if (a) { printf ("Can`t change config param! \n"); return -1; }
	else printf ("Config parameter changed! \n");

	a = Config_close();
	printf ("Config library closed! \n");

return 0;
}
