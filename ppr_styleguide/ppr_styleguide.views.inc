<?php

/**
 * @file
 * Panels Pane Report Styleguide views info.
 */

/**
 * Implements hook_views_data().
 */
function ppr_styleguide_views_data_alter(&$data) {
  // The styleguide status field.
  $data['panels_pane_report']['styleguide_status'] = array(
    'title' => t('Styleguide Status'),
    'help' => t('Status of the styleguide validation.'),
    'field' => array('handler' => 'PanelsPaneReportStyleguideStatusField'),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'PanelsPaneReportStyleguideFilter'),
  );

  // The styleguide details field.
  $data['panels_pane_report']['styleguide_details'] = array(
    'title' => t('Styleguide Details'),
    'help' => t('Details about the styleguide validation.'),
    'field' => array('handler' => 'views_handler_field'),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_string'),
  );

  return $data;
}

/**
 * Implements hook_views_default_views_alter().
 */
function ppr_styleguide_views_default_views_alter(&$views) {
  /* Field: Panels Pane Report: Styleguide*/
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_details']['id'] = 'styleguide_details';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_details']['table'] = 'panels_pane_report';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_details']['field'] = 'styleguide_details';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_details']['label'] = '';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_details']['exclude'] = TRUE;
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_details']['element_label_colon'] = FALSE;
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_details']['hide_alter_empty'] = FALSE;
  /* Field: Panels Pane Report: Styleguide Status */
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['id'] = 'styleguide_status';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['table'] = 'panels_pane_report';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['field'] = 'styleguide_status';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['label'] = 'Styleguide';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['element_label_colon'] = FALSE;
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['element_default_classes'] = FALSE;
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['type'] = 'unicode-yes-no';
  $views['panels_pane_report']->display['default']->display_options['fields']['styleguide_status']['not'] = 0;

  /* Filter criterion: Panels Pane Report: Styleguide Status */
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['id'] = 'styleguide_status';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['table'] = 'panels_pane_report';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['field'] = 'styleguide_status';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['value'] = 'All';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['exposed'] = TRUE;
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['expose']['operator_id'] = '';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['expose']['label'] = 'Styleguide';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['expose']['operator'] = 'styleguide_status_op';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['expose']['identifier'] = 'styleguide_status';
  $views['panels_pane_report']->display['default']->display_options['filters']['styleguide_status']['expose']['remember_roles'] = array();
}

/**
 * Render options for styleguide status field.
 */
class PanelsPaneReportStyleguideStatusField extends views_handler_field_boolean {

  /**
   * Override render().
   */
  public function render($values) {
    if ($values->panels_pane_report_styleguide_status === NULL || !isset($values->panels_pane_report_styleguide_details)) {
      return '<div class="ppr-styleguide-neutral"></div>';
    }

    $styleguide_class = ($values->panels_pane_report_styleguide_status) ? 'ppr-styleguide-pass' : 'ppr-styleguide-fail';
    $details = unserialize($values->panels_pane_report_styleguide_details);
    $filter_status = isset($this->view->exposed_input['styleguide_status']) ? $this->view->exposed_input['styleguide_status'] : 0;
    $html = '<div class="' . $styleguide_class . '">';

    if (!empty($details)) {
      foreach ($details as $sgid => $success) {
        if ($filter_status === '0' && $success) {
          continue;
        }

        $entity = entity_load_single('ppr_styleguide', $sgid);
        $status = ($success) ? '✔' : '✖';
        $html .= "{$status} {$entity->title}<br>";
      }
    }
    $html .= '</div>';
    return $html;
  }

}

/**
 * Filter styleguide options.
 */
class PanelsPaneReportStyleguideFilter extends panels_pane_report_views_handler_filter_true_false_null_operator {

  /**
   * Override get_value_options().
   */
  function get_value_options() {
    parent::get_value_options();
    $results = db_query('SELECT DISTINCT title, sgid, allowed FROM {ppr_styleguide}', array(), array('target' => 'slave'))->fetchAll();
    if (!empty($results)) {
      $this->value_options[NULL] = '-------------';
      foreach ($results as $row) {
        $idx = "sg:{$row->sgid}";
        $status = ($row->allowed) ? '✔' : '✖';
        $this->value_options[$idx] = "{$status} {$row->title}";
      }
    }
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
    elseif (preg_match('/^sg:(\d+)$/', $this->value, $matches)) {
      $like_str = ppr_styleguide_assemble_like_str($matches[1]);
      $this->query->add_where($this->options['group'], '{styleguide_details}', $like_str, 'LIKE');
    }
  }

}
