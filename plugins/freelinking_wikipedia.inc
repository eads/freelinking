<?php
// wikipedia plugin for freelinking

// allows for a link like [[w:Matthew Good]] to be expanded to
// en.wikipedia.org/wiki/Matthew_Good

$freelinking['wikipedia'] = array(
  'indicator' => '/w(iki(pedia)?|p)?/',
  'translate' => array(' ' => '_'),
  'replacement' => 'http://' . variable_get('freelinking_wiki_languagecode', 'en') . '.wikipedia.org/wiki/%1',
  'tip' => t('Link to a wikipedia page.'),
);

function freelinking_wikipedia_settings() {
  $form['wikipedia']['freelinking_wikipedia_languagecode'] = array(
    '#title' => t('Language code'),
    '#type'  => 'textfield',
    '#default_value' => variable_get('freelinking_wiki_languagecode', 'en'),
    '#description' => t('Enter the two-character language code to use for wikipedia links'),
    '#size' => 2,
    '#maxlength' => 2,
  );
  return $form;
} // endfunction freelinking_wikipedia_settings

// vim:tw=300 nowrap syn=php
