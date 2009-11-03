<?php
// $Id$
/**
 * Search Plugin for Freelinking
 */

$freelinking['search'] = array(
  'indicator' => '/search/',
  'translate' => array(' ' => '+'),
  'callback' => 'freelinking_search_callback',
);

$freelinking['google'] = array(
  'indicator' => '/google/',
  'translate' => array(' ' => '+'),
  'callback' => 'freelinking_google_callback',
);

function freelinking_search_callback($target) {
  $link = array();

  if (module_exists('search') && user_access('search content')) {
    $link[1] = 'search/node/' . $target['dest'];
  }
  // search does not exist, handle failover
  else {
    if ($plugin = variable_get('freelinking_search_failover', 'error') == 'error') {
      // send back an error message.
      return array('error' => t('Search Access Denied'));
    }
    else {
      // specify a plugin for next effort
      return array('plugin' => $plugin);
    }
  }

  $link[0] = $target['text'] ? $target['text'] : $target['dest'];
  $link[2]['attributes']['title'] =
    t('Search this site for content like %dest', array('%dest' => $target['dest']));
  return $link;
}

/**
 * Settings callback for Search plugin
 */
function freelinking_search_settings() {
  $form['search']['freelinking_search_failover'] = array(
    '#type' => 'select',
    '#title' => t('Failover Option'),
    '#default_value' => variable_get('freelinking_search_failover', 'error'),
    '#options' => array('error' => t('Error Message'), 'google' => 'Google'),
    '#description' => t('If Search is disabled or inaccessible do something else.'),
  );
  return $form;
}

/**
 * Replacement callback for Google plugin
 */
function freelinking_google_callback($target) {
  $title = $target['text'] ? $target['text'] : 'Google Search "'
    . $target['dest'] . '"';

  $url = 'http://www.google.com/search?q=' . $target['dest'];
  $link = array($title, $url);
}
