<?php

namespace Drupal\junashare_service\Plugin\resource\DataProvider;

use Drupal\restful\Plugin\resource\DataProvider\DataProviderNode;

/**
 * Class DataProviderYaoxiangNode
 * @package Drupal\junashare_service\Plugin\resource\DataProvider
 */
class DataProviderYaoxiangNode extends DataProviderNode {

  /**
   * {@inheritdoc}
   */
  protected function getEntityFieldQuery() {
    $query = parent::getEntityFieldQuery();
    $query->fieldCondition('field_product_valid_period', 'value', REQUEST_TIME, '<=');
    $query->fieldCondition('field_product_valid_period', 'value2', REQUEST_TIME, '>=');
    return $query;
  }
}
