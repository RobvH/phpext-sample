#include "php_sample.h"

PHP_FUNCTION(sample_hello_world)
{
	php_printf("Hello World!\n");
}

PHP_FUNCTION(sample_long)
{
	RETVAL_LONG(42);	/* does not return */
	php_printf("The answer.\n");
	return;
}

static zend_function_entry php_sample_functions[] = {   /* function_entry seems to be replaced with zend_function_entry */
	PHP_FE(sample_hello_world,         NULL)
	PHP_FALIAS(sample_hi, sample_hello_world, NULL)
	PHP_FE(sample_long, NULL)
	{ NULL, NULL, NULL }
};

zend_module_entry sample_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_SAMPLE_EXTNAME,
	php_sample_functions, /* Functions */
	NULL, /* MINIT */
	NULL, /* MSHUTDOWN */
	NULL, /* RINIT */
	NULL, /* RSHUTDOWN */
	NULL, /* MINFO */
	PHP_SAMPLE_EXTVER,
	STANDARD_MODULE_PROPERTIES
};


/* Enable building as a shared module */
#ifdef COMPILE_DL_SAMPLE
ZEND_GET_MODULE(sample)
#endif
