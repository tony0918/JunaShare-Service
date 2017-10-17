<?php

function users_user_portal_index() {
  $variables = [
    'title' => t('User Status Overview'),
    'type' => 'ul',
    'attributes' => [],
    'items' => [],
  ];

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->propertyCondition('status', 1);
  $num_rows = $query->count()->execute();

  $query2 = clone($query)->fieldCondition('field_certed', 'value', 1, '=');
  $num_rows_1 = $query2->count()->execute();

  $items = [
    [
      'data' => t('总用户数： @user_num', ['@user_num' => $num_rows]),
    ],
    [
      'data' => t('已认证用户数：@user_num_1', ['@user_num_1' => $num_rows_1]),
    ],
  ];

  $variables['items'] = $items;
  $form = drupal_get_form('users_juna_export_user_form');
  return theme_item_list($variables) . drupal_render($form);

}

/**
 * Build export form
 *
 * @return mixed
 */
function users_juna_export_user_form() {
  $form['export']['export_certed_user_info'] = [
    '#type' => 'submit',
    '#value' => t('Export Certed User Info'),
    '#submit' => ['export_certed_user_info_submit'],
  ];
  return $form;
}

function export_certed_user_info_submit() {
  set_time_limit(0);
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_certed', 'value', 1, '=');
  $result = $query->execute();

  $uids = array_keys($result['user']);
  $rows = entity_load('user', $uids);
  $arr = [];
  foreach ($rows as $key => $value) {
    $arr[$key]['uid'] = $value->uid;
    $arr[$key]['mobile'] = $value->name;
    $arr[$key]['full_name'] = empty($value->field_username) ? '' : $value->field_username[LANGUAGE_NONE][0]['safe_value'];
    $arr[$key]['company'] = empty($value->field_company_name) ? '' : $value->field_company_name[LANGUAGE_NONE][0]['safe_value'];
    $arr[$key]['address'] = empty($value->field_company_address) ? '' : $value->field_company_address[LANGUAGE_NONE][0]['safe_value'];
    $arr[$key]['image'] = empty($value->field_cert) ? '' : file_create_url($value->field_cert[LANGUAGE_NONE][0]['uri']);
  }

  $fp = fopen('php://output', 'w');
  fputs($fp, $bom = (chr(0xEF) . chr(0xBB) . chr(0xBF)));
  $header = ['id', '手机', '名字', '公司名字', '公司地址', 'images'];
  // 将数据通过fputcsv写到文件句柄
  fputcsv($fp, $header);
  $cnt = 0;
  $limit = 8000;
  header('Content-Type: application/vnd.ms-excel; charset=UTF-8');
  header('Content-Disposition: attachment;filename="' . urlencode('user_info') . '.csv');
  header('Cache-Control: max-age=0');
  header("Content-Transfer-Encoding: binary");
  // 打开PHP文件句柄，php://output 表示直接输出到浏览器
  foreach ($arr as $key => $value) {
    if ($limit == $cnt) { // 刷新一下输出buffer，防止由于数据过多造成问题
      ob_flush();
      flush();
      $cnt = 0;
    }
    $content = [
      $value['uid'],
      $value['mobile'],
      $value['full_name'],
      $value['company'],
      $value['address'],
      $value['image'],
    ];
    fputcsv($fp, $content);
  }
  fclose($fp);
  exit;
}

/**
 * Implementation of form users_juna_admin_list.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function users_juna_admin_list($form, &$form_state) {
  $form['filters'] = [
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  ];
  $form['filters']['mobile'] = [
    '#type' => 'textfield',
    '#title' => t('Mobile Number'),
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => empty($_SESSION['users_juna_admin_list_filter']['mobile']) ? '' : $_SESSION['users_juna_admin_list_filter']['mobile'],
  ];
  $form['filters']['cert'] = [
    '#type' => 'select',
    '#title' => t('认证状态'),
    '#options' => [
      0 => t('未认证'),
      1 => t('已认证'),
      2 => t('认证中'),
      3 => t('全部'),
    ],
    '#default_value' => (isset($_SESSION['users_juna_admin_list_filter']['cert']) && $_SESSION['users_juna_admin_list_filter']['cert'] >= 0) ? $_SESSION['users_juna_admin_list_filter']['cert'] : 3,
  ];
  $form['filters']['submit'] = ['#type' => 'submit', '#value' => t('Filter')];

  $header = [
    'uid' => [
      'data' => t('uid'),
      'type' => 'property',
      'specifier' => 'uid',
      'sort' => 'desc',
    ],
    'mobile' => t('Mobile'),
    'full_name' => t('Full Name'),
    'company' => t('Company'),
    'cert' => [
      'data' => t('Cert Status'),
      'type' => 'field',
      'specifier' => ['field' => 'field_status', 'column' => 'value'],
    ],
    'access' => t('Last Access'),
    'created' => t('Register'),
  ];

  if (user_access('user_admin_permissions')) {
    $header['operation'] = t('Operation');
  }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->propertyCondition('status', 1)
    ->pager(100)
    ->tableSort($header);

  if (!empty($_SESSION['users_juna_admin_list_filter']['mobile'])) {
    $query->propertyCondition('name', $_SESSION['users_juna_admin_list_filter']['mobile'], '=');
  }

  if (isset($_SESSION['users_juna_admin_list_filter']['cert'])
    && in_array($_SESSION['users_juna_admin_list_filter']['cert'], [0, 1, 2])
  ) {
    $query->fieldCondition('field_certed', 'value', $_SESSION['users_juna_admin_list_filter']['cert'], '=');
  }

  $result = $query->execute();
  $rows = [];
  if (isset($result['user'])) {
    $uids = array_keys($result['user']);
    $re = entity_load('user', $uids);
    $cert_arr = ['未认证', '已认证', '认证中'];
    $op = user_access('user_admin_permissions');
    foreach ($re as $key => $value) {
      $rows[$key]['uid'] = l($value->uid, "user/$value->uid", ['attributes' => ['target' => '_blank']]);
      $rows[$key]['mobile'] = $value->name;
      $rows[$key]['full_name'] = empty($value->field_username) ? '' : $value->field_username[LANGUAGE_NONE][0]['safe_value'];
      $rows[$key]['company'] = empty($value->field_company_name) ? '' : $value->field_company_name[LANGUAGE_NONE][0]['safe_value'];
      $rows[$key]['cert'] = empty($value->field_certed) ? '' : $cert_arr[$value->field_certed[LANGUAGE_NONE][0]['value']];
      $rows[$key]['access'] = format_date($value->access);
      $rows[$key]['created'] = format_date($value->created, 'short');
      if ($op) {
        $rows[$key]['operation'] = l('Edit', "user/$value->uid/edit", ['attributes' => ['target' => '_blank']]);
      }
    }
  }
  $form['filters']['#title'] .= ' ' . t('Total Matched Record') . ' (' . count($rows) . ')';
  $form['nodes'] = [
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('暂无用户.'),
  ];
  $form['pager'] = ['#markup' => theme('pager')];
  $form['#attached']['css'] = [
    drupal_get_path('module', 'users') . '/users.css',
  ];
  return $form;
}

function users_juna_admin_list_submit($form, &$form_state) {
  $_SESSION['users_juna_admin_list_filter']['mobile'] = trim($form_state['values']['mobile']);
  $_SESSION['users_juna_admin_list_filter']['cert'] = trim($form_state['values']['cert']);
}

function users_juna_mock_users() {
  //  $arr = array(
  //    130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
  //    144, 147,
  //    150, 151, 152, 153, 155, 156, 157, 158, 159,
  //    176, 177, 178,
  //    180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
  //  );
  //  $tmp = array();
  //  for ($i = 0; $i < 1000; $i++) {
  //    $tmp[] = $arr[array_rand($arr)] . mt_rand(100, 999) . 'O' . mt_rand(1000, 9999);
  //  }
  //  foreach ($tmp as $key => $value) {
  //    $account = new stdClass();
  //    $account->is_new = TRUE;
  //    $account->mail = $value . '@mockuser.junashare.com';
  //    $account->name = $value;
  //    $account->status = 1;
  //    user_save($account);
  //  }

  // Make certed user.
  $csv = drupal_realpath(file_default_scheme() . '://csv/user2.csv');

  if (file_exists($csv)) {
    $csv = fopen($csv, 'r');

    while (!feof($csv)) {
      $data = fgetcsv($csv);
      $uid = (int) filter_var($data[0], FILTER_SANITIZE_NUMBER_INT);
      $uri = file_default_scheme() . '://csv/userimgs/' . $data[4] . '.jpg';
      $file = db_select('file_managed', 'fm')
        ->condition('fm.uri', $uri)
        ->fields('fm')->execute()->fetchAssoc();
      if ($file && isset($file['fid'])) {
        $file = file_load($file['fid']);
        file_usage_add($file, 'users', 'user', $uid);
      }
      else {
        $file = new stdClass();
        $file->fid = NULL;
        $file->uri = $uri;
        $file->filename = drupal_basename($uri);
        $file->filemime = file_get_mimetype($uri);
        $file->uid = $uid;
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
      }
      $account = user_load($uid);
      $account->field_username['und']['0']['value'] = $data[1];
      $account->field_company_name['und'][0]['value'] = $data[2];
      $account->field_company_address['und'][0]['value'] = $data[3];
      $account->field_cert['und'][0] = (array) $file;
      $account->field_cert_type['und'][0]['value'] = 1;
      $account->field_certed['und'][0]['value'] = 1;
      $edit['pass'] = '123456';
      user_save($account, $edit);
    }

    fclose($csv);
  }
  return 'ok';
}

/**
 * 批量认证用户并发券
 *
 * @return string
 */
function users_juna_users_batched_verify() {
  set_time_limit(0);
  module_load_include('inc', 'coupon', 'includes/coupon_helper');
  $csv = drupal_realpath(file_default_scheme() . '://csv/user_verify.csv');
  if (file_exists($csv)) {
    $csv = fopen($csv, 'r');
    while (!feof($csv)) {
      $data = fgetcsv($csv);

      // Fetch user by userId
      //      $uid = (int) filter_var($data[0], FILTER_SANITIZE_NUMBER_INT);

      // Fetch user by username (mobile)
      $mobile = filter_var($data[0], FILTER_SANITIZE_NUMBER_INT);
      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'user')
        ->propertyCondition('status', 1)
        ->propertyCondition('name', $mobile)
        ->execute();
      $user = array_pop($result['user']);
      $uid = $user->uid;

      $account = user_load($uid);

      // For user information update
      $account->field_username['und'][0]['value'] = $data[1];
      $account->field_company_name['und'][0]['value'] = $data[2];
      $account->field_company_address['und'][0]['value'] = $data[3];

      // For user certification
      //      $account->field_certed['und'][0]['value'] = 1;

      user_save($account);
      //      coupon_helper_deliver_a_coupon($uid,
      //        strtotime('2017-10-09 00:00:00'),
      //        strtotime('2017-10-15 23:59:59')
      //      );
    }

    fclose($csv);
  }

  return 'ok';
}
