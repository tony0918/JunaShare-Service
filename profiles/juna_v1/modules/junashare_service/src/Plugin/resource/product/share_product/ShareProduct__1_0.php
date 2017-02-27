<?php

namespace Drupal\junashare_service\Plugin\resource\product\share_product;

use Drupal\restful\Plugin\resource\ResourceNode;

/**
 * Class ShareProduct__1_0
 * @package Drupal\junashare_service\Plugin\resource\product\share_product
 *
 * @Resource(
 *   name = "shareproduct:1.0",
 *   resource = "shareproduct",
 *   label = "ShareProduct",
 *   description = "Export the Share Product.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *        "share_product"
 *      },
 *     "sort": {
 *       "sticky": "DESC",
 *       "created": "DESC"
 *     },
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class ShareProduct__1_0 extends ResourceNode {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $public_fields = parent::publicFields();

    $public_fields['nid'] = $public_fields['id'];
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
    $public_fields['in_box'] = array(
      'callback' => array($this, 'getInBoxStatus')
    );
    $public_fields['current_period'] = array(
      'callback' => array($this, 'getCurrentPeriod')
    );
    $public_fields['duration'] = array(
      'callback' => array($this, 'getDuration')
    );

    // Clean up some fields.
    unset($public_fields['self']);
    $public_fields['id']['methods'] = array();
    $public_fields['sticky'] = array('property' => 'sticky', 'methods' => array());
    $public_fields['created'] = array('property' => 'created', 'methods' => array());

    return $public_fields;
  }

  /**
   * {@inheritdoc}
   */
  protected function dataProviderClassName() {
    return 'Drupal\junashare_service\Plugin\resource\DataProvider\DataProviderShareProductNode';
  }

  public function getInBoxStatus($interpreter) {
    global $user;
    $result = array();
    if ($user->uid > 0) {
      $q = db_select('product_box', 'pb')
        ->fields('pb', array('id'))
        ->condition('uid', $user->uid, '=')
        ->condition('nid', $interpreter->getWrapper()->value()->vid, '=');
      $result = $q->execute()->fetchAll();
    }
    return empty($result) ? 0 : 1;
  }

  public function getCurrentPeriod() {
    $hour = (int) format_date(REQUEST_TIME, 'custom', 'H');
    if (8 > $hour) {
      return array('round' => 0, 'remaining_time' => 0);
    }
    if (12 > $hour) {
      return array('round' => 1, 'remaining_time' => strtotime('today 12:00:00') - REQUEST_TIME);
    }
    if (16 > $hour) {
      return array('round' => 2, 'remaining_time' => strtotime('today 16:00:00') - REQUEST_TIME);
    }
    if (20 > $hour) {
      return array('round' => 3, 'remaining_time' => strtotime('today 20:00:00') - REQUEST_TIME);
    }
    return array('round' => 4, 'remaining_time' => strtotime('tomorrow 00:00:00') - REQUEST_TIME);
  }

  public function getDuration($interpreter) {
    $duration = array();
    $node = $interpreter->getWrapper()->value();
    $start = $node->field_product_valid_period[LANGUAGE_NONE][0]['value'];
    $end = $node->field_product_valid_period[LANGUAGE_NONE][0]['value2'];
    if (8 <= (int) format_date(REQUEST_TIME, 'custom', 'H')) {
      if ($start <= strtotime('today 8:00:00') && $end >= strtotime('today 11:59:59')) {
        array_push($duration, 1);
      }
      if ($start <= strtotime('today 12:00:00') && $end >= strtotime('today 15:59:59')) {
        array_push($duration, 2);
      }
      if ($start <= strtotime('today 16:00:00') && $end >= strtotime('today 19:59:59')) {
        array_push($duration, 3);
      }
      if ($start <= strtotime('today 20:00:00') && $end >= strtotime('today 23:59:59')) {
        array_push($duration, 4);
      }
    }
    return $duration;
  }

}
