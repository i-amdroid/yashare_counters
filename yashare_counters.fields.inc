<?php
/**
 * @file
 * Field for Yandex.Share buttons with counters.
 */

/**
 * Implements hook_field_info().
 */
function yashare_counters_field_info() {
  return array(
    'yashare_counters_buttons' => array(
      'label' => t('Yandex.Share with Counters'),
      'description' => t('Displays Yandex.Share buttons with counters.'),
      'default_widget' => 'yashare_counters_default_widget',
      'default_formatter' => 'yashare_counters_default_formatter',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function yashare_counters_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_formatter_info().
 */
function yashare_counters_field_formatter_info() {
  return array(
    'yashare_counters_default_formatter' => array(
      'label' => t('Default'),
      'field types' => array('yashare_counters_buttons'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function yashare_counters_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  $link = NULL;
  $title = NULL;
  $description = NULL;
  $image = NULL;

  if ($entity_type == 'node') {
    $link = 'node/' . $entity->nid;
    $title = $entity->title; 
    if (isset($entity->field_image)) {
      $imageArray = (array) $entity->field_image;
      $image = file_create_url($imageArray['und'][0]['uri']);
    }
  }
  elseif ($entity_type == 'taxonomy_term') {
    $link = 'taxonomy/term/' . $entity->tid;
    $title = $entity->name;
  }

  switch ($display['type']) {
    case 'yashare_counters_default_formatter':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#attached' => array(
            'js' => array(
              '//yandex.st/share/cnt.share.js' => array(
                'type' => 'external',
              ),
            ),
          ),
          '#value' => yashare_counters_block_content($link, $title, $description, $image),
        );
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function yashare_counters_field_widget_info() {
  return array(
    'yashare_counters_default_widget' => array(
      'label' => t('Default'),
      'field types' => array('yashare_counters_buttons'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function yashare_counters_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $widget = $element;
  $widget['#delta'] = $delta;

  switch ($instance['widget']['type']) {
    case 'yashare_counters_default_widget':
      $widget += array(
        '#type' => 'value',
        '#value' => '',
      );
      break;
  }

  $element['smth'] = $widget;
  return $element;
}
