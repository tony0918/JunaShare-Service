<?php

/**
 * Implementation of admin/juna/orders menu callback.
 * @return null|string
 */
function orders_admin_juna_orders_index() {
  $form = drupal_get_form('orders_juna_export_comments_form');
  return drupal_render($form);
}

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
    '#date_format' => DATE_FORMAT_DATETIME,
    '#date_timezone' => $timezone,
    '#default_value' => $start,
    '#date_year_range' => '-3:+3',
  );
  $form['filters']['end'] = array(
    '#type' => 'date_popup',
    '#title' => t('End'),
    '#date_type' => DATE_DATETIME,
    '#date_format' => DATE_FORMAT_DATETIME,
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
  $form['actions']['export'] = array(
    '#type' => 'button',
    '#value' => t('Export'),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('orders_order_list_export'),
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
    'delivery_time' => t('Delivery Time'),
  );

  if (!isset($_SESSION['order_list_filters'])) {
    $_SESSION['order_list_filters'] = array();
  }

  $query = _orders_order_list_filter_build_query($_SESSION['order_list_filters']);
  $query->extend('PagerDefault')->limit(100)
    ->extend('TableSort')->orderByHeader($header);
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
    $rows[$key]['delivery_time'] = $value->delivery_time ? format_date($value->delivery_time) : '';
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

/**
 * The order detail and edit page.
 * @param $form
 * @param $form_state
 * @param $oid
 * @return mixed
 */
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

/**
 * Implementation of the order edit form submit callback.
 * @param $form
 * @param $form_state
 */
function orders_admin_juna_orders_detail_edit_form_submit($form, &$form_state) {
  $fields = array(
    'status' => $form_state['values']['order_status'],
    'delivery_time' => REQUEST_TIME,
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

/**
 * Build a filter query depends on filter form values.
 * @param $values
 * @return $this|\SelectQueryInterface
 */
function _orders_order_list_filter_build_query($values) {
  $status_name = array('待付款', '已付款', '待发货', '待收货', '已评价', '交易完成', '待评价', '已取消');
  $order_types = array('share_product' => '申领', 'product' => '购买', 'swingshare' => '摇享');
  $query = db_select('product_order', 'po')
    ->fields('po', array('sorderid', 'nid', 'uid', 'created', 'status', 'delivery_time', 'arrival_time'));

  $query->join('node', ' n', 'po.nid = n.nid');
  $query->condition('n.status', NODE_PUBLISHED, '=');
  $query->fields('n', array('title', 'type'));

  $query->join('users', 'u', 'po.uid=u.uid');
  $query->addField('u', 'name');

  if (!empty($values['mobile'])) {
    $query->condition('u.name', trim($values['mobile']), '=');
  }

  if (!empty($values['nid'])) {
    $query->condition('po.nid', trim($values['nid']), '=');
  }

  if (isset($values['order_type']) && in_array($values['order_type'], array_keys($order_types))) {
    $query->condition('po.product_type', $values['order_type'], '=');
  }

  if (isset($values['order_status']) && in_array($values['order_status'] - 1, array_keys($status_name))) {
    $query->condition('po.status', $values['order_status'] - 1);
  }

  if (isset($values['start']) && !empty(trim($values['start']))) {
    $query->condition('po.created', strtotime($values['start']), '>=');
  }

  if (isset($values['end']) && !empty(trim($values['end']))) {
    $query->condition('po.created', strtotime($values['end']), '<=');
  }

  $query->leftJoin('field_data_field_username', 'fdfu', 'po.uid = fdfu.entity_id');
  $query->addField('fdfu', 'field_username_value');

  $query->leftJoin('field_data_field_company_name', 'fdfcn', 'po.uid = fdfcn.entity_id');
  $query->addField('fdfcn', 'field_company_name_value');

  $query->leftJoin('field_data_field_company_address', 'fdfca', 'po.uid = fdfca.entity_id');
  $query->addField('fdfca', 'field_company_address_value');

  $query->leftJoin('coupon_usage', 'cu', 'cu.sorderid=po.sorderid');
  $query->addField('cu', 'couponid');

  return $query;
}

/**
 * Implementation of order list export callback.
 * @param $form
 * @param $form_state
 */
function orders_order_list_export($form, &$form_state) {
  $query = _orders_order_list_filter_build_query($form_state['values']);
  $result = $query->execute()->fetchAllAssoc('sorderid');
  _orders_export_to_cvs('订单记录', $result);
}

function _orders_export_to_cvs($filename, $data = array()) {
  $fp = fopen('php://output', 'w');
  fputs($fp, $bom = (chr(0xEF) . chr(0xBB) . chr(0xBF)));
  $header = array('手机', '名字', '公司名字', '公司地址', '商品名称');
  // 将数据通过fputcsv写到文件句柄
  fputcsv($fp, $header);
  $cnt = 0;
  $limit = 8000;
  header('Content-Type: application/vnd.ms-excel; charset=UTF-8');
  header('Content-Disposition: attachment;filename="' . urlencode($filename) . '.csv');
  header('Cache-Control: max-age=0');
  header("Content-Transfer-Encoding: binary");
  // 打开PHP文件句柄，php://output 表示直接输出到浏览器
  foreach ($data as $key => $value) {
    if ($limit == $cnt) { // 刷新一下输出buffer，防止由于数据过多造成问题
      ob_flush();
      flush();
      $cnt = 0;
    }
    $content = array(
      $value->name,
      $value->field_username_value,
      $value->field_company_name_value,
      $value->field_company_address_value,
      $value->title
    );
    fputcsv($fp, $content);
  }
  fclose($fp);
  exit;
}


/**
 * Build export form
 * @return mixed
 */
function orders_juna_export_comments_form() {
  $form['export']['export_comments'] = array(
    '#type' => 'submit',
    '#value' => t('Export Comments'),
    '#submit' => array('orders_export_comments_submit'),
  );
  return $form;
}

/**
 * export form submit callback
 */
function orders_export_comments_submit() {
  $query = db_select('product_order', 'po')
    ->condition('po.comment', '', '!=')
    ->fields('po', array('id', 'comment'))
    ->orderBy('id', 'DESC')
    ->range(0, 8000);

  $query->join('node', 'n', 'po.nid = n.nid');
  $query->fields('n', array('title'));

  $query->join('users', 'u', 'po.uid = u.uid');
  $query->addField('u', 'name', 'mobile');

  $query->join('field_data_field_username', 'f1', 'po.uid = f1.entity_id');
  $query->addField('f1', 'field_username_value', 'full_name');

  $query->join('field_data_field_company_name', 'f2', 'po.uid = f2.entity_id');
  $query->addField('f2', 'field_company_name_value', 'company');

  $rows = $query->execute()->fetchAllAssoc('id');

  $fp = fopen('php://output', 'w');
  fputs($fp, $bom = (chr(0xEF) . chr(0xBB) . chr(0xBF)));
  $header = array('Order Id', 'Product', 'Name', 'Company', 'Mobile', 'Comment');
  // 将数据通过fputcsv写到文件句柄
  fputcsv($fp, $header);
  $cnt = 0;
  $limit = 8000;
  header('Content-Type: application/vnd.ms-excel; charset=UTF-8');
  header('Content-Disposition: attachment;filename="' . urlencode('comments') . '.csv');
  header('Cache-Control: max-age=0');
  header("Content-Transfer-Encoding: binary");
  // 打开PHP文件句柄，php://output 表示直接输出到浏览器
  foreach ($rows as $key => $value) {
    if ($limit == $cnt) { // 刷新一下输出buffer，防止由于数据过多造成问题
      ob_flush();
      flush();
      $cnt = 0;
    }
    $content = array(
      $value->id,
      $value->title,
      $value->full_name,
      $value->company,
      $value->mobile,
      $value->comment
    );
    fputcsv($fp, $content);
  }
  fclose($fp);
  exit;
}

function orders_admin_juna_orders_updatepackage() {
  set_time_limit(0);
  $csv = drupal_realpath(file_default_scheme() . '://csv/package.csv');
  $num = 0;
  if (file_exists($csv)) {
    $csv = fopen($csv, 'r');
    $start = 1508083200;

    while (!feof($csv)) {
      $data = fgetcsv($csv);
      $mobile = preg_replace('~[^0-9]+~i', '', $data[0]);
      $provider = $data[6];
      $number = $data[7];

      $query = db_select('users', 'u')
        ->condition('u.name', $mobile);
      $query->join('product_order', 'po', 'po.uid = u.uid');
      $query->condition('po.created', $start, '<')
        ->condition('po.status', ORDER_STATUS_UNDELIVERED)
        ->fields('po');

      $result = $query->execute()->fetchAll();

      $transaction = db_transaction();
      foreach ($result as $key => $value) {
        db_update('product_order')
          ->fields(array(
            'express_provider' => $provider,
            'express_id' => $number,
            'updated' => REQUEST_TIME,
            'delivery_time' => REQUEST_TIME,
            'status' => ORDER_STATUS_DELIVERED
          ))
          ->condition('id', $value->id, '=')
          ->execute();
        $num++;
      }
      unset($transaction);
    }

    fclose($csv);
  }
  return 'OK ' . $num;
}
