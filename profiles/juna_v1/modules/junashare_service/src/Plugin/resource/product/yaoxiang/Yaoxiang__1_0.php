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
 *        "yaoxiang_product",
 *        "swingshare"
 *      }
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Yaoxiang__1_0 extends ResourceNode {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $public_fields = parent::publicFields();
    $public_fields['nid'] = $public_fields['id'];
    $public_fields['stock'] = array(
      'property' => 'field_product_total_amount'
    );
    $public_fields['sku_product'] = array(
      'property' => 'field_related_sku_product',
      'resource' => array(
        'name' => 'skuproduct',
        'majorVersion' => 1,
        'minorVersion' => 0
      )
    );
    $public_fields['provider'] = array(
      'property' => 'field_seller',
      'resource' => array(
        'name' => 'seller',
        'majorVersion' => 1,
        'minorVersion' => 0
      )
    );

    // Clean up some fields.
    unset($public_fields['self']);
    $public_fields['id']['methods'] = array();

    return $public_fields;
  }

  /**
   * {@inheritdoc}
   */
  protected function dataProviderClassName() {
    return 'Drupal\junashare_service\Plugin\resource\DataProvider\DataProviderYaoxiangNode';
  }

  public function additionalHateoas($data) {
    $hour = (int) format_date(REQUEST_TIME, 'custom', 'H');
    $remaining = 0;
    if ($hour > 8 && $hour < 22) {
      $remaining = strtotime('today 22:00:00') - REQUEST_TIME;
    }
    return ['remaining' => $remaining];
  }
}
