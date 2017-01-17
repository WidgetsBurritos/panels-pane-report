<?php

/**
 * @file
 * Admin file for the Panels Pane Report module.
 */

/**
 * Admin configuration page.
 */
function panels_pane_report_admin() {
  $form = array();

  $period = drupal_map_assoc(array(
    900, 1800, 3600, 7200, 10800, 21600, 32400, 43200, 64800, 86400,
    172800, 259200, 604800, 1209600, 2419200,
  ), 'format_interval');
  $period[0] = t('Never');

  $form['panels_pane_report_cron_interval'] = array(
    '#type' => 'select',
    '#title' => t('Cron Interval (in seconds)'),
    '#default_value' => variable_get('panels_pane_report_cron_interval', PANELS_PANE_REPORT_DEFAULT_CRON_INTERVAL),
    '#options' => $period,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
