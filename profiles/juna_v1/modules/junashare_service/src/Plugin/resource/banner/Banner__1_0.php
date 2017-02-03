<?php

namespace Drupal\junashare_service\Plugin\resource\banner;

use Drupal\restful\Plugin\resource\ResourceNode;
use Drupal\junashare_service\Plugin\resource\JunashareServiceUtilities;

/**
 * Banner接口
 * Class Banner__1_0
 * @package Drupal\junashare_service\Plugin\resource\banner
 *
 * @Resource(
 *   name = "banner:1.0",
 *   resource = "banner",
 *   label = "Banner",
 *   description = "Export the banner.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *        "ad_banner"
 *      }
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Banner__1_0 extends ResourceNode {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $public_fields = parent::publicFields();
    $utility = JunashareServiceUtilities::getInstance();

    $public_fields['type'] = array('property' => 'field_target_type');

    $public_fields['parameter'] = array('property' => 'field_parameter_for_target');

    // Have to use original image because image style function caused file size increased.
    $public_fields['images'] = array(
      'property' => 'field_banner_image',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );

    $public_fields['column'] = array(
      'property' => 'field_columns',
      'resource' => array(
        'name' => 'columns',
        'majorVersion' => 1,
        'minorVersion' => 0
      ),
      'methods' => array()
    );

    // Clean up some fields.
    unset($public_fields['label'], $public_fields['self']);
    $public_fields['id']['methods'] = array();

    return $public_fields;
  }


}
