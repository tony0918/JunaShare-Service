<?php
/**
 *批量创建白领券
 *
 */
function create_coupon_batch($num = 500) {
  for ($i = 1; $i <= $num; $i++) {
    $node = new stdClass();
    $node->type = 'coupon';
    node_object_prepare($node);
    global $language;
    $lang_name = $language->language;
    $node->language = $lang_name;
    $node->title = '白领券';
    $node->status = 1;
    $node->promote = 0;  // (1 or 0): promoted to front page or not
    $node->sticky = 0;  // (1 or 0): sticky at top of lists or not
    $node->comment = 1;
    $node->uid = 1;
    $node->created = REQUEST_TIME;
    //$node->field_uid[LANGUAGE_NONE][0]['uid'] = mt_rand(1,100);

    $start = date('j');
    $end = date('t') - 7;

    if ($end <= $start) {
      $end = date('t');
    }
    $startwith = mt_rand($start, $end);
    $starttime = date('Y-m-d', mktime(0, 0, 0, date("m"), date("d") + ($startwith - $start), date("Y")));
    $endtime = date('Y-m-d', strtotime($starttime) + 86400 * 7 - 1);
    if ($end == date('t')) {
      $endtime = date('Y-m-d', mktime(0, 0, 0, date("m"), date("t"), date("Y")));
    }
    $node->field_starttime[LANGUAGE_NONE][0] = ['value' => $starttime, 'format' => $starttime];
    $node->field_endtime [LANGUAGE_NONE][0] = ['value' => $endtime, 'format' => $endtime];
    node_save($node);
    //print_r($node);exit;
    //node_save($node);
  }
}

/**
 *自动创建白领券，用于自动发放白领券
 *@$type 类型，1为第一周券，2为第二张券
 */
function create_coupon() {
  //$node->field_uid[LANGUAGE_NONE][0]['uid'] = mt_rand(1,100);
  $user = user_load_multiple([], ['status' => 1]);
  foreach ($user as $k => $v) {
    if ($v->uid == 1) {
      continue;
    }
    $query = new EntityFieldQuery();
    $maxday = date('t');
    $date = date('Y-m-t');
    $result = $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'coupon')
      //->fieldCondition('field_status', 'value', 1, '=')
      ->fieldCondition('field_uid', 'uid', $v->uid, '=')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->propertyCondition('field_endtime', 'value', $date, '<=')
      ->propertyCondition('field_starttime', 'value', date('Y-m-01'), '>=')
      ->fieldOrderBy('field_endtime', 'value', 'DESC')
      ->execute();
    $coupon = new stdClass();
    $coupon->type = 'coupon';
    node_object_prepare($coupon);
    global $language;
    $lang_name = $language->language;
    $coupon->language = $lang_name;
    $coupon->title = '白领券';
    $coupon->status = 1;
    $coupon->promote = 0;  // (1 or 0): promoted to front page or not
    $coupon->sticky = 0;  // (1 or 0): sticky at top of lists or not
    $coupon->comment = 1;
    $coupon->uid = 1;
    $coupon->created = REQUEST_TIME;
    if (empty($result)) {

      $rand = mt_rand(1, 15);
      $starttime = strtotime(date('Y-m-01')) + ($rand - 1) * 86400;
      $coupon->field_starttime['und'][0]['value'] = date('Y-m-d', $starttime);
      $coupon->field_endtime['und'][0]['value'] = date('Y-m-d', $starttime + 86400 * 7 - 1);
      $coupon->field_uid['und'][0]['uid'] = $v->uid;
      $coupon->field_status['und'][0]['value'] = 1;
      $coupon->field_sendtime['und'][0]['value'] = date('Y-m-d', REQUEST_TIME);
      //echo "1";
      //print_r($coupon);
      node_save($coupon);
      $coupon1 = new stdClass();
      $coupon1->type = 'coupon';
      node_object_prepare($coupon1);
      global $language;
      $lang_name = $language->language;
      $coupon1->language = $lang_name;
      $coupon1->title = '白领券';
      $coupon1->status = 1;
      $coupon1->promote = 0;  // (1 or 0): promoted to front page or not
      $coupon1->sticky = 0;  // (1 or 0): sticky at top of lists or not
      $coupon1->comment = 1;
      $coupon1->uid = 1;
      $coupon1->created = REQUEST_TIME;
      $coupon1->field_uid['und'][0]['uid'] = $v->uid;
      $maxday = date('t');
      $day = date('d', $starttime + 86400 * 7 - 1);
      if ($day >= 15) {
        if ($day == 21) {
          if (date('t') == 28) {
            $coupon1->field_starttime['und'][0]['value'] = $starttime + 86400;
            $coupon1->field_endtime = date('Y-m-d', strtotime(date('t')));
          }
          elseif (date('t') == 29) {
            $rand = mt_rand(22, 23);
            $coupon1->field_starttime['und'][0]['value'] = strtotime(date('Y-m-01')) + ($rand - 1) * 86400;
            $coupon1->field_endtime = strtotime(date('Y-m-01')) + ($rand + 6) * 86400;
          }
          elseif (date('t') == 30) {
            $rand = mt_rand(22, 24);
            $coupon1->field_starttime['und'][0]['value'] = strtotime(date('Y-m-01')) + ($rand - 1) * 86400;
            $coupon1->field_endtime = strtotime(date('Y-m-01')) + ($rand + 6) * 86400;
          }
          elseif (date('t') == 31) {
            $rand = mt_rand(22, 25);
            $coupon1->field_starttime['und'][0]['value'] = strtotime(date('Y-m-01')) + ($rand - 1) * 86400;
            $coupon1->field_endtime = strtotime(date('Y-m-01')) + ($rand + 6) * 86400;
          }
        }
        elseif ($day == 22) {
          if (date('t') == 28) {
            //$coupon1 -> field_starttime['und'][0]['value'] = $starttime+86400;
            //$coupon1 -> field_endtime = date('Y-m-d',strtotime(date('t')));
          }
          elseif (date('t') == 29) {
            $coupon1->field_starttime['und'][0]['value'] = $starttime + 86400;
            $coupon1->field_endtime['und'][0]['value'] = date('Y-m-d', strtotime(date('t')));
          }
          elseif (date('t') == 30) {
            $rand = mt_rand(23, 24);
            $coupon1->field_starttime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand - 1) * 86400);
            $coupon1->field_endtime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand + 6) * 86400);
          }
          elseif (date('t') == 31) {
            $rand = mt_rand(23, 25);
            $coupon1->field_starttime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand - 1) * 86400);
            $coupon1->field_endtime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand + 6) * 86400);
          }
        }
        else {
          $endday = date('t') - 7;
          $rand = mt_rand($day + 1, $endday);
          $coupon1->field_starttime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand - 1) * 86400);
          $coupon1->field_endtime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand + 6) * 86400);
        }
        //$coupon1 -> field_starttime['und'][0]['value'] = $starttime+7*86400;
        //$coupon1 -> field_endtime = date('Y-m-d',strtotime(date('t')));
      }
      else {
        if (date('t') == 28) {
          $rand = mt_rand(16, 21);
        }
        elseif (date('t') == 29) {
          $rand = mt_Rand(16, 22);
        }
        elseif (date('t') == 30) {
          $rand = mt_rand(16, 23);
        }
        elseif (date('t') == 31) {
          $rand = mt_rand(16, 24);
        }
        $coupon1->field_starttime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand - 1) * 86400);
        $coupon1->field_endtime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + ($rand + 6) * 86400 - 1);

      }
      $coupon1->field_sendtime['und'][0]['value'] = date('Y-m-d', REQUEST_TIME);
      node_save($coupon1);
    }
    else {
      $tids = array_keys($result['node']);
      $node = node_load_multiple($tids);
      if (count($tids) == 1) {
        $day = date('d', strtotime($node[$tids[0]]->field_starttime['und'][0]['value']));
        $maxday = date('t');
        if ($day <= 15) {
          $coupon->field_starttime['und'][0]['value'] = date('Y-m-d', strtotime($node[$tids[0]]->field_starttime['und'][0]['value']) + 86400);
          $coupon->field_endtime['und'][0]['value'] = date('Y-m-d', strtotime($node[$tids[0]]->field_starttime['und'][0]['value']) + 86400 + 86400 * 7 - 1);
          $coupon->field_uid['und'][0]['uid'] = $v->uid;
          $coupon->field_status['und'][0]['value'] = 1;
          $coupon->field_sendtime['und'][0]['value'] = date('Y-m-d', REQUEST_TIME);
          //echo "2";
          //print_r($coupon);exit;
          node_save($coupon);

        }
        else {
          //print_r($node);exit;
          $coupon->field_starttime['und'][0]['value'] = date('Y-m-d', strtotime(date('Y-m-01')) + (mt_rand(1, 15) - 1) * 86400);
          $coupon->field_endtime['und'][0]['value'] = date('Y-m-d', strtotime($coupon->field_starttime['und'][0]['value']) + 86400 * 7 - 1);
          $coupon->field_uid['und'][0]['uid'] = $v->uid;
          $coupon->field_status['und'][0]['value'] = 1;
          $coupon->field_sendtime['und'][0]['value'] = date('Y-m-d', REQUEST_TIME);
          //echo "3";
          //print_r($coupon);exit;
          node_save($coupon);
        }


      }
    }

  }
  /*$start = date('j');
  $end = date('t')-7;

  if($end <= $start){
    $end = date('t');
  }
  $startwith = mt_rand($start,$end);
  $starttime = date('Y-m-d',mktime(0, 0, 0, date("m")  , date("d")+($startwith-$start), date("Y")));
  $endtime = date('Y-m-d',strtotime($starttime)+86400*7-1);
  if($end == date('t')){
    $endtime = date('Y-m-d',mktime(0, 0, 0, date("m")  , date("t"), date("Y")));
  }
  $node -> field_starttime[LANGUAGE_NONE][0] = ['value' => $starttime,'format'=>$starttime];
  $node -> field_endtime [LANGUAGE_NONE][0]= ['value' => $endtime,'format' => $endtime];
  node_save($node);*/
}

/**
 *手动发放白领券
 *
 */
function send_coupon($nid, $uid) {
  $node = node_load($nid);
  module_load_include('inc', 'orders', 'includes/config');
  $config = getConfig();
  if ($node->type != 'coupon') {
    return ['errcode' => '10032', 'msg' => $config['10032']];
  }
  else {
    if ($node->field_endtime['und'][0]['value'] < strtotime(date('Y-m-d', REQUEST_TIME))) {
      return ['errcode' => '10033', 'msg' => $config['10033']];
    }
    elseif ($node->field_status['und']['0']['value'] == '2') {
      return ['errcode' => '10034', 'msg' => $config['10034']];
    }
    elseif ($node->field_status['und']['0']['value'] == '3') {
      return ['errcode' => '10035', 'msg' => $config['10035']];
    }
  }
  $node->field_sendtime['und'][0]['value'] = REQUEST_TIME;
  $node->field_uid['und'][0]['uid'] = $uid;
  $node->field_status['und'][0]['value'] = 1;
  node_save($node);
}

/**
 * Sending coupons to all of certified users.
 * @param $users
 * @param $start
 * @param $end
 * @return bool
 */
function coupon_helper_deliver_coupons($users, $start, $end) {
  foreach ($users as $key => $value) {
    coupon_helper_create_a_coupon_node($value->uid, $start, $end);
  }
  return TRUE;
}

/**
 * Sending a coupon to a specific user.
 * @param $uid
 * @param $start
 * @param $end
 */
function coupon_helper_deliver_a_coupon($uid, $start, $end) {
  return coupon_helper_create_a_coupon_node($uid, $start, $end);
}

/**
 * Creating a coupon node.
 * @param $uid
 * @param $start
 * @param $end
 */
function coupon_helper_create_a_coupon_node($uid, $start, $end) {
  $node = new stdClass();
  $node->title = '全场白领';
  $node->type = 'coupon';
  node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->field_uid[LANGUAGE_NONE][0]['uid'] = $uid;
  $node->field_starttime[LANGUAGE_NONE][0]['value'] = date('Y-m-d', strtotime($start));
  $node->field_endtime[LANGUAGE_NONE][0]['value'] = date('Y-m-d', strtotime($end));
  $node->field_status[LANGUAGE_NONE][0]['value'] = 1;
  $node->field_sendtime[LANGUAGE_NONE][0]['value'] = date('Y-m-d', REQUEST_TIME);
  $node = node_submit($node);
  return node_save($node);
}
