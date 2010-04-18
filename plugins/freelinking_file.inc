<?php
// $Id$

$freelinking['file'] = array(
  'indicator' => '/^file$/i',
  'callback' => 'freelinking_file_file_callback',
  'settings' => 'freelinking_file_file_settings',
);
$freelinking['image'] = array(
  'indicator' => '/^image$/i',
  'callback' => 'freelinking_file_image_callback',
  'settings' => 'freelinking_file_image_settings',
);

function freelinking_file_file_callback($target, $plugin) {
  // Is relative path?
  $pos = strpos($target['dest'], '/'); 
  if ($pos === FALSE || $pos > 0) {
    if ($target['other']['file_path']) {
      $path = $target['other']['file_path'];
    }
    else {
     $path = variable_get('freelinking_file_file_path', file_directory_path());
    }
    $path .= '/' . $target['dest'];
  }
  elseif ($pos == 0) {
    // scrape off the starting slash that marks this as an unmodified path from site root.
    $path = substr($target['dest'], 1);
  }

  $check_path = $path;
  if (!file_check_path($check_path)) {
    return array('failover' => 'error',
      'message' => t('File Not Found'));
  }

  if ($target['text']) {  
    $title = $target['text'];
  }
  else {
    $title = basename($path);
  }

  return array($title, file_create_url($path));
}

function freelinking_file_image_callback($target, $plugin) {
  $file = freelinking_file_file_callback($target, $plugin);
  if ($file['failover']) {
    return $file;
  }
  return $file;
}

function freelinking_file_file_settings() {
  $path = file_directory_path();
  $form['freelinking_file_file_path'] = array(
    '#title' => t('File Basepath'),
    '#type' => 'textfield',
    '#default_value' => variable_get('freelinking_file_file_basepath', $path),
    '#description' => t('Set the base path for files. Files should usually begin with !path',
      array('!path' => $path)),
  );
  return $form;
}

function freelinking_file_image_settings() {
  $path = file_directory_path();
  $form['freelinking_file_image_path'] = array(
    '#title' => t('Image Basepath'),
    '#type' => 'textfield',
    '#default_value' => variable_get('freelinking_file_image_basepath', $path),
    '#description' => t('Set the base path for images. Images should usually begin with !path',
      array('!path' => $path)),
  );
  return $form;
}
