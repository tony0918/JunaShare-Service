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

//    $public_fields['display_name'] = array('property' => 'field_product_display_name');
    $public_fields['market_price'] = array('property' => 'field_product_price');
//    $public_fields['short_desc'] = array('property' => 'field_product_short_description');
    $public_fields['detail_desc'] = array(
      'property' => 'field_product_description',
      'sub_property' => 'value',
      'process_callbacks' => array(
        array($utility, 'skuProductDetailWrapper')
      )
    );
    $public_fields['main_kv'] = array(
      'property' => 'field_product_main_kv',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );
    $public_fields['image_for_share_list'] = array(
      'property' => 'field_product_share_list_image',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );
    $public_fields['image_on_item_in_box'] = array(
      'property' => 'field_product_image_in_box',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );
    $public_fields['image_on_coupon'] = array(
      'property' => 'field_product_image_on_coupon',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );
    $public_fields['image_on_order'] = array(
      'property' => 'field_product_image_on_order',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );
    $public_fields['image_for_yigou_list'] = array(
      'property' => 'field_product_image_on_yigou',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );
    $public_fields['brand_links'] = array(
      'property' => 'field_product_brand_link',
    );

    // Clean up some fields.
    unset($public_fields['label'], $public_fields['self']);
    $public_fields['id']['methods'] = array();

    return $public_fields;
  }
}
