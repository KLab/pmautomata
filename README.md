# PMAutomata

## Install

Run following commands:
~~~
phpize
./configure
make
sudo make install
~~~

and edit php.ini to add `extension=pmautomata.so`

## Usage

### Class PMAUTOMATA

 * static bool isLoaded(int $id)
   * If an automaton is loaded in $id, return `TRUE`, otherwise return `FALSE`.

 * static void build(int $id, array $patterns)
   * $patterns must be array of strings.
   * This method builds an automaton into $id.

 * static bool match(int $id, string $text)
   * If $text includes a pattern of the automaton in $id, return `TRUE`, otherwise return `FALSE`.

## Sample

```php
<?php
PMAUTOMATA::build(42, array('hoge', 'piyo'));
var_dump(PMAUTOMATA::match(42, 'hogehoge')); // true
var_dump(PMAUTOMATA::match(42, 'HogeHoge')); // false, PMAutomata is case-sensitive.
var_dump(PMAUTOMATA::match(42, 'fugafuga')); // false

var_dump(PMAUTOMATA::isLoaded(1729)); // false

PMAUTOMATA::build(1729, array('foo', 'bar'));
var_dump(PMAUTOMATA::match(1729, 'hogehoge')); // false
var_dump(PMAUTOMATA::match(1729, 'foo bar.')); // true

var_dump(PMAUTOMATA::isLoaded(1729)); // true
```
