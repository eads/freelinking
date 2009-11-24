# Freelinking for Case Tracker
$Id$

This module provides a Freelinking 3 plugin for the [Case Tracker] 
(http:drupal.org/project/casetracker) module. Freelinks should be of the form 
`case:<nid>`. Optionally, you might use `case:#<nid>`. As default, that would 
be [[#77]].

They are transformed as follows:

## Plugin Output
Let's follow the output of two freelinks that use this plugin, a model and an 
issue from my tracker.

  [[case:NNN]]
  [[case:291]]

### The Link Text
  #NNN: Issue Title
  #291: Styled Links for Case Tracker

### The URL (href)
  node/NNN
  node/291

### The CSS Classes
  freelink freelink-casetracker casetracker-project-<pid>
    casetracker-status-<label> casetracker-type-<label> casetracker-priority-<label> 

  freelink freelink-casetracker casetracker-project-28
    casetracker-status-resolved casetracker-type-feature-request casetracker-priority-normal 

### The Title/Tooltip
   #NNN: Status(<label>) - Type(<label>) - Priority(<label>)
   #291: Status(open) - Type(feature-request) - Priority(normal)

## Link Styling
The classes attached to each link make it possible to style it in a variety of 
ways so users can see an quick issue summary at a glance.

If you check off "Wrap links in <span>" in your Freelinking settings, each 
link will also be wrapped in a span tag with the same set of classes 
described above. This is useful for advanced link styling, such as adding 
icons before the link.

Freelinking for Casetracker comes packaged with a stylesheet that 
demonstrates some basic styling options for a default casetracker 
installation. It applies standard styling patterns seen on Drupal.org and 
elsewhere:

  1. An icon for issue Type, 
  2. Background color for Status,
  3. Text color or style for Priority. 

If you do not want this styling included, just go to 
admin/settings/freelinking and check on the casetracker setting "Ignore 
Casetracker Link Theming". This will stop including the stylesheet.

#### Theming Links by Project
If you would like to theme links based on project-id (the nid of the referenced 
project), you may do that. This option was included to provide support for a 
"featured project" having specially styled links.

## Tailoring Links to Suit You
Using hook_freelink_alter(), you may tailor your case tracker links even further.

All the casetracker-specific information about the case may be accessed via
links['extra'] as follows:

  Array(
    'pid' => <project id>
    'status' => <issue status> 
    'type' => <issue type>
    'priority' => <issue priority>
  )

To transform your casetracker links, such as by inserting the project title into 
the tooltip, you would implement something like the following:

  function custom_module_freelink_alter(&$link, $target, $plugin_name, $plugin) {
    switch($plugin_name) {
      case 'casetracker':
        $project = node_load($link['extra']['pid']);
        $link[2]['attributes']['title'] .= '- Project(' . $node->title . ')';
        break;
    }
  }

## Links
http://drupal.org/project/freelinking
http://drupal.org/project/casetracker
http://drupal.org/project/freelinking_casetracker

## Maintainers
Grayside (http://drupal.org/user/346868) {Original Creator}
