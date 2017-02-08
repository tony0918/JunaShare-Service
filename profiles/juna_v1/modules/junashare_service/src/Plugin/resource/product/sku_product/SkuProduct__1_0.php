<?php

namespace Drupal\junashare_service\Plugin\resource\product\sku_product;

use Drupal\junashare_service\Plugin\resource\JunashareServiceUtilities;
use Drupal\restful\Plugin\resource\ResourceNode;

/**
 * Class SkuProduct__1_0
 * @package Drupal\junashare_service\Plugin\resource\product\sku_product
 *
 * @Resource(
 *   name = "skuproduct:1.0",
 *   resource = "skuproduct",
 *   label = "SkuProduct",
 *   description = "Export the SkuProduct.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *        "sku_product"
 *      }
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class SkuProduct__1_0 extends ResourceNode {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $public_fields = parent::publicFields();
    $utility = JunashareServiceUtilities::getInstance();

    $public_fields['sku'] = array('property' => 'field_product_sku');
    $public_fields['displayName'] = array('property' => 'field_product_display_name');
    $public_fields['marketPrice'] = array('property' => 'field_product_display_name');
    $public_fields['shortDesc'] = array('property' => 'field_product_short_description');
    $public_fields['detailDesc'] = array(
      'property' => 'field_product_description',
      'sub_property' => 'value'
    );
    $public_fields['mainKv'] = array(
      'property' => 'field_product_main_kv',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );
    $public_fields['imageForXiangshenme'] = array(
      'property' => 'field_product_share_list_image',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );

    return $public_fields;
  }
}
