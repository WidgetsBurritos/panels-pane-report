<?php

/**
 * @file
 * Contains views-specific code for the Panels Pane Report module.
 */

/**
 * Implements hook_views_pre_render().
 */
function panels_pane_report_views_pre_render(&$view) {
  if ($view->name === 'panels_pane_report') {
    $last_run = variable_get(PanelsPaneReport::LAST_RUN_VARIABLE, -1);
    $last_run_pretty = ($last_run >= 0) ? t('!ago ago', array('!ago' => format_interval(REQUEST_TIME - $last_run))) : t('never');
    $last_run_string = t('<strong>Report last ran: </strong> !last_run', array('!last_run' => $last_run_pretty));
    $view->header['result']->options['content'] = $last_run_string . '<br>' . $view->header['result']->options['content'];
  }
}

/**
 * Implements hook_views_data().
 */
function panels_pane_report_views_data() {
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

  // The build mode field.
  $data['panels_pane_report']['style_mode'] = array(
    'title' => t('Style Mode'),
    'help' => t('Indicates what style mode the node is rendered with.'),
    'field' => array('handler' => 'views_handler_field'),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'panels_pane_report_style_mode'),
  );

  // The pattern storage id field.
  $data['panels_pane_report']['pattern_title'] = array(
    'title' => t('Pattern Title'),
    'help' => t('Title of the Pattern.'),
    'field' => array('handler' => 'views_handler_field'),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_string'),
  );

  // The page path field.
  $data['panels_pane_report']['page_title'] = array(
    'title' => t('Panel Page Title'),
    'help' => t('Title of the Page.'),
    'field' => array('handler' => 'views_handler_field'),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_string'),
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
    'field' => array('handler' => 'panels_pane_report_pattern_status_field'),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_boolean_operator'),
  );

  // The patter node status field.
  $data['panels_pane_report']['page_status'] = array(
    'title' => t('Page Status'),
    'help' => t('Status of the Page.'),
    'field' => array('handler' => 'panels_pane_report_page_status_field'),
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
    'filter' => array('handler' => 'views_handler_filter_string'),
  );

  // The display layout field.
  $data['panels_pane_report']['display_layout'] = array(
    'title' => t('Display Layout'),
    'help' => t('Indicates what layout the display is in.'),
    'field' => array('handler' => 'views_handler_field'),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'panels_pane_report_display_layout'),
  );

  return $data;
}

/**
 * Implements hook_views_default_views().
 */
function panels_pane_report_views_default_views() {
  $views = array(
    'panels_pane_report' => _panels_pane_report_view_panels_pane_report(),
  );

  return $views;
}

/**
 * Returns the Panels report view.
 */
function _panels_pane_report_view_panels_pane_report() {
  $view = new view();
  $view->name = 'panels_pane_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'panels_pane_report';
  $view->human_name = 'Panels Pane Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Panels Pane Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access panels pane report';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '21600';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '-1';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '25, 50, 100, 200, 500';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['empty'] = TRUE;
  $handler->display->display_options['header']['result']['content'] = '<strong>Displaying:</strong> @start - @end of @total';
  /* Field: Panels Pane Report: Bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['label'] = '';
  $handler->display->display_options['fields']['bundle']['exclude'] = TRUE;
  $handler->display->display_options['fields']['bundle']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['bundle']['alter']['text'] = ':[bundle]';
  $handler->display->display_options['fields']['bundle']['element_label_colon'] = FALSE;
  /* Field: Panels Pane Report: Build Mode */
  $handler->display->display_options['fields']['build_mode']['id'] = 'build_mode';
  $handler->display->display_options['fields']['build_mode']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['build_mode']['field'] = 'build_mode';
  $handler->display->display_options['fields']['build_mode']['label'] = '';
  $handler->display->display_options['fields']['build_mode']['exclude'] = TRUE;
  $handler->display->display_options['fields']['build_mode']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['build_mode']['alter']['text'] = '<br />Build Mode: [build_mode]';
  $handler->display->display_options['fields']['build_mode']['element_label_colon'] = FALSE;
  /* Field: Panels Pane Report: Style Mode */
  $handler->display->display_options['fields']['style_mode']['id'] = 'style_mode';
  $handler->display->display_options['fields']['style_mode']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['style_mode']['field'] = 'style_mode';
  $handler->display->display_options['fields']['style_mode']['label'] = '';
  $handler->display->display_options['fields']['style_mode']['exclude'] = TRUE;
  $handler->display->display_options['fields']['style_mode']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['style_mode']['alter']['text'] = '<br />Style: [style_mode]';
  $handler->display->display_options['fields']['style_mode']['element_label_colon'] = FALSE;
  /* Field: Panels Pane Report: Entity Type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['label'] = 'Entity';
  $handler->display->display_options['fields']['entity_type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['entity_type']['alter']['text'] = '[entity_type][bundle] [build_mode] [style_mode]';
  /* Field: Panels Pane Report: Pattern Status */
  $handler->display->display_options['fields']['pattern_status']['id'] = 'pattern_status';
  $handler->display->display_options['fields']['pattern_status']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['pattern_status']['field'] = 'pattern_status';
  $handler->display->display_options['fields']['pattern_status']['label'] = '';
  $handler->display->display_options['fields']['pattern_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['pattern_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pattern_status']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['pattern_status']['not'] = 0;
  /* Field: Panels Pane Report: Pattern Title */
  $handler->display->display_options['fields']['pattern_title']['id'] = 'pattern_title';
  $handler->display->display_options['fields']['pattern_title']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['pattern_title']['field'] = 'pattern_title';
  $handler->display->display_options['fields']['pattern_title']['label'] = '';
  $handler->display->display_options['fields']['pattern_title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['pattern_title']['element_label_colon'] = FALSE;
  /* Field: Panels Pane Report: Pattern Path */
  $handler->display->display_options['fields']['pattern_storage_id']['id'] = 'pattern_storage_id';
  $handler->display->display_options['fields']['pattern_storage_id']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['pattern_storage_id']['field'] = 'pattern_storage_id';
  $handler->display->display_options['fields']['pattern_storage_id']['label'] = 'Pattern';
  $handler->display->display_options['fields']['pattern_storage_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['pattern_storage_id']['alter']['text'] = '<span class="pattern-status">[pattern_status]</span> [pattern_storage_id]<br>
  [pattern_title]';
  /* Field: Panels Pane Report: Panel Page Title */
  $handler->display->display_options['fields']['page_title']['id'] = 'page_title';
  $handler->display->display_options['fields']['page_title']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['page_title']['field'] = 'page_title';
  $handler->display->display_options['fields']['page_title']['label'] = '';
  $handler->display->display_options['fields']['page_title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['page_title']['element_label_colon'] = FALSE;
  /* Field: Panels Pane Report: Page Status */
  $handler->display->display_options['fields']['page_status']['id'] = 'page_status';
  $handler->display->display_options['fields']['page_status']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['page_status']['field'] = 'page_status';
  $handler->display->display_options['fields']['page_status']['label'] = '';
  $handler->display->display_options['fields']['page_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['page_status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['page_status']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['page_status']['not'] = 0;
  /* Field: Panels Pane Report: Panel Page Path */
  $handler->display->display_options['fields']['page_storage_id']['id'] = 'page_storage_id';
  $handler->display->display_options['fields']['page_storage_id']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['page_storage_id']['field'] = 'page_storage_id';
  $handler->display->display_options['fields']['page_storage_id']['label'] = 'Page';
  $handler->display->display_options['fields']['page_storage_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['page_storage_id']['alter']['text'] = '<span class="page-status">[page_status]</span> [page_storage_id]<br>
  [page_title]';
  /* Field: Panels Pane Report: Display Layout */
  $handler->display->display_options['fields']['display_layout']['id'] = 'display_layout';
  $handler->display->display_options['fields']['display_layout']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['display_layout']['field'] = 'display_layout';
  $handler->display->display_options['fields']['display_layout']['label'] = '';
  $handler->display->display_options['fields']['display_layout']['exclude'] = TRUE;
  $handler->display->display_options['fields']['display_layout']['element_label_colon'] = FALSE;
  /* Field: Panels Pane Report: Display Storage ID */
  $handler->display->display_options['fields']['display_storage_id']['id'] = 'display_storage_id';
  $handler->display->display_options['fields']['display_storage_id']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['display_storage_id']['field'] = 'display_storage_id';
  $handler->display->display_options['fields']['display_storage_id']['exclude'] = TRUE;
  /* Field: Panels Pane Report: Display Storage Type */
  $handler->display->display_options['fields']['display_storage_type']['id'] = 'display_storage_type';
  $handler->display->display_options['fields']['display_storage_type']['table'] = 'panels_pane_report';
  $handler->display->display_options['fields']['display_storage_type']['field'] = 'display_storage_type';
  $handler->display->display_options['fields']['display_storage_type']['label'] = 'Container';
  $handler->display->display_options['fields']['display_storage_type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['display_storage_type']['alter']['text'] = '[display_storage_type] [[display_storage_id]]<br>Layout: [display_layout]';
  /* Sort criterion: Panels Pane Report: Entity Type */
  $handler->display->display_options['sorts']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['sorts']['entity_type']['table'] = 'panels_pane_report';
  $handler->display->display_options['sorts']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['sorts']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['entity_type']['expose']['label'] = 'Entity Type';
  /* Sort criterion: Panels Pane Report: Bundle */
  $handler->display->display_options['sorts']['bundle']['id'] = 'bundle';
  $handler->display->display_options['sorts']['bundle']['table'] = 'panels_pane_report';
  $handler->display->display_options['sorts']['bundle']['field'] = 'bundle';
  $handler->display->display_options['sorts']['bundle']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['bundle']['expose']['label'] = 'Bundle';
  /* Sort criterion: Panels Pane Report: Build Mode */
  $handler->display->display_options['sorts']['build_mode']['id'] = 'build_mode';
  $handler->display->display_options['sorts']['build_mode']['table'] = 'panels_pane_report';
  $handler->display->display_options['sorts']['build_mode']['field'] = 'build_mode';
  $handler->display->display_options['sorts']['build_mode']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['build_mode']['expose']['label'] = 'Build Mode';
  /* Sort criterion: Panels Pane Report: Display Storage ID */
  $handler->display->display_options['sorts']['display_storage_id']['id'] = 'display_storage_id';
  $handler->display->display_options['sorts']['display_storage_id']['table'] = 'panels_pane_report';
  $handler->display->display_options['sorts']['display_storage_id']['field'] = 'display_storage_id';
  $handler->display->display_options['sorts']['display_storage_id']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['display_storage_id']['expose']['label'] = 'Container ID';
  /* Filter criterion: Entity ID */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['ui_name'] = 'Entity ID';
  $handler->display->display_options['filters']['combine']['operator'] = 'allwords';
  $handler->display->display_options['filters']['combine']['group'] = 1;
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = 'Entity ID/Title';
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'entity_id';
  $handler->display->display_options['filters']['combine']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    11 => 0,
    3 => 0,
    10 => 0,
    7 => 0,
    12 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
    19 => 0,
  );
  $handler->display->display_options['filters']['combine']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['combine']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['combine']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['combine']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['combine']['expose']['autocomplete_dependent'] = 0;
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'pattern_title' => 'pattern_title',
    'pattern_storage_id' => 'pattern_storage_id',
    'page_title' => 'page_title',
    'page_storage_id' => 'page_storage_id',
  );
  /* Filter criterion: Panels Pane Report: Entity Type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['group'] = 1;
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Entity Type';
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    11 => 0,
    3 => 0,
    10 => 0,
    7 => 0,
    12 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    13 => 0,
  );
  /* Filter criterion: Panels Pane Report: Bundle */
  $handler->display->display_options['filters']['bundle']['id'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['bundle']['field'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['group'] = 1;
  $handler->display->display_options['filters']['bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['bundle']['expose']['operator_id'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['label'] = 'Bundle';
  $handler->display->display_options['filters']['bundle']['expose']['operator'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    11 => 0,
    3 => 0,
    10 => 0,
    7 => 0,
    12 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    13 => 0,
  );
  /* Filter criterion: Panels Pane Report: Build Mode */
  $handler->display->display_options['filters']['build_mode']['id'] = 'build_mode';
  $handler->display->display_options['filters']['build_mode']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['build_mode']['field'] = 'build_mode';
  $handler->display->display_options['filters']['build_mode']['group'] = 1;
  $handler->display->display_options['filters']['build_mode']['exposed'] = TRUE;
  $handler->display->display_options['filters']['build_mode']['expose']['operator_id'] = 'build_mode_op';
  $handler->display->display_options['filters']['build_mode']['expose']['label'] = 'Build Mode';
  $handler->display->display_options['filters']['build_mode']['expose']['operator'] = 'build_mode_op';
  $handler->display->display_options['filters']['build_mode']['expose']['identifier'] = 'build_mode';
  $handler->display->display_options['filters']['build_mode']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    11 => 0,
    3 => 0,
    10 => 0,
    7 => 0,
    12 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    13 => 0,
  );
  /* Filter criterion: Panels Pane Report: Style Mode */
  $handler->display->display_options['filters']['style_mode']['id'] = 'style_mode';
  $handler->display->display_options['filters']['style_mode']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['style_mode']['field'] = 'style_mode';
  $handler->display->display_options['filters']['style_mode']['group'] = 1;
  $handler->display->display_options['filters']['style_mode']['exposed'] = TRUE;
  $handler->display->display_options['filters']['style_mode']['expose']['operator_id'] = 'style_mode_op';
  $handler->display->display_options['filters']['style_mode']['expose']['label'] = 'Style Mode';
  $handler->display->display_options['filters']['style_mode']['expose']['operator'] = 'style_mode_op';
  $handler->display->display_options['filters']['style_mode']['expose']['identifier'] = 'style_mode';
  $handler->display->display_options['filters']['style_mode']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Panels Pane Report: Pattern Status */
  $handler->display->display_options['filters']['pattern_status']['id'] = 'pattern_status';
  $handler->display->display_options['filters']['pattern_status']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['pattern_status']['field'] = 'pattern_status';
  $handler->display->display_options['filters']['pattern_status']['value'] = 'All';
  $handler->display->display_options['filters']['pattern_status']['group'] = 1;
  $handler->display->display_options['filters']['pattern_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['pattern_status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['pattern_status']['expose']['label'] = 'Pattern is Active?';
  $handler->display->display_options['filters']['pattern_status']['expose']['operator'] = 'pattern_status_op';
  $handler->display->display_options['filters']['pattern_status']['expose']['identifier'] = 'pattern_status';
  $handler->display->display_options['filters']['pattern_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    11 => 0,
    3 => 0,
    10 => 0,
    7 => 0,
    12 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    13 => 0,
  );
  /* Filter criterion: Panels Pane Report: Page Status */
  $handler->display->display_options['filters']['page_status']['id'] = 'page_status';
  $handler->display->display_options['filters']['page_status']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['page_status']['field'] = 'page_status';
  $handler->display->display_options['filters']['page_status']['value'] = 'All';
  $handler->display->display_options['filters']['page_status']['group'] = 1;
  $handler->display->display_options['filters']['page_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['page_status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['page_status']['expose']['label'] = 'Panel Page is Active?';
  $handler->display->display_options['filters']['page_status']['expose']['operator'] = 'page_status_op';
  $handler->display->display_options['filters']['page_status']['expose']['identifier'] = 'page_status';
  $handler->display->display_options['filters']['page_status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    11 => 0,
    3 => 0,
    10 => 0,
    7 => 0,
    12 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    13 => 0,
  );
  /* Filter criterion: Panels Pane Report: Display Storage Type */
  $handler->display->display_options['filters']['display_storage_type']['id'] = 'display_storage_type';
  $handler->display->display_options['filters']['display_storage_type']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['display_storage_type']['field'] = 'display_storage_type';
  $handler->display->display_options['filters']['display_storage_type']['group'] = 1;
  $handler->display->display_options['filters']['display_storage_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['display_storage_type']['expose']['operator_id'] = 'display_storage_type_op';
  $handler->display->display_options['filters']['display_storage_type']['expose']['label'] = 'Container';
  $handler->display->display_options['filters']['display_storage_type']['expose']['operator'] = 'display_storage_type_op';
  $handler->display->display_options['filters']['display_storage_type']['expose']['identifier'] = 'display_storage_type';
  $handler->display->display_options['filters']['display_storage_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    11 => 0,
    3 => 0,
    10 => 0,
    7 => 0,
    12 => 0,
    5 => 0,
    6 => 0,
    8 => 0,
    13 => 0,
  );
  /* Filter criterion: Panels Pane Report: Display Layout */
  $handler->display->display_options['filters']['display_layout']['id'] = 'display_layout';
  $handler->display->display_options['filters']['display_layout']['table'] = 'panels_pane_report';
  $handler->display->display_options['filters']['display_layout']['field'] = 'display_layout';
  $handler->display->display_options['filters']['display_layout']['exposed'] = TRUE;
  $handler->display->display_options['filters']['display_layout']['expose']['operator_id'] = 'display_layout_op';
  $handler->display->display_options['filters']['display_layout']['expose']['label'] = 'Display Layout';
  $handler->display->display_options['filters']['display_layout']['expose']['operator'] = 'display_layout_op';
  $handler->display->display_options['filters']['display_layout']['expose']['identifier'] = 'display_layout';
  $handler->display->display_options['filters']['display_layout']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/reports/panels-pane-report';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Panels Pane Report';
  $handler->display->display_options['menu']['description'] = 'Browse the panels pane report.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['panels_pane_report'] = array(
    t('Master'),
    t('Panels Pane Report'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<strong>Displaying:</strong> @start - @end of @total'),
    t(':[bundle]'),
    t('<br />Build Mode: [build_mode]'),
    t('<br />Style: [style_mode]'),
    t('Entity'),
    t('[entity_type][bundle] [build_mode] [style_mode]'),
    t('Pattern'),
    t('<span class="pattern-status">[pattern_status]</span> [pattern_storage_id]<br>
  [pattern_title]'),
    t('Page'),
    t('<span class="page-status">[page_status]</span> [page_storage_id]<br>
  [page_title]'),
    t('Display Storage ID'),
    t('Container'),
    t('[display_storage_type] [[display_storage_id]]'),
    t('Entity Type'),
    t('Bundle'),
    t('Build Mode'),
    t('Container ID'),
    t('Entity ID/Title'),
    t('Style Mode'),
    t('Pattern is Active?'),
    t('Panel Page is Active?'),
  );

  return $view;
}
