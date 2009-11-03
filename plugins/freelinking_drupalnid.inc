<?php
// drupal.org nid plugin for freelinking

// allows for a link like [[donid:12345]] to be expanded to
// a link to drupal.org/node/12345

$freelinking['drupalorgnid'] = array(
  'indicator' => '/d(rupal)?o(rg)?/',
  'callback' => 'freelinking_drupalorgnid_callback',
);


function freelinking_drupalorgnid_callback($target) { // resolve nid to a node title
  if (!is_numeric($target['dest'])) {
    return array('error' =>
      t('Invalid Node ID !nid', array('!nid' => $target['dest'])));
  }

  $url = 'http://drupal.org/node/' . $target['dest'];
  if (!$target['text'] && variable_get('drupalorgnid_http_request', true)) {
    $result = drupal_http_request($url, array(), 'GET');
    $found_title = preg_match('/<h1.*>(.*)<\/h1>/', $result->data, $matches);

    if ($found_title) { // regex worked to scarf title of page
      $replacement = array('Drupal.org: "' . $matches[1] . '"', $url);
    }
  }
  else if ($target['text']) {
    $replacement = array($target['text'], $url);
  }

  if (empty($replacement)) {
    $replacement = array('Drupal.org Node: ' . $target['dest'], $url);
  }

  return $replacement;
} // endfunction freelinking_drupalorgnid_callback

function freelinking_drupalorgnid_settings() {
  $form['drupalorgnid']['drupalorgnid_http_request'] = array(
    '#type' => 'checkbox',
    '#title' => t('Get node title'),
    '#default_value' => variable_get('drupalorgnid_http_request', true),
    '#description' => t('Get node title from drupal.org.'),
  );
  return $form;
}


// vim:tw=300 nowrap syn=php

