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

#if (PHP_MAJOR_VERSION > 5 || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0))

/*  function &sample_reference_a() 
 *  {
 *      if (!isset($GLOBALS['a'])) {
 *          $GLOBALS['a'] = null;
 *      }
 *      
 *      return $GLOBALS['a'];
 *  }    
 */

/* Result:
 * $a = 1;
 * $c = $a;
 * $b =& sample_reference_a();
 * $b = 2;
 * $c = 3;
 * echo $a . ' ' . $b . ' ' . $c . PHP_EOL;
 * OUTPUT: 2 2 3
 */

PHP_FUNCTION(sample_reference_a)
{
	zval **a_prt, *a;

	/* Fetch $a from the global symbol table */
	if (zend_hash_find(&EG(symbol_table), "a", sizeof("a"), (void**) &a_prt) == SUCCESS) {
		a = *a_prt;
	} else {
		/* $GLOBALS['a'] doesn't exist yet, create it */
		ALLOC_INIT_ZVAL(a);

		zend_hash_add(&EG(symbol_table), "a", sizeof("a"), &a, sizeof(zval*), NULL);
	}

	/* toss out the old return_value */
	zval_ptr_dtor(return_value_ptr);

	if (!zval_isref_p(a) && zval_refcount_p(a) > 1) {
		/* $a is in a copy-on-write reference set
		 * It must be separated before it can be used.
		 */
		zval *newa;

		MAKE_STD_ZVAL(newa);
		*newa = *a;
		zval_copy_ctor(newa);

		zval_unset_isref_p(newa);
		zval_set_refcount_p(newa, 1);

		/* update will cause the copy-on-write for whatever was in the reference set with a */
		zend_hash_update(&EG(symbol_table), "a", sizeof("a"), &newa, sizeof(zval*), NULL);

		a = newa;
	}

	/* Promote to full-reference and increase refcount */
	zval_set_isref_p(a);
	zval_addref_p(a);

	*return_value_ptr = a;
}
#endif /* PHP >= 5.1.0 */

PHP_FUNCTION(sample_byref_calltime) /* calltime pass by reference is removed */ 
{
	zval *a;
	int addtl_len = sizeof(" (modified by ref!)") - 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &a) == FAILURE) {
		RETURN_NULL();
	}

	if (!zval_isref_p(a)) {
		/* parameter was not passed by reference,
		 * leave without doing anything
		 */
		return;
	}
	/* Make sure the variable is a string */
	convert_to_string(a);

	/* Enlarge a's buffer to hold the additional data */
	Z_STRVAL_P(a) = erealloc(Z_STRVAL_P(a), Z_STRLEN_P(a) + addtl_len + 1);

	memcpy(Z_STRVAL_P(a) + Z_STRLEN_P(a), " (modified by ref!)", addtl_len + 1);

	Z_STRLEN_P(a) += addtl_len;
}

/* demonstrates passing parameters */
PHP_FUNCTION(sample_getlong)
{
	long foo;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &foo) == FAILURE) {
		RETURN_NULL();
	}

	php_printf("The integer value of the parameter you "
			"passed is: %ld\n", foo);

	RETURN_TRUE;
}

/* demonstrates multiple parameters */
PHP_FUNCTION(sample_hello_world2)
{
	char *name;
	int name_len;
	char *greeting;
	int greeting_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &name, &name_len, &greeting, &greeting_len) == FAILURE) {
		RETURN_NULL();
	}

	php_printf("Hello ");
	PHPWRITE(greeting, greeting_len);
	php_printf(" ");
	PHPWRITE(name, name_len);
	php_printf("!\n");
}

/* demonstrates optional parameters */
PHP_FUNCTION(sample_hello_world3)
{
	char *name;
	int name_len;
	char *greeting = "Mr. / Mrs. ";
	int greeting_len = sizeof("Mr. / Mrs. ") - 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", 
		&name, &name_len, &greeting, &greeting_len) == FAILURE) {
		RETURN_NULL();
	}

	php_printf("Hello ");
	PHPWRITE(greeting, greeting_len);
	php_printf(" ");
	PHPWRITE(name, name_len);
	php_printf("!\n");
}

/* demonstrating return a zval IS_NULL vs real null 
 * sample_arg_fullnull(null);
*/
PHP_FUNCTION(sample_arg_fullnull)
{
	zval* val;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &val) == FAILURE) {
		RETURN_NULL();
	}

	if (Z_TYPE_P(val) == IS_NULL) {
		/* val = php_sample_make_defaultval(TSRMLS_CC); */
		php_printf("Got a zval that IS_NULL\n");
	}
}
/* this variation tells zpp that we can accept a real null */
PHP_FUNCTION(sample_arg_nullok)
{
	zval* val;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z!", &val) == FAILURE) {
		/* zpp handles firing off the warning about the arguments not matching the 
		 * expected parameters.
		 */
		RETURN_NULL();
	}

	if (!val) {
		php_printf("Got a real null\n");	
	}
}

/* Demonstrates using zend_get_parameters */
PHP_FUNCTION(sample_onearg_zgp)
{
	zval *firstarg;

	if (zend_get_parameters(ZEND_NUM_ARGS(), 1, &firstarg) == FAILURE) {
		/* need to manually fire of the arguments/parameters mismatch */
		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Expected at least 1 argument.");

		RETURN_NULL();
	}

	php_printf("Received a zval for one argument.\n");
}

/* zend_get_parameters_ex does not explicitly separate copy-on-write reference sets. */
PHP_FUNCTION(sample_onearg_zgp_ex)
{
	zval **firstarg;

	/* zgp_ex is deprecated */
	if (zend_get_parameters_ex(1, &firstarg) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	php_printf("Received a zval for one argument.\n");	
}

/* Handling an arbitrary number of arguments */
PHP_FUNCTION(sample_var_dump)
{
	int i, argc = ZEND_NUM_ARGS();
	zval *** args;

	args = (zval ***)safe_emalloc(argc, sizeof(zval **), 0);

	if (ZEND_NUM_ARGS() == 0 ||
		zend_get_parameters_array_ex(argc, args) == FAILURE) {
		efree(args);
		WRONG_PARAM_COUNT;
	}

	for (i=0; i<argc; ++i) {
		php_var_dump(args[i], 1 TSRMLS_CC);
	}

	efree(args);
}

static zend_function_entry php_sample_functions[] = {   /* function_entry seems to be replaced with zend_function_entry */
	PHP_FE(sample_hello_world,         NULL)
	PHP_FALIAS(sample_hi, sample_hello_world, NULL)
	PHP_FE(sample_long, NULL)
	PHP_FE(sample_return_long, NULL)
	PHP_FE(sample_array_range, NULL)
#if (PHP_MAJOR_VERSION > 5 || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0))
	PHP_FE(sample_reference_a, php_sample_retref_arginfo)
#endif
	PHP_FE(sample_byref_calltime, NULL)
	PHP_FALIAS(sample_byref_compiletime, sample_byref_calltime, php_sample_byref_arginfo)
	PHP_FE(sample_getlong, NULL)
	PHP_FE(sample_hello_world2, NULL)
	PHP_FE(sample_hello_world3, NULL)
	PHP_FE(sample_arg_fullnull, NULL)
	PHP_FE(sample_arg_nullok, NULL)
	PHP_FE(sample_onearg_zgp, NULL)
	PHP_FE(sample_onearg_zgp_ex, NULL)
	PHP_FE(sample_var_dump, NULL)
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
