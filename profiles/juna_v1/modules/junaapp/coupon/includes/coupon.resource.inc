<?php

/**
 *我的摇享
 */
function coupon_my_yaoxiang_list($page) {
  global $user;
  if (!$user->uid) {
    return services_error(t('用户未登陆'), 403);
  }

  if ($page > 0) {
    $page--;
  }
  else {
    $page = 0;
  }
  $query = db_select('user_wingshare_log', 'uwl');
  $query->condition('uwl.uid', $user->uid)
    ->fields('uwl', array('status', 'pdate', 'nid', 'swingnum'))
    ->orderBy('uwl.id', 'DESC')
    ->range($page * SINGLE_PAGE_LIMIT, SINGLE_PAGE_LIMIT);

  $query->leftJoin('field_data_field_related_sku_product', 'f3', 'uwl.nid = f3.entity_id');

  $query->leftJoin('field_data_field_product_image_on_coupon', 'f7', 'f7.entity_id = f3.field_related_sku_product_nid');
  $query->leftJoin('file_managed', 'fm', 'f7.field_product_image_on_coupon_fid = fm.fid');
  $query->addField('fm', 'uri', 'pictures');

  $rows = $query->execute()->fetchAllAssoc('nid');

  $results = array();
  $pdate = format_date(REQUEST_TIME, 'custom', 'Y-n-j');
  $hour = (int) format_date(REQUEST_TIME, 'custom', 'H');
  foreach ($rows as $key => $value) {
    $value->remaintime = 0;
    if ($hour >= 8 && $hour < 22 && $value->pdate == $pdate) {
      $value->status = 2;
      $value->remaintime = strtotime('today 22:00:00') - REQUEST_TIME;
    }
    $value->pictures = file_create_url($value->pictures);
    $results[] = $value;
  }

  return ['data' => $results];
}
