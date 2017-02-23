<?php

namespace Drupal\junashare_service\Plugin\resource\seller;

use Drupal\junashare_service\Plugin\resource\JunashareServiceUtilities;
use Drupal\restful\Plugin\resource\ResourceNode;

/**
 * Class Seller__1_0
 * @package Drupal\junashare_service\Plugin\resource\seller
 *
 * @Resource(
 *   name = "seller:1.0",
 *   resource = "seller",
 *   label = "seller",
 *   description = "Export the seller",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *       "seller"
 *     }
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Seller__1_0 extends ResourceNode {

  /**
   * {@inheritdoc}
   */
  protected function publicFields() {
    $public_fields = parent::publicFields();
    $utility = JunashareServiceUtilities::getInstance();

    $public_fields['introduce'] = array(
      'property' => 'field_intro',
    );
    $public_fields['logo'] = array(
      'property' => 'field_company_logo',
      'process_callbacks' => array(
        array($utility, 'imageProcess')
      )
    );

    // Clean up some fields.
    unset($public_fields['label'], $public_fields['self']);
    $public_fields['id']['methods'] = array();

    return $public_fields;
  }
}
