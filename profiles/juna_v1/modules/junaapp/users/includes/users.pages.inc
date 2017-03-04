<?php

/**
 * Implementation of form users_juna_admin_list.
 * @param $form
 * @param $form_state
 * @return mixed
 */
function users_juna_admin_list($form, &$form_state) {
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  $form['filters']['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile Number'),
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => empty($_SESSION['users_juna_admin_list_filter']['mobile']) ? '' : $_SESSION['users_juna_admin_list_filter']['mobile'],
  );
  $form['filters']['cert'] = array(
    '#type' => 'select',
    '#title' => t('认证状态'),
    '#options' => array(
      0 => t('未认证'),
      1 => t('已认证'),
      2 => t('认证中'),
      3 => t('All')
    ),
    '#empty_value' => '请选择',
    '#default_value' => empty($_SESSION['users_juna_admin_list_filter']['cert']) ? '' : $_SESSION['users_juna_admin_list_filter']['cert'],
  );
  $form['filters']['submit'] = array('#type' => 'submit', '#value' => t('Filter'));

  $header = array(
    'uid' => array(
      'data' => t('uid'),
      'type' => 'property',
      'specifier' => 'uid',
      'sort' => 'desc'
    ),
    'mobile' => t('Mobile'),
    'full_name' => t('Full Name'),
    'company' => t('Company'),
    'cert' => array(
      'data' => t('Cert Status'),
      'type' => 'field',
      'specifier' => array('field' => 'field_status', 'column' => 'value')
    ),
    'cert_pic' => t('Cert Pic'),
  );

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->propertyCondition('status', 1)
    ->pager(100)
    ->tableSort($header);

  if (!empty($_SESSION['users_juna_admin_list_filter']['mobile'])) {
    $query->propertyCondition('name', $_SESSION['users_juna_admin_list_filter']['mobile'], '=');
  }

  if (!empty($_SESSION['users_juna_admin_list_filter']['cert'])
    && in_array($_SESSION['users_juna_admin_list_filter']['cert'], [1, 2, 3])
  ) {
    $query->fieldCondition('field_certed', 'value', $_SESSION['users_juna_admin_list_filter']['cert'], '=');
  }

  $result = $query->execute();
  $rows = [];
  if (isset($result['user'])) {
    $uids = array_keys($result['user']);
    $re = entity_load('user', $uids);
    $cert_arr = array('未认证', '已认证', '认证中');
    foreach ($re as $key => $value) {
      $rows[$key]['uid'] = l($value->uid, "user/$value->uid/edit", array('attributes' => array('target' => '_blank')));
      $rows[$key]['mobile'] = $value->name;
      $rows[$key]['full_name'] = empty($value->field_username) ? '' : $value->field_username[LANGUAGE_NONE][0]['safe_value'];
      $rows[$key]['company'] = empty($value->field_company_name) ? '' : $value->field_company_name[LANGUAGE_NONE][0]['safe_value'];
      $rows[$key]['cert'] = empty($value->field_certed) ? '' : $cert_arr[$value->field_certed[LANGUAGE_NONE][0]['value']];
      $rows[$key]['cert_pic'] = empty($value->field_cert) ? '' : l('Show', file_create_url($value->field_cert[LANGUAGE_NONE][0]['uri']), array('attributes' => array('target' => '_blank')));
    }
  }
  $form['nodes'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('暂无用户.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

function users_juna_admin_list_submit($form, &$form_state) {
  $_SESSION['users_juna_admin_list_filter']['mobile'] = trim($form_state['values']['mobile']);
  $_SESSION['users_juna_admin_list_filter']['cert'] = trim($form_state['values']['cert']);
}
