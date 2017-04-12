<?php
/**
 * @file
 * juna_private_message_feature.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function juna_private_message_feature_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'privatemsg_message-privatemsg_message-field_message_type'.
  $field_instances['privatemsg_message-privatemsg_message-field_message_type'] = array(
    'bundle' => 'privatemsg_message',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'privatemsg_message',
    'field_name' => 'field_message_type',
    'label' => 'Message type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'privatemsg_message-privatemsg_message-field_passed'.
  $field_instances['privatemsg_message-privatemsg_message-field_passed'] = array(
    'bundle' => 'privatemsg_message',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '是否通过身份认证',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'privatemsg_message',
    'field_name' => 'field_passed',
    'label' => 'Passed',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 0,
      ),
      'type' => 'options_onoff',
      'weight' => 0,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Message type');
  t('Passed');
  t('是否通过身份认证');

  return $field_instances;
}
