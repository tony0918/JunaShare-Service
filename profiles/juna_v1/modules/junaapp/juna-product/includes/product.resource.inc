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

  $query = db_select('product_box', 'pb')
    ->condition('pb.uid', $user->uid, '=')
    ->fields('pb', array('created', 'nid'));
  $query->leftJoin('node', 'n', 'pb.nid = n.nid');
  $query->fields('n', array('title', 'type'));

  $query->leftJoin('field_data_field_price', 'fdfp', 'fdfp.entity_id = pb.nid');
  $query->addField('fdfp', 'field_price_value');

  $query->leftJoin('field_data_field_remain_num', 'f1', 'f1.entity_id = pb.nid');
  $query->addField('f1', 'field_remain_num_value');

  $query->leftJoin('field_data_field_related_sku_product', 'f2', 'f2.entity_id = pb.nid');
  $query->leftJoin('field_data_field_product_image_in_box', 'f3', 'f3.entity_id = f2.field_related_sku_product_nid');
  $query->leftJoin('field_data_field_product_price', 'f4', 'f4.entity_id = f2.field_related_sku_product_nid');
  $query->addField('f4', 'field_product_price_value');

  $query->leftJoin('file_managed', 'fm', 'f3.field_product_image_in_box_fid = fm.fid');
  $query->addField('fm', 'uri');

  $rows = $query->execute()->fetchAll();

  $box_data = array();
  foreach ($rows as $k => $v) {
    $obj = new stdClass();
    $obj->nid = $v->nid;
    $obj->created = format_date($v->created, 'custom', 'm月d日');
    $obj->label = $v->title;
    $obj->remaining = $v->field_remain_num_value;
    $obj->image_on_item_in_box = file_create_url($v->uri);
    $obj->type_machine_name = $v->type;
    switch ($v->type) {
      case 'product':
        $obj->type = '壹购商品';
        $obj->field_price = $v->field_price_value;
        break;
      case 'share_product':
        $obj->type = '白领商品';
        $obj->field_price = $v->field_product_price_value;
        break;
    }
    $box_data[] = $obj;
  }

  return ['data' => $box_data, 'remain_boxproduct_num' => 5 - sizeof($box_data)];
}
