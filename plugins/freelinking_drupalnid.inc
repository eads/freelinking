<?php
// drupal.org nid plugin for freelinking

// allows for a link like [[donid:12345]] to be expanded to
// a link to drupal.org/node/12345

$freelinking['drupalorgnid'] = array(
  'indicator' => '/d(rupal)?o(rg)?nid/',
  'callback' => 'freelinking_drupalorgnid_callback',
);

function freelinking_drupalorgnid_callback($target) { // resolve nid to a node title
  $url = 'http://drupal.org/node/' . $target[1];
  $result = drupal_http_request($url, array(), 'GET');
  $found_title = preg_match('/<h1.*>(.*)<\/h1>/', $result->data, $matches);
  if ($found_title) { // regex worked to scarf title of page
    $replacement = l('Drupal.org: "' . $matches[1] . '"', $url);
  }
  return $replacement;
} // endfunction freelinking_drupalorgnid_callback

// vim:tw=300 nowrap syn=php

