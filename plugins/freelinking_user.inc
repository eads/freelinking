<?php
/**
 * Freelinking User Plugins
 *
 * @file
 *   Provides automatic link creation to user profile pages.
 */

$freelinking['user'] = array(
  'indicator' => '/(u|user|username|uid|userid)$/',
  'tip' => t('Link to a user profile by name or uid.'),
  'callback' => 'freelinking_user_callback',
  'failover' => array('search'),
);

function freelinking_user_callback($target, $plugin) {
  if (is_numeric($target['dest'])) {
    $user = user_load($target['dest']);
  }
  else {
    $user = user_load(array('name' => $target['dest']));
  }

  if (!$user) {
    return array('failover' => variable_get('freelinking_user_failover', 'error'),
      'message' => t('User "%user" not found', array('%user' => $target['dest']))
    );
  }

  $title = $target['text'] ? $target['text'] : $user->name;
  $url = 'user/' . $user->uid;
  $tooltip = $target['tooltip'] ? $target['tooltip'] : t('View user profile.');

  return array($title, $url, array(
    'attributes' => array(
      'title' => $tooltip)
    )
  );
}