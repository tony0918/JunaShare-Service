<?php
/**
 * @file
 * juna_basic_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function juna_basic_features_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function juna_basic_features_image_default_styles() {
  $styles = array();

  // Exported image style: banner_style.
  $styles['banner_style'] = array(
    'label' => 'Banner Style',
    'effects' => array(
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 750,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function juna_basic_features_node_info() {
  $items = array(
    'ad_banner' => array(
      'name' => t('Ad Banner'),
      'base' => 'node_content',
      'description' => t('广告Banner'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
