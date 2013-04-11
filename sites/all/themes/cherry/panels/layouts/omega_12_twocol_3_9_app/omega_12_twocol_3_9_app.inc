<?php

/**
 * Implements hook_panels_layouts().
 */
function cherry_omega_12_twocol_3_9_app_panels_layouts() {
  $items['omega_12_twocol_3_9_app'] = array(
    'title' => t('Two Column 3-9 app'),
    'category' => t('Omega: 12 Column Grid'),
    'icon' => 'omega_12_twocol_3_9_app.png',
    'theme' => 'omega_12_twocol_3_9_app',
    'admin css' => 'omega_panels_admin_12.css',
    'regions' => array(
      'left_above' => t('Left above'),
      'left_below' => t('Left below'),
      'right_above' => t('Right above'),
      'tab_1' => t('Tab 1'),
      'tab_2' => t('Tab 2'),
      'tab_3' => t('Tab 3'),
      'tab_4' => t('Tab 4'),
    ),
  );

  return $items;
}

function cherry_preprocess_omega_12_twocol_3_9_app(&$vars) {
  /*$options = array();
  $options['type'] = 'file';
  $options['scope'] = 'header';
  $options['group'] = JS_THEME;
  drupal_add_js($vars['layout']['path'] . '/apptabs.js', $options);	*/
}
