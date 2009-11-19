<?php
// $Id$
/**
 * Drupal.org Project plugin for freelinking
 *
 * @file
 * Allows for a link like [[drupalproject:Freelinking]] to be expanded to
 * drupal.org/project/freelinking
 */
$freelinking['drupalproject'] = array(
  'indicator' => '/d(rupal)?project/',
  'translate' => array(' ' => '_'),
  'replacement' => 'http://drupal.org/project/%1',
  'tip' => t('Link to a project on drupal.org by name.'),
);

