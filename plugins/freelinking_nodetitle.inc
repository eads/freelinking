<?php
// nodetitle plugin for freelinking

// allows for a link like [[nodetitle:Freelinking filter]] to be expanded to
// a link to the node titled "Freelinking filter" or a link to create
// the node.

$freelinking['nodetitle'] = array(
  'indicator' => '/n(t|odetitle)/',
  'callback' => 'freelinking_nodetitle_callback',
  'run on view' => TRUE,
);

function freelinking_nodetitle_callback($target) { // resolve $target into a link to the node or a create link
  $sql = 'SELECT nid FROM {node} WHERE title = "%s" ';
  if (variable_get('freelinking_nodetitle_searchcontenttype', 'none') != 'none') { // restrict title search to a content type
    $sql .= 'AND type ="%s';
    $result = db_query($sql, $target[1], variable_get('freelinking_nodetitle_searchcontenttype'));
  } // endif title search restriction
  else {
    $result = db_query($sql, $target[1]);
  } // endifelse no title search restriction
  while ($node = db_fetch_object($result)) { // should be only one
    $nid = $node->nid;
  } // endwhile looping through (one) node
  
  if ($nid) { // I got your freelink right here
    $replacement =  l($target[1], 'node/' . $nid, array(
      'attributes' => array('class' => 'freelink',),
    ));
  } // endif a node was found matching the target
  else { // Didn't find one. Present a link to create/do a search
    $notfoundaction = variable_get('freelinking_nodetitle_notfoundaction', 'create');
    if ($notfoundaction == 'create' or ($notfoundaction == 'noaccesssearch' && node_access('create', variable_get('freelinking_nodetitle_newcontenttype', 'story')))) {
      $replacement = l($target[1], 'node/add/' . variable_get('freelinking_nodetitle_newcontenttype', 'story') , array(
        'attributes' => array(
          'class' => 'freelink notfound',
          'title' => 'Create content titled "' . $target[1] . '"',
        ),
        'query' => array('edit[title]' => $target[1]),
        ));
    } // endif create
    else { // either no create action or no access rights, so search
      $replacement = l($target[1], 'search/node/' . $target[1], array(
        'attributes' => array(
          'class' => 'freelink notfound freelink-search',
          'title' => 'Search this site for content like "' . $target[1] . '"',
        ),
      ));
    } // endifelse search
  } // endifelse no node found
  return $replacement;
}

function freelinking_nodetitle_settings() { // settings for the nodetitle plugin
  $form['freelinking_nodetitle_newcontenttype'] = array(
    '#title' => t('Default for new content'),
    '#type'  => 'select',
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('freelinking_nodetitle_newcontenttype', 'story'),
    '#description'  => t('Content that is to be created from a freelink should be of this type.'),
  );
  $form['freelinking_nodetitle_searchcontenttype'] = array(
    '#title' => t('Restrict freelinks to this content type'),
    '#type'  => 'select',
    '#options' => array_merge(array('none' => t('No restriction')), node_get_types('names')),
    '#default_value' => variable_get('freelinking_nodetitle_searchcontenttype', 'none'),
    '#description' => t('Lookup by title to find a freelink will be restricted to this content type only.'),
  );
  $form['freelinking_nodetitle_notfoundaction'] = array(
    '#title' => t('If a suitable content is not found:'),
    '#type' => 'select',
    '#options' => array(
      'create' => t('Only try to create content. Do not search for content. May give a 403 if the user does not have permission'),
      'noaccesssearch' => t('Try to create content, but if the user does not have permission, do a search. Requires search.module'),
      'search'  => t('Only search for content. Do not try to create new content. Requires search.module'),
    ),
    '#default_value' => variable_get('freelinking_nodetitle_notfoundaction', 'create'),
    '#description' => t('What should freelinking do when content to link to is not found?'),
  );
  
  return $form;
} // endfunction freelinking_nodetitle_settings()

// vim:tw=300 nowrap syn=php

