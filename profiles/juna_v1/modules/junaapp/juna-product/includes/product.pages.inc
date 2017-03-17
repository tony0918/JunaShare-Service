<?php

/**
 * Implementation of admin/juna/product page callback
 * @return null|string
 */
function product_admin_juna_product_index() {
  return t('Welcome to product management portal.');
}

function product_admin_juna_product_list_filter_form($form, &$form_state) {
  $now = REQUEST_TIME;
  $form['starttime'] = array(

    '#type' => 'textfield',

    '#title' => t('开始时间'),

    '#default_value' => empty($_SESSION['product_applylist_filter']['starttime']) ? '' : $_SESSION['product_applylist_filter']['starttime'],

    '#size' => 60,

    '#maxlength' => 128,

    '#description' => '日期格式为1990-2-1'

  );
  $form['endtime'] = array(

    '#type' => 'textfield',

    '#title' => t('结束时间时间'),

    '#default_value' => empty($_SESSION['product_applylist_filter']['endtime']) ? '' : $_SESSION['product_applylist_filter']['endtime'],

    '#size' => 60,

    '#maxlength' => 128,

    '#description' => '日期格式为1990-2-1'

  );
  $form['uid'] = array(

    '#type' => 'textfield',

    '#title' => t('用户id'),

    '#default_value' => empty($_SESSION['product_applylist_filter']['uid']) ? '' : $_SESSION['product_applylist_filter']['uid'],


    '#size' => 60,

    '#maxlength' => 128,


  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('搜索'));
  $form['export'] = array('#type' => 'submit', '#value' => t('导出搜索结果'));
}

function product_admin_juna_product_list() {
  $header = array(
    'id' => array('data' => t('id'), 'field' => 'usl.id', 'sort' => 'desc'),
    'nid' => array('data' => t('商品id'), 'field' => 'usl.nid'),
    'uid' => array('data' => t('用户id'), 'field' => 'usl.uid'),
    'productname' => t('商品名称'),
    'created' => t('申领时间'),
    'mobile' => t('手机号'),
  );
  $query = db_select('product_order', 'usl')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('usl', array('id', 'nid', 'uid'))
    ->orderByHeader($header);
  $query->leftJoin('node', 'n', 'n.nid=usl.nid');
  $query->fields('n', array('title'));
  $query->fields('usl', array('created'));
  $query->leftJoin('users', 'u', 'usl.uid=u.uid');
  $query->fields('u', array('name'));
  $query->condition('usl.product_type', 'share_product');
  $filters = ['usl.product_type', 'product_share'];
  if (!empty($_SESSION['product_applylist_filter'])) {
    $filters = $_SESSION['product_applylist_filter'];

  }
  $valid = ['starttime', 'endtime', 'uid'];
  foreach ($filters as $k => $v) {
    if (in_array($k, $valid)) {
      if ($k == 'starttime') {
        $query->condition('usl.created', strtotime($v), '>=');
      }
      elseif ($k == 'endtime') {
        $query->condition('usl.created', strtotime(date('Y-m-d 23:59:59', strtotime($v))), '<=');
      }
      else {
        $query->condition('usl.' . $k, $v);
      }
    }

  }
  $result = $query->execute();
  $rows = [];
  $destination = drupal_get_destination();
  while ($record = $result->fetchAssoc()) {
    $record['nid'] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => $record['nid'],
        '#href' => '/node/' . $record['nid'],
        '#options' => array('query' => $destination),
        //'#query' => $,
      )
    );
    $record['uid'] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => $record['uid'],
        '#href' => '/user/' . $record['uid'],
        '#options' => array('query' => $destination),
        //'#query' => $,
      )
    );
    $record['created'] = date('Y-m-d H:i:s', $record['created']);
    $rows[] = $record;
  }
  //print_r($rows);exit;
  $form['nodes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('暂无申请记录'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}
