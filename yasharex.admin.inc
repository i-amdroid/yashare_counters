<?php
/**
 * @file
 * Administrative page forms and functions for the Yandex.Share Ex module.
 */

/**
 * Menu callback administration settings.
 */
function yasharex_admin_settings() {
  $form['style'] = array(
    '#type' => 'fieldset',
    '#title' => t('Style'),
  );

  $form['style']['small_size'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Use small buttons.'),
    '#default_value' => variable_get('small_size', YASHAREX_SMALL)
  );


  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Services'),
    '#theme' => 'yasharex_admin_settings_services_table',
  );

  foreach (yasharex_services_list() as $service_id => $service_name) {
    $form['services'][$service_id]['yasharex_service_' . $service_id . '_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => drupal_placeholder($service_name),
      '#default_value' => variable_get('yasharex_service_' . $service_id . '_enabled', 1),
    );
    $form['services'][$service_id]['yasharex_service_' . $service_id . '_weight'] = array(
      '#type' => 'weight',
      // '#title' => t('Weight'),
      '#delta' => 10,
      '#default_value' => variable_get('yasharex_service_' . $service_id . '_weight', 0),
      '#attributes' => array('class' => array('yasharex-service-weight')),
    );
  }

  return system_settings_form($form, TRUE);
}


function theme_yasharex_admin_settings_services_table($vars) {
  $form = $vars['form'];

  $header = array(
    array('data' => t('Service name'), 'colspan' => 2),
    //t('Weight'),
  );
  $rows = array();

  foreach (element_children($form) as $service_id) {
    $row = array(
      drupal_render($form[$service_id]['yasharex_service_' . $service_id . '_enabled']),
      drupal_render($form[$service_id]['yasharex_service_' . $service_id . '_weight']),
    );
    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'yasharex-services')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('yasharex-services', 'order', 'sibling', 'yasharex-service-weight');

  return $output;
}