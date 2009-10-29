<?php
// nid plugin for freelinking
// Originially by [grayside](http://drupal.org/user/346868)
// See http://drupal.org/node/486954

// allows for a link like [[node:<nid>]], [[n:<nid>]], or [[node:<nid>]] to be expanded to
// a link to the node with the title associated with that nid.
// A "could not find nid" message is displayed if the nid could not be found.

$freelinking['nid'] = array(
  'indicator' => '/(n(id|ode)?)$/',
  'callback' => 'freelinking_nid_callback',
  'run on view' => TRUE,
);

function freelinking_nid_callback($target) { // resolve $target into a link to the node or display failure
  $sql = 'SELECT title FROM {node} WHERE nid = "%s" ';
  $result = db_query($sql, $target[1]);

  while ($node = db_fetch_object($result)) { // should be only one
    $title = $node->title;
  } // endwhile looping through (one) node

  if ($title) { // I got your freelink right here
    $title = $target[2] ? $target[2] : $title;
    $tooltip = $target[3] ? $target[3] : $title;

    $replacement =  l($title, 'node/' . $target[1], array(
      'attributes' => array(
        'class' => 'freelink',
        'title' => $tooltip,
      ),
    ));
  } // endif a title was found matching the target
  else { // Didn't find one. goto fail.
    $replacement = '[' . t('Could not find nid') . ' ' . $nid . ']';
  } // endifelse no node found
  return $replacement;
}

// vim:tw=300 nowrap syn=php

