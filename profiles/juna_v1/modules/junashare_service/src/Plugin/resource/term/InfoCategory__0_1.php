<?php

namespace Drupal\junashare_service\Plugin\resource\term;

use Drupal\restful\Plugin\resource\ResourceEntity;
use Drupal\restful\Plugin\resource\ResourceInterface;

/**
 * Class InfoCategory__0_1
 * @package Drupal\junashare_service\Plugin\resource\term
 *
 * @Resource(
 *   name = "infocategory:1.0",
 *   resource = "infocategory",
 *   label = "InfoCategory",
 *   description = "Export the InfoCategory taxonomy term.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "taxonomy_term",
 *     "bundles": {
 *       "info_category"
 *     },
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class InfoCategory__0_1 extends ResourceEntity implements ResourceInterface {

}
