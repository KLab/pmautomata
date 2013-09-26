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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <map>
#include <string>
#include <queue>
extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pmautomata.h"
}

/* If you declare any globals in php_pmautomata.h uncomment this:
*/
ZEND_DECLARE_MODULE_GLOBALS(pmautomata)

/* True global resources - no need for thread safety here */
static int le_pmautomata;

/* {{{ pmautomata_functions[]
 *
 * Every user visible function must have an entry in pmautomata_functions[].
 */
const zend_function_entry pmautomata_functions[] = {
	PHP_ME(pmautomata, isLoaded, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(pmautomata, build, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(pmautomata, match, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	{0,0,0}	/* Must be the last line in pmautomata_functions[] */
};
/* }}} */

/* {{{ pmautomata_module_entry
 */
zend_module_entry pmautomata_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"pmautomata",
	pmautomata_functions,
	PHP_MINIT(pmautomata),
	PHP_MSHUTDOWN(pmautomata),
	PHP_RINIT(pmautomata),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pmautomata),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pmautomata),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PMAUTOMATA
ZEND_GET_MODULE(pmautomata)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("pmautomata.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pmautomata_globals, pmautomata_globals)
    STD_PHP_INI_ENTRY("pmautomata.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pmautomata_globals, pmautomata_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_pmautomata_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_pmautomata_init_globals(zend_pmautomata_globals *pmautomata_globals)
{
	pmautomata_globals->global_value = 0;
	pmautomata_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pmautomata)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "PMAUTOMATA", pmautomata_functions);
	pmautomata_ce = zend_register_internal_class(&ce TSRMLS_CC);
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pmautomata)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(pmautomata)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pmautomata)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pmautomata)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pmautomata support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

Automaton::Automaton() {
	root = new Node;
	instances.push_back(root);
}

Node* Automaton::getNext(Node* r, char c) {
	std::map<char, Node*>::iterator it = r->next.find(c);
	if (it != r->next.end()) return it->second;
	if (r == root) return root;
	return 0;
}

void Automaton::add(const char* buf, int len) {
	Node* t = root;
	for (int j = 0; j < len; ++ j) {
		std::map<char, Node*>::iterator it = t->next.find(buf[j]);
		if (it != t->next.end()) {
			t = it->second;
		} else {
			Node* n = new Node;
			instances.push_back(n);
			t->next.insert(std::make_pair(buf[j], n));
			t = n;
		}
	}
	t->ac = true;
}

void Automaton::build() {
	std::queue<Node*> Q;
	for (std::map<char,Node*>::iterator i = root->next.begin(); i != root->next.end(); ++ i) {
		i->second->fail=root;
		Q.push(i->second);
	}
	while(!Q.empty()){
		Node* t = Q.front(); Q.pop();
		for (std::map<char,Node*>::iterator i = t->next.begin(); i != t->next.end(); ++ i) {
			char c = i->first;
			Q.push(i->second);
			Node* r = t->fail;
			while (!getNext(r, c)) r = r->fail;
			i->second->fail = getNext(r, c);
			if (!getNext(r, c)) throw 1;
			if (getNext(r, c)->ac) i->second->ac = true;
		}
	}
}

bool Automaton::match(const char *buf, int len) {
	Node* r = root;
	for (int i = 0; i < len; ++ i) {
		while (!getNext(r, buf[i])) r = r->fail;
		r = getNext(r, buf[i]);
		if (r->ac) return true;
	}
	return false;
}

PHP_METHOD(pmautomata, isLoaded)
{
	long id;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &id) == FAILURE) return;
	
	if (PMAUTOMATA_G(pma_list).count(id)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

PHP_METHOD(pmautomata, build)
{
	long id;
	zval *a;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "la", &id, &a) == FAILURE) return;
	HashTable *hash = Z_ARRVAL_P(a);
	int num = zend_hash_num_elements(hash);
	
	if (PMAUTOMATA_G(pma_list).count(id)) {
		//@todo clean up
		PMAUTOMATA_G(pma_list).erase(id);
	}
	Automaton* pma = new Automaton;
	for (int i = 0; i < num; ++ i) {
		zval **data;
		if (zend_hash_get_current_data(hash, (void **)&data) == FAILURE) return;
		if (Z_TYPE_PP(data) != IS_STRING) continue;
		pma->add((*data)->value.str.val, (*data)->value.str.len);
		zend_hash_move_forward(hash);
	}
	pma->build();
	PMAUTOMATA_G(pma_list).insert(std::make_pair(id, pma));
}

PHP_METHOD(pmautomata, match)
{
	long id;
	char *arg_buf = 0;
	int arg_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &id, &arg_buf, &arg_len) == FAILURE) return;
	
	std::map<long, Automaton*>::iterator it = PMAUTOMATA_G(pma_list).find(id);
	if (it != PMAUTOMATA_G(pma_list).end() && it->second->match(arg_buf, arg_len)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
