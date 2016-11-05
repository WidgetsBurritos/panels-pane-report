<?php
/**
 * @file
 * Class for generating a Panel Report.
 */

/**
 * Class PanelsPaneReport for building custom reports.
 */
class PanelsPaneReport {
  const NODE_DELETED = '[deleted]';
  const NODE_UNREFERENCED = 0;
  const LAST_RUN_VARIABLE = 'panels_pane_report_last_run';

  /**
   * Adds an item to the report.
   *
   * @param array $item_array
   *   Associative array containing item_type, source_id, source_path,
   *   destination_path, source_status and destination_status values.
   *
   * @throws \Exception
   *   If function fails for any reason, throws a generic exception.
   */
  public static function reportItem(array $item_array) {
    // If row already exists update it. Otherwise insert it.
    db_merge('panels_pane_report')
      ->key([
        'entity_type' => $item_array['entity_type'],
        'pattern_storage_id' => $item_array['pattern_storage_id'],
        'page_storage_id' => $item_array['page_storage_id'],
        'display_storage_id' => $item_array['display_storage_id'],
      ])
      ->fields($item_array)
      ->execute();
  }

  /**
   * Returns array of fields used for exposing data to views.
   *
   * @return array
   *   Array containing field info to expose to views. Empty array if none.
   */
  public static function retrieveViewsData() {
    $data['panels_pane_report']['table']['group'] = t('Panels Pane Report');
    $data['panels_pane_report']['table']['base'] = array(
      'title' => t('Panels Report'),
      'help' => t('Contains records we want exposed to Views.'),
    );

    // The entity type field.
    $data['panels_pane_report']['entity_type'] = array(
      'title' => t('Entity Type'),
      'help' => t('Indicates what type of entity the pattern is.'),
      'field' => array('handler' => 'views_handler_field'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'panels_pane_report_entity_type'),
    );

    // The bundle field.
    $data['panels_pane_report']['bundle'] = array(
      'title' => t('Bundle'),
      'help' => t('Indicates what bundle the pattern is.'),
      'field' => array('handler' => 'views_handler_field'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'panels_pane_report_bundle'),
    );

    // The build mode field.
    $data['panels_pane_report']['build_mode'] = array(
      'title' => t('Build Mode'),
      'help' => t('Indicates what build mode the node is in.'),
      'field' => array('handler' => 'views_handler_field'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'panels_pane_report_build_mode'),
    );

    // The pattern storage id field.
    $data['panels_pane_report']['pattern_storage_id'] = array(
      'title' => t('Pattern Path'),
      'help' => t('Storage ID of the Pattern.'),
      'field' => array('handler' => 'panels_pane_report_pattern_storage_id_field'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'views_handler_filter_numeric'),
    );

    // The page path field.
    $data['panels_pane_report']['page_storage_id'] = array(
      'title' => t('Panel Page Path'),
      'help' => t('URL of the Pattern.'),
      'field' => array('handler' => 'panels_pane_report_page_storage_id_field'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'views_handler_filter_numeric'),
    );

    // The pattern node status field.
    $data['panels_pane_report']['pattern_status'] = array(
      'title' => t('Pattern Status'),
      'help' => t('Status of the Pattern.'),
      'field' => array('handler' => 'views_handler_field_boolean'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'views_handler_filter_boolean_operator'),
    );

    // The patter node status field.
    $data['panels_pane_report']['page_status'] = array(
      'title' => t('Page Status'),
      'help' => t('Status of the Page.'),
      'field' => array('handler' => 'views_handler_field_boolean'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'views_handler_filter_boolean_operator'),
    );

    // The display storage type field.
    $data['panels_pane_report']['display_storage_type'] = array(
      'title' => t('Display Storage Type'),
      'help' => t('Indicates what storage type of the display is.'),
      'field' => array('handler' => 'views_handler_field'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'panels_pane_report_display_storage_type'),
    );

    // The display storage type field.
    $data['panels_pane_report']['display_storage_id'] = array(
      'title' => t('Display Storage ID'),
      'help' => t('Indicates what storage id of the display is.'),
      'field' => array('handler' => 'views_handler_field'),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'panels_pane_report_display_storage_id'),
    );

    return $data;
  }

  /**
   * Retrieves a list of all build modes used in the database.
   *
   * @return array
   *   Contains all build modes detected during scan. Empty array if none.
   */
  public static function retrieveAllBuildModes() {
    return db_query('SELECT DISTINCT build_mode FROM {panels_pane_report}', [], ['target' => 'slave'])->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all display storage types used in the database.
   *
   * @return array
   *   Contains all storage types detected during scan. Empty array if none.
   */
  public static function retrieveAllDisplayStorageTypes() {
    return db_query('SELECT DISTINCT display_storage_type FROM {panels_pane_report}', [], ['target' => 'slave'])->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all entity types used in the database.
   *
   * @return array
   *   Contains all entity types detected during scan. Empty array if none.
   */
  public static function retrieveAllEntityTypes() {
    return db_query('SELECT DISTINCT entity_type FROM {panels_pane_report}', [], ['target' => 'slave'])->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all bundles used in the database.
   *
   * @return array
   *   Contains all bundles detected during scan. Empty array if none.
   */
  public static function retrieveAllBundles() {
    return db_query('SELECT DISTINCT bundle FROM {panels_pane_report}', [], ['target' => 'slave'])->fetchAllKeyed(0, 0);
  }

  /**
   * Generates a pane mapping array.
   */
  public static function generatePaneMapping() {
    // Determine which mini panels have been disabled.
    $disabled_mini_panels = variable_get('default_panels_mini', []);

    // Retrieve panel info from database and assemble our pane mapping.
    $panels = db_query('SELECT pp.configuration, pp.type, pp.subtype, pd.storage_type, pd.storage_id FROM {panels_pane} pp LEFT JOIN {panels_display} pd ON pp.did = pd.did ORDER BY pp.configuration ASC', [], ['target' => 'slave'])->fetchAll();

    foreach ($panels as $panel) {
      $config = unserialize($panel->configuration);

      // Determine appropriate storage key, based on type.
      $storage_key = ($panel->type == 'node') ? $config['nid'] : $panel->subtype;

      // Set entity info if not already done.
      if (!isset($pane_mapping[$storage_key]['entity_info'])) {
        switch ($panel->type) {
          case "node";
            $pane_mapping[$storage_key]['entity_info'] = db_query("SELECT 'node' AS type, type AS bundle, status FROM {node} WHERE nid = :nid", [':nid' => $storage_key], ['target' => 'slave'])->fetchAssoc();
            break;

          case "panels_mini":

            $pane_mapping[$storage_key]['entity_info'] = [
              'type' => $panel->type,
              'status' => (int) !isset($disabled_mini_panels[$storage_key]),
            ];

            break;

          case "block":
          default:
            // Since blocks don't have to be enabled to get loaded, to avoid
            // confusion, we will set all block statuses to 1.
            $pane_mapping[$storage_key]['entity_info'] = [
              'type' => $panel->type,
              'status' => 1,
            ];
        }
      }

      // If entity exists but is not referenced anywhere, mark it.
      if (!isset($panel->storage_id)) {
        $panel->storage_id = self::NODE_UNREFERENCED;
        $page_status = NODE_NOT_PUBLISHED;
      }
      elseif ($panel->storage_type == 'panels_mini') {
        $page_status = db_query('SELECT status FROM {node} WHERE nid = :nid', [':nid' => $config['nid']], ['target' => 'slave'])->fetchField();
      }
      else {
        $page_status = db_query('SELECT status FROM {node} WHERE nid = :nid', [':nid' => $panel->storage_id], ['target' => 'slave'])->fetchField();
      }

      // Add instance to map.
      $pane_mapping[$storage_key]['instances'][] = [
        'panel' => $panel,
        'build_mode' => isset($config['build_mode']) ? $config['build_mode'] : '',
        'page_status' => $page_status,
      ];
    }

    // Sort by pattern storage id.
    ksort($pane_mapping);

    return $pane_mapping;
  }

  /**
   * Retrieves page storage ids for the specified instance.
   */
  public static function retrievePageStorageIdsForInstance($instance_info, $pane_mapping) {
    $storage_id = $instance_info['panel']->storage_id;
    if (is_numeric($storage_id)) {
      $page_storage_ids = [$storage_id];
    }
    elseif (!empty($pane_mapping[$storage_id]['instances'])) {
      // If pattern exists inside a minipanel mark all pages its included.
      $page_storage_ids = array_map(function ($instance) {
        return $instance['panel']->storage_id;
      }, $pane_mapping[$storage_id]['instances']);
    }
    else {
      // If pattern exists inside mini panel that is unreferenced.
      $page_storage_ids = [self::NODE_UNREFERENCED];
    }
    return $page_storage_ids;
  }

  /**
   * Runs the full report.
   */
  public static function runReport() {
    // Wipe the table so we're dealing with a fresh report.
    db_truncate('panels_pane_report')->execute();

    // Retrieve pane mapping.
    $pane_mapping = self::generatePaneMapping();

    // Generate report.
    foreach ($pane_mapping as $pattern_storage_id => $details) {
      foreach ($details['instances'] as $instance_info) {
        $page_storage_ids = self::retrievePageStorageIdsForInstance($instance_info, $pane_mapping);

        foreach ($page_storage_ids as $page_storage_id) {
          $item_array = [
            'entity_type' => isset($details['entity_info']['type']) ? $details['entity_info']['type'] : self::NODE_DELETED,
            'bundle' => isset($details['entity_info']['bundle']) ? $details['entity_info']['bundle'] : '',
            'pattern_storage_id' => $pattern_storage_id,
            'pattern_status' => (int) $details['entity_info']['status'],
            'page_storage_id' => $page_storage_id,
            'page_status' => (int) $instance_info['page_status'],
            'build_mode' => $instance_info['build_mode'],
            'display_storage_type' => isset($instance_info['panel']->storage_type) ? $instance_info['panel']->storage_type : '',
            'display_storage_id' => isset($instance_info['panel']->storage_id) ? $instance_info['panel']->storage_id : 0,
          ];

          self::reportItem($item_array);
        }
      }

      // Anything that's not a mini panel is no longer needs.
      if ($details['entity_info']['type'] !== 'panels_mini') {
        unset($pane_mapping[$pattern_storage_id]);
      }
    }

    variable_set(self::LAST_RUN_VARIABLE, REQUEST_TIME);
  }

}
