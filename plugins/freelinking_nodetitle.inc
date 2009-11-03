<?php

/**
 * Nodetitle plugin for Freelinking
 * allows for a link like [[nodetitle:Freelinking filter]] to be expanded to
 * a link to the node titled "Freelinking filter" or a link to create
 * the node.
 */

$freelinking['nodetitle'] = array(
  'indicator' => '/n(t|odetitle)/',
  'callback' => 'freelinking_nodetitle_callback',
  'run on view' => TRUE,
);

/**
 * Replacement callback for nodetitle plugin
 */
function freelinking_nodetitle_callback($target) { // resolve $target into a link to the node or a create link
  $sql = 'SELECT nid FROM {node} WHERE title = "%s" ';
  if (variable_get('freelinking_nodetitle_searchcontenttype', 'none') != 'none') { // restrict title search to a content type
    $sql .= 'AND type ="%s" ';
    $result = db_query($sql, $target['dest'], variable_get('freelinking_nodetitle_searchcontenttype'), 'none');
  } // endif title search restriction
  else {
    $result = db_query($sql, $target['dest']);
  } // endifelse no title search restriction
  while ($node = db_fetch_object($result)) { // should be only one
    $nid = $node->nid;
  } // endwhile looping through (one) node

  $title = $target['text'] ? $target['text'] : $target['dest'];
  if ($nid) { // I got your freelink right here
    $replacement =  array($title, 'node/' . $nid);
  } // endif a node was found matching the target
  else { // Didn't find one. Present a link to create/do a search
    $notfoundaction = variable_get('freelinking_nodetitle_notfoundaction', 'create');
    if ($notfoundaction == 'create' or ($notfoundaction == 'noaccesssearch' && node_access('create', variable_get('freelinking_nodetitle_newcontenttype', 'story')))) {
      $replacement = array($target['dest'], 'node/add/' . variable_get('freelinking_nodetitle_newcontenttype', 'story') , array(
        'attributes' => array(
          'class' => 'notfound',
          'title' => 'Create content titled "' . $target['dest'] . '"',
        ),
        'query' => array('edit[title]' => $target['dest']),
        ));
    } // endif create
    else { // either no create action or no access rights, so search
      return array('plugin' => 'search');
    } // endifelse search
  } // endifelse no node found
  return $replacement;
}

/**
 * Nodetitle Settings Callback
 */
function freelinking_nodetitle_settings() { // settings for the nodetitle plugin
  $form['nodetitle']['freelinking_nodetitle_newcontenttype'] = array(
    '#title' => t('Default for new content'),
    '#type'  => 'select',
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('freelinking_nodetitle_newcontenttype', 'story'),
    '#description'  => t('Content that is to be created from a freelink should be of this type.'),
  );
  $form['nodetitle']['freelinking_nodetitle_searchcontenttype'] = array(
    '#title' => t('Restrict freelinks to this content type'),
    '#type'  => 'select',
    '#options' => array_merge(array('none' => t('No restriction')), node_get_types('names')),
    '#default_value' => variable_get('freelinking_nodetitle_searchcontenttype', 'none'),
    '#description' => t('Lookup by title to find a freelink will be restricted to this content type only.'),
  );

  $failover_option['create'] = t('Add a link to create content. (Without Permission: Access Denied)');
  if (module_exists('search') && user_access('search content')) {
    $failover_option['noaccesssearch'] = t('Add a link to create content. (Without Permission: Search Content)');
    $failover_option['search'] = t('Add a link to Search Content.');
  }
  else if ($search_plugin = variable_get('freelinking_search_failover', 'error') != 'error') {
    $failover_option['noaccesssearch'] =
      t('Add a link to create content. (Without Permission: %search Search Content)',
        array('%search' => ucfirst($search_plugin)));
    $failover_option['search'] = t('Add a link to %search Search Content.',
      array('%search' => ucfirst($search_plugin)));
  }


  $form['nodetitle']['freelinking_nodetitle_notfoundaction'] = array(
    '#title' => t('If a suitable content is not found'),
    '#type' => 'select',
    '#options' => $failover_option,
    '#default_value' => variable_get('freelinking_nodetitle_notfoundaction', 'create'),
    '#description' => t('What should freelinking do when content to link to is not found?'),
  );
  return $form;
} // endfunction freelinking_freelinking_settings()

// vim: tw=300 nowrap syn=php
