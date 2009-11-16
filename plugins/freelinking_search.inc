<?php
// $Id$
/**
 * Search Plugin for Freelinking
 */

$freelinking['search'] = array(
  'indicator' => '/search/',
  'translate' => array(' ' => '+'),
  'callback' => 'freelinking_search_callback',
  'tip' => t('Run a local search for the specified terms.'),
);

$freelinking['google'] = array(
  'indicator' => '/google/',
  'translate' => array(' ' => '+'),
  'callback' => 'freelinking_google_callback',
  'tip' => t('Search google for the specified terms.'),
);

function freelinking_search_callback($target) {
  global $base_url;
  $link = array();
  $failover = variable_get('freelinking_search_failover', 'error');

  if (!module_exists('search')) {
    if ($failover == 'error') {
      // send back an error message.
      return array('error' => t('Search Unavailable'));
    }
    else {
      // specify a plugin for next effort
      $target['other']['site'] = $base_url;
      return array('plugin' => $failover, 'target' => $target);
    }
  }

  $link[0] = $target['text'] ? $target['text'] : $target['dest'];
  $link[1] = 'search/node/' . $target['dest'];
  $link[2]['attributes']['title'] =
    t('Search this site for content like "%dest"', array('%dest' => $target['dest']));
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
  global $language;
  if (!$language) {
    $lang = 'en';
  }
  else {
    $lang = $language->language;
  }

  if ($target['text']) {
    $title = $target['text'];
  }
  else {
    $title = 'Google Search "' . $target['dest'] . '"';
  }
  
  $link = array($title, 'http://www.google.com/search', array(
    'query' => array('q' => $target['dest']),
    'fragment' => 'hl=' . $lang,
    'external' => TRUE,
    'attributes' => array(
      'title' => t('Search Google for content like "%dest"',
        array('%dest' => $target['dest'])),
    ),
  ));
  if ($target['other']['site']) {
    $link[2]['query']['as_sitesearch'] = urlencode($target['other']['site']);
  }
  return $link;
}
