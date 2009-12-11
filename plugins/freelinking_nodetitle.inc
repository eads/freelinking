<?php
// $Id$
/**
 * Nodetitle plugin for Freelinking
 *
 * @file
 * Allows for a link like [[nodetitle:Freelinking filter]] to be expanded to
 * a link to the node titled "Freelinking filter" or a link to create the node.
 */

$freelinking['nodetitle'] = array(
  'indicator' => '/nt|nodetitle|title/',
  'callback' => 'freelinking_nodetitle_callback',
  'tip' => t('Link to a local node by title'),
  'run on view' => TRUE,
);

/**
 * Replacement callback for nodetitle plugin
 */
function freelinking_nodetitle_callback($target, $plugin) {
  // get the nid and best guess at language for the title.
  list($nid, $language) =
    _freelinking_nodetitle_get_nid_by_lang($target);

  // if no node was found, identify proceed with configured failover
  if (!$nid) {
    return _freelinking_nodetitle_failure($target);
  }

  // construct values for link
  $title = $target['text'] ? $target['text'] : $target['dest'];

  if (!$target['tooltip']) {
    $target['tooltip'] = freelinking_internal_tooltip('node', $nid);
  }
  // return link structure
  return array($title, 'node/' . $nid, array(
    'attributes' => array('title' => $tooltip),
    'language' => $language,
  ));
}

/**
 * Nodetitle Settings Callback
 */
function freelinking_nodetitle_settings() {
  // Restrict nodetitle plugin to search specified content type
  $form['freelinking_nodetitle_searchcontenttype'] = array(
    '#title' => t('Restrict freelinks to this content type'),
    '#type'  => 'select',
    '#options' => array_merge(array('none' => t('No restriction')), node_get_types('names')),
    '#default_value' => variable_get('freelinking_nodetitle_searchcontenttype', 'none'),
    '#description' => t('Lookup by title to find a freelink will be restricted to this content type only.'),
  );

  $failover_option['none'] = t('Do nothing');

  // if Create Node plugin is available, it's an option!
  if (module_exists('freelinking_prepopulate')) {
    $failover_option['create'] = t('Add a link to create content. (Without Permission: Access Denied)');
  }
  else {
    $extra_description ='<br /><strong>'
      . t('Note: Enable %module for Content Creation option.', array('%module' => 'Freelinking Create'))
      . '</strong>';
  }

  // if search is available, have a search failover
  if (module_exists('search')) {
    $failover_option['search'] = t('Add a link to Search Content');
  }
  else {
    $extra_description .= '<br /><strong>'
      . t('Note: Enable the %module module for internal search option.', array('%module' => 'Search'))
      . '</strong>';
    // if search is unavailable offer it's own (applicable)search fallback. [Google, etc]
    if (($search_plugin = variable_get('freelinking_search_failover', 'error')) != 'error') {
      $failover_option['search'] = t('Add a link to %search Search Content.',
        array('%search' => drupal_ucfirst($search_plugin)));
    }
  }
  $failover_option['error'] = t('Insert an error message');

  $form['freelinking_nodetitle_failover'] = array(
    '#title' => t('If a suitable content is not found'),
    '#type' => 'select',
    '#options' => $failover_option,
    '#default_value' => variable_get('freelinking_nodetitle_failover',
      _freelinking_nodetitle_default_failover()),
    '#description' => t('What should freelinking do when the page is not found?')
      . $extra_description,
  );
  return $form;
} // endfunction freelinking_freelinking_settings()

/**
 * Grab the nid associated with the title.
 * Attempt some degree of language sensibility
 */
function _freelinking_nodetitle_get_nid_by_lang($target) {
  $title = $target['dest'];
  if ($target['type']) {
    $type = check_plain($target['type']);
  }
  else {
    $type = variable_get('freelinking_nodetitle_searchcontenttype', 'none');
  }

  // Run a language-sensitive query
  if (module_exists('locale')) {
    $language = default_language('language');
    // Identify and remove language argument (de:Pagename)
    if (preg_match('/^(\w\w):/', $target['dest'], $matches)) {
      $language = $matches[1];
      $target['dest'] = drupal_substr($target['dest'], 3);
    }

    $sql = "SELECT nid, language FROM {node} WHERE title = '%s' and language = '%s'";
    if ($type && $type != 'none') {
      $sql .= " AND type = '%s'";
      $result = db_query($sql, $title, $language, $type);
    }
    else {
      $result = db_query($sql, $title, $language);
    }
  }
  // if locale does not exist or it provided an empty result, get anything.
  if (empty($result)) {
    $sql = "SELECT nid FROM {node} WHERE title = '%s'";
    if ($type && $type != 'none') {
      $sql .= " AND type = '%s'";
      $result = db_query($sql, $title, $type);
    }
    else {
      $result = db_query($sql, $title);
    }
  }
  $node = db_fetch_object($result);
  // get the nid and it's language
  if (isset($node)) {
    return array($node->nid, $node->language);
  }

  return;
}

/**
 * Determining the proper failure response per plugin configuration
 */
function _freelinking_nodetitle_failure($target) {
  $failover = variable_get('freelinking_nodetitle_failover',
    _freelinking_nodetitle_default_failover());

  switch ($failover) {
    case 'create':
      return array('failover' => 'createnode');
    case 'search':
      return array('failover' => 'search');
    case 'error':
      return array('failover' => 'error', 'message' => t('%title not found',
        array('%title' => $target['dest'])));
  }
  // do nothing
  return FALSE;
}

/**
 * Find the best default failover
 */
function _freelinking_nodetitle_default_failover() {
  if (module_exists('freelinking_create')) {
    return 'create node';
  }
  elseif (module_exists('search')) {
    return 'search';
  }
  return 'none';
}

// vim: tw=300 nowrap syn=php
