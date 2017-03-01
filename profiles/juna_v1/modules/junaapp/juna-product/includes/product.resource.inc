<?php

/**
 * Implementation of mybox api.
 * @return array|mixed
 */
function product_my_box() {
  global $user;
  if (empty($user->uid)) {
    return services_error(t('用户未登录.'), 403);
  }

  $rows = db_select('product_box', 'pb')
    ->condition('pb.uid', $user->uid, '=')
    ->fields('pb', array('created', 'nid'))
    ->execute()
    ->fetchAllAssoc('nid');

  $box_data = array();
  if (!empty($rows)) {
    $node_arr = node_load_multiple(array_keys($rows));
    foreach ($node_arr as $key => $node) {
      $row = prepare_product_node_fields($node);
      if (isset($node->field_related_sku_product[LANGUAGE_NONE][0]['nid'])) {
        $row['sku_product'] = prepare_sku_product_object($node->field_related_sku_product[LANGUAGE_NONE][0]['nid']);
      }
      if (isset($node->field_seller[LANGUAGE_NONE][0]['nid'])) {
        $row['provider'] = prepare_product_provider_object($node->field_seller[LANGUAGE_NONE][0]['nid']);
      }
      $row['in_box'] = 1;
      $box_data[] = $row;
    }
  }

  return ['data' => $box_data, 'remain_boxproduct_num' => 5 - sizeof($box_data)];
}
