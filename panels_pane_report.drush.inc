<?php
/**
 * @file
 * Drush commands for Panels Pane Report.
 */
/**
 * Implements hook_drush_help().
 */
function panels_pane_report_drush_help($section) {
  switch ($section) {
    case 'drush:ppr-generate':
      return dt('Generates the panels pane report.');
    case 'drush:ppr-retrieve':
      return dt('Retrieves the panels pane report.');
  }
}

/**
 * Implements hook_drush_command().
 */
function panels_pane_report_drush_command() {
  $items['ppr-generate'] = array(
    'description' => dt('Generates the Panels Pane Report.'),
    'aliases' => array('ppr-g'),
    'examples' => array(
      'drush ppr-generate' => dt('Generates the Panels Pane Report.'),
    ),
  );
  $items['ppr-retrieve'] = array(
    'description' => dt('Retrieves the Panels Pane Report in the specified format.'),
    'aliases' => array('ppr-r'),
    'examples' => array(
      'drush ppr-retrieve [json]' => dt('Retrieves the Panels Pane Report as JSON.'),
      'drush ppr-retrieve serialized' => dt('Retrieves the Panels Pane Report as Serialized PHP.'),
    ),
  );
  return $items;
}

/**
 * Drush callback for generating the panels pane report.
 */
function drush_panels_pane_report_ppr_generate() {
  PanelsPaneReport::runReport();
}

/**
 * Drush callback for displaying the panels pane report results.
 */
function drush_panels_pane_report_ppr_retrieve($export_type = 'json') {
  $rows = db_select('panels_pane_report', 'ppr')
    ->fields('ppr')
    ->execute()
    ->fetchAll();
  switch($export_type) {
    case "serialized":
      drush_print(serialize($rows));
    case "json":
    default:
      drush_print(json_encode($rows));
  }

}
