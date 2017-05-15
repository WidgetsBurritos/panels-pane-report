<?php
/**
 * @file
 * Drush commands for Panels Pane Report.
 */
/**
 * Implements hook_drush_help().
 */
function ppr_styleguide_drush_help($section) {
  switch ($section) {
    case 'drush:ppr-styleguide-criteria':
      return dt('Retrieves styleguide criteria list.');
    case 'drush:ppr-styleguide-report':
      return dt('Retrieves styleguide report. Generates shell error if there are failures.');
  }
}

/**
 * Implements hook_drush_command().
 */
function ppr_styleguide_drush_command() {
  $items['ppr-styleguide-criteria'] = array(
    'description' => dt('Retrieves list of all styleguide critiera.'),
    'aliases' => array('ppr-sgc'),
    'examples' => array(
      'drush ppr-styleguide-criteria' => dt('Retrieves list of all styleguide critiera.'),
    ),
  );
  $items['ppr-styleguide-report'] = array(
    'description' => dt('Retrieves styleguide statuses. Generates shell error if there are failures.'),
    'aliases' => array('ppr-sgr'),
    'examples' => array(
      'drush ppr-styleguide-report' => dt('Retrieves the styleguide report. Generates shell error if there are failures.'),
    ),
  );
  return $items;
}

/**
 * Drush callback for retrieving styleguide criteria list.
 */
function drush_ppr_styleguide_criteria() {
  $styleguide = ppr_styleguide_retrieve_styleguide();
  foreach ($styleguide as $status => $criteria) {
    drush_print(ucwords($status) . ':');
    drush_print();
    foreach ($criteria as $sgcid => $rules) {
      $sg = entity_load_single('ppr_styleguide', $sgcid);
      drush_print($sg->title . ':', 4);
      foreach ($rules as $rule) {
        drush_print(implode('', $rule), 8);
      }
      drush_print();
    }
  }
}

/**
 * Drush callback for retrieving styleguide status.
 */
function drush_ppr_styleguide_report() {
  $failing = ppr_styleguide_retrieve_rules_by_status(0);
  $failing_entities = entity_load('ppr_styleguide', array_keys($failing));
  if (empty($failing_entities)) {
    return drush_log('error', 'yo');
  }
  foreach ($failing_entities as $failing_entity) {
    drush_print(dt('✖ ') . $failing_entity->title);
  }
  return drush_set_error('PPR_STYLEGUIDE_FAILURE', dt('Styleguide validation failed'));
}
