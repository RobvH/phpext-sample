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
    ZEND_BEGIN_ARG_INFO_EX(php_sample_retref_arginfo, 0, 1, 0)
    ZEND_END_ARG_INFO ()
#endif /* PHP >= 5.1.0 */

#endif /* PHP_SAMPLE_H */
