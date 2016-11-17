<?php
/**
 * @file
 * Custom views handlers/fields for Rackspace Reports.
 *
 * NOTE: File intentionally excluded from phpcs lint check, since we're overriding
 * classes and methods that don't follow camel case requirements.
 */

/**
 * Filters for item type fields.
 */
class panels_pane_report_entity_type extends views_handler_filter_in_operator {
  /**
   * Override get_value_options().
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_options = PanelsPaneReport::retrieveAllEntityTypes();
  }
}

/**
 * Filters for item type fields.
 */
class panels_pane_report_bundle extends views_handler_filter_in_operator {
  /**
   * Override get_value_options().
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_options = PanelsPaneReport::retrieveAllBundles();
  }
}

/**
 * Filters for item type fields.
 */
class panels_pane_report_build_mode extends views_handler_filter_in_operator {
  /**
   * Override get_value_options().
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = PanelsPaneReport::retrieveAllBuildModes();
  }
}

/**
 * Filters for item type fields.
 */
class panels_pane_report_style_mode extends views_handler_filter_in_operator {
  /**
   * Override get_value_options().
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = PanelsPaneReport::retrieveAllStyleModes();
  }
}

/**
 * Filters for item type fields.
 */
class panels_pane_report_display_storage_type extends views_handler_filter_in_operator {
  /**
   * Override get_value_options().
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = PanelsPaneReport::retrieveAllDisplayStorageTypes();
  }
}

/**
 * Render options for pattern nid fields.
 */
class panels_pane_report_pattern_storage_id_field extends views_handler_field {
  /**
   * Override render().
   *
   * @param $values
   * @return null|string|void
   */
  function render($values) {
    $storage_id = $values->panels_pane_report_pattern_storage_id;
    if ((string) $storage_id !== (string) PanelsPaneReport::NODE_UNREFERENCED) {
      $options = array(
        'query' => drupal_get_destination(),
        'attributes' => array('target' => '_blank'),
      );

      switch ($values->panels_pane_report_entity_type) {
        case "node":
          $path = 'node/' . $storage_id;
          return l(drupal_get_path_alias($path), $path, $options);

        case "block":
          $path = 'admin/structure/block/manage/panels_mini/' . $storage_id;
          return l($storage_id, $path, $options);

        case "panels_mini":
          $path = 'admin/structure/mini-panels/list/' . $storage_id;
          return l($storage_id, $path, $options);

      }
    }

    return t('n/a');
  }
}

/**
 * Render options for page nid fields.
 */
class panels_pane_report_page_storage_id_field extends views_handler_field {
  /**
   * Override render().
   *
   * @param $values
   * @return null|string|void
   */
  function render($values) {
    if ($values->panels_pane_report_page_storage_id == PanelsPaneReport::NODE_UNREFERENCED) {
      return t('No reference');
    }
    else {
      $node_url = 'node/' . $values->panels_pane_report_page_storage_id;

      return l(drupal_get_path_alias($node_url), $node_url, array(
        'query' => drupal_get_destination(),
        'attributes' => array('target' => '_blank'),
      ));
    }
  }
}
