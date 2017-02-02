<?php

namespace Drupal\junashare_service\Plugin\resource\product\luckydraw;

use Drupal\restful\Plugin\resource\ResourceNode;

/**
 * 摇享商品接口[WIP]
 * Class Luckydrawproduct__1_0
 * @package Drupal\junashare_service\Plugin\resource\product\luckydraw
 *
 * @Resource(
 *   name = "luckydrawproduct:1.0",
 *   resource = "luckydrawproduct",
 *   label = "LuckydrawProduct",
 *   description = "Export the lucky draw product.",
 *   authenticationTypes = TRUE,
 *   authenticationOptional = TRUE,
 *   dataProvider = {
 *     "entityType": "node",
 *     "bundles": {
 *        "lucky_draw_product"
 *      }
 *   },
 *   majorVersion = 1,
 *   minorVersion = 0
 * )
 */
class Luckydrawproduct__1_0 extends ResourceNode {

}
