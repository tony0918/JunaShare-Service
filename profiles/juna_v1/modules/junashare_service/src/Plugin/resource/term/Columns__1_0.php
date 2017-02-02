<?php

namespace Drupal\junashare_service\Plugin\resource\term;

use Drupal\restful\Plugin\resource\ResourceEntity;
use Drupal\restful\Plugin\resource\ResourceInterface;

/**
 * Taxonomy中的Columns借口
 * Class Columns__1_0
 * @package Drupal\junashare_service\Plugin\resource\term
 *
 * @Resource(
 *   name = "columns:1.0",
 *   resource = "columns",
 *   label = "Columns",
 *   description = "Export the columns taxonomy term.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "taxonomy_term",
 *     "bundles": {
 *       "columns"
 *     },
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Columns__1_0 extends ResourceEntity implements ResourceInterface {

}
