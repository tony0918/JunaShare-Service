<?php

namespace Drupal\juna_push\Plugin\resource;

use Drupal\restful\Http\RequestInterface;
use Drupal\restful\Plugin\resource\ResourceDbQuery;

/**
 * Class GetuiUserBinding__1_0
 * @package Drupal\juna_push\Plugin\resource
 *
 * @Resource(
 *   name = "getui_user_binding:1.0",
 *   resource = "getui_user_binding",
 *   label = "Getui User Binding",
 *   description = "Bind User with Getui cid.",
 *   dataProvider = {
 *     "tableName": "juna_getui_push_user_cid",
 *     "idColumn": "id",
 *     "primary": "id",
 *     "idField": "id",
 *   },
 *   authenticationTypes = {
 *     "cookie"
 *   },
 *   authenticationOptional = FALSE,
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class GetuiUserBinding__1_0 extends ResourceDbQuery {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $fields = array();
    $fields['id'] = array('property' => 'id', 'methods' => array(RequestInterface::METHOD_POST));
    $fields['uid'] = array('property' => 'uid', 'methods' => array(RequestInterface::METHOD_POST));
    $fields['cid'] = array('property' => 'cid', 'methods' => array(RequestInterface::METHOD_POST));
    return $fields;
  }
}
