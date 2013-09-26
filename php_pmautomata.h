/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_PMAUTOMATA_H
#define PHP_PMAUTOMATA_H

extern zend_module_entry pmautomata_module_entry;
#define phpext_pmautomata_ptr &pmautomata_module_entry

#ifdef PHP_WIN32
#	define PHP_PMAUTOMATA_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_PMAUTOMATA_API __attribute__ ((visibility("default")))
#else
#	define PHP_PMAUTOMATA_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(pmautomata);
PHP_MSHUTDOWN_FUNCTION(pmautomata);
PHP_RINIT_FUNCTION(pmautomata);
PHP_RSHUTDOWN_FUNCTION(pmautomata);
PHP_MINFO_FUNCTION(pmautomata);

static zend_class_entry *pmautomata_ce;
PHP_METHOD(pmautomata, isLoaded);
PHP_METHOD(pmautomata, build);
PHP_METHOD(pmautomata, match);

struct Node {
	std::map<char, Node*> next;
	bool ac;
	Node* fail;
	Node() : ac(false), fail(0) {}
};

struct Automaton {
	Node* root;
	std::vector<Node*> instances;
	Node* getNext(Node* r, char c);
	Automaton();
	void add(const char* val, int len);
	void build();
	bool match(const char *buf, int len);
};

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

*/
ZEND_BEGIN_MODULE_GLOBALS(pmautomata)
	std::map<long, Automaton*> pma_list;
ZEND_END_MODULE_GLOBALS(pmautomata)

/* In every utility function you add that needs to use variables 
   in php_pmautomata_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as PMAUTOMATA_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define PMAUTOMATA_G(v) TSRMG(pmautomata_globals_id, zend_pmautomata_globals *, v)
#else
#define PMAUTOMATA_G(v) (pmautomata_globals.v)
#endif

#endif	/* PHP_PMAUTOMATA_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
