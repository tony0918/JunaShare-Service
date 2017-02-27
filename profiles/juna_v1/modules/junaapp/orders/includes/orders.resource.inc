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

  $query->join('field_data_field_image_for_list', 'f', 'f.entity_id = po.nid');
  $query->join('file_managed', 'fm', 'f.field_image_for_list_fid = fm.fid');
  $query->fields('n', array('title', 'type'))->fields('fm', array('uri'));

  $data = $query->execute()->fetchAll();
  $status_arr = array('待付款', '已付款', '待发货', '待收货', '已评价', '交易完成', '待评价', '已取消');
  $product_types = array('share_product' => '白领商品', 'product' => '壹购商品', 'swingshare' => '壹购商品');
  foreach ($data as $k => $v) {
    $data[$k]->product_type = $product_types[$v->product_type];
    $data[$k]->status = $status_arr[$v->status];
    if ($v->uri) {
      $data[$k]->uri = file_create_url($v->uri);
    }
  }
  return ['success' => 1, 'data' => $data];
}
