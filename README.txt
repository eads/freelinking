# freelinking.module -- a freelinking filter for Drupal
---

by [ea. Farris] [1], with portions adapted from work done by Christopher
Whipple on his [wiki.module] [2].

## What it does

freelinking.module allows content authors to link to other pieces of
content (ie., nodes) in the Drupal site easily, using CamelCase words
and freelinking delimiters, currently defined as double-square brackets
( [[ and ]] ).

When enabled, the freelinking filter searches the body of nodes looking
for "CamelCase" style words (words that begin with a capital letter and
have one or more capitalized words run together) and words or phrases
enclosed in double-square brackets [[like this]]. These words become
clickable to a node with the words as the title. If no node so titled
exists, the link will attempt to create the node and present the user
with the node creation form, with the title already filled in.

## Installation and activation

For installation instrutions, and for information on how to activate
this module, see INSTALL.txt

## Configuration

Currently, freelinking.module supports the following configuration
options:

- What kind of node will the filter attempt to create if a target node
  was not found? This can be any node type. A simple flexinode with
  title and body, that is editable by anyone, could turn Drupal into a
  wiki. Defaults to 'blog.'

- What kind of node will be searched for, when looking up a title? This
  should be the same as the creation node type, above, or new
  freelinking-created content won't ever be found. Defaults to 'no
  restrictions,' meaning all content types are eligible to be the target
  of a freelink.

Other options planned, but not yet implemented, include:

- Flexible delimiters for phrases. Double-square brackets are going to
  be used to link to the [wikipedia] [3], so this filter should give
  some choice as to what will be used as the freelinking delimiters.
- Restrict freelinking to nodes created by the same user. For example,
  Author1 writing a blog entry with freelinks should expect his links to
  resolve to other content by him, and not just any content on the site.
  This would give multi-authored sites into several private wikis, each
  linking only to his or her own content.

---
References

[1] : mailto:eafarris@gmail.com
[2] : http://ninjafish.org/project/wiki
[3] : http://www.wikipedia.net
