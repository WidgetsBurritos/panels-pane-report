<?php

/**
 * @file
 * Custom views handlers/fields for Panels Pane Reports.
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
 * Filters for item type fields.
 */
class panels_pane_report_display_layout extends views_handler_filter_in_operator {

  /**
   * Override get_value_options().
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = PanelsPaneReport::retrieveAllDisplayLayouts();
  }

}

/**
 * Render options for pattern nid fields.
 */
class panels_pane_report_pattern_storage_id_field extends views_handler_field {

  /**
   * Override render().
   */
  function render($values) {
    $storage_id = $values->panels_pane_report_pattern_storage_id;
    if ((string) $storage_id !== (string) PanelsPaneReport::NODE_UNREFERENCED) {
      $options = array(
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
class panels_pane_report_pattern_status_field extends views_handler_field_boolean {

  /**
   * Override render().
   */
  function render($values) {
    if ($values->panels_pane_report_pattern_status === NULL) {
      return '';
    }
    return parent::render($values);
  }

}

/**
 * Render options for page nid fields.
 */
class panels_pane_report_page_status_field extends views_handler_field_boolean {

  /**
   * Override render().
   */
  function render($values) {
    if ($values->panels_pane_report_page_status === NULL) {
      return '';
    }
    return parent::render($values);
  }

}

/**
 * Render options for page nid fields.
 */
class panels_pane_report_page_storage_id_field extends views_handler_field {

  /**
   * Override render().
   */
  function render($values) {
    if ($values->panels_pane_report_page_storage_id === (string) PanelsPaneReport::NODE_UNREFERENCED) {
      return t('No reference');
    }
    $link_attributes = array('attributes' => array('target' => '_blank'));
    if ($values->panels_pane_report_display_storage_type == 'page_manager') {
      return l($values->panels_pane_report_page_storage_id, $values->panels_pane_report_page_storage_id, $link_attributes);
    }
    else {
      $node_url = 'node/' . $values->panels_pane_report_page_storage_id;
      return l(drupal_get_path_alias($node_url), $node_url, $link_attributes);
    }
  }

}

/**
 * Filter for true/false/null fields.
 */
class panels_pane_report_views_handler_filter_true_false_null_operator extends views_handler_filter_boolean_operator {

  /**
   * Override get_value_options().
   */
  function get_value_options() {
    $this->value_options = array(
      '1' => t('✔ True'),
      '0' => t('✖ False'),
      '-1' => t('∅ Null'),
    );
  }

  /**
   * Override query().
   */
  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    if ($this->value === '1') {
      $this->query->add_where($this->options['group'], $field, 1, '=');
    }
    elseif ($this->value === '0') {
      $this->query->add_where($this->options['group'], $field, 0, '=');
    }
    elseif ($this->value === '-1') {
      $this->query->add_where($this->options['group'], $field, NULL, 'IS NULL');
    }
  }

}
