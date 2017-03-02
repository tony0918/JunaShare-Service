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
    $public_fields['type_machine_name'] = array(
      'property' => 'type'
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
    $public_fields['in_box'] = array(
      'callback' => array($this, 'getInBoxStatus')
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

  public function additionalHateoas($data) {
    $hour = (int) format_date(REQUEST_TIME, 'custom', 'H');
    $rounds = array(
      array(
        'round' => 8,
        'start' => strtotime('today 08:00:00'),
        'end' => strtotime('today 11:59:59'),
      ),
      array(
        'round' => 12,
        'start' => strtotime('today 12:00:00'),
        'end' => strtotime('today 15:59:59'),
      ),
      array(
        'round' => 16,
        'start' => strtotime('today 16:00:00'),
        'end' => strtotime('today 19:59:59'),
      ),
      array(
        'round' => 20,
        'start' => strtotime('today 20:00:00'),
        'end' => strtotime('today 23:59:59'),
      )
    );
    foreach ($rounds as $key => &$val) {
      if ($val['round'] < $hour && $hour < $val['round'] + 4) {
        $val['selected'] = 1;
        $val['remain'] = $val['end'] - REQUEST_TIME;
      }
      else {
        $val['selected'] = 0;
        $val['remain'] = 0;
      }
      unset($val['round']);
    }
    return ['time_period' => $rounds];
  }
}
