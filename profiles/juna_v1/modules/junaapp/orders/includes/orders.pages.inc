<?php

/**
 * Implementation of orders_admin_order_list_filter_form().
 * @param $form
 * @param $form_state
 * @return mixed
 */
function orders_admin_order_list_filter_form($form, &$form_state) {
  $mobile = isset($_SESSION['order_list_filters']['mobile']) ? $_SESSION['order_list_filters']['mobile'] : '';
  $nid = isset($_SESSION['order_list_filters']['nid']) ? $_SESSION['order_list_filters']['nid'] : '';
  $timezone = variable_get('date_default_timezone', @date_default_timezone_get());
  $format = 'Y-m-d';
  $start = isset($_SESSION['order_list_filters']['start']) ? $_SESSION['order_list_filters']['start'] : '';
  $end = isset($_SESSION['order_list_filters']['end']) ? $_SESSION['order_list_filters']['end'] : '';
  $order_status = isset($_SESSION['order_list_filters']['order_status']) ? $_SESSION['order_list_filters']['order_status'] : '';
  $order_type = isset($_SESSION['order_list_filters']['order_type']) ? $_SESSION['order_list_filters']['order_type'] : '';

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  $form['filters']['start'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start'),
    '#date_type' => DATE_DATETIME,
    '#data_format' => $format,
    '#date_timezone' => $timezone,
    '#default_value' => $start,
    '#date_year_range' => '-3:+3',
  );
  $form['filters']['end'] = array(
    '#type' => 'date_popup',
    '#title' => t('End'),
    '#date_type' => DATE_DATETIME,
    '#data_format' => $format,
    '#date_timezone' => $timezone,
    '#default_value' => $end,
    '#date_year_range' => '-3:+3',
  );
  $form['filters']['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Number'),
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $mobile,
  );
  $form['filters']['nid'] = array(
    '#type' => 'textfield',
    '#title' => t('商品nid'),
    '#default_value' => $nid,
  );
  $form['filters']['order_status'] = array(
    '#type' => 'select',
    '#title' => t('Order Status'),
    '#default_value' => $order_status,
    '#options' => array('全部', '待付款', '已付款', '待发货', '待收货', '已评价', '交易完成', '待评价', '已取消')
  );
  $form['filters']['order_type'] = array(
    '#type' => 'select',
    '#title' => t('Order Type'),
    '#default_value' => $order_type,
    '#options' => array('all' => '全部', 'share_product' => '申领', 'product' => '购买', 'swingshare' => '摇享')
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter')
  );
  return $form;
}

/**
 * Implementation of menu item admin/juna/orders/list callback.
 * @param $form
 * @param $form_state
 * @return array|mixed
 */
function orders_admin_list($form, &$form_state) {
  $status_name = array('待付款', '已付款', '待发货', '待收货', '已评价', '交易完成', '待评价', '已取消');
  $order_types = array('share_product' => '申领', 'product' => '购买', 'swingshare' => '摇享');
  $header = array(
    'sorderid' => array(
      'data' => t('Order ID'),
      'field' => 'po.sorderid',
    ),
    'title' => t('Product'),
    'uid' => t('Mobile'),
//    'weixin_order_id' => array('data' => t('微信订单号'), 'field' => 'po.weixin_order_id'),
    'status' => array(
      'data' => t('Status'),
      'field' => 'po.status'
    ),
    'order_type' => t('Order Type'),
    'coupon_id' => t('Coupon ID'),
    'created' => array(
      'data' => t('Date'),
      'field' => 'po.created',
      'sort' => 'desc',
    ),
  );

  $query = db_select('product_order', 'po')
    ->extend('PagerDefault')->limit(100)
    ->extend('TableSort')->orderByHeader($header)
    ->fields('po', array('sorderid', 'nid', 'uid', 'created', 'status'));

  $query->join('node', ' n', 'po.nid = n.nid');
  $query->condition('n.status', NODE_PUBLISHED, '=');
  $query->fields('n', array('title', 'type'));

  $query->join('users', 'u', 'po.uid=u.uid');
  $query->addField('u', 'name');

  $query->leftJoin('coupon_usage', 'cu', 'cu.sorderid=po.sorderid');
  $query->addField('cu', 'couponid');

  if (isset($_SESSION['order_list_filters']['mobile']) && !empty(trim($_SESSION['order_list_filters']['mobile']))) {
    $query->condition('u.name', trim($_SESSION['order_list_filters']['mobile']), '=');
  }

  if (isset($_SESSION['order_list_filters']['nid']) && !empty(trim($_SESSION['order_list_filters']['nid']))) {
    $query->condition('po.nid', trim($_SESSION['order_list_filters']['nid']), '=');
  }

  if (isset($_SESSION['order_list_filters']['order_type'])
    && in_array($_SESSION['order_list_filters']['order_type'], array_keys($order_types))
  ) {
    $query->condition('po.product_type', $_SESSION['order_list_filters']['order_type'], '=');
  }

  if (isset($_SESSION['order_list_filters']['order_status'])
    && in_array($_SESSION['order_list_filters']['order_status'] - 1, array_keys($status_name))
  ) {
    $query->condition('po.status', $_SESSION['order_list_filters']['order_status'] - 1);
  }

  if (isset($_SESSION['order_list_filters']['start']) && !empty(trim($_SESSION['order_list_filters']['start']))) {
    $query->condition('po.created', strtotime($_SESSION['order_list_filters']['start']), '>=');
  }

  if (isset($_SESSION['order_list_filters']['end']) && !empty(trim($_SESSION['order_list_filters']['end']))) {
    $query->condition('po.created', strtotime($_SESSION['order_list_filters']['end']), '<=');
  }

  $result = $query->execute()->fetchAll();
  $rows = [];
  foreach ($result as $key => $value) {
    $rows[$key]['sorderid'] = l($value->sorderid, "admin/juna/orders/detail/$value->sorderid/edit");
    $rows[$key]['title'] = l("(nid:$value->nid) $value->title", 'node/' . $value->nid);
    $rows[$key]['uid'] = l($value->name, 'user/' . $value->uid);
    $rows[$key]['status'] = $status_name[$value->status];
    $rows[$key]['order_type'] = $order_types[$value->type];
    $rows[$key]['coupon_id'] = l($value->couponid, "node/$value->couponid");
    $rows[$key]['created'] = format_date($value->created);
  }

  $form = orders_admin_order_list_filter_form($form, $form_state);
  $form['filters']['#title'] .= ' ' . t('Total Matched Record') . ' (' . count($rows) . ')';
  $form['nodes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('暂无订单.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Implementation of orders_admin_list_submit().
 * @param $form
 * @param $form_state
 */
function orders_admin_list_submit($form, &$form_state) {
  $_SESSION['order_list_filters'] = $form_state['values'];
}


function orders_admin_juna_orders_detail_edit_form($form, &$form_state, $oid) {
  $query = db_select('product_order', 'po')
    ->condition('po.sorderid', $oid, '=')
    ->fields('po');

  $query->join('node', ' n', 'po.nid = n.nid');
  $query->fields('n', array('title', 'type'));

  $query->leftJoin('coupon_usage', 'cu', 'cu.sorderid=po.sorderid');
  $query->addField('cu', 'couponid');

  $result = $query->execute()->fetchAssoc();

  $user = user_load($result['uid']);

  $form['order_id'] = array(
    '#type' => 'item',
    '#title' => t('Order ID'),
    '#markup' => $oid,
  );
  $form['oid'] = array(
    '#type' => 'value',
    '#value' => $oid,
  );
  $form['product'] = array(
    '#type' => 'item',
    '#title' => t('Product'),
    '#markup' => $result['title'],
  );
  $form['mobile'] = array(
    '#type' => 'item',
    '#title' => t('Customer Mobile'),
    '#markup' => $user->name,
  );
  $form['name'] = array(
    '#type' => 'item',
    '#title' => t('Customer Name'),
    '#markup' => $user->field_username[LANGUAGE_NONE][0]['safe_value'],
  );
  $form['company'] = array(
    '#type' => 'item',
    '#title' => t('Customer Company'),
    '#markup' => $user->field_company_name[LANGUAGE_NONE][0]['safe_value'],
  );
  $form['address'] = array(
    '#type' => 'item',
    '#title' => t('Customer Address'),
    '#markup' => $user->field_company_address[LANGUAGE_NONE][0]['safe_value'],
  );
  $form['created'] = array(
    '#type' => 'item',
    '#title' => t('Order Date'),
    '#markup' => format_date($result['created']),
  );
  $form['order_status'] = array(
    '#type' => 'select',
    '#title' => t('Order Status'),
    '#default_value' => $result['status'],
    '#options' => array('待付款', '已付款', '待发货', '待收货', '已评价', '交易完成', '待评价', '已取消'),
  );
  $form['express'] = array(
    '#type' => 'fieldset',
    '#title' => t('Express Information'),
    '#states' => array(
      'visible' => array(
        ':input[name="order_status"]' => array(
          'value' => 3
        )
      )
    ),
  );
  $form['express']['express_provider'] = array(
    '#type' => 'textfield',
    '#title' => t('Express Provider'),
    '#default_value' => $result['express_provider'],
  );
  $form['express']['express_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Express ID'),
    '#default_value' => $result['express_id'],
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function orders_admin_juna_orders_detail_edit_form_submit($form, &$form_state) {
  $fields = array(
    'status' => $form_state['values']['order_status'],
    'updated' => REQUEST_TIME,
  );
  if (ORDER_STATUS_DELIVERED == $form_state['values']['order_status']) {
    $fields = array_merge($fields, array(
      'express_id' => $form_state['values']['express_id'],
      'express_provider' => $form_state['values']['express_provider'],
    ));
  }

  db_update('product_order')
    ->condition('sorderid', $form_state['values']['oid'], '=')
    ->fields($fields)->execute();

  drupal_set_message(t('Update Successfully.'));
  $form_state['redirect'] = array(
    'admin/juna/orders/list',
    array(),
    302,
  );
}
