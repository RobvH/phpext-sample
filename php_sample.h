#ifndef PHP_SAMPLE_H
#define PHP_SAMPLE_H

/* Define extension properties */
#define PHP_SAMPLE_EXTNAME    "sample"
#define PHP_SAMPLE_EXTVER     "1.0"

#ifdef ZTS
#include "TSRM.h"
#endif

/* Import configure options 
   when building outside the PHP
   source tree */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Include the php std header */
#include <php.h>

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry sample_module_entry;
#define phpext_sample_ptr &sample_module_entry

#if (PHP_MAJOR_VERSION > 5 || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0))
// static 
                          /* name, pass_rest_by_reference, return_reference, requried_num_args */
    ZEND_BEGIN_ARG_INFO_EX(php_sample_retref_arginfo, 0, 1, 0)
    ZEND_END_ARG_INFO ()
#endif /* PHP >= 5.1.0 */

#ifdef ZEND_ENGINE_2
ZEND_BEGIN_ARG_INFO(php_sample_byref_arginfo, 0) /* pass_rest_by_reference = 0 */
	ZEND_ARG_PASS_INFO(1) /* byref = 1 */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(php_sample_count_array_arginfo, 0)
	ZEND_ARG_ARRAY_INFO(0, "arr", 0)
ZEND_END_ARG_INFO()
#else
static unsigned char php_sample_byref_arginfo[] = { 1, BYREF_FORCE };
#endif

#endif /* PHP_SAMPLE_H */
