<?php

/**
 * Implements hook_panels_layouts().
 */
function cherry_omega_12_twocol_3_9_home_panels_layouts() {
  $items['omega_12_twocol_3_9_home'] = array(
    'title' => t('Two Column 3-9 home'),
    'category' => t('Omega: 12 Column Grid'),
    'icon' => 'omega_12_twocol_3_9_home.png',
    'theme' => 'omega_12_twocol_3_9_home',
    'admin css' => 'omega_panels_admin_12.css',
    'regions' => array(
      'top' => t('Top'),
      'left' => t('Left'),
      'right' => t('Right'),
    ),
  );

  return $items;
}
