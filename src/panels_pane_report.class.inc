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
      ->key(array(
        'entity_type' => $item_array['entity_type'],
        'pattern_storage_id' => $item_array['pattern_storage_id'],
        'page_storage_id' => $item_array['page_storage_id'],
        'display_storage_id' => $item_array['display_storage_id'],
      ))
      ->fields($item_array)
      ->execute();
  }

  /**
   * Retrieves a list of all build modes used in the database.
   *
   * @return array
   *   Contains all build modes detected during scan. Empty array if none.
   */
  public static function retrieveAllBuildModes() {
    return db_query('SELECT DISTINCT build_mode FROM {panels_pane_report}', array(), array('target' => 'slave'))->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all style modes used in the database.
   *
   * @return array
   *   Contains all style modes detected during scan. Empty array if none.
   */
  public static function retrieveAllStyleModes() {
    return db_query('SELECT DISTINCT style_mode FROM {panels_pane_report}', array(), array('target' => 'slave'))->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all display storage types used in the database.
   *
   * @return array
   *   Contains all storage types detected during scan. Empty array if none.
   */
  public static function retrieveAllDisplayStorageTypes() {
    return db_query('SELECT DISTINCT display_storage_type FROM {panels_pane_report}', array(), array('target' => 'slave'))->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all display storage layouts used in the database.
   *
   * @return array
   *   Contains all layouts detected during scan. Empty array if none.
   */
  public static function retrieveAllDisplayLayouts() {
    return db_query('SELECT DISTINCT display_layout FROM {panels_pane_report}', array(), array('target' => 'slave'))->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all entity types used in the database.
   *
   * @return array
   *   Contains all entity types detected during scan. Empty array if none.
   */
  public static function retrieveAllEntityTypes() {
    return db_query('SELECT DISTINCT entity_type FROM {panels_pane_report}', array(), array('target' => 'slave'))->fetchAllKeyed(0, 0);
  }

  /**
   * Retrieves a list of all bundles used in the database.
   *
   * @return array
   *   Contains all bundles detected during scan. Empty array if none.
   */
  public static function retrieveAllBundles() {
    return db_query('SELECT DISTINCT bundle FROM {panels_pane_report}', array(), array('target' => 'slave'))->fetchAllKeyed(0, 0);
  }

  /**
   * Generates a pane mapping array.
   */
  public static function generatePaneMapping() {
    $pane_mapping = array();

    // Determine which mini panels have been disabled.
    $disabled_mini_panels = variable_get('default_panels_mini', array());

    // Retrieve panel info from database and assemble our pane mapping.
    $panels = db_query('SELECT pp.configuration, pp.style, pp.type, pp.subtype, pd.storage_type, pd.storage_id, pd.layout FROM {panels_pane} pp LEFT JOIN {panels_display} pd ON pp.did = pd.did ORDER BY pp.configuration ASC', array(), array('target' => 'slave'))->fetchAll();

    foreach ($panels as $panel) {
      $config = unserialize($panel->configuration);
      $style = unserialize($panel->style);

      // Determine appropriate storage key, based on type.
      $storage_key = ($panel->type == 'node') ? $config['nid'] : $panel->subtype;

      // Set entity info if not already done.
      if (!isset($pane_mapping[$storage_key]['entity_info'])) {
        switch ($panel->type) {
          case 'node':
            $pane_mapping[$storage_key]['entity_info'] = db_query("SELECT 'node' AS type, title, type AS bundle, status FROM {node} WHERE nid = :nid", array(':nid' => $storage_key), array('target' => 'slave'))->fetchAssoc();
            break;

          case 'panels_mini':
            $pane_mapping[$storage_key]['entity_info'] = array(
              'type' => $panel->type,
              'title' => db_query('SELECT admin_title FROM {panels_mini} WHERE name = :name', array(':name' => $storage_key), array('target' => 'slave'))->fetchField(),
              'status' => (int) !isset($disabled_mini_panels[$storage_key]),
            );

            break;

          case 'block':
            // Since blocks don't have to be enabled to get loaded, to avoid
            // confusion, we will set all block statuses to 1.
            $pane_mapping[$storage_key]['entity_info'] = array(
              'type' => $panel->type,
              'title' => db_query('SELECT title FROM {block} WHERE delta = :delta', array(':delta' => $storage_key), array('target' => 'slave'))->fetchField(),
              'status' => NULL,
            );

          default:
            $pane_mapping[$storage_key]['entity_info'] = array(
              'type' => $panel->type,
              'title' => '',
              'status' => NULL,
            );

        }
      }

      // If entity exists but is not referenced anywhere, mark it.
      $is_mini_panel = $panel->storage_type == 'panels_mini';
      if (!isset($panel->storage_id) || $is_mini_panel && !isset($config['nid'])) {
        $panel->storage_id = self::NODE_UNREFERENCED;
      }

      // Add instance to map.
      $pane_mapping[$storage_key]['instances'][] = array(
        'panel' => $panel,
        'build_mode' => isset($config['build_mode']) ? $config['build_mode'] : '',
        'style_mode' => isset($style['style']) ? $style['style'] : '',
      );
    }

    // Sort by pattern storage id.
    ksort($pane_mapping);

    return $pane_mapping;
  }

  /**
   * Retrieves a title and status for the specified panel.
   */
  public static function retrievePageInfoByIdAndType($storage_id, $storage_type) {
    switch ($storage_type) {
      case 'node':
      case 'panels_node':
        $result = db_query("SELECT nid as storage_id, title, status FROM {node} WHERE nid = :nid", array(':nid' => $storage_id), array('target' => 'slave'))->fetchAssoc();
        if ($result) {
          return $result;
        }
        break;

      case 'panels_mini':
        // Determine which mini panels have been disabled.
        $disabled_mini_panels = variable_get('default_panels_mini', array());
        return array(
          'storage_id' => $storage_id,
          'title' => (string) db_query('SELECT admin_title FROM {panels_mini} WHERE name = :name', array(':name' => $storage_id), array('target' => 'slave'))->fetchField(),
          'status' => (int) !isset($disabled_mini_panels[$storage_id]),
        );

      case 'panelizer_entity':
        list ($entity_type, $entity_id, $view_mode) = explode(':', $storage_id);
        return self::retrievePageInfoByIdAndType($entity_id, $entity_type);

      case 'page_manager':
        $result = db_query("SELECT p.path as storage_id, p.admin_title AS title FROM {page_manager_handlers} h INNER JOIN {page_manager_pages} p ON h.subtask = p.name WHERE h.name = :name", array(':name' => $storage_id), array('target' => 'slave'))->fetchAssoc();
        if ($result) {
          // Determine which pages have been disabled.
          $disabled_page_manager_pages = variable_get('default_page_manager_pages', array());
          $result['status'] = (int) !isset($disabled_page_manager_pages[$storage_id]);
          return $result;
        }

    }

    return array(
      'storage_id' => $storage_id,
      'title' => '',
      'status' => NULL,
    );
  }

  /**
   * Retrieves page storage ids for the specified instance.
   */
  public static function retrievePageStorageIdsForInstance($instance_info, $pane_mapping) {
    $storage_id = $instance_info['panel']->storage_id;
    switch($instance_info['panel']->storage_type) {
      case 'panels_node':
      case 'panelizer_entity':
      case 'page_manager':
        return array($storage_id => $instance_info['panel']->storage_type);

      case 'panels_mini':
        // If pattern exists inside a minipanel mark all pages its included.
        if (!empty($pane_mapping[$storage_id]['instances'])) {
          $page_storage_ids = array();
          foreach ($pane_mapping[$storage_id]['instances'] as $instance) {
            $page_storage_ids[$instance['panel']->storage_id] = $instance['panel']->storage_type;
          }
          return $page_storage_ids;
        }
    }

    return array(self::NODE_UNREFERENCED => self::NODE_UNREFERENCED);
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

        foreach ($page_storage_ids as $page_storage_id => $page_storage_type) {
          $page_info = self::retrievePageInfoByIdAndType($page_storage_id, $page_storage_type);

          $item_array = array(
            'entity_type' => isset($details['entity_info']['type']) ? $details['entity_info']['type'] : self::NODE_DELETED,
            'bundle' => isset($details['entity_info']['bundle']) ? $details['entity_info']['bundle'] : '',
            'pattern_storage_id' => $pattern_storage_id,
            'pattern_status' => $details['entity_info']['status'],
            'pattern_title' => isset($details['entity_info']['title']) ? $details['entity_info']['title'] : '',
            'page_storage_id' => $page_info['storage_id'],
            'page_status' => $page_info['status'],
            'page_title' => $page_info['title'],
            'build_mode' => $instance_info['build_mode'],
            'style_mode' => $instance_info['style_mode'],
            'display_storage_type' => isset($instance_info['panel']->storage_type) ? $instance_info['panel']->storage_type : '',
            'display_storage_id' => isset($instance_info['panel']->storage_id) ? $instance_info['panel']->storage_id : 0,
            'display_layout' => isset($instance_info['panel']->layout) ? $instance_info['panel']->layout : '',
          );

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
