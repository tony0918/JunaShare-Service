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
    if (8 > (int) format_date(REQUEST_TIME, 'custom', 'H')) {
      // 请求时间小于8点，显示昨天最后一场
      $start = strtotime('yesterday 20:00:00');
      $end = strtotime('yesterday 23:59:59');
    }
    else {
      $start = strtotime('today 8:00:00');
      $end = strtotime('tomorrow 23:59:59');
    }

    $query->fieldCondition('field_product_valid_period', 'value', $start, '<=');
    $query->fieldCondition('field_product_valid_period', 'value2', $end, '>=');
    return $query;
  }

}
