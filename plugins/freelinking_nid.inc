<?php
// nid plugin for freelinking
// Originially by [grayside](http://drupal.org/user/346868)
// See http://drupal.org/node/486954

// allows for a link like [[node:<nid>]], [[n:<nid>]], or [[node:<nid>]] to be expanded to
// a link to the node with the title associated with that nid.
// A "could not find nid" message is displayed if the nid could not be found.

$freelinking['nid'] = array(
  'indicator' => '/(n(id|ode)?)$/A',
  'callback' => 'freelinking_nid_callback',
  'tip' => t('Link to a local node by nid'),
  'run on view' => TRUE,
);

function freelinking_nid_callback($target) { // resolve $target into a link to the node or display failure
  if (!is_numeric($target['dest'])) {
    return array('error' =>
      t('Invalid Node ID "!nid"', array('!nid' => $target['dest']))
    );
  }
  $sql = 'SELECT title FROM {node} WHERE nid = "%s" ';
  $result = db_query($sql, $target['dest']);

  while ($node = db_fetch_object($result)) { // should be only one
    $title = $node->title;
  } // endwhile looping through (one) node

  // Node not found for $nid.
  if (!$title) {
    return array('error' =>
      t('Invalid Node ID "!nid"', array('!nid' => $target['dest']))
    );
  }

  $title = $target['text'] ? $target['text'] : $title;
  if (!$target['tooltip']) {
    $target['tooltip'] = freelinking_internal_tooltip('node', $target['dest']);
  }

  $replacement =  array($title, 'node/' . $target['dest'], array(
    'attributes' => array('title' => $target['tooltip']),
  ));
  return $replacement;
}

// vim:tw=300 nowrap syn=php

