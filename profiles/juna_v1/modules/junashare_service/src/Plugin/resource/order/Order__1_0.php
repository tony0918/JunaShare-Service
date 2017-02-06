<?php

namespace Drupal\junashare_service\Plugin\resource\order;

use Drupal\restful\Plugin\resource\ResourceDbQuery;

/**
 * Class Order__1_0
 * @package Drupal\junashare_service\Plugin\resource\order
 *
 * @Resource(
 *   name = "order:1.0",
 *   resource = "order",
 *   label = "order list",
 *   description = "Expose order items.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "tableName": "node",
 *     "idColumn": "nid",
 *     "primary": "nid",
 *     "idField": "nid",
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Order__1_0 extends ResourceDbQuery {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $fields = array();

    $fields['id'] = array('property' => 'nid');
    $fields['vid'] = array('property' => 'vid');
    $fields['uid'] = array('property' => 'uid');
//    $fields['serialized'] = array('property' => 'serialized_field');

    return $fields;
  }
}
