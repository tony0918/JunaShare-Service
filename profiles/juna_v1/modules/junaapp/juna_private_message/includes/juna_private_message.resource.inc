<?php

/**
 * Implementation of POST /mymessage.json with tid
 * @param $tid
 * @return array|mixed
 */
function juna_private_message_setread_message($mid) {
  global $user;
  if (!$user->uid) {
    return services_error(t('用户未登陆'), 403);
  }
  privatemsg_message_change_status($mid, PRIVATEMSG_READ);
  return ['data' => 1];
}

/**
 * Implementation of /mymessage.json
 * @return array|mixed
 */
function juna_private_message_my_message($page) {
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
  $query = db_select('pm_index', 'pi');
  $query->condition('pi.recipient', $user->uid)
    ->condition('pi.type', 'user')
    ->fields('pi', array('mid', 'recipient', 'is_new'))
    ->orderBy('pi.mid', 'DESC')
    ->range($page * SINGLE_PAGE_LIMIT, SINGLE_PAGE_LIMIT);

  $query->join('pm_message', 'pm', 'pm.mid=pi.mid');
  $query->fields('pm', array('subject', 'body', 'timestamp'));

  $list = $query->execute()->fetchAll();
  foreach ($list as $key => &$value) {
    $value->message_type = MESSAGE_TYPE_NOTIFICATION;
    $value->body = check_plain($value->body);
    $value->timestamp = format_date($value->timestamp, 'custom', 'Y-m-d H:i');
    unset($value->recipient);
  }
  return ['data' => $list];
}
