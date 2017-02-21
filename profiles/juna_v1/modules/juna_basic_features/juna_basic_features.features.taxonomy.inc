<?php
/**
 * @file
 * juna_basic_features.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function juna_basic_features_taxonomy_default_vocabularies() {
  return array(
    'columns' => array(
      'name' => 'Columns',
      'machine_name' => 'columns',
      'description' => '栏目的分类，例如享什么，壹购等',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'info_category' => array(
      'name' => 'Info Category',
      'machine_name' => 'info_category',
      'description' => '适用于信息或者规则界面的分类',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
