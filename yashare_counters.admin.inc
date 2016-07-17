<?php
/**
 * @file
 * Administrative page forms and functions for the Yandex.Share with Counters.
 */

/**
 * Menu callback administration settings.
 */
function yashare_counters_admin_settings() {
  $form['style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Style'),
  );

  $form['style']['yashare_counters_small_size'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use small buttons.'),
    '#default_value' => variable_get('yashare_counters_small_size', YASHARE_COUNTERS_SMALL),
  );

  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Services'),
    '#theme' => 'yashare_counters_admin_settings_services_table',
  );

  foreach (yashare_counters_services_list() as $service_id => $service_name) {
    $form['services'][$service_id]['yashare_counters_service_' . $service_id . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => drupal_placeholder($service_name),
      '#default_value' => variable_get('yashare_counters_service_' . $service_id . '_enabled', 1),
    );
    $form['services'][$service_id]['yashare_counters_service_' . $service_id . '_weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => variable_get('yashare_counters_service_' . $service_id . '_weight', 0),
      '#attributes' => array('class' => array('yashare-counters-service-weight')),
    );
  }

  return system_settings_form($form, TRUE);
}

/**
 * Theme function for settings table.
 */
function theme_yashare_counters_admin_settings_services_table($vars) {
  $form = $vars['form'];

  $header = array(
    array('data' => t('Service name'), 'colspan' => 2),
  );
  $rows = array();

  foreach (element_children($form) as $service_id) {
    $row = array(
      drupal_render($form[$service_id]['yashare_counters_service_' . $service_id . '_enabled']),
      drupal_render($form[$service_id]['yashare_counters_service_' . $service_id . '_weight']),
    );
    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'yashare-counters-services'),
  ));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('yashare-counters-services', 'order', 'sibling', 'yashare-counters-service-weight');

  return $output;
}
