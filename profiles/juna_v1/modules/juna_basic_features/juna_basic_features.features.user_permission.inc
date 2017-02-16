<?php
/**
 * @file
 * juna_basic_features.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function juna_basic_features_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access content'.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access coupon creation form'.
  $permissions['access coupon creation form'] = array(
    'name' => 'access coupon creation form',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'coupon',
  );

  // Exported permission: 'access coupon list'.
  $permissions['access coupon list'] = array(
    'name' => 'access coupon list',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'coupon',
  );

  // Exported permission: 'access coupon portal'.
  $permissions['access coupon portal'] = array(
    'name' => 'access coupon portal',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'coupon',
  );

  // Exported permission: 'access toolbar'.
  $permissions['access toolbar'] = array(
    'name' => 'access toolbar',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'toolbar',
  );

  // Exported permission: 'access user profiles'.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer fields'.
  $permissions['administer fields'] = array(
    'name' => 'administer fields',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field',
  );

  // Exported permission: 'administer filters'.
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'administer privatemsg settings'.
  $permissions['administer privatemsg settings'] = array(
    'name' => 'administer privatemsg settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'administer shortcuts'.
  $permissions['administer shortcuts'] = array(
    'name' => 'administer shortcuts',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'shortcut',
  );

  // Exported permission: 'allow disabling privatemsg'.
  $permissions['allow disabling privatemsg'] = array(
    'name' => 'allow disabling privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'create ad_banner content'.
  $permissions['create ad_banner content'] = array(
    'name' => 'create ad_banner content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create info_page content'.
  $permissions['create info_page content'] = array(
    'name' => 'create info_page content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create page content'.
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create sku_product content'.
  $permissions['create sku_product content'] = array(
    'name' => 'create sku_product content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create yaoxiang_product content'.
  $permissions['create yaoxiang_product content'] = array(
    'name' => 'create yaoxiang_product content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'customize shortcut links'.
  $permissions['customize shortcut links'] = array(
    'name' => 'customize shortcut links',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'shortcut',
  );

  // Exported permission: 'delete any ad_banner content'.
  $permissions['delete any ad_banner content'] = array(
    'name' => 'delete any ad_banner content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any info_page content'.
  $permissions['delete any info_page content'] = array(
    'name' => 'delete any info_page content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any page content'.
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any sku_product content'.
  $permissions['delete any sku_product content'] = array(
    'name' => 'delete any sku_product content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any yaoxiang_product content'.
  $permissions['delete any yaoxiang_product content'] = array(
    'name' => 'delete any yaoxiang_product content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own ad_banner content'.
  $permissions['delete own ad_banner content'] = array(
    'name' => 'delete own ad_banner content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own info_page content'.
  $permissions['delete own info_page content'] = array(
    'name' => 'delete own info_page content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own page content'.
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own sku_product content'.
  $permissions['delete own sku_product content'] = array(
    'name' => 'delete own sku_product content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own yaoxiang_product content'.
  $permissions['delete own yaoxiang_product content'] = array(
    'name' => 'delete own yaoxiang_product content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete privatemsg'.
  $permissions['delete privatemsg'] = array(
    'name' => 'delete privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'edit any ad_banner content'.
  $permissions['edit any ad_banner content'] = array(
    'name' => 'edit any ad_banner content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any info_page content'.
  $permissions['edit any info_page content'] = array(
    'name' => 'edit any info_page content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any page content'.
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any sku_product content'.
  $permissions['edit any sku_product content'] = array(
    'name' => 'edit any sku_product content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any yaoxiang_product content'.
  $permissions['edit any yaoxiang_product content'] = array(
    'name' => 'edit any yaoxiang_product content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own ad_banner content'.
  $permissions['edit own ad_banner content'] = array(
    'name' => 'edit own ad_banner content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own info_page content'.
  $permissions['edit own info_page content'] = array(
    'name' => 'edit own info_page content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own page content'.
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own sku_product content'.
  $permissions['edit own sku_product content'] = array(
    'name' => 'edit own sku_product content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own yaoxiang_product content'.
  $permissions['edit own yaoxiang_product content'] = array(
    'name' => 'edit own yaoxiang_product content',
    'roles' => array(
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'read all private messages'.
  $permissions['read all private messages'] = array(
    'name' => 'read all private messages',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'read privatemsg'.
  $permissions['read privatemsg'] = array(
    'name' => 'read privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'reply only privatemsg'.
  $permissions['reply only privatemsg'] = array(
    'name' => 'reply only privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'select text format for privatemsg'.
  $permissions['select text format for privatemsg'] = array(
    'name' => 'select text format for privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'switch shortcut sets'.
  $permissions['switch shortcut sets'] = array(
    'name' => 'switch shortcut sets',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'shortcut',
  );

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format full_html'.
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use tokens in privatemsg'.
  $permissions['use tokens in privatemsg'] = array(
    'name' => 'use tokens in privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'view roles recipients'.
  $permissions['view roles recipients'] = array(
    'name' => 'view roles recipients',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg_roles',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'system',
  );

  // Exported permission: 'write privatemsg'.
  $permissions['write privatemsg'] = array(
    'name' => 'write privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'write privatemsg to roles'.
  $permissions['write privatemsg to roles'] = array(
    'name' => 'write privatemsg to roles',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'privatemsg_roles',
  );

  return $permissions;
}
