# freelinking.module -- a freelinking filter for Drupal
---

## Freelinking 3 for Drupal 6

Freelinking 3 is a complete rewrite of the freelinking module.
Freelinking 3 will be released for Drupal 6 and Drupal 7, and perhaps
for future versions as well.

Freelinking 3 breaks from previous versions in some significant ways:

* There is no database table in FL3. Previous versions of Freelinking
  use a table to keep track of freelinks and their targets.
* Plugins are available to enhance FL3's functionality. See the
  PLUGINs.TXT file for "drop-in" plugins, and MODULES.TXT for plugin 
  modules. FL3 ships with some of each.
* Freelinks made with the "nodetitle" plugin (which mimics the behavior
  of previous versions of freelinking) do not run through the
  'freelinking/' namespace.
* FL3 requires the [prepopulate](http://drupal.org/project/prepopulate)
  module for its "nodetitle" plugin.
* FL3 now provides partial support for Markdown syntax, able to process 
  indicator:target pairs in place of Markdown link URLs.

As of Alpha-3, some features are still missing:

* There is no consideration for freelinks made with the "nodetitle"
  plugin which did not exist at submission time but have been created
  later. This was the main reason for using the 'freelinking/' namespace
  in previous versions.

## Maintainers
* eafarris <eafarris@gmail.com> (Original Creator)
* grayside <grayside@gmail.com> 


$Id$
vim: tw=72 syn=mkd
