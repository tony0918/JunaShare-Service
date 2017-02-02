<?php
/**
 * @file
 * juna_basic_features.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function juna_basic_features_taxonomy_default_vocabularies() {
  return array(
    'columns' => array(
      'name' => 'Columns',
      'machine_name' => 'columns',
      'description' => '栏目的分类，例如享什么，壹购等',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
