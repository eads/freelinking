<?php
// $Id$
/**
 * Freelinking 3 API
 *
 * @file
 *   API for Freelinking 3. These functions are subject to change without 
 * warning for all Alpha releases.
 */

/**
 * Process the target text into a link with the specified plugin.
 *
 * @param $plugin
 *   The name of the plugin to be used to render the link.
 *
 * @param $target
 *   The target text extracted from the freelink. Arguments separated by |'s.
 *
 * @param $format
 *   The format id currently invoking the Freelinking filter. Might be used in
 * the future to tweak plugin behavior.
 *
 * @param $rendered
 *   Boolean value. If true, link will be returned as rendered HTML. If false,
 * link will be returned as arguments for l().
 *
 * @return
 *   An array as per the arguments of l() or a string of HTML markup.
 *
 * @see l()
 */
function freelinking_get_freelink($plugin, $target, $format = NULL, $rendered = TRUE) {
  $target = freelinking_parse_target($target);
  $plugins = freelinking_get_plugins($format);
  $link = _freelinking_build_freelink($plugins, $plugin, $target);

  if (!$rendered || !is_array($link)) {
    return $link;
  }
  if ($link['error']) {
    return theme('freelink_error', $plugin_name, $link['error']);
  }
  return theme('freelink', $plugin_name, $link);
}

/**
 * Invoke hook_freelinking() to validate & sort available FL plugins.
 * 
 * This function is necessary to build any sort of interface involving plugin 
 * configuration. It is not the function to use in the creation of new 
 * plugins or overriding existing plugins. For that you want hook_freelinking().
 *
 * Internal Note: Currently the plugins are generated for each plugin on 
 * the idea that different formats might have a number of configuration 
 * overrides. If this turns out to not be the case a single cached version of 
 * the plugins might be better.
 *
 * @param $format
 *   The Text Format ID of the currently processed piece of text. Allows 
 * format-specific plugin overrides such as disabling certain plugins in a given 
 * format. The default 'all' refers to the plugins as provided by hook_freelinking.
 * 
 * @return
 *   An array of all plugins. Each plugin is itself an array.
 * 
 * @see PLUGIN.txt
 */
function freelinking_get_plugins($format = 'all') {
  static $plugins;
  if ($plugins[$format]) {
    return $plugins[$format];
  }
  $freelinking = module_invoke_all('freelinking');

  // Validate & standardize plugins.
  foreach ($freelinking as $name => &$plugin) {
    // Confirm correct structure in plugin
    if (!$plugin['indicator']
      || (!$plugin['replacement'] && !$plugin['callback'])
      || ($plugin['callback'] && !function_exists($plugin['callback']))) {
        drupal_set_message(t('Freelinking plugin "!plugin" is invalid.',
          array('!plugin' => $name)), 'warning');
        watchdog('filter', 'Freelinking plugin "!plugin" is invalid.',
          array('!plugin' => $name), WATCHDOG_WARNING);
    } // end if
    
    // Set "enabled" by format when explicitly set in format configuration..
    $plugin_enabled = variable_get(
      'freelinking_' . $name . '_enabled_format_' . $format, ''
    );
    if ($plugin_enabled) {
      $plugin['enabled'] = $plugin_enabled;
    }
    
    // Rearrange weight scheme to use core comparison function.
    $plugin['#weight'] = $plugin['weight'];
    unset($plugin['weight']);

    // Set explicit defaults.
    $plugin += array('enabled' => TRUE, 'html' => TRUE);
  } // end foreach

  // element_sort() uses '#weight', the hash is added above to support this.
  uasort($freelinking, 'element_sort');

  $plugins[$format] = $freelinking;
  return $freelinking;
} // end freelinking_get_plugins()
