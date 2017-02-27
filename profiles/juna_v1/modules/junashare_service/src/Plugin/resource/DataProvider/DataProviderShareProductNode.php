<?php

namespace Drupal\junashare_service\Plugin\resource\DataProvider;

use Drupal\restful\Plugin\resource\DataProvider\DataProviderNode;

/**
 * Class DataProviderShareProductNode
 * @package Drupal\junashare_service\Plugin\resource\DataProvider
 */
class DataProviderShareProductNode extends DataProviderNode {

  /**
   * {@inheritdoc}
   */
  protected function getEntityFieldQuery() {
    $query = parent::getEntityFieldQuery();
    $period = 0;
    $start = 0;
    $end = 0;
    if (8 > (int) format_date(REQUEST_TIME, 'custom', 'H')) {
      // 请求时间小于8点，显示昨天最后一场
      $start = strtotime('yesterday 20:00:00');
      $end = strtotime('yesterday 23:59:59');
    }
    else {
      if (isset($_GET['time']) && in_array($_GET['time'], array(1, 2, 3, 4))) {
        $period = $_GET['time'];
      }
      switch ($period) {
        case 1:
          $start = strtotime('today 8:00:00');
          $end = strtotime('today 11:59:59');
          break;
        case 2:
          $start = strtotime('today 12:00:00');
          $end = strtotime('today 15:59:59');
          break;
        case 3:
          $start = strtotime('today 16:00:00');
          $end = strtotime('today 19:59:59');
          break;
        case 4:
          $start = strtotime('today 20:00:00');
          $end = strtotime('today 23:59:59');
          break;
      }
    }
    $query->fieldCondition('field_product_valid_period', 'value', $start, '<=');
    $query->fieldCondition('field_product_valid_period', 'value2', $end, '>=');
    return $query;
  }

}
