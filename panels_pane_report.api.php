<?php

/**
 * @file
 * Hooks related to panels panel report.
 */

/**
 * Modifies a Panels Pane Report item array right before added to the database.
 *
 * @param array $item_array
 *   Item array element.
 */
function hook_panels_pane_report_report_item_alter(array &$item_array) {
  $item_array['some_field'] = 'some_value';
}
