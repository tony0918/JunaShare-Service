<?php

namespace Drupal\junashare_service\Plugin\resource\pages;

use Drupal\restful\Plugin\resource\ResourceNode;

/**
 * Class InfoPage__1_0
 * @package Drupal\junashare_service\Plugin\resource\pages
 *
 * @Resource(
 *   name = "infopage:1.0",
 *   resource = "infopage",
 *   label = "InfoPage",
 *   description = "Export the InfoPage.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *        "info_page"
 *      },
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0,
 * )
 */
class InfoPage__1_0 extends ResourceNode {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $public_fields = parent::publicFields();
    $public_fields['items'] = array('property' => 'field_info_item');

    $public_fields['category'] = array(
      'property' => 'field_info_item_category',
      'resource' => array(
        'name' => 'infocategory',
        'majorVersion' => 1,
        'minorVersion' => 0
      )
    );

    // Clean up some fields.
    unset($public_fields['label'], $public_fields['self']);
    $public_fields['id']['methods'] = array();

    return $public_fields;
  }


}
