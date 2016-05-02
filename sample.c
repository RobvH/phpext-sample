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

PHP_FUNCTION(sample_return_long)
{
	RETURN_LONG(42);	/* reference table on pg 69 */
}

PHP_FUNCTION(sample_array_range)
{
	if (return_value_used) {
		int i;
		/* Return an array from 0 - 999 */
		array_init(return_value);

		for(i = 0; i < 1000; ++i) {
			add_next_index_long(return_value, i);
		}

		return;
	}

	/* Save the effort */
	php_printf("Static return-only function called without processing output.\n");
	php_error_docref(NULL TSRMLS_CC, E_NOTICE, 
		"Static return-only function called without processing output.");

	RETURN_NULL();
}

static zend_function_entry php_sample_functions[] = {   /* function_entry seems to be replaced with zend_function_entry */
	PHP_FE(sample_hello_world,         NULL)
	PHP_FALIAS(sample_hi, sample_hello_world, NULL)
	PHP_FE(sample_long, NULL)
	PHP_FE(sample_return_long, NULL)
	PHP_FE(sample_array_range, NULL)
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
