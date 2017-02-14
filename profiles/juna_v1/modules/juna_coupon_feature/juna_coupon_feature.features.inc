<?php
/**
 * @file
 * juna_coupon_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function juna_coupon_feature_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function juna_coupon_feature_node_info() {
  $items = array(
    'coupon' => array(
      'name' => t('Coupon'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
