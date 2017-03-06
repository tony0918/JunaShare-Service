<?php

/**
 * Implementation of /juna/orders.json api
 * @param null $status
 * @return array|mixed
 */
function orders_lists($status = NULL) {
  global $user;
  if (!$user->uid) {
    return services_error(t('用户未登录'), 406);
  }

  orders_expire_unpaid_order_by_uid($user->uid);

  $query = db_select('product_order', 'po')
    ->condition('po.uid', $user->uid, '=')
    ->fields('po')->orderBy('po.created', 'DESC')
    ->extend('PagerDefault')->limit(SINGLE_PAGE_LIMIT);
  if (!is_null($status)) {
    // 如果传递status，过滤不同状态的订单
    $query->condition('po.status', $status, '=');
  }
  $query->join('node', 'n', 'po.nid = n.nid');
  $query->condition('n.status', NODE_PUBLISHED, '=');

  $query->join('field_data_field_related_sku_product', 'f3', 'po.nid = f3.entity_id');

  $query->join('field_data_field_product_image_on_order', 'f', 'f.entity_id = f3.field_related_sku_product_nid');
  $query->join('file_managed', 'fm', 'f.field_product_image_on_order_fid = fm.fid');
  $query->fields('n', array('title', 'type'))->fields('fm', array('uri'));

  $data = $query->execute()->fetchAll();
  $status_arr = array('待付款', '已付款', '待发货', '待收货', '已评价', '交易完成', '待评价', '已取消');
  $product_types = array(
    'share_product' => '白领商品',
    'product' => '壹购商品',
    'swingshare' => '摇享商品',
    'yaoxiang_product' => '摇享商品'
  );
  foreach ($data as $k => $v) {
    $data[$k]->product_type = $product_types[$v->product_type];
    $data[$k]->status = $status_arr[$v->status];
    if ($v->uri) {
      $data[$k]->uri = file_create_url($v->uri);
    }
  }
  return ['success' => 1, 'data' => $data];
}

/**
 * Retrieve an order detail information.
 * @param $oid
 * @return array
 */
function orders_order_resource_retrieve($oid) {
  global $user;
  if (!$user->uid) {
    return services_error(t('用户未登录'), 406);
  }

  $query = db_select('product_order', 'po')
    ->condition('po.sorderid', $oid, '=')
    ->fields('po', array('id', 'created', 'total_price', 'express_provider', 'express_id', 'sorderid', 'status'));

  $query->join('node', 'n', 'po.nid = n.nid');
  $query->fields('n', array('title', 'type'));

  $query->join('users', 'u', 'po.uid = u.uid');
  $query->addField('u', 'name', 'mobile');

  $query->join('field_data_field_username', 'f1', 'po.uid = f1.entity_id');
  $query->addField('f1', 'field_username_value', 'full_name');

  $query->join('field_data_field_company_address', 'f2', 'po.uid = f2.entity_id');
  $query->addField('f2', 'field_company_address_value', 'address');

  $query->leftJoin('field_data_field_related_sku_product', 'f3', 'po.nid = f3.entity_id');

  $query->leftJoin('field_data_field_product_price', 'f5', 'f3.field_related_sku_product_nid = f5.entity_id');
  $query->addField('f5', 'field_product_price_value', 'market_price');

  $query->leftJoin('field_data_field_price', 'f6', 'f6.entity_id = po.nid');
  $query->addField('f6', 'field_price_value', 'price');

  $query->leftJoin('field_data_field_product_image_on_order', 'f4', 'f4.entity_id = f3.field_related_sku_product_nid');
  $query->leftJoin('file_managed', 'fm', 'f4.field_product_image_on_order_fid = fm.fid');
  $query->addField('fm', 'uri');

  $data = $query->execute()->fetchAssoc();
  if ('product' != $data['type']) {
    $data['price'] = 0;
    $data['total_price'] = 0;
  }
  $product_types = array(
    'share_product' => '白领商品',
    'product' => '壹购商品',
    'swingshare' => '摇享商品',
    'yaoxiang_product' => '摇享商品'
  );
  $data['type'] = $product_types[$data['type']];
  $data['uri'] = file_create_url($data['uri']);
  $data['created'] = format_date($data['created'], 'custom', 'Y-m-d H:i:s');
  $data['mobile'] = orders_mask_user_mobile($data['mobile']);
  $data['express_fee'] = 0;

  return ['data' => $data];
}

/**
 * Tool function to mask user mobile with star.
 * @param $mobile
 * @return mixed
 */
function orders_mask_user_mobile($mobile) {
  return substr_replace($mobile, '****', 3, 4);
}
