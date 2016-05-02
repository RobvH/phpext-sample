#include <php.h>

#include "php_sample.h"

zend_module_entry sample_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_SAMPLE_EXTNAME,
	NULL, /* Functions */
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
