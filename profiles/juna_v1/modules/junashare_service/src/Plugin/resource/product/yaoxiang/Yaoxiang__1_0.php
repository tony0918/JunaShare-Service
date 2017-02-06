<?php

namespace Drupal\junashare_service\Plugin\resource\product\yaoxiang;

use Drupal\restful\Plugin\resource\ResourceNode;

/**
 * Class Yaoxiang__1_0
 * @package Drupal\junashare_service\Plugin\resource\product\yaoxiang
 *
 * @Resource(
 *   name = "yaoxiang:1.0",
 *   resource = "yaoxiang",
 *   label = "Yaoxiang",
 *   description = "Export the Yaoxiang Product.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *        "yaoxiang_product"
 *      }
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Yaoxiang__1_0 extends ResourceNode {
  protected function publicFields() {
    $public_fields = parent::publicFields();

    $public_fields['skuProductItem'] = array(
      'property' => 'field_sku_product_item',
      'resource' => array(
        'name' => 'skuproduct',
        'majorVersion' => 1,
        'minorVersion' => 0
      )
    );

    return $public_fields;
  }


}
