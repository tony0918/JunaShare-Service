<?php

namespace Drupal\junashare_service\Plugin\resource\product\yigou;

use Drupal\restful\Plugin\resource\DataInterpreter\DataInterpreterInterface;
use Drupal\restful\Plugin\resource\ResourceNode;

/**
 * Class Yigou__1_0
 * @package Drupal\junashare_service\Plugin\resource\product\yigou
 *
 * @Resource(
 *   name = "yigou:1.0",
 *   resource = "yigou",
 *   label = "Yigou",
 *   description = "Export the Yigou Product.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *       "product"
 *     },
 *     "sort": {
 *       "sticky": "DESC",
 *       "created": "DESC"
 *     },
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Yigou__1_0 extends ResourceNode {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $public_fields = parent::publicFields();

    $public_fields['nid'] = $public_fields['id'];
    $public_fields['price'] = array(
      'property' => 'field_price'
    );
    $public_fields['stock'] = array(
      'property' => 'field_total_num'
    );
    $public_fields['remaining'] = array(
      'property' => 'field_remain_num'
    );
    $public_fields['num_in_box'] = array(
      'property' => 'field_inbox_num'
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
    $public_fields['time_remaining'] = array(
      'callback' => array($this, 'getRemainingTime'),
    );

    // Clean up some fields.
    unset($public_fields['label'], $public_fields['self']);
    $public_fields['id']['methods'] = array();

    return $public_fields;
  }

  /**
   * {@inheritdoc}
   */
  protected function dataProviderClassName() {
    return 'Drupal\junashare_service\Plugin\resource\DataProvider\DataProviderYigouNode';
  }

  public function getRemainingTime(DataInterpreterInterface $interpreter) {
    if (8 > (int) format_date(REQUEST_TIME, 'custom', 'h')) {
      $result = strtotime('today 08:00') - REQUEST_TIME;
    }
    else {
      $result = strtotime('tomorrow 08:00') - REQUEST_TIME;
    }
    return $result;
  }
}
