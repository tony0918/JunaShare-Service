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
      $row['created'] = format_date($rows[$node->nid]->created, 'custom', 'Y-m-d H:i:s');
      switch ($node->type) {
        // 显示在我的盒子中的价值，如果是壹购商品，就用壹购实体的价格，如果是白领商品或者其他商品，就用SKU实体的市场价格
        case YIGOU_PRODUCT_CONTENT_TYPE:
          $row['price'] = $node->field_price[LANGUAGE_NONE][0]['value'];
          break;
        default:
          $row['price'] = $row['sku_product']['market_price'];
          break;
      }
      $box_data[] = $row;
    }
  }

  return ['data' => $box_data, 'remain_boxproduct_num' => 5 - sizeof($box_data)];
}

/**
 * Add product into box.
 * @param $nid
 * @return array|mixed
 */
function product_tobox($nid) {
  global $user;
  if (!$user->uid) {
    return services_error(t('用户未登录.'), 403);
  }
  $query = db_select('node', 'n');
  $query->condition('n.nid', $nid, '=')
    ->condition('n.status', NODE_PUBLISHED, '=')
    ->condition('n.type', array('share_product', 'product'), 'IN')
    ->fields('n');
  $query->join('field_data_field_product_valid_period', 'f1', 'f1.entity_id = n.nid');
  $query->condition('f1.field_product_valid_period_value', REQUEST_TIME, '<=')
    ->condition('f1.field_product_valid_period_value2', REQUEST_TIME, '>=');
  $node = $query->execute()->fetchAllAssoc('n.nid');
  if (empty($node)) {
    return services_error(t('商品已下架.'), 406);
  }
  else {
    $query = db_select('product_box', 'pb')
      ->condition('pb.uid', $user->uid, '=')
      ->fields('pb', array('nid'));
    $items = $query->execute()->fetchAllAssoc('nid');
    if (count($items) < 5) {
      if (!in_array($nid, array_keys($items))) {
        $box = new stdClass();
        $box->nid = $nid;
        $box->uid = $user->uid;
        $box->product_type = array_shift($node)->type;
        $box->created = REQUEST_TIME;
        drupal_write_record('product_box', $box);
      }
      return ['message' => '加入盒子成功', 'nid' => $nid];
    }
    else {
      return services_error(t('盒子已满，请先从盒子中删除部分商品.'), 406);
    }
  }
}

/**
 * Create a unpaid Yigou Product order.
 * @param $nid
 * @return array|mixed
 */
function product_make_a_purchase($nid) {
  global $user;
  if (empty($user->uid)) {
    return services_error(t('用户未登录.'), 403);
  }
  $u = user_load($user->uid);
  if (empty($u->field_certed) || (empty($u->field_certed['und'][0]['value']))) {
    return services_error(t('请先完成认证'), 406);
  }
  elseif ($u->field_certed['und'][0]['value'] == 2) {
    return services_error(t('认证资料审核中'), 406);
  }

  // 确定该商品已经被收入盒子，如果已经收入，就没有时间限制了
  $in_box = db_select('product_box', 'pb')
    ->condition('pb.uid', $user->uid, '=')
    ->condition('pb.nid', $nid, '=')
    ->fields('pb', array('id'))
    ->execute()
    ->fetchAssoc('nid');

  $node = NULL;
  if ($in_box) {
    // 在盒子中
    $query = db_select('node', 'n');
    $query->condition('n.nid', $nid, '=')
      ->condition('n.status', NODE_PUBLISHED, '=')
      ->condition('n.type', YIGOU_PRODUCT_CONTENT_TYPE, '=')
      ->fields('n');
    // 库存表
    $query->join('field_data_field_total_num', 'f2', 'f2.entity_id = n.nid');
    $query->addField('f2', 'field_total_num_value', 'stock');

    $query->join('field_data_field_price', 'f3', 'f3.entity_id = n.nid');
    $query->addField('f3', 'field_price_value', 'price');

    $node = $query->execute()->fetchAssoc('n.nid');
  }
  else {
    // 不在盒子中
    $query = db_select('node', 'n');
    $query->condition('n.nid', $nid, '=')
      ->condition('n.status', NODE_PUBLISHED, '=')
      ->condition('n.type', YIGOU_PRODUCT_CONTENT_TYPE, '=')
      ->fields('n');
    $query->join('field_data_field_product_valid_period', 'f1', 'f1.entity_id = n.nid');
    $query->condition('f1.field_product_valid_period_value', REQUEST_TIME, '<=')
      ->condition('f1.field_product_valid_period_value2', REQUEST_TIME, '>=');
    // 库存表
    $query->join('field_data_field_total_num', 'f2', 'f2.entity_id = n.nid');
    $query->addField('f2', 'field_total_num_value', 'stock');

    $query->join('field_data_field_price', 'f3', 'f3.entity_id = n.nid');
    $query->addField('f3', 'field_price_value', 'price');

    $node = $query->execute()->fetchAssoc('n.nid');
  }

  if (!$node) {
    return services_error(t('商品限时结束.'), 406);
  }

  // 得到所有订单
  $orders = _product_get_orders($nid);
  $num_of_orders = count($orders);
//  if ($num_of_orders > 0 && in_array($user->uid, array_keys($orders)) && !_users_is_developer($user->uid)) {
//    // 该用户已经购买过该商品
//    return services_error(t('对不起，您已购买过该商品'), 406);
//  }

  $stock = (int) $node['stock'];
  if ($num_of_orders >= $stock) {
    // 如果订单数额 >= 总库存数，售罄
    return services_error(t('商品已售罄.'), 406);
  }

//  if (!_users_is_developer($user->uid) && _check_user_purchased_today($user->uid)) {
//    return services_error(t('对不起，您今天已购买过其他商品.'), 406);
//  }

  $order = _order_create_a_order($user->uid, $nid, YIGOU_PRODUCT_CONTENT_TYPE, $node['price']);

  // 成功购买商品，删除盒子中收藏商品
  db_delete('product_box')
    ->condition('uid', $user->uid)
    ->condition('nid', $nid)
    ->execute();

  return ['message' => '购买成功', 'nid' => $nid, 'data' => $order];
}

/**
 * Apply a Share Product.
 * @param $nid
 * @return array|mixed
 */
function product_share_product_apply($nid) {
  global $user;
  if (empty($user->uid)) {
    @watchdog('juna_service::product_share_product_apply', 'ERROR::', array(), WATCHDOG_DEBUG);
    return services_error(t('用户未登录.'), 403);
  }

  $u = user_load($user->uid);
  if (empty($u->field_certed) || (empty($u->field_certed['und'][0]['value']))) {
    return services_error(t('请先完成认证'), 406);
  }
  elseif ($u->field_certed['und'][0]['value'] == 2) {
    return services_error(t('认证资料审核中'), 406);
  }

  // 确定该商品已经被收入盒子，如果已经收入，就没有时间限制了
  $in_box = db_select('product_box', 'pb')
    ->condition('pb.uid', $user->uid, '=')
    ->condition('pb.nid', $nid, '=')
    ->fields('pb', array('id'))
    ->execute()
    ->fetchAssoc('nid');

  $coupon = _coupon_check_coupon_available_by_uid($user->uid, REQUEST_TIME);
  if (!$coupon) {
    return services_error(t('领取失败，您没有白领券'), 406);
  }

  $node = NULL;
  if ($in_box) {
    // 在盒子中
    $query = db_select('node', 'n');
    $query->condition('n.nid', $nid, '=')
      ->condition('n.status', NODE_PUBLISHED, '=')
      ->condition('n.type', SHARE_PRODUCT_CONTENT_TYPE, '=')
      ->fields('n');
    // 库存表
    $query->join('field_data_field_total_num', 'f2', 'f2.entity_id = n.nid');
    $query->addField('f2', 'field_total_num_value', 'stock');

    $node = $query->execute()->fetchAssoc('n.nid');
  }
  else {
    // 不在盒子中
    $hour = (int) format_date(REQUEST_TIME, 'custom', 'H');
    if (8 > $hour && $hour >= 0) {
      watchdog('juna_service::product_share_product_apply', 'ERROR::商品限时结束::@sanity_check',
        array('@sanity_check' => $hour), WATCHDOG_DEBUG);
      return services_error(t('商品限时结束.'), 406);
    }

    $query = db_select('node', 'n');
    $query->condition('n.nid', $nid, '=')
      ->condition('n.status', NODE_PUBLISHED, '=')
      ->condition('n.type', SHARE_PRODUCT_CONTENT_TYPE, '=')
      ->fields('n');
    $query->join('field_data_field_product_valid_period', 'f1', 'f1.entity_id = n.nid');
    $query->condition('f1.field_product_valid_period_value', REQUEST_TIME, '<=')
      ->condition('f1.field_product_valid_period_value2', REQUEST_TIME, '>=');
    // 库存表
    $query->join('field_data_field_total_num', 'f2', 'f2.entity_id = n.nid');
    $query->addField('f2', 'field_total_num_value', 'stock');

    $node = $query->execute()->fetchAssoc('n.nid');
  }

  if (!$node) {
    return services_error(t('商品限时结束.'), 406);
  }

  // 得到所有订单
  $orders = _product_get_orders($nid);
  $num_of_orders = count($orders);
  if ($num_of_orders > 0 && in_array($user->uid, array_keys($orders))) {
    // 该用户已经申领过该商品
    return services_error(t('对不起，您已申领过该商品'), 406);
  }

  $stock = (int) $node['stock'];
  if ($num_of_orders >= $stock) {
    // 如果订单数额 >= 总库存数，售罄
    return services_error(t('商品已领完.'), 406);
  }

  $order = _order_create_a_order($user->uid, $nid, SHARE_PRODUCT_CONTENT_TYPE, 0);
  coupon_consume_a_coupon($coupon->nid, $nid, $user->uid, $order->sorderid, SHARE_PRODUCT_CONTENT_TYPE);

  // 删除盒子中收藏的白领商品
  db_delete('product_box')
    ->condition('uid', $user->uid)
    ->condition('nid', $nid)
    ->execute();
  return ['message' => '申请成功', 'nid' => $nid, 'data' => $order];
}

/**
 * Enroll a yaoxiang product.
 * @return array|mixed
 */
function product_yaoxiang_product_enrollment() {
  global $user;
  if (!$user->uid) {
    return services_error(t('用户未登录'), 403);
  }

  $u = user_load($user->uid);
  if (empty($u->field_certed) || (empty($u->field_certed['und'][0]['value']))) {
    return services_error(t('请先完成认证'), 406);
  }
  elseif ($u->field_certed['und'][0]['value'] == 2) {
    return services_error(t('认证资料审核中'), 406);
  }

  $hour = (int) format_date(REQUEST_TIME, 'custom', 'H');
  if (8 > $hour && $hour >= 22 && !_users_is_developer($user->uid)) {
    return services_error(t('摇享已结束.'), 406);
  }

  $coupon = _coupon_check_coupon_available_by_uid($user->uid, REQUEST_TIME);
  if (!$coupon) {
    return services_error(t('领取失败，您没有白领券'), 406);
  }


  $node = _product_get_current_yaoxiang();
  if (empty($node)) {
    return services_error(t('暂无摇享商品'), 406);
  }
  $node = array_pop($node);

  $enrollments = _get_all_yaoxiang_enrollment_by_nid($node->nid);
  if (in_array($user->uid, array_keys($enrollments))) {
    return services_error(t('您今天已参加摇享'), 406);
  }

  $record = new stdClass();
  $record->uid = $user->uid;
  $record->nid = $node->nid;
  $record->swingnum = 'NO.' . (count($enrollments) + 1);
  $record->created = REQUEST_TIME;
  $record->pdate = format_date(REQUEST_TIME, 'custom', 'Y-m-d');
  $record->title = $node->type;
  $record->status = 0;
  $record->couponid = $coupon->nid;
  drupal_write_record('user_wingshare_log', $record);

  coupon_freeze_a_coupon($coupon->nid);

  return ['data' => $record];
}

/**
 * Helper function to get current available yaoxiang product.
 * @return mixed
 */
function _product_get_current_yaoxiang() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->entityCondition('bundle', array(YAOXIANG_PRODUCT_CONTENT_TYPE, NEW_YAOXIANG_PRODUCT_CONTENT_TYPE))
    ->fieldCondition('field_product_valid_period', 'value', REQUEST_TIME, '<=')
    ->fieldCondition('field_product_valid_period', 'value2', REQUEST_TIME, '>=')
    ->range(0, 1);
  $result = $query->execute();
  return $result['node'];
}

/**
 * Helper function to get all of enrollment by product nid.
 * @param $nid
 * @return mixed
 */
function _get_all_yaoxiang_enrollment_by_nid($nid) {
  $query = db_select('user_wingshare_log', 'uwl');
  $query->condition('uwl.nid', $nid, '=')
    ->fields('uwl');
  return $query->execute()->fetchAllAssoc('uid');
}

/**
 * Get all orders related to nid.
 * @param $nid
 * @return mixed
 */
function _product_get_orders($nid) {
  $query = db_select('product_order', 'po')
    ->condition('po.nid', $nid, '=')
    ->condition('po.status', ORDER_STATUS_CANCEL, '<>')
    ->fields('po', array('uid'));
  return $query->execute()->fetchAllAssoc('uid');
}

/**
 * To check whether the user has purchased or not.
 * @param $uid
 * @return mixed
 */
function _check_user_purchased_today($uid) {
  $start = strtotime('today 00:00:00');
  $end = strtotime('today 23:59:59');
  $query = db_select('product_order', 'po')
    ->fields('po', array('id'))
    ->condition('po.uid', $uid, '=')
    ->condition('po.product_type', YIGOU_PRODUCT_CONTENT_TYPE, '=')
    ->condition('po.created', array($start, $end), 'BETWEEN');
  return count($query->execute()->fetchAllAssoc('id'));
}
