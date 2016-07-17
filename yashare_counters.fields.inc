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
      'settings' => array(
        'image_field' => 'field_image',
        'image_style' => 'thumbnail',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function yashare_counters_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $element = array();

  $link = NULL;
  $title = NULL;
  $description = NULL;
  $image = NULL;

  if ($entity_type == 'node') {
    $link = 'node/' . $entity->nid;
    $title = $entity->title;
  }
  elseif ($entity_type == 'taxonomy_term') {
    $link = 'taxonomy/term/' . $entity->tid;
    $title = $entity->name;
  }

  if ($images = field_get_items($entity_type, $entity, $settings['image_field'])) {
    if ($settings['image_style']) {
      $image = image_style_url($settings['image_style'], $images[0]['uri']);
    }
    else {
      $image = file_create_url($images[0]['uri']);
    }
  }

  switch ($display['type']) {
    case 'yashare_counters_default_formatter':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'div',
          '#value' => yashare_counters_block_content($link, $title, $description, $image),
          '#attached' => array(
            'js' => array(
              'https://yastatic.net/share2/share.js' => array(
                'type' => 'external',
                // See https://www.drupal.org/node/1664602.
                'async' => 'async',
              ),
            ),
          ),
        );
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function yashare_counters_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $elements = array();

  $options = array();
  $fields_info = field_info_instances($instance['entity_type'], $instance['bundle']);
  if (!empty($fields_info)) {
    foreach ($fields_info as $field_name => $value) {
      $field_info = field_info_field($field_name);
      if (in_array($field_info['type'], array('image'))) {
        $options[$field_name] = $fields_info[$field_name]['label'];
      }
    }
  }
  else {
    // Fallback for using image field in views.
    $fields_info = field_info_field_map();
    foreach ($fields_info as $field_name => $field_info) {
      if (in_array($field_info['type'], array('image'))) {
        $options[$field_name] = $field_name;
      }
    }
  }
  $elements['image_field'] = array(
    '#type' => 'select',
    '#title' => t('Image field'),
    '#description' => t('Choose image field to use as image to share.'),
    '#default_value' => $settings['image_field'],
    '#empty_option' => t('None'),
    '#options' => $options,
  );

  $options = image_style_options(FALSE);
  $elements['image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image style'),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => $options,
    '#states' => array(
      'invisible' => array(
        ':input[name="fields[' . $instance['field_name'] . '][settings_edit_form][settings][image_field]"]' => array('value' => ''),
      ),
    ),
  );

  return $elements;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function yashare_counters_field_formatter_settings_summary($field, $instance, $view_mode) {
  $settings = $instance['display'][$view_mode]['settings'];
  $output = array();

  $field_instances = field_info_instances($instance['entity_type'], $instance['bundle']);
  if (isset($field_instances[$settings['image_field']])) {
    $output[] = t('Image field to share: @field', array('@field' => $field_instances[$settings['image_field']]['label']));

    $image_styles = image_style_options(FALSE);
    // Unset possible 'No defined styles' option.
    unset($image_styles['']);
    // Styles could be lost because of enabled/disabled modules that define
    // their styles in code.
    if (isset($image_styles[$settings['image_style']])) {
      $output[] = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
    }
    else {
      $output[] = t('Image style: @style', array('@style' => t('Original image')));
    }
  }
  else {
    $output[] = t('Image field to share: @field', array('@field' => t('None')));
  }

  return implode('<br />', $output);
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
