<?php

/**
 * Including some useful utilities for Junashare Service.
 */

namespace Drupal\junashare_service\Plugin\resource;

use Drupal\restful\Plugin\resource\Field\ResourceFieldBase;

final class JunashareServiceUtilities {

  public static function getInstance() {
    static $inst = NULL;
    if ($inst === NULL) {
      $inst = new JunashareServiceUtilities();
    }
    return $inst;
  }

  /**
   * Process callback, Remove Drupal specific items from the image array.
   *
   * @param array $value
   *   The image array.
   *
   * @return array
   *   A cleaned image array.
   */
  public function imageProcess($value) {
    if (ResourceFieldBase::isArrayNumeric($value)) {
      $output = array();
      foreach ($value as $item) {
        $output[] = $this->imageProcess($item);
      }
      return $output;
    }
    return array(
      'fid' => $value['fid'],
      'self' => file_create_url($value['uri']),
      'filemime' => $value['filemime'],
      'filesize' => $value['filesize'],
      'width' => $value['width'],
      'height' => $value['height'],
      'styles' => isset($value['image_styles']) ? $value['image_styles'] : new \stdClass()
    );
  }

  /**
   * Adding additional div to wrap the html content
   * to fix dynamic height of webview issue in IOS.
   * @param $value
   * @return string
   */
  public function skuProductDetailWrapper($value) {
    return theme_sku_product_detail_wrapper($value);
  }
}
